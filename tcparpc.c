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
	puts("Enter string");
	char st[6];
	scanf("%s", st);
	printf("%s", st);
	int cnt = connect(fd, (struct sockaddr *)&s, sizeof(s));
	if (cnt < 0)
	{
		puts("Error in connection");
		return 0;
	}
	char buff[6];
	bzero(buff, 6);
	int sz = recv(fd, buff, 6, 0);
	if (strcmp(buff, st) == 0)
	{
		puts(buff);
		puts("found");
		char ans[5] = "done";
		send(fd, ans, 5, 0);
		close(fd);
	}
	else
	{
		puts("not found");
	}
	close(fd);
}
