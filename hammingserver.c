#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MX_LEN 1024
#define PORT 8080
void print_reverse(char *c, int n)
{
	for (int i = n - 1; i >= 0; i--)
	{
		printf("%c", *(c + i));
	}
	printf("\n");
}
int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		printf("Error in creating socket\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Socket created successfully\n");
	}
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&addr, addr_len) == -1)
	{
		printf("Error in binding socket\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Socket bind successful\n");
	}
	if (listen(server_fd, 5) == -1)
	{
		printf("Listen failed\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Server is listening\n");
	}
	int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
	if (client_fd == -1)
	{
		printf("Accept failed\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Connection made with client\n");
	}
	printf("Enter the binary string\n");
	char code[MX_LEN], hamming_code[MX_LEN];
	bzero(code, MX_LEN), bzero(hamming_code, MX_LEN);
	scanf("%s", code + 1);
	int n = strlen(code + 1);
	int r = 0;
	while ((1 << r) < r + n)
	{
		r += 1;
	}
	printf("The number of redundant bits are %d\n", r);
	for (int i = 1, j = 1; i <= r + n; i++)
	{
		if (i & (i - 1))
		{
			hamming_code[i] = code[j++];
		}
		else
		{
			hamming_code[i] = '0';
		}
	}
	for (int i = 1; i <= r + n; i++)
	{
		if ((i & (i - 1)) && hamming_code[i] == '1')
		{
			for (int j = 0; j < r; j++)
			{
				if (i >> j & 1)
				{
					hamming_code[1 << j] = '1' + '0' - hamming_code[1 << j];
				}
			}
		}
	}
	printf("The encoded message: ");
	print_reverse(hamming_code + 1, strlen(hamming_code + 1));
	printf("Enter the position at which to introduce an error\n");
	int error_position;
	scanf("%d", &error_position);
	if (1 <= error_position && error_position <= r + n)
	{
		hamming_code[error_position] = '0' + '1' -
									   hamming_code[error_position];
		printf("The string with the error: ");
		print_reverse(hamming_code + 1, strlen(hamming_code + 1));
	}
	else
	{
		printf("Invalid position for an error\n");
	}
	if (send(client_fd, hamming_code, 1 + strlen(hamming_code + 1), 0) == -1)
	{
		printf("Error sending the hamming code to the client\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Hamming code sent successfully to the client\n");
	}
	return 0;
}