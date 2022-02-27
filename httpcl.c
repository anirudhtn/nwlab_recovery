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
	char domain[mx], path[mx];
	int i = 0, j = 0;
	while (argv[1][i] != '/')
	{
		domain[i] = argv[1][i];
		++i;
	}
	domain[i] = '\0';
	while (argv[1][i] != '\0')
	{
		path[j++] = argv[1][i++];
	}
	path[j] = '\0';
	struct hostent *host;
	host = gethostbyname(domain);
	char *IP;
	IP = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
	printf("The domain is %s\nThe path is %s\nThe IP address is %s\nThe host name is %s\n", domain, path, IP, host->h_name);
	struct sockaddr_in s;
	s.sin_family = AF_INET;
	s.sin_port = htons(80);
	s.sin_addr.s_addr = inet_addr(IP);
	int cnt = connect(fd, (struct sockaddr *)&s, sizeof(s));
	if (cnt < 0)
	{
		puts("Error in connecting to server");
		return 0;
	}

	// struct hostent *host; host=gethostbyname(domain); char *ip=inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
	// sprintf(buff,"GET %s HTTP/1.1\r\nhost:%s\r\n\r\n",domain,path)
	// write(fd,buff,sizeof(buff));
	// read(fd,store,sizeof(store));

	char buff[mx];
	bzero(buff, mx);
	sprintf(buff, "GET %s HTTP/1.1\r\nHost:%s\r\n\r\n", path, domain);
	puts(buff);
	char filename[mx], store[mx];
	bzero(store, sizeof(store));
	write(fd, buff, sizeof(buff));
	read(fd, store, sizeof(store));
	FILE *fp;
	puts("Enter filename .html");
	scanf("%s", filename);
	fp = fopen(filename, "w");
	fputs(store, fp);
	puts("Success!");
	return 0;
}
