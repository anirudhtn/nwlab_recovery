#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void main()
{
    int sock[5], maxsock;
    for (int i = 0; i < 5; i++)
    {
        sock[i] = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock[i] > maxsock)
            maxsock = sock[i];
    }
    struct sockaddr_in serv_addr[5], cli_addr;
    for (int i = 0; i < 5; i++)
    {
        serv_addr[i].sin_port = htons(7000 + i);
        serv_addr[i].sin_family = AF_INET;
        serv_addr[i].sin_addr.s_addr = INADDR_ANY;
        bind(sock[i], (struct sockaddr *)&serv_addr[i], sizeof(serv_addr[i]));
    }
    fd_set sockset;
    FD_ZERO(&sockset);
    maxsock = maxsock + 1;
    while (1)
    {
        for (int i = 0; i < 5; i++)
            FD_SET(sock[i], &sockset);
        int n = select(maxsock, &sockset, NULL, NULL, NULL);
        for (int i = 0; i < 5; i++)
        {
            if (FD_ISSET(sock[i], &sockset))
            {
                int len = sizeof(cli_addr);
                printf("\nMessage from UDP client %d: ", i + 1);
                char buffer[100];
                n = recvfrom(sock[i], buffer, sizeof(buffer), 0,
                             (struct sockaddr *)&cli_addr, &len);
                puts(buffer);
                char message[100] = "Received";
                sendto(sock[i], (const char *)message, sizeof(message), 0,
                       (struct sockaddr *)&cli_addr, sizeof(cli_addr));
            }
        }
    }
}