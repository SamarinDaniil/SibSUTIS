#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include "config.h"


char board[BOARD_SIZE][BOARD_SIZE];

// Инициализация доски
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }
}

// Изменение символа на доске
void changeSymbol(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        board[x][y] = '*';
    } else {
        printf("Некорректные координаты\n");
    }
}

// Вывод доски
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("_|");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf("%d ", i);
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    struct hostent* hp;

    
    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Клиент не может открыть TCP-сокет.");
        exit(1);
    }
    
    // Задание параметров сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    hp = gethostbyname(SERVER_IP);
    if (hp == NULL)
    {
        printf("Не удалось получить IP-адрес для указанного хоста\n");
        exit(1);
    }

    memcpy((char*)&server_addr.sin_addr, hp->h_addr_list[0], hp->h_length);
    
    // Подключение к серверу
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при подключении к серверу");
        exit(1);
    }
    // Чтение приветственного сообщения от сервера
    initializeBoard(board);
    printf("Игры: Искатели сокровищ(на трех игроков) \n У каждого игрорка своя карта 8x8, где он ищет свое сокровище, кто первый нашел тот и победил \n Карта начинается с позиции (0,0)\n");
    memset(buffer, 0, sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    printf("%s", buffer);
    int length = strlen(buffer);
    char lastChar = buffer[length - 2];
    int numPlayer = lastChar - '0';
    printf("Индекс игрока: %c\n", lastChar);
    
    //Game start
    memset(buffer, 0, sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    printf("%s", buffer);
    //
    sleep(2);
    system("clear");
    // Основной игровой цикл
    int con = 0;
    while (1) {
        // Чтение хода от игрока
        con++;
        printf("Ход №%d.\n", con);
        int endgame = 0;
        for (int count = 0; count < MAX_PLAYERS; count++)
        {
            if (count == numPlayer-1)
            {
                int x = -1, y = -1;
                printf("Индекс игрока: %c\n", lastChar);
                printBoard(board);
                while (1 == 1) {
                    printf("Введите координаты раскопки клада(x y): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    sscanf(buffer, "%d %d", &x, &y);
                    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                        break;
                    } else {
                        printf("Некорректные координаты\n");
                    }
                }
                
                changeSymbol(board, x, y);
                buffer[strcspn(buffer, "\n")] = 0;

                // Отправка хода серверу
                send(sockfd, buffer, strlen(buffer), 0);

            }
            
            // Получение ответа от сервера
            memset(buffer, 0, sizeof(buffer));

            ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead < 0) {
                perror("Ошибка при приеме сообщения");
                exit(1);
            }
            buffer[bytesRead] = '\0';
            printf("%s", buffer);
            // Проверка на конец игры
            if (strstr(buffer, "нашел сокровище!") != NULL) {
                endgame = 1;
                break;
            }
        }
        sleep(2);
        system("clear");
        if (endgame == 1)
        {

            break;
        }
    }

    // Закрытие соединения
    close(sockfd);

    return 0;
}