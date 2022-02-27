#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main(int argc, char **argv)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in s;
    s.sin_addr.s_addr = INADDR_ANY;
    s.sin_port = htons(atoi(argv[1]));
    s.sin_family = AF_INET;
    char buffer[100];
    bzero(buffer, sizeof(buffer));
    while (1)
    {
        printf("\nEnter message you want to send to server : ");
        scanf("%s", buffer);
        sendto(sock, (const char *)buffer, sizeof(buffer), 0, (struct sockaddr *)&s, sizeof(s));
        printf("\nMessage from server : ");
        bzero(buffer, sizeof(buffer));
        int len = sizeof(s);
        int n = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&s, &len);
        puts(buffer);
    }
}