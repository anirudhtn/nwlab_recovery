#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
const int mx = 1024;

int main(int argc, char **argv)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in s;
	s.sin_family = AF_INET;
	s.sin_port = htons(atoi(argv[1]));
	s.sin_addr.s_addr = INADDR_ANY;
	int cnt = connect(fd, (struct sockaddr *)&s, sizeof(s));
	while (1)
	{
		char st[20];
		bzero(st, sizeof(st));
		puts("Enter string");
		scanf("%s", st);
		send(fd, st, sizeof(st), 0);
		if (strcmp(st, "bye") == 0)
			break;
		char buffer[20];
		bzero(buffer, sizeof(buffer));
		int p = recv(fd, buffer, sizeof(buffer), 0);
		puts("Message received!");
		puts(buffer);
	}
	close(fd);
	return 0;
}
