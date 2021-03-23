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
	char dataSending[MESSAGE_SIZE] = "";
	int server_sockfd = 0;
	int client_sockfd = 0;
    char str[MESSAGE_SIZE] = "";
	char server_database[SERVER_SIZE];

	/* Inicializar server_database */         
	for (int i = 0; i < SERVER_SIZE; i++) {
		server_database[i] = 'T';
	}

	// to be removed -- soh pra checar a leitura dps
	server_database[1] = '1';
	server_database[3] = '3';
	server_database[5] = '5';



	/* Inicialização de sockets */         
	struct sockaddr_in address;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = 9734;	
	bind(server_sockfd, (struct sockaddr*)&address , sizeof(address));
	listen(server_sockfd , 20);
 
	/* Esperando um unico cliente uma unica vez */         
	printf("\nServer rodando, aguardando contato com cliente\n");
	fflush(stdout);
	client_sockfd = accept(server_sockfd, (struct sockaddr*)NULL, NULL);
	printf("\nCliente conectado\n");
	fflush(stdout);
	while(1)
	{
		/* Operações pós conexão */         

		// assumindo "ler" e "sair"
		// 

        read(client_sockfd, &str, MESSAGE_SIZE);
		// printf("%s\n", str);
		// printf("%d\n", strlen(str)+1);
		printf("\nRecebemos: %s\n", str);
		fflush(stdout);

		if(strcmp(str,"sair")==0){
            break;
		}

		// [ ] - Le(Posicao4, tamanho)
		char starting_position_arg[100] = "";
		char size_arg[100] = "";
		int starting_position, size;

		read(client_sockfd, &starting_position_arg, 100);
		printf("\n starting_position_arg: %s\n", starting_position_arg);
		fflush(stdout);

		read(client_sockfd, &size_arg, 100);
		printf("\n size_arg: %s\n", size_arg);
		fflush(stdout);

		starting_position = atoi(starting_position_arg);
        size = atoi(size_arg);

		char output[SERVER_SIZE] = "";

		for(int i=starting_position; i < (starting_position+size); i++) {
			printf("%d\n", i);
			fflush(stdout);
			output[i-starting_position] = server_database[i];
			printf("%d\n\n", output[i-starting_position]);
			fflush(stdout);
		}

		printf("\n server_database: %s\n", server_database);
		fflush(stdout);

		printf("\n output: %s\n", output);
		fflush(stdout);

		write(client_sockfd, &output, SERVER_SIZE);
        sleep(1);
     }
 	 close(client_sockfd);
 
     return 0;
}