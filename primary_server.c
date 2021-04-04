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

void escrever_operation (int starting_position, int size, char* message, char* server_database) {
	// TODO: this is a separate function that both the primary and secondary need
	for(int i=0; i < size; i++) {
		server_database[i+starting_position] = message[i];
	}
}

void escrever_server_procedure(int client_sockfd, char* server_database){
	char starting_position_arg[MESSAGE_SIZE];
	// If we are to write all over the server, this needs SERVER_SIZE_TOTAL
	char message[SERVER_SIZE_TOTAL];
	char size_arg[MESSAGE_SIZE];
	int starting_position, size;
	
	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &message, SERVER_SIZE_TOTAL);
	printf("\n starting_position_arg: %s\n", message);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);
	
	starting_position = atoi(starting_position_arg);
	size = atoi(size_arg);

	escrever_operation(starting_position, size, message, server_database);
}

void ler_operation (int starting_position, int size, int output_offset, char* output, char* server_database) {
	/* 
	starting_position is needed in cases where we ask for information in the middle of the database
	output_offset is needed in cases where we are visiting a secondary server for the first time
	*/

	for(int i=0; i < size; i++) {
		output[i+output_offset] = server_database[i+starting_position];
	}
}

void ler_server_procedure(int client_sockfd, char* server_database) {
	char starting_position_arg[MESSAGE_SIZE] = "";
	char size_arg[MESSAGE_SIZE] = "";
	int starting_position, size;
	int output_offset = 0;
	// If we are to concatenate all answers, this needs SERVER_SIZE_TOTAL
	char output[SERVER_SIZE_TOTAL] = "";

	read(client_sockfd, &starting_position_arg, MESSAGE_SIZE);
	printf("\n starting_position_arg: %s\n", starting_position_arg);
	fflush(stdout);

	read(client_sockfd, &size_arg, MESSAGE_SIZE);
	printf("\n size_arg: %s\n", size_arg);
	fflush(stdout);

	starting_position = atoi(starting_position_arg);
	size = atoi(size_arg);

	bool entered_a_server_yet = false;
	// cycle through every server understanding which ones are involved
	// isso aqui seria a lista de servers secundarios... se eu tivesse uma lista de servers secundarios...
	// ou eu posso dizer que o i=0 eh o server atual
	// entao se depois eu precisar acessar um server secundário em lista eu faço algo como
	// server_connection[i-1]
	for (int i = 0; i < SERVER_SIZE_PER_PARTITION; i++):
		// checks if current server is involved.
		if starting_position >= SERVER_SIZE_PER_PARTITION {
			starting_position = starting_position - SERVER_SIZE_PER_PARTITION
			continue;
		}

		entered_a_server_yet = true;

		// do stuff
		// ta mas ai eu preciso fazer o ler operation diretamente nesse server
		// mas fazer other_server->ler_operation caso seja uma conexao em outro server.
		// po nem isso, o outro server DE FATO vai precisar ter mecanismos pra ler e escrever
		// assim como esse aqui tem...
		// entao nao faz o menos sentido o que eu tava pensando de só repassar as informações
		// e ser feliz... argh
		// ta acho que eu vou meter tudo isso numa branch secundária e recomeçar a solução
		// olhando primeiro pro server secundário	
		ler_operation (starting_position, size, output_offset, output, server_database);

		// from now on starting_position is always zero on other servers
		starting_position = 0

		// faz uma conta de modulo pra entender se o começo ou o fim do teu range ta no server da vez
		// pra primeira rodada voce passa normalmente starting position e size

		// pras proximas iterações vc faz algo como
		// if starting+size < SERVER_SIZE_PER_PARTITION: break
		// starting_position = 0

		// isso soh eh criado se break nao acontecer.
		// 1) total_obtained = total_obtained + (SERVER_SIZE_PER_PARTITION - starting_position)
		// preciso inicializar isso fora da função em zero se for o caso.
		// alternativamente eu posso dar o break dps de calcular "total_obtained"
		// pq "total_obtained" na verdade mostra o maximo que poderia ser pego entao se ele
		// passar de size entao da pra dar break aqui tambem.
		
		// message_offset = total_obtained_yet
		// size = size - total_obtained_yet

		// nao pode mais passar o size total, senao vc estoura o server,

	// Also, eu consigo fazer um for sequencial justamente aqui pra passar por todos os servers (:

	printf("\n output: %s\n", output);
	fflush(stdout);

	write(client_sockfd, &output, SERVER_SIZE_PER_PARTITION);
}

int main()
{
	char dataSending[MESSAGE_SIZE] = "";
	int primary_server_sockfd = 0;
	int secondary_server_sockfd = 0;
	int client_sockfd = 0;
    char command[MESSAGE_SIZE] = "";
	char server_database[SERVER_SIZE_PER_PARTITION];

	initialize_server_database(server_database);

	/* Inicialização de sockets */         
	struct sockaddr_in address;
	primary_server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = 9734;	
	bind(primary_server_sockfd, (struct sockaddr*)&address , sizeof(address));
	listen(primary_server_sockfd , 20);

	// /* Esperando um unico SERVER uma unica vez */         
	// printf("\nServer rodando, aguardando conexões de servers secundários\n");
	// fflush(stdout);
	// secondary_server_sockfd = accept(primary_server_sockfd, (struct sockaddr*)NULL, NULL);
	// printf("\nServidores secundários conectados\n");
	// fflush(stdout);

	/* Esperando um unico cliente uma unica vez */         
	printf("\nServer rodando, aguardando contato com cliente\n");
	fflush(stdout);
	client_sockfd = accept(primary_server_sockfd, (struct sockaddr*)NULL, NULL);
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