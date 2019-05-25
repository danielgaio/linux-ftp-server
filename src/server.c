#include "header.h"

int start_server(int port) {
	// server_connection_socket é o que vai atender
	int server_listen_socket, server_connection_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer_entrada[BUFFER_SIZE], buffer_saida[BUFFER_SIZE];
  	char **bufferDividido;
	int ordem = 0;

	// Creating socket file descriptor
	// socket - create an endpoint for communication
	// int socket(int domain, int type, int protocol);
	// AF_INET             IPv4 Internet protocols
	// SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based
	//                     byte  streams.  An out-of-band data transmission mecha‐
	//                     nism may be supported.
	if ((server_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("A criacao do socket de escuta falhou\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Socket de escuta criado\n");
	}

	//int reuse = 1;
	/* Address can be reused instantly after program exits */
  	//setsockopt(server_listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	// The htons() function converts the unsigned short integer hostshort from
	//     host byte order to network byte order.
	address.sin_port = htons( port );

	// int bind(int sockfd, const struct sockaddr *addr,
	//              socklen_t addrlen);
	if (bind(server_listen_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
		perror("bind failed\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Bind executado com sucesso\n");
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
	if (listen(server_listen_socket, 3) == -1) {
		perror("listen\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Comando Listen() executado com sucesso\n");
	}

	if ((server_connection_socket = accept(server_listen_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
		perror("accept\n");
		exit(EXIT_FAILURE);
	}else{
		printf("Comando accept() executado\n");
	}

	char msg_bem_vindo[BUFFER_SIZE]={0};

	strcat(msg_bem_vindo, "220 Bem vindo ao servidor FTP\n");
	write(server_connection_socket, msg_bem_vindo, strlen(msg_bem_vindo));
	printf("Mensagem de boas vindas enviada\n");
	read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
	printf("Msg do cliente: %s", buffer_entrada);
	strcat(buffer_saida, "331 Nome de usuario okay, preciso da senha\n");
	printf("buffer_saida: %s\n", buffer_saida);
	write(server_connection_socket, buffer_saida, strlen(buffer_saida));
	printf("Pedido de senha enviado\n");

	system("pause");

	printf("Adentrando ao loop\n");
	while (1){
		//bufferDividido = quebrarString(buffer_entrada," ");
		//printf("%s\n", bufferDividido[0]);

		/*if(ordem == 0 && strcmp(bufferDividido[0], "USER") == 0 && strcmp(bufferDividido[1], "anonymous") == 0){
			enviarValor(server_connection_socket, "331");
			//buffer_entrada = pegaValor(server_connection_socket, buffer_entrada);
			ordem = 1;
		}else if(ordem == 1 && strcmp(bufferDividido[0], "PASS") == 0 && strcmp(bufferDividido[1], "") == 0 ){
			enviarValor(server_connection_socket, "230");
			//buffer_entrada = pegaValor(server_connection_socket, buffer_entrada);
			ordem = 2;
		}*/
	}

	return server_connection_socket;
}

//Resposta do cliente
char * pegaValor(int server_connection_socket, char *buffer_entrada){
	//char buffer_entrada[1024] = {0};
	//char *buffer_entrada=(char*)malloc(sizeof(char)*1024);
	int valread = recv(server_connection_socket ,buffer_entrada,0, 1024);
	return buffer_entrada;
}

//Enviar comando
void  enviarValor(int server_connection_socket, char *hello){
	send(server_connection_socket , hello , strlen(hello) , 0 );
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
