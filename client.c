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
    char dataReceived[MESSAGE_SIZE];
    struct sockaddr_in address;
    char str[MESSAGE_SIZE];
 

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
        printf("\nÓtimo! Agora me envie algo para ser ecoado\n");
        fflush(stdout);
        scanf("%s", str);
        if(strcmp(str,"sair")==0){
            break;
        }
        write(sockfd, &str, strlen(str)+1);
        read(sockfd, &dataReceived, MESSAGE_SIZE);
        printf("%s\n", dataReceived);
		fflush(stdout);
    }
    
    if( n < 0)
    {
        printf("Erro \n");
    }
 
    return 0;
}