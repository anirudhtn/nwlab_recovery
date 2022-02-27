#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s, c;
    bzero((struct sockaddr *)&s, sizeof(s));
    bzero((struct sockaddr *)&c, sizeof(c));
    s.sin_addr.s_addr = INADDR_ANY;
    s.sin_family = AF_INET;
    s.sin_port = htons(atoi(argv[1]));
    if (bind(fd, (struct sockaddr *)&s, sizeof(s)) < 0)
    {
        puts("Bind error");
        return 0;
    }
    listen(fd, 5);
    int len = sizeof(c);
    int cfd = accept(fd, (struct sockaddr *)&c, &len);
    char buffer[20];
    while (1)
    {
        bzero(buffer, sizeof(buffer));
        int sz = recv(cfd, buffer, sizeof(buffer), 0);
        puts(buffer);
        if (strcmp(buffer, "bye") == 0)
        {
            break;
        }
        bzero(buffer, sizeof(buffer));
        puts("Enter message!");
        scanf("%s", buffer);
        send(cfd, buffer, sizeof(buffer), 0);
    }
    close(fd);
    close(cfd);
    return 0;
}