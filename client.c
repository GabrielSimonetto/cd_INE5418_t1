#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "config.h"


void ler_client_procedure(int sockfd) {
    // send starting position
    char starting_position[MESSAGE_SIZE] = "";
    printf("starting position: \n");
    fflush(stdout);
    scanf("%s", starting_position);        
    write(sockfd, &starting_position, MESSAGE_SIZE);

    // send size
    char size[MESSAGE_SIZE] = "";
    printf("size: \n");
    fflush(stdout);
    scanf("%s", size);        
    write(sockfd, &size, MESSAGE_SIZE);

    // read resultado
    char output[SERVER_SIZE] = "";
    read(sockfd, &output, SERVER_SIZE);
    printf("\n OUTPUT: %s\n", output);
    fflush(stdout);
}


int main()
{
    int sockfd = 0;
    int n = 0;
    char dataReceived[MESSAGE_SIZE] = "";
    struct sockaddr_in address;
    char command[MESSAGE_SIZE] = "";
    size_t initial_position_argument;
    size_t length_argument;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("O socket nao pode ser criado \n");
        return 1;
    }
 
    address.sin_family = AF_INET;
    address.sin_port = 9734;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("Conexão falhou, confira se o server foi iniciado\n");
        return 1;
    }

    while (1)
    {
        printf("Entre com um dos comandos a seguir:\n \t\tescrever\n\t\tler\n\t\tsair\n");
        fflush(stdout);
        scanf("%s", command);
        // send command to server
		write(sockfd, &command, MESSAGE_SIZE);

		if (strcmp(command, "escrever") == 0) {
			// escrever_client_procedure();
			printf("todo");
		} else if (strcmp(command, "ler") == 0) {
			ler_client_procedure(sockfd);
		} else if (strcmp(command, "sair") == 0){
            break;
		} else {
			printf("\nComando nao reconhecido, aguardando novo comando.\n");
		}
    }
    
    if( n < 0)
    {
        printf("Erro \n");
    }
 
    return 0;
}