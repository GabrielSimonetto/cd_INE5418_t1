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



void initialize_server_database(char* server_database) {

	// Inicializar com um caractere facil de ver      
	for (int i = 0; i < SERVER_SIZE; i++) {
		server_database[i] = 'T';
	}

	// Colocar uns caracteres pra diferenciar as operações
	server_database[1] = '1';
	server_database[3] = '3';
	server_database[5] = '5';

	// Printar arrays nao funciona muito bem sem isso,
	//   nós não temos nenhuma proteção contra um cliente sobreescrever isso, 
	//   e a gente ainda nao vai fazer nada a respeito disso.
	server_database[SERVER_SIZE-1] = '\0'; 
}

void escrever_server_procedure(int client_sockfd, char* server_database){
	char starting_position_arg[100];
	char message[100];
	char size_arg[100];
	int starting_position, size;
	
	read(client_sockfd, &starting_position_arg, 100);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &message, 100);
	printf("\n starting_position_arg: %s\n", message);
	fflush(stdout);

	read(client_sockfd, &size_arg, 100);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);
	
	starting_position = atoi(starting_position_arg);
	size = atoi(size_arg);

	for(int i=0; i < size; i++) {
		server_database[i+starting_position] = message[i];
	}

}

void ler_server_procedure(int client_sockfd, char* server_database) {
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

	for(int i=0; i < size; i++) {
		printf("%d\n", i);
		fflush(stdout);
		output[i] = server_database[i+starting_position];
		printf("%d\n\n", output[i]);
		fflush(stdout);
	}

	printf("\n server_database: %s\n", server_database);
	fflush(stdout);

	printf("\n output: %s\n", output);
	fflush(stdout);

	write(client_sockfd, &output, SERVER_SIZE);
}

int main()
{
    time_t clock;
	char dataSending[MESSAGE_SIZE] = "";
	int server_sockfd = 0;
	int client_sockfd = 0;
    char command[MESSAGE_SIZE] = "";
	char server_database[SERVER_SIZE];

	initialize_server_database(server_database);

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

	/* Operações pós conexão */         
	while(1)
	{
		printf("\nAguardando comando do cliente...\n");
		fflush(stdout);
        read(client_sockfd, &command, MESSAGE_SIZE);

		printf("\nRecebemos: %s\n", command);
		fflush(stdout);

		if (strcmp(command, "escrever") == 0) {
			escrever_server_procedure(client_sockfd, server_database);
			printf("todo");
		} else if (strcmp(command, "ler") == 0) {
			ler_server_procedure(client_sockfd, server_database);
		} else if (strcmp(command, "sair") == 0){
            break;
		} else {
			printf("\nComando nao reconhecido, aguardando novo comando.\n");
		}
    }
 	close(client_sockfd);
 
    return 0;
}