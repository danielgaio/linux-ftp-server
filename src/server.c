//Fonte: https://www.geeksforgeeks.org/socket-programming-cc/

// Server side C/C++ program to demonstrate Socket programming
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "header.h"


int start_server(int port) {
	int server_fd,new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//char buffer[1024] = {0};

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 21
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );

	// Forcefully attaching socket to the port 21
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}


	/// Enviar as respostas
	//	valread = read( new_socket , buffer, 1024);
	//	printf("%s\n",buffer );
	//	send(new_socket , hello , strlen(hello) , 0 );
	//	printf("Hello message sent\n");

	return new_socket;
}
//Resposta do cliente
char * pegaValor(int new_socket, char *buffer){
  //char buffer[1024] = {0};
	//char *buffer=(char*)malloc(sizeof(char)*1024);
	int valread = read(new_socket ,buffer, 1024);
	return buffer;
}
//Enviar comando
void  enviarValor(int new_socket, char *hello){
	send(new_socket , hello , strlen(hello) , 0 );
//	printf("Hello message sent\n");
}

/*int qtd_espacos(char *str){
    int espacos = 0;
    while(str){
        if (strcmp(str," ")==0){
           espacos++;
        }
        str++;
    }
    return espacos;
}

char* quebrarString (char *str){
	int qtd_strings = qtd_espacos(str) + 1, i = 0;

	char *pch = strtok (str," ");

	for (int cont=0;pch != NULL;cont++){
			char *palavra = (char*)malloc(sizeof(char)*1024);
			strcpy(palavra, pch); //copiar cada string para a posição correta
			ponteiro[cont]=palavra;
			pch = strtok (NULL, " ");
	}
	return *ponteiro;
}*/

char **quebrarString (char *str,char *limitador){
    char **ponteiro=(char**)malloc(sizeof(char*)*50);;
	char *ptr = strtok(str,limitador);
	int p = 0;
	while (ptr != NULL){
        ponteiro[p]=ptr;
        p = p + 1;
        ptr = strtok(NULL, limitador);
	 }
	  for (int i=0;i<10;i++){
        if(ponteiro!=NULL){
            printf("%s\n",ponteiro[i]);
        }
    }

    return ponteiro;
}
