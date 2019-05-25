#include "header.h"

int start_server(int port) {
	// server_connection_socket é o que vai atender
	int server_listen_socket, server_connection_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer_entrada[BUFFER_SIZE], buffer_saida[BUFFER_SIZE];
	//int ordem = 0;
	//char **bufferDividido;
	char comando[8], argumento[128];

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

<<<<<<< HEAD
	char msg_bem_vindo[BUFFER_SIZE]={0};

=======
	// =================== Inicialização conexão =====================
	char msg_bem_vindo[BUFFER_SIZE];
>>>>>>> b1b847883b56d8288050c8e74342ba2a6c761935
	strcat(msg_bem_vindo, "220 Bem vindo ao servidor FTP\n");

	write(server_connection_socket, msg_bem_vindo, strlen(msg_bem_vindo));
	printf("Mensagem de boas vindas enviada\n");

	read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
	printf("Msg do cliente: %s", buffer_entrada);

	memset(buffer_saida, 0, sizeof buffer_saida);
	strcat(buffer_saida, "331 Nome de usuario okay, preciso da senha\n");
	printf("buffer_saida: %s", buffer_saida);
	write(server_connection_socket, buffer_saida, strlen(buffer_saida));
	printf("Pedido de senha enviado\n");

	memset(&buffer_entrada, 0, sizeof buffer_entrada);
	read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
	printf("Senha do usuario: %s\n", buffer_entrada);

	memset(buffer_saida, 0, sizeof buffer_saida);
	strcat(buffer_saida, "230 Usuario logado\n");
	printf("buffer_saida: %s", buffer_saida);
	write(server_connection_socket, buffer_saida, strlen(buffer_saida));

	memset(&buffer_entrada, 0, sizeof buffer_entrada);
	read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
	printf("Proximo comando: %s\n", buffer_entrada);

	memset(buffer_saida, 0, sizeof buffer_saida);
	strcat(buffer_saida, "215 UNIX Type: L8\n");
	printf("buffer_saida: %s", buffer_saida);
	write(server_connection_socket, buffer_saida, strlen(buffer_saida));
	// =================== Inicialização conexão =====================

	printf("Adentrando ao loop\n");
	memset(&buffer_entrada, 0, sizeof buffer_entrada);

	while (read(server_connection_socket, buffer_entrada, BUFFER_SIZE)){

		sscanf(buffer_entrada,"%s %s", comando, argumento);

		printf("Comando: %s - Argumento: %s\n", comando, argumento);

		memset(&buffer_entrada, 0, sizeof buffer_entrada);
	}

<<<<<<< HEAD
	return ponteiro;
}
=======
	return server_connection_socket;
}

/*
COMANDOS:
USER <SP> <username> <CRLF>
PASS <SP> <password> <CRLF>
ACCT <SP> <account-information> <CRLF>
CWD  <SP> <pathname> <CRLF>
CDUP <CRLF>
SMNT <SP> <pathname> <CRLF>
QUIT <CRLF>
REIN <CRLF>
PORT <SP> <host-port> <CRLF>
PASV <CRLF>
TYPE <SP> <type-code> <CRLF>
STRU <SP> <structure-code> <CRLF>
MODE <SP> <mode-code> <CRLF>
RETR <SP> <pathname> <CRLF>
STOR <SP> <pathname> <CRLF>
STOU <CRLF>
APPE <SP> <pathname> <CRLF>
ALLO <SP> <decimal-integer>
[<SP> R <SP> <decimal-integer>] <CRLF>
REST <SP> <marker> <CRLF>
RNFR <SP> <pathname> <CRLF>
RNTO <SP> <pathname> <CRLF>
ABOR <CRLF>
DELE <SP> <pathname> <CRLF>
RMD  <SP> <pathname> <CRLF>
MKD  <SP> <pathname> <CRLF>
PWD  <CRLF>
LIST [<SP> <pathname>] <CRLF>
NLST [<SP> <pathname>] <CRLF>
SITE <SP> <string> <CRLF>
SYST <CRLF>
STAT [<SP> <pathname>] <CRLF>
HELP [<SP> <string>] <CRLF>
NOOP <CRLF>

RESPOSTAS:
200 Command okay.
202 Command not implemented, superfluous at this site.
211 System status, or system help reply.
212 Directory status.
213 File status.
214 Help message.
On how to use the server or the meaning of a particular
non-standard command.  This reply is useful only to the
human user.
215 NAME system type.
Where NAME is an official system name from the list in the
Assigned Numbers document.
220 Service ready for new user.
221 Service closing control connection.
Logged out if appropriate.
225 Data connection open; no transfer in progress.
226 Closing data connection.
Requested file action successful (for example, file
transfer or file abort).
227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).
230 User logged in, proceed.
250 Requested file action okay, completed.
257 "PATHNAME" created.

331 User name okay, need password.
332 Need account for login.200 Command okay.
202 Command not implemented, superfluous at this site.
211 System status, or system help reply.
212 Directory status.
213 File status.
214 Help message.
On how to use the server or the meaning of a particular
non-standard command.  This reply is useful only to the
human user.
215 NAME system type.
Where NAME is an official system name from the list in the
Assigned Numbers document.
220 Service ready for new user.
221 Service closing control connection.
Logged out if appropriate.
225 Data connection open; no transfer in progress.
226 Closing data connection.
Requested file action successful (for example, file
transfer or file abort).
227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).
230 User logged in, proceed.
250 Requested file action okay, completed.
257 "PATHNAME" created.

331 User name okay, need password.
332 Need account for login.
350 Requested file action pending further information.

421 Service not available, closing control connection.
This may be a reply to any command if the service knows it
must shut down.
425 Can't open data connection.
426 Connection closed; transfer aborted.
450 Requested file action not taken.
File unavailable (e.g., file busy).
451 Requested action aborted: local error in processing.
452 Requested action not taken.
Insufficient storage space in system.
350 Requested file action pending further information.

421 Service not available, closing control connection.
This may be a reply to any command if the service knows it
must shut down.
425 Can't open data connection.
426 Connection closed; transfer aborted.
450 Requested file action not taken.
File unavailable (e.g., file busy).
451 Requested action aborted: local error in processing.
452 Requested action not taken.
Insufficient storage space in system.
*/
>>>>>>> b1b847883b56d8288050c8e74342ba2a6c761935
