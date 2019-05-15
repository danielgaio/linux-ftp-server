//Fonte: https://www.geeksforgeeks.org/socket-programming-cc/

// Server side C/C++ program to demonstrate Socket programming
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "header.h"

int start_server(int port) {
	// server_fd é o socket de escuta, new socket é o que vai atender
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char *buffer = (char*)malloc(sizeof(char)*1024);
  char **bufferDividido;
	int ordem = 0;
	//char buffer[1024] = {0};

	// Creating socket file descriptor
	// socket - create an endpoint for communication
	// int socket(int domain, int type, int protocol);
	// AF_INET             IPv4 Internet protocols
	// SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based
	//                     byte  streams.  An out-of-band data transmission mecha‐
	//                     nism may be supported.
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 21
	// int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	// The htons() function converts the unsigned short integer hostshort from
	//     host byte order to network byte order.
	address.sin_port = htons( port ); // port = 21

	// Forcefully attaching socket to the port 21
	// int bind(int sockfd, const struct sockaddr *addr,
	//              socklen_t addrlen);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// int listen(int sockfd, int backlog);
	/*The  sockfd  argument  is  a file descriptor that refers to a socket of
	type SOCK_STREAM or SOCK_SEQPACKET.

	The backlog argument defines the maximum length to which the  queue  of
	pending  connections  for sockfd may grow.  If a connection request ar‐
	rives when the queue is full, the client may receive an error  with  an
	indication  of ECONNREFUSED or, if the underlying protocol supports re‐
	transmission, the request may be ignored so that a later  reattempt  at
	connection succeeds.*/
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/*
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}*/



	while (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)){
		enviarValor(new_socket, "220");
		//send(new_socket , "220" , strlen("220") , 0 );
		//send(new_socket , "220" , strlen("220") , 0 );
		//enviarValor(new_socket, "331 Please specify your password.");
		buffer = pegaValor(new_socket, buffer);
		printf("%s\n", buffer);
		bufferDividido = quebrarString(buffer," ");
		printf("%s\n", bufferDividido[0]);

		if(ordem == 0 && strcmp(bufferDividido[0], "USER") == 0 && strcmp(bufferDividido[1], "anonymous") == 0){
			enviarValor(new_socket, "331");
			buffer = pegaValor(new_socket, buffer);
			ordem = 1;
		}else if(ordem == 1 && strcmp(bufferDividido[0], "PASS") == 0 && strcmp(bufferDividido[1], "") == 0 ){
			enviarValor(new_socket, "230");
			buffer = pegaValor(new_socket, buffer);
			ordem = 2;
		}
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
	int valread = recv(new_socket ,buffer,0, 1024);
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
