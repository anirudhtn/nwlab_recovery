#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    char buff[100];
    while (1)
    {
        printf("Client:");
        scanf("%s", buff);
        int len = sizeof(serv_addr);
        sendto(sock, buff, sizeof(buff), 0, (struct sockaddr *)&serv_addr, len);
        recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&serv_addr, &len);
        printf("Server:%s\n", buff);
    }
    close(sock);
    return 0;
}