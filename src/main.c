#include <stdio.h>
#include <netdb.h> // sockets

int main(){

    int server_socket, conecta;

    struct sockaddr_in alvo; // sockaddr_in vem de netdb.h

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    alvo.sin_family = AF_INET;
    alvo.sin_port = htons(8080);
    alvo.sin_addr.s_addr = inet_addr("192.168.0.1"); // ip roteador de casa daniel gaio

    conecta = connect(server_socket, (struct sockaddr *)&alvo, sizeof alvo);

    if(conecta == 0){
        printf("Porta aberta\n");
        close(server_socket);
        close(conecta);
    }else{
        printf("Porta fechada\n");
    }

    return 0;
}