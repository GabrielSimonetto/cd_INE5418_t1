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
#include <sys/un.h>

#include "config.h"



void initialize_server_database(char* server_database) {

	// Inicializar com um caractere facil de ver      
	for (int i = 0; i < SERVER_SIZE_PER_PARTITION; i++) {
		server_database[i] = 'T';
	}

	// Colocar uns caracteres pra diferenciar as operações
	server_database[1] = '1';
	server_database[3] = '3';
	server_database[5] = '5';

	// Printar arrays nao funciona muito bem sem isso,
	//   nós não temos nenhuma proteção contra um cliente sobreescrever isso, 
	//   e a gente ainda nao vai fazer nada a respeito disso.
	server_database[SERVER_SIZE_PER_PARTITION-1] = '\0'; 
}

void escrever_server_procedure(int client_sockfd, char* server_database){
	char starting_position_arg[MESSAGE_SIZE];
	char message[MESSAGE_SIZE];
	char size_arg[MESSAGE_SIZE];
	int starting_position, size;
	
	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &message, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", message);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);
	
	starting_position = atoi(starting_position_arg);
	size = atoi(size_arg);

	for(int i=0; i < size; i++) {
		server_database[i+starting_position] = message[i];
	}
}

void ler_server_procedure(int client_sockfd, char* server_database) {
	char starting_position_arg[MESSAGE_SIZE] = "";
	char size_arg[MESSAGE_SIZE] = "";
	int starting_position, size;

	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);

	starting_position = atoi(starting_position_arg);
	size = atoi(size_arg);

	char output[SERVER_SIZE_PER_PARTITION] = "";

	for(int i=0; i < size; i++) {
		output[i] = server_database[i+starting_position];
	}

	printf("\n server_database: %s\n", server_database);
	fflush(stdout);

	printf("\n output: %s\n", output);
	fflush(stdout);

	write(client_sockfd, &output, SERVER_SIZE_PER_PARTITION);
}

int main()
{
    char command[MESSAGE_SIZE] = "";
	char server_database[SERVER_SIZE_PER_PARTITION];

	initialize_server_database(server_database);

	/* Tentando se conectar com o dispatcher*/         
	int dispatcher_fd = 0;
	struct sockaddr_un address;

	if ((dispatcher_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        printf("O socket nao pode ser criado \n");
        return 1;
    }

    address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	if (connect(dispatcher_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("Conexão falhou, confira se o dispatcher foi iniciado\n");
        return 1;
    }
	printf("\nDispatcher conectado number: %d\n", dispatcher_fd);
	char hello = "";
	read(dispatcher_fd, &hello, 1);
	printf("\nRecebido a mensagem: %c\n", hello);
	fflush(stdout);

	/* Operações pós conexão */         
	while(1)
	{
		printf("\nAguardando comando do cliente...\n");
		fflush(stdout);
		printf("\nCommand before reading from dispatcher: %s\n", command);
		fflush(stdout);
        read(dispatcher_fd, &command, MESSAGE_SIZE);

		printf("\nCommand AFTER reading from dispatcher: %s\n", command);
		fflush(stdout);

		if (strcmp(command, "escrever") == 0) {
			escrever_server_procedure(dispatcher_fd, server_database);
		} else if (strcmp(command, "ler") == 0) {
			ler_server_procedure(dispatcher_fd, server_database);
		} else {
			printf("\nComando nao reconhecido, aguardando novo comando.\n");
		}
    }
 
    return 0;
}