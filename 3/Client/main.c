#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 500
#define PORT 5223

struct sockaddr_in addr;
int sock;

void sleepFun(int num) {
    for (int i = 0; i < num; i++) {
        printf("%d\n", num - i);
        sleep(1);
    }
}
void setSocket(char *ip) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(PORT);
}
void setConnection(int sock) {
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("set connection...\n");
    int num = 2 + rand() % 9;
    sleepFun(num);
}
void sendMessage(int sock, char* message) {
    send(sock, message, strlen(message), 0);
    printf("send message...\n");
    int num = 2 + rand() % 9;
    sleepFun(num);
}
void getMessage(int sock) {
    char buf[sizeof(BUFSIZE)];
    int byteRead = recv(sock, buf, BUFSIZE - 1, 0);
    buf[byteRead] = '\0';
    printf("get message: %s\n", buf);
}

int main () {
    srand(time(NULL));

    printf("in ip:\n");

    char *ip = (char*) malloc(sizeof(char) * BUFSIZE);
    fgets(ip, BUFSIZE, stdin);
    setSocket(ip);
    setConnection(sock);

    printf("in message:\n");

    char* message = (char*) malloc(sizeof(char) * BUFSIZE);
    fgets(message, BUFSIZE, stdin);
    sendMessage(sock, message);
    getMessage(sock);

    free(message);
    free(ip);
    close(sock);
    return 0;
}