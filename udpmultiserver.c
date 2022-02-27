#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
int main(int argc, char **argv)
{
    int sock[5], maxsock;
    for (int i = 0; i < 5; ++i)
    {
        sock[i] = socket(AF_INET, SOCK_DGRAM, 0);
        maxsock = (sock[i] > maxsock) ? sock[i] : maxsock;
    }
    struct sockaddr_in server[5], client;
    for (int i = 0; i < 5; ++i)
    {
        server[i].sin_port = htons(7030 + i);
        server[i].sin_family = AF_INET;
        server[i].sin_addr.s_addr = INADDR_ANY;
        bind(sock[i], (struct sockaddr *)&server[i], sizeof(server[i]));
    }
    fd_set sockfds;
    FD_ZERO(&sockfds);
    ++maxsock;
    while (1)
    {
        for (int i = 0; i < 5; ++i)
        {
            FD_SET(sock[i], &sockfds);
        }
        int sel = select(maxsock, &sockfds, NULL, NULL, NULL);
        for (int i = 0; i < 5; ++i)
        {
            if (FD_ISSET(sock[i], &sockfds))
            {
                int len = sizeof(client);
                printf("\nMessage from client %d : ", i + 1);
                char buffer[100];
                bzero(buffer, sizeof(buffer));
                int n = recvfrom(sock[i], buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len);
                puts(buffer);
                bzero(buffer, sizeof(buffer));
                printf("\nEnter message to send to client %d : ", i + 1);
                scanf("%s", buffer);
                sendto(sock[i], (const char *)buffer, sizeof(buffer), 0, (struct sockaddr *)&client, len);
            }
        }
    }
}