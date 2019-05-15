//Fonte: https://www.geeksforgeeks.org/socket-programming-cc/

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

#include <string.h>
///Bliotecas buscadas pelo koch
#include <arpa/inet.h>
#include <unistd.h>
////

#define PORT 21

int main(int argc, char const *argv[]) {
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from cliente";
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
int variavel=1;
int ordem=0;
	printf("Conectou .....");
//while (variavel==1){
		printf("Conectou .....");
	valread = read( sock , buffer, 1024);
	if(ordem == 0 && valread==220){
			hello = "USER anonymous ";
			send(sock , hello , strlen(hello) , 0 );
			ordem=1;
	}else if(ordem == 1 && valread==331 ){
			hello = "PASS ";
			send(sock , hello , strlen(hello) , 0 );
			ordem=2;
	}else if(ordem == 2 &&  valread==230){
			hello = "Conectou";
			send(sock , hello , strlen(hello) , 0 );
			printf("Conectou");
			}


//}
//	send(sock , hello , strlen(hello) , 0 );
/*
if(ordem == 0 && strcmp(valread, "220") == 0){
		hello = "USER anonymous ";
		send(sock , hello , strlen(hello) , 0 );
		ordem=1;
}else if(ordem == 1 && strcmp(valread, "331") == 0 ){
		hello = "PASS ";
		send(sock , hello , strlen(hello) , 0 );
		ordem=2;
}else if(ordem == 2 &&  strcmp(valread, "230") == 0 ){
		hello = "Conectou";
		send(sock , hello , strlen(hello) , 0 );
		printf("Conectou");
		}


*/
//	valread = read( sock , buffer, 1024);
//	printf("%s\n",buffer );

///Primeiro comando USER




//printf("%s\n",buffer );
	return 0;
}
