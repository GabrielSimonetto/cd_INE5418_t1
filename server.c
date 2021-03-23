#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <time.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

#include "config.h"


int main()
{
    time_t clock;
	char dataSending[MESSAGE_SIZE];
	int server_sockfd = 0;
	int client_sockfd = 0;
    char str[MESSAGE_SIZE];

	struct sockaddr_in address;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = 9734;	
	bind(server_sockfd, (struct sockaddr*)&address , sizeof(address));
	listen(server_sockfd , 20);
 
	printf("\nServer rodando, aguardando contato com cliente\n");
	fflush(stdout);
	client_sockfd = accept(server_sockfd, (struct sockaddr*)NULL, NULL);
	printf("\nCliente conectado\n");
	fflush(stdout);
	while(1)
	{
        read(client_sockfd, &str, MESSAGE_SIZE);
		// printf("%s\n", str);
		// printf("%d\n", strlen(str)+1);
		printf("\nRecebemos: %s\n", str);
		fflush(stdout);

		if(strcmp(str,"sair")==0){
            break;
        }
 
		write(client_sockfd, &str, strlen(str)+1);
        sleep(1);
     }
	close(client_sockfd);
 
     return 0;
}