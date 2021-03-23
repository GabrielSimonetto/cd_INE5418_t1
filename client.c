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

        // TODO: refatorar socket pra ter o mesmo nome dos 2 lados
        // TODO: refatorar pro makefile usar .o

        // write ler
		write(sockfd, &command, MESSAGE_SIZE);
        if(strcmp(command,"sair")==0){
            break;
        }

        // write inicial
        char input[MESSAGE_SIZE] = "";
        printf("ponto_inicial\n");
        fflush(stdout);
        scanf("%s", input);        
		write(sockfd, &input, MESSAGE_SIZE);

        // write size
        char input2[MESSAGE_SIZE] = "";
        printf("size\n");
        fflush(stdout);
        scanf("%s", input2);        
		write(sockfd, &input2, MESSAGE_SIZE);

        // read resultado
        char output[SERVER_SIZE] = "";
		read(sockfd, &output, SERVER_SIZE);
		printf("\n OUTPUT: %s\n", output);
		fflush(stdout);

        // ON HOLD
        //     eh melhor iniciar primeiro e ai fazer a leitura primeiro :D

    //     // nao sei se eh aqui que resolve tudo isso amiguinho '-'
    //     if (strcmp(command,"escrever") == 0) {
    //         printf("Insira a palavra a ser escrita")
    //         TODO: enfiar uma palavra aqui

    //         word_length = TODO

    //         printf("Insira a posição inicial de inserção entre 0 e %d", SERVER_SIZE-word_length);
    //         // size_t initial_position_argument;
    //         // size_t length_argument;

    // // [ ] - Escreve(Posicao1, &buffer, tam_buffer)
    // // [ ] - Le(Posicao4, tamanho)
    //     } else if (strcmp(command,"ler") == 0) {

    //     } else if (strcmp(command,"sair") == 0) {
    //         printf("Até mais!");
	// 	    fflush(stdout);
    //         break;         
    //     } else {
    //         printf("Comando inválido, por favor tente novamente");
	// 	    fflush(stdout);
    //     }

        // if(strcmp(command,"sair")==0){
        //     break;
        // }
        // write(sockfd, &command, strlen(str)+1);
        // read(sockfd, &dataReceived, MESSAGE_SIZE);
        // printf("%s\n", dataReceived);
		// fflush(stdout);
    }
    
    if( n < 0)
    {
        printf("Erro \n");
    }
 
    return 0;
}