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
 
int main()
{
    int sockfd = 0;
    int n = 0;
    char dataReceived[100];
    struct sockaddr_in address;
    char str[100];
 

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
        scanf("%s", str);
        printf("%s\n", str);
        printf("%d\n", strlen(str)+1);
        if(strcmp(str,"sair")==0){
            break;
        }
        write(sockfd, &str, strlen(str)+1);
        read(sockfd, &dataReceived, 100);
        // printf("%s\n", dataReceived);    
    }
    
    // read(sockfd, &dataReceived, 1);
    // printf("Caractere recebido pelo server: %c\n", dataReceived);

    if( n < 0)
    {
        printf("Erro \n");
    }
 
    return 0;
}