#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
    int fd, chk, fd_final;
    //creating a socket referred to using its file descriptor
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        printf("Error in creating socket!");
    //sockaddr_in for internet address
    struct sockaddr_in address, from_client;
    bzero(&address, sizeof(address));
    //Port number, IP Address etc. which are required paramaters for binding them
    //to the socket
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    //binding the socket with the specified IP Address and Port number
    chk = bind(fd, (struct sockaddr *)&address, sizeof(address));
    if (chk != 0)
        printf("Error in allocating address to socket!");
    printf("\nWaiting for client\n");
    //listening for clients
    if ((listen(fd, 3) != 0))
    {
        printf("Error in listening to connections");
        return 0;
    }
    bzero(&from_client, sizeof(from_client));
    int length = sizeof(from_client);
    //accepting the connection from the client to establish a communication between
    //client and server
    fd_final = accept(fd, (struct sockaddr *)&from_client, &length);
    if (fd_final == -1)
    {
        printf("Error in accepting connection request");
        return 0;
    }
    char buff[100000], temp[100000];
    read(fd_final, buff, sizeof(buff));
    //reading the file and writing contents into buff
    int file;
    file = open(buff, O_RDONLY);
    printf("\nFile Name received.\n");
    if (file < 0)
    {
        printf("Invalid Path!");
    }
    else
    {
        bzero(buff, sizeof(buff));
        char c;
        int n = 0, t;
        t = read(file, &c, 1);
        while (t != 0)
        {
            buff[n++] = c;
            t = read(file, &c, 1);
        };
        write(fd_final, buff, sizeof(buff));
        printf("\nFile Transferred\n");
    }
    close(file);
    close(fd_final);
}