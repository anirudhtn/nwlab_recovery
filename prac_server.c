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
const int nn = 1024;
typedef struct obj
{
    int cnt;
    char name[20];
    char ip[20][20];
} dns;

int main(int argc, char **argv)
{
    int fd[5], mx = -1;
    struct sockaddr_in s[5];
    for (int i = 0; i < 5; ++i)
    {
        fd[i] = socket(AF_INET, SOCK_DGRAM, 0);
        mx = (mx < fd[i] ? fd[i] : mx);
    }
    for (int i = 0; i < 5; ++i)
    {
        s[i].sin_family = AF_INET;
        s[i].sin_addr.s_addr = INADDR_ANY;
        s[i].sin_port = htons(6000 + i);
        bind(fd[i], (struct sockaddr *)&s[i], sizeof(s[i]));
    }
    fd_set sockfds;
    FD_ZERO(&sockfds);
    dns ob[2];
    for (int i = 0; i < 2; ++i)
    {
        ob[i].cnt = 0;
    }
    strcpy(ob[0].name, "www.google.com");
    strcpy(ob[0].ip[ob[0].cnt++], "111.111.11");
    strcpy(ob[1].name, "www.youtube.com");
    strcpy(ob[1].ip[ob[1].cnt++], "222.222.22");
    while (1)
    {
        for (int i = 0; i < 5; ++i)
            FD_SET(fd[i], &sockfds);
        int sel = select(mx, &sockfds, NULL, NULL, NULL);
        for (int i = 0; i < 5; ++i)
        {
            if (FD_ISSET(fd[i], &sockfds))
            {
                char opt[5];
                bzero(opt, 5);
                int len = sizeof(s[i]);
                recvfrom(fd[i], opt, 5, 0, (struct sockaddr *)&s[i], &len);
                if (strcmp(opt, "one") == 0)
                {
                    char buffer[20], buffer2[200];
                    bzero(buffer, 20);
                    bzero(buffer2, 200);
                    puts("here,,,,,,");
                    recvfrom(fd[i], buffer, 20, 0, (struct sockaddr *)&s[i], &len);
                    puts("got this");
                    puts(buffer);
                    for (int i = 0; i < 2; ++i)
                    {
                        printf("%d %d", strcmp(ob[i].name, buffer), i);
                        if (strcmp(ob[i].name, buffer) == 0)
                        {
                            printf("%d %d", strcmp(ob[i].name, buffer), i);
                            for (int j = 0; j < ob[i].cnt; ++j)
                            {
                                strcat(buffer2, ob[i].ip[j]);
                                strcat(buffer2, "|");
                            }
                        }
                    }
                    puts("Sending this ");
                    puts(buffer2);
                    sendto(fd[i], buffer2, sizeof(buffer2), 0, (struct sockaddr *)&s[i], len);
                }
                else
                {
                    puts("maybe.....");
                    char buffer[200], buffer2[200];
                    bzero(buffer, 200);
                    bzero(buffer2, 200);
                    recvfrom(fd[i], buffer, 200, 0, (struct sockaddr *)&s[i], &len);
                    char first[20], second[20];
                    int p1 = 0, p2 = 0, ok = 0;
                    for (int i = 0; i < strlen(buffer); ++i)
                    {
                        if (buffer[i] == '|')
                            ok = 1;
                        else
                        {
                            if (ok)
                            {
                                second[p2++] = buffer[i];
                            }
                            else
                            {
                                first[p1++] = buffer[i];
                            }
                        }
                    }
                    for (int i = 0; i < 2; ++i)
                    {
                        if (strcmp(ob[i].name, first) == 0)
                        {
                            strcpy(ob[i].ip[ob[i].cnt++], second);
                        }
                    }
                    puts("ADDED : ");
                    puts(first);
                    puts(second);
                }
            }
        }
    }
    return 0;
}