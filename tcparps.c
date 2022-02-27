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
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s;
    s.sin_family = AF_INET;
    s.sin_port = htons(atoi(argv[1]));
    s.sin_addr.s_addr = INADDR_ANY;
    int len = sizeof(s);
    bind(fd, (struct sockaddr *)&s, len);
    if (listen(fd, 4) < 0)
    {
        puts("Listening error");
        return 0;
    }
    char buff2[6], buff[6] = "hello";
    struct sockaddr_in c;
    int ok = 0;
    int cfd;
    while (1)
    {
        int len = sizeof(c);
        cfd = accept(fd, (struct sockaddr *)&c, &len);
        if (cfd < 0)
        {
            puts("error accept");
            return 0;
        }
        send(cfd, buff, 6, 0);
        bzero(buff2, 6);
        if (recv(cfd, buff2, 6, 0) > 0)
        {
            puts("match found");
            ok = 1;
            puts(buff2);
        }
        close(cfd);
        if (ok)
            break;
    }
    return 0;
}