#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENT   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET "\x1B[0m"

void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}

int main(int argc, char const *argv[])
{
	int sockfd, new_sockfd, addrlen, bytes_readed, client_num, i;
	char mens_serv[100];
	/*FILE * chat;*/
	/*Socket*/
	struct sockaddr_in my_addr, remote_addr;

	if(argc != 2)
	{
		printf("USE: ./server <Clients number>\n");
		return 1;
	}
	client_num = atoi(argv[1]);
	if(client_num == 0)
	{
		printf("%sUSE: ./server <Clients number (integer)>%s\n", YELLOW, RESET);
		return 1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket");
		exit(1);
	}

	/*Asignacion de valores al tipo de socket*/
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr("192.168.1.39");
	my_addr.sin_port = htons(4321);
	memset(&(my_addr.sin_zero), '\0', 8);
	/*Funcion bind*/
	if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	/*Se pone a escuchar*/
	if(listen(sockfd, 5) == -1)
	{
		perror("listen");
		exit(1);
	}

	printf("%sStarting server...\n", YELLOW);
	printf("Server IP -> %s%s%s\n", BLUE, inet_ntoa(my_addr.sin_addr), RESET);

	addrlen = sizeof(struct sockaddr);
	for(i =0; i < client_num - 1; i++)
	{
		fork();
	}
	while(1)
	{
		new_sockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &addrlen);
		if(new_sockfd == -1)
		{
			perror("accept");
			exit(1);
		}
		printf("%sNew client: %s%s%s\n", GREEN, BLUE, inet_ntoa(remote_addr.sin_addr), RESET);
	
		while(1)
		{
			bytes_readed = recv(new_sockfd, (void *) &mens_serv, 100, 0);
			if(bytes_readed == 0)
			{
				printf("%s[%s] exit%s\n", RED, inet_ntoa(remote_addr.sin_addr), RESET);
				break;
			}
			printf("%s%s%s -> %s%s", BLUE, inet_ntoa(remote_addr.sin_addr), YELLOW, RESET, mens_serv);
			send(new_sockfd, "Server answer", 100, 0);
		}
		close(new_sockfd);
	}

	printf("Closing server...\n");
	return 0;
}