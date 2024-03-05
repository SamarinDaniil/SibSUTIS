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
#define BUFLEN 81

void *handleClient(void *arg);

int main()
{
    int sockMain, clientSocket, length;
    struct sockaddr_in servAddr, clientAddr;

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
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleClient, (void *)&clientSocket) != 0)
        {
            perror("Не удалось создать поток.");
            close(clientSocket);
        }
        pthread_detach(thread);
    }
    close(sockMain);
    return 0;
}

void *handleClient(void *arg)
{
    int clientSocket = *(int *)arg;
    char buf[BUFLEN + 1];
    int msgLength;
    memset(buf, 0, sizeof(buf));

    // Обработка данных от клиента
    while ((msgLength = recv(clientSocket, buf, BUFLEN, 0)) > 0)
    {
        printf("СЕРВЕР: Длина сообщения - %d\n", msgLength);
        printf("СЕРВЕР: Сообщение: %s\n\n", buf);

        int x = atoi(buf);
        x *= 2;
        x %= 10;
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
    }

    close(clientSocket);
    pthread_exit(NULL);
}