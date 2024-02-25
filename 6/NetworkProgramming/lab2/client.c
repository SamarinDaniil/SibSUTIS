#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <unistd.h>
#define BUFLEN 81
char buffer[BUFLEN];

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in servAddr;
    struct hostent* hp;

    if (argc < 4)
    {
        printf("Ввести: %s <hostname> <port> <message>\n", argv[0]);
        exit(1);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Ошибка при создании сокета\n");
        exit(1);
    }
    memset((char *)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));

    hp = gethostbyname(argv[1]);
    if (hp == NULL)
    {
        printf("Не удалось получить IP-адрес для указанного хоста\n");
        exit(1);
    }

    memcpy((char*)&servAddr.sin_addr, hp->h_addr_list[0], hp->h_length);

    if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Ошибка при подключении к серверу");
        exit(1);
    }

    for ( ;; )
    {
        if (send(sock, argv[3], strlen(argv[3]), 0) < 0) {
            perror("Ошибка при отправке сообщения");
            exit(1);
        }
        printf("Сообщение отправлено\n");
        ssize_t bytesRead = recv(sock, buffer, BUFLEN, 0);
        if (bytesRead < 0) {
            perror("Ошибка при приеме сообщения");
            exit(1);
        }
        buffer[bytesRead] = '\0';  // Добавляем завершающий нулевой символ
        printf("Получено сообщение от сервера: %s\n", buffer);
        int x = atoi(argv[3]);
        sleep(x);
    }
    close(sock);

    return 0;
}