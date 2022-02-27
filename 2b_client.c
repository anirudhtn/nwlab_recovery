#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
	int fd, chk;
	char buff[10000], x;
	//creating a socket referred to using its file descriptor
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		printf("Error in creating socket!");
	//sockaddr_in for internet address
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	//Port number, IP Address etc. which are required paramaters for connecting to the server
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[2]));
	address.sin_addr.s_addr = inet_addr(argv[1]);
	//to connect to the server based on the specified IP Address and port number
	chk = connect(fd, (struct sockaddr *)&address, sizeof(address));
	if (chk != 0)
		printf("ERROR IN CONNECTING TO SERVER");
	bzero(buff, sizeof(buff));
	printf("\nEnter the path of file: ");
	scanf("%s", buff);
	int n = 0;
	char filename[1000000];
	strcpy(filename, buff);
	char *token = strtok(filename, ".");
	strcat(token, "_new.txt");
	write(fd, buff, sizeof(buff));
	printf("\nFile name transferred.\n");
	bzero(buff, sizeof(buff));
	read(fd, buff, sizeof(buff));
	int file, t;
	mode_t m = S_IRWXU;
	file = creat(token, m);
	char c;
	int len, knt = 0;
	len = strlen(buff);
	while (knt != len)
	{
		c = buff[knt++];
		write(file, &c, 1);
	}
	printf("\nFile is stored at: %s\n", filename);
	close(file);
	close(fd);
}