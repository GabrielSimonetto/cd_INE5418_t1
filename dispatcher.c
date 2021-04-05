#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
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

///////////////// DISPATCHER AS ClIENT TO SERVER /////////////////

void escrever_dispatcher_to_server_procedure(int server_sockfd, char* starting_position, char* message, char* size, char* command) {
	// TODO: Atualizar lista de servers depois

    // send command
    write(server_sockfd, &command, MESSAGE_SIZE);

    // send starting position
    write(server_sockfd, &starting_position, MESSAGE_SIZE);

    // send message
    write(server_sockfd, &message, MESSAGE_SIZE);

    // send size
    write(server_sockfd, &size, MESSAGE_SIZE);
}

void ler_dispatcher_to_server_procedure(int server_sockfd, char* starting_position, char* size, char* partial_server_output, char* command) {
    // send command
	printf("\nler_dispatcher_to_server_procedure: RECEBEMOS COMANDO %s\n", command);
    write(server_sockfd, &command, MESSAGE_SIZE);

    // send starting position
    write(server_sockfd, &starting_position, MESSAGE_SIZE);

    // send size
    write(server_sockfd, &size, MESSAGE_SIZE);

    // read results
    // char output[SERVER_SIZE_TOTAL] = "";
	// read(server_sockfd, &output, SERVER_SIZE_TOTAL);
    // printf("\n OUTPUT: %s\n", output);
	read(server_sockfd, &partial_server_output, SERVER_SIZE_TOTAL);
    printf("\n OUTPUT: %s\n", partial_server_output);
    fflush(stdout);
}


////////////////// DISPATCHER AS SERVER TO CLIENT //////////////////


void escrever_client_to_dispatcher_procedure(int client_sockfd, int server_sockfd, char* command){
	char starting_position_arg[MESSAGE_SIZE];
	// If we are to write all over the server, this needs SERVER_SIZE_TOTAL
	char message[SERVER_SIZE_TOTAL];
	char size_arg[MESSAGE_SIZE];
	
	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &message, SERVER_SIZE_TOTAL);
	printf("\n starting_position_arg: %s\n", message);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);
	
	// TODO: Atualizar lista de servers depois
	escrever_dispatcher_to_server_procedure(server_sockfd, starting_position_arg, message, size_arg, command);
}


void ler_client_to_dispatcher_procedure(int client_sockfd, int server_sockfd, char* command) {
	char starting_position_arg[MESSAGE_SIZE] = "";
	char size_arg[MESSAGE_SIZE] = "";
	// If we are to concatenate all answers, this needs SERVER_SIZE_TOTAL
	char output[SERVER_SIZE_TOTAL] = "";

	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);

	// LOGICA DE REPASSAMENTO DE QUERIES PRA CADA SERVER //
	// TUDO ISSO AQUI VAI ENTRAR NUM FOR QUANDO HOUVEREM MULTIPLOS SERVER //

	// this one however only needs the server's capacity (:
	char partial_server_output[SERVER_SIZE_PER_PARTITION];
	ler_dispatcher_to_server_procedure(server_sockfd, starting_position_arg, size_arg, partial_server_output, command);

	// update our final answer
	// this makes more sense inside the for of many servers.
	int size = atoi(size_arg);
	int partial_size = size;
	int output_offset = 0; // stores how much we have already written on output
						// e.g. first_server --> estestos | output_offset = 8
						//		second server --> ...copio! | start writing on 8!
						// estetoscopio tem 13 caracteres, o segundo server vai receber 5 (ou size-8)
	for(int i=0; i < partial_size; i++) {
		output[i+output_offset] = partial_server_output[i];
	}

	printf("\n output: %s\n", output);
	fflush(stdout);

	// giving the results to client
	write(client_sockfd, &output, SERVER_SIZE_PER_PARTITION);
}

int main()
{
	int client_sockfd = 0;
	int dispatcher_sockfd = 0;
	// TODO: Atualizar lista de servers depois
	int servers_sockfd = 0;
    char command[MESSAGE_SIZE] = "";

	/* Inicialização de sockets server */         
	struct sockaddr_un dispatcher_address; 
	struct sockaddr_un servers_address;
	dispatcher_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	dispatcher_address.sun_family = AF_UNIX;
	strcpy(dispatcher_address.sun_path, "server_socket");
	bind(dispatcher_sockfd, (struct sockaddr *) &dispatcher_address, sizeof(dispatcher_address));
	listen(dispatcher_sockfd, 5);

	/* Esperando um unico SERVER uma unica vez */         
	printf("\nDispatcher rodando, aguardando conexões de servers\n");
	fflush(stdout);
	int servers_len = sizeof(servers_address);
	servers_sockfd = accept(dispatcher_sockfd, (struct sockaddr *)&servers_address, &servers_len);
	char hello = "h";
	write(servers_sockfd, &hello, 1);
	printf("\nServidores conectados\n");
	fflush(stdout);

	/* Inicialização de sockets clientes */         
	struct sockaddr_in address;
	dispatcher_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = 9734;	
	bind(dispatcher_sockfd, (struct sockaddr*)&address , sizeof(address));
	listen(dispatcher_sockfd , 20);

	/* Esperando um unico cliente uma unica vez */         
	printf("\nDispatcher rodando, aguardando contato com cliente\n");
	fflush(stdout);
	client_sockfd = accept(dispatcher_sockfd, (struct sockaddr*)NULL, NULL);
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
			// TODO: Atualizar lista de servers depois
			escrever_client_to_dispatcher_procedure(client_sockfd, servers_sockfd, command);
		} else if (strcmp(command, "ler") == 0) {
			// TODO: Atualizar lista de servers depois
			ler_client_to_dispatcher_procedure(client_sockfd, servers_sockfd, command);
		} else if (strcmp(command, "sair") == 0){
            break;
		} else {
			printf("\nComando nao reconhecido, aguardando novo comando.\n");
		}
    }
 	close(client_sockfd);
 
    return 0;
}