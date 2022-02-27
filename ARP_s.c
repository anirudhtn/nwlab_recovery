// TCP ARP
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
	
#define TRUE 1
#define FALSE 0
#define PORT 5050

int main(int argc , char *argv[])
{
	int opt = TRUE;
	int master_socket , addrlen , new_socket , valread ;
	struct sockaddr_in address;		
	char buffer[1025];   
    char source_ip[20] , dest_ip[20] , source_mac[20] , dest_mac[20] , data[17] , ARPpacket[100];
    char operation[2] = "1";

	printf("Destination IP : ");
	scanf("%s", dest_ip);
	printf("Source IP :");
	scanf("%s", source_ip);
	printf("Source MAC : ");
	scanf("%s", source_mac);
	printf("16 bit data :");
	scanf("%s", data);	
    strcpy(dest_mac, "00-00-00-00-00-00");

	printf("Developing ARP Request packet\n");
    strcpy(ARPpacket, operation);
    strcat(ARPpacket, "|");
    strcat(ARPpacket, source_mac);
    strcat(ARPpacket, "|");
    strcat(ARPpacket, source_ip);
    strcat(ARPpacket, "|");
    strcat(ARPpacket, dest_mac);
    strcat(ARPpacket, "|");
    strcat(ARPpacket, dest_ip);

	puts(ARPpacket);

	master_socket = socket(AF_INET , SOCK_STREAM , 0);
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	if (listen(master_socket, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	addrlen = sizeof(address);
	printf("The ARP Request packet is broadcasted.\n");
	printf("Waiting for ARP Reply...\n");
	while(TRUE)
	{
		bzero(&buffer, sizeof(buffer));
        
		new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    		printf("%d", new_socket);

        send(new_socket, ARPpacket, strlen(ARPpacket), 0);
       
        if ((valread = read( new_socket , buffer, 1024)) <= 0)
        {                  
            close( new_socket );
        }
        else
        {
            buffer[valread] = '\0';
			printf("ARP Reply received\n");
            printf("%s\n", buffer);			
			
			char *tokens = strtok(buffer, "|");
			tokens = strtok(0, "|"); // dest_mac
			if(strcmp(tokens, dest_mac) == 0) 	{
				printf("Corrupted Packet\n");
				close(new_socket);
				continue;
			}
			printf("Sent data packet to: %s\n", tokens);
			send(new_socket , data , strlen(data) , 0 ); 
			close(new_socket);	
			break;
        }      
    }	
	close(master_socket);

	return 0;
}
