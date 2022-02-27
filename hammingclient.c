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
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd == -1)
	{
		printf("Error in creating socket\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Socket created successfully\n");
	}
	struct sockaddr_in server_addr;
	int addr_len = sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	if (connect(client_fd, (struct sockaddr *)&server_addr, addr_len) == -1)
	{
		printf("Connection failed\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Connected to the server\n");
	}
	char hamming_code[MX_LEN], original_code[MX_LEN];
	bzero(hamming_code, MX_LEN), bzero(original_code, MX_LEN);
	int sz = recv(client_fd, hamming_code, MX_LEN, 0);
	if (sz == -1)
	{
		printf("Receive failed\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Successfully received encoded message ");
		print_reverse(hamming_code + 1, strlen(hamming_code + 1));
	}
	int r = 0;
	while ((1 << r) < sz - 1)
	{
		r += 1;
	}
	printf("The number of redundant bits are %d\n", r);
	int error_position = 0;
	for (int j = 0; j < r; j++)
	{
		int xor = 0;
		for (int i = 1; i < sz; i++)
		{
			if (i >> j & 1)
			{
				xor ^= hamming_code[i] - '0';
			}
		}
		error_position |= xor << j;
	}
	if (error_position > 0)
	{
		hamming_code[error_position] = '0' + '1' -
									   hamming_code[error_position];
	}
	for (int i = 1, j = 1; i <= sz; i++)
	{
		if (i & (i - 1))
		{
			original_code[j++] = hamming_code[i];
		}
	}
	if (error_position == 0)
	{
		printf("There was no error in the message\n");
		printf("The received message is ");
		print_reverse(original_code + 1, strlen(original_code + 1));
	}
	else
	{
		printf("There was an error at position %d\n", error_position);
		printf("The corrected message is ");
		print_reverse(original_code + 1, strlen(original_code + 1));
	}
	return 0;
}