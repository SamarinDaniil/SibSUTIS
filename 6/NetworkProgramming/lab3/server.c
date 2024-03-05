#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define BUFLEN 81

void *handleClient(void *arg);

typedef struct thread_data
{
    int socketId;
    FILE *log_file;
} thread_data_t;


const char* FILENAME = "shared_data.txt";
pthread_mutex_t mut;

int main()
{
    int sockMain, clientSocket, length;
    struct sockaddr_in servAddr, clientAddr;
    FILE *log_file = fopen(FILENAME, "w+");

    if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Сервер не может открыть TCP-сокет.");
        exit(1);
    }

    memset((char *)&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = 0;

    if (bind(sockMain, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Связывание сервера неудачно.");
        exit(1);
    }

    length = sizeof(servAddr);
    if (getsockname(sockMain, (struct sockaddr *)&servAddr, (socklen_t *)&length) < 0)
    {
        perror("Вызов getsockname неудачен.");
        exit(1);
    }

    printf("СЕРВЕР: номер порта - %d\n", ntohs(servAddr.sin_port));

    listen(sockMain, 5);

    pthread_mutex_init(&mut, NULL);
    for (;;)
    {
        length = sizeof(clientAddr);

        clientSocket = accept(sockMain, (struct sockaddr *)&clientAddr, (socklen_t *)&length);
        if (clientSocket < 0)
        {
            perror("Ошибка при принятии входящего соединения.");
            exit(1);
        }
        printf("СЕРВЕР: Принято входящее соединение.\n");
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);
        printf("СЕРВЕР: IP адрес клиента: %s\n", clientIp);
        printf("СЕРВЕР: PORT клиента: %d\n", ntohs(clientAddr.sin_port));

        // Создание нового потока для обработки клиента
        thread_data_t data = {clientSocket, log_file};
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleClient, (void *)&data) != 0)
        {
            perror("Не удалось создать поток.");
            close(clientSocket);
        }
        pthread_detach(thread);
    }
    close(sockMain);
    pthread_mutex_destroy(&mut);
    return 0;
}

void *handleClient(void *args)
{
    thread_data_t *data = (thread_data_t *)args;
    int clientSocket = data->socketId;
    FILE *log_file = data->log_file;
    char buf[BUFLEN + 1];
    int msgLength;
    memset(buf, 0, sizeof(buf));

    // Обработка данных от клиента
    while ((msgLength = recv(clientSocket, buf, BUFLEN, 0)) > 0)
    {
        printf("СЕРВЕР: Длина сообщения - %d\n", msgLength);
        printf("СЕРВЕР: Сообщение: %s\n\n", buf);
        //
        int x = atoi(buf);
        int y = x;
        x *= 2;
        x %= 10;
        //
        pthread_mutex_lock(&mut);
        fprintf(log_file, "%d %d %d\n", clientSocket, x, y);
        fflush(log_file);
        pthread_mutex_unlock(&mut);
        //
        sprintf(buf, "%d", x);

        if (send(clientSocket, buf, msgLength, 0) < 0)
        {
            perror("Ошибка отправки сообщения обратно клиенту.");
            exit(1);
        }
        printf("СЕРВЕР: Сообщение отправлено обратно клиенту.\n");

        memset(buf, 0, sizeof(buf));
    }

    if (msgLength == 0)
    {
        printf("СЕРВЕР: Клиент отключился.\n");
    }
    else if (msgLength < 0)
    {
        perror("Плохой socket клиента.");
        exit(1);
        pthread_exit(NULL);
    }

    close(clientSocket);
    pthread_exit(NULL);
}