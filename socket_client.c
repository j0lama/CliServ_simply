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

int main(int argc, char const *argv[])
{
	int sockfd;
	char mensaje[100], respuesta[100];
	int conection;
	/*Socket*/
	struct sockaddr_in serv_addr;
	/*Asignacion de valores al tipo de socket del servidor*/
	if(argc != 2)
	{
		printf("%sUSE: ./client <Server IP>%s\n", YELLOW, RESET);
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4321);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(&(serv_addr.sin_zero), '\0', 8);
	/*Creacion del socket*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	conection = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
	if(conection == -1)
	{
		printf("%sError conecting to %s%s\n", RED, argv[1], RESET);
		return 1;
	}
	printf("%sConnected to: %s%s%s\n", GREEN, BLUE, argv[1], RESET);
	do
	{
		printf("%sWrite a messaje: %s", YELLOW, RESET);
		fgets(mensaje, 100, stdin);
		if(strcmp(mensaje, "Salir\n") == 0)
		{
			printf("%sServer disconnected.%s\n",RED, RESET);
			break;
		}
		send(sockfd, mensaje, 100, 0);
		recv(sockfd, (void *) &respuesta, 100, 0);
		printf("%s%s %s(Server) -> %s%s\n", BLUE,inet_ntoa(serv_addr.sin_addr), YELLOW, RESET, respuesta);
	}while(strcmp(mensaje, "Salir") != 0);
	close(sockfd);
	
	return 0;
}