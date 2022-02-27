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
#include <fcntl.h>
const int mx = 1024;
int main(int argc, char **argv)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	char domain[mx], path[mx];
	int i = 0, j = 0;
	while (argv[1][i] != '/')
	{
		domain[i] = argv[1][i];
		++i;
	}
	domain[i] = '\0';
	while (argv[1][i] != '\0')
		path[j++] = argv[1][i++];
	path[j] = '\0';
	struct hostent *host;
	host = gethostbyname(domain);
	char *IP = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
	struct sockaddr_in s;
	s.sin_port = htons(80);
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = inet_addr(IP);
	printf("The domain is %s\nThe path is %s\nThe IP address is %s\nThe host name is %s\n", domain, path, IP, host->h_name);
	int cnt = connect(sock, (struct sockaddr *)&s, sizeof(s));
	char buff[mx], store[mx], filename[mx];
	bzero(buff, sizeof(buff));
	bzero(store, mx);
	sprintf(buff, "GET %s HTTP/1.1\r\nHost:%s\r\n\r\n", path, domain);
	write(sock, buff, mx);
	read(sock, store, mx);
	puts("Enter filename .html");
	bzero(filename, mx);
	scanf("%s", filename);
	FILE *fp;
	fp = fopen(filename, "w");
	fputs(store, fp);
	puts("Done");
	// /lose(sock);
	return 0;
}
