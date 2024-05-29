#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "config.h"

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int sockfd;
    Position hidden_treasure;
    int game_id;
    int player_num;
} Player;

typedef struct {
    Player players[MAX_PLAYERS];
    int current_player_count;
    int game_id;
    int is_active;
} Game;

Game games[MAX_GAMES];
int current_game_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *game_thread(void *arg) {
    sleep(1);
    Game *game = (Game *)arg;
    char buffer[1024];
    pthread_mutex_lock(&mutex);
    if (!game->is_active) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            close(game->players[i].sockfd);
        }
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
        //return;
    }
    pthread_mutex_unlock(&mutex);
    sprintf(buffer, "Игра началась!\n");
    for (int j = 0; j < MAX_PLAYERS; j++) {
        send(game->players[j].sockfd, buffer, strlen(buffer), 0);
    }
    
    while (1) {

        for (int i = 0; i < MAX_PLAYERS; i++) {

            int sockfd = game->players[i].sockfd;
            memset(buffer, 0, sizeof(buffer));
            
            // Получить ход игрока
            ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            buffer[bytesRead] = '\0';
            printf("%s", buffer);
            if (bytesRead == 0)
            {
                printf("СЕРВЕР: Клиент отключился.\n");
                game->is_active = 0;
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    close(game->players[i].sockfd);
                }
                pthread_exit(NULL);
            }
            else if (bytesRead < 0)
            {
                perror("СЕРВЕР: Плохой socket клиента.");
                game->is_active = 0;
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    close(game->players[i].sockfd);
                }
                pthread_exit(NULL);
            }
            
            // Обработка хода (поиск сокровища)
            int x, y;
            sscanf(buffer, "%d %d", &x, &y);
            
            if (x == game->players[i].hidden_treasure.x && y == game->players[i].hidden_treasure.y) {
                sprintf(buffer, "Игрок %d нашел сокровище!\n", i + 1);
                for (int j = 0; j < MAX_PLAYERS; j++) {
                    if (send(game->players[j].sockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("СЕРВЕР: Ошибка при отправке сообщения");
                        exit(1);
                    }
                }
                game->is_active = 0;
                break;
            } else {
                sprintf(buffer, "Игрок %d копает (%d, %d) - сокровищ нет\n", i + 1, x, y);
                for (int j = 0; j < MAX_PLAYERS; j++) {
                    if (send(game->players[j].sockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("СЕРВЕР: Ошибка при отправке сообщения");
                        exit(1);
                    }
                }
            }
        }
    }

    // Закрытие соединений с игроками
    for (int i = 0; i < MAX_PLAYERS; i++) {
        close(game->players[i].sockfd);
    }
    game->is_active = 0;

    pthread_exit(NULL);
}

void start_game(Game *game) {
    pthread_t tid;
    pthread_create(&tid, NULL, game_thread, (void *)game);
}

void *handle_client(void *arg) {
    int sockfd = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);

    // Найти или создать игру для игрока
    Game *game = NULL;
    for (int i = 0; i < current_game_count; i++) {
        if (games[i].current_player_count < MAX_PLAYERS) {
            game = &games[i];
            break;
        } else {
            if (i == current_game_count-1)
            {
                i = 0;
            }
        }
    }

    if (game == NULL && current_game_count < MAX_GAMES) {
        game = &games[current_game_count++];
        game->game_id = current_game_count;
        game->current_player_count = 0;
        game->is_active = 0;
    }

    int player_num = game->current_player_count++;
    game->players[player_num].sockfd = sockfd;
    game->players[player_num].hidden_treasure.x = 1; //rand() % BOARD_SIZE;
    game->players[player_num].hidden_treasure.y = 1; //rand() % BOARD_SIZE;
    game->players[player_num].game_id = game->game_id;
    game->players[player_num].player_num = player_num;

    // Проверка на заполнение игры
    if (game->current_player_count == MAX_PLAYERS) {
        game->is_active = 1;
        start_game(game);
    }
    if (current_game_count == MAX_GAMES-1)
    {
        current_game_count = 0;
    }

    pthread_mutex_unlock(&mutex);

    // Сообщение игроку о его статусе ожидания/начала игры
    char message[256];
    if (game->is_active) {
        printf("СЕРВЕР: Игра %d началась. Вы игрок № %d\n", game->game_id, player_num + 1);
        sprintf(message, "Игра %d началась. Вы игрок № %d\n", game->game_id, player_num + 1);
    } else {
        printf("СЕРВЕР: Подождите больше игроков для начала игры %d. Вы игрок № %d\n", game->game_id, player_num + 1);
        sprintf(message, "Подождите больше игроков для начала игры %d. Вы игрок № %d\n", game->game_id, player_num + 1);
    }
    if (send(sockfd, message, strlen(message),0) < 0) {
        perror("СЕРВЕР: Ошибка отправки сообщения обратно клиенту.");
        //exit(1);
    }

    pthread_exit(NULL);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t cli_len;
    int *new_sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("СЕРВЕР: Сервер не может открыть TCP-сокет.");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("СЕРВЕР: Связывание сервера неудачно.");
        close(sockfd);
        exit(1);
    }

    printf("СЕРВЕР: Включен!\n");
    listen(sockfd, 5);
    cli_len = sizeof(client_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &cli_len);
        if (newsockfd < 0) {
            perror("Ошибка при принятии входящего соединения.");
            continue;
        }
        printf("СЕРВЕР: Принято входящее соединение.\n");
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), clientIp, INET_ADDRSTRLEN);
        printf("СЕРВЕР: IP адрес клиента: %s\n", clientIp);
        printf("СЕРВЕР: PORT клиента: %d\n", ntohs(client_addr.sin_port));

        pthread_t tid;
        new_sockfd = malloc(sizeof(int));
        *new_sockfd = newsockfd;
        pthread_create(&tid, NULL, handle_client, (void *)new_sockfd);
    }

    close(sockfd);
    return 0;
}