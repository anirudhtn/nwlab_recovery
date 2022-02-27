// TCP - ARP
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#define MAX 80
#define PORT 5050
#define ADDR struct sockaddr
int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in servaddr;
	char buff[MAX];
	int valread, flag=0;

    char source_ip[20] , dest_ip[20] , source_mac[20] , dest_mac[20];
    
    char operation[3] = "2|";
	char message[100];
    
    printf("Enter the IP address : ");
    scanf("%s", source_ip);
    printf("Enter the Mac address: ");
    scanf("%s", source_mac);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (ADDR*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
    
    bzero(buff, MAX);    
    read(sockfd, buff, sizeof(buff));

    printf("ARP Request Received:\n");
    printf("%s\n", buff);

    char *tokens = strtok(buff, "|");  
    if(strcmp(tokens, "1") == 0) {
        strcat(message, "2|");
    } 
    else {
        strcat(message, "1|");
    }
    tokens = strtok(0, "|");
    
    strcat(message, source_mac);
    strcat(message, "|");
    strcat(message, source_ip);  
    strcat(message, "|");
    strcat(message, tokens); 
    strcpy(dest_mac, tokens);
    strcat(message, "|");
    tokens = strtok(0, "|");
    strcat(message, tokens); 

    tokens = strtok(0, "|");
    tokens = strtok(0, "|");
    if(strcmp(tokens, source_ip) == 0) {
        flag = 1;
        printf("IP address matches\n");
    }
    printf("%d", sockfd);
    if(flag == 1) {
        write(sockfd, message, sizeof(message));
        printf("Message sent : %s\n", message);
        bzero(buff, MAX);
        if( (valread = read(sockfd, buff, sizeof(buff))) > 0 )
            printf("Data Recieved from %s : %s\n", dest_mac, buff);
        else
            printf("Corrupted ARP packet sent\n");
    }
    else {
        printf("IP address does not match.\n");
    }
    
	close(sockfd);
	return 0;
}
