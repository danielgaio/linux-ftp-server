// gcc main.cpp header.h server.c -o main

#include <netdb.h> // sockets
#include <cstdlib>
#include "header.h"

int main(){
    int new_socket;
    char *hello = (char*)"ola";
    hello = (char*)malloc(sizeof(char)*5);
    new_socket = start_server(21);
    char *buffer = (char*)malloc(sizeof(char)*1024);
    char bufferDividido;
    //bufferDividido = (char*)malloc(sizeof(char)*50);
    int variavel = 1;
    int ordem = 0;
    char *vetor;
    vetor = (char*)malloc(sizeof(char)*50);
    if(new_socket== 0){
        perror("Erro ao atender cliente!\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Servidor atendeu cliente com sucesso!\n");
        while (variavel == 1){
          enviarValor(new_socket,"220");
          buffer=pegaValor(new_socket, buffer);
          printf("%s\n",buffer);
          bufferDividido = quebrarString(buffer);
          printf("%s\n", bufferDividido);
          //bufferDividido= strtok(buffer," ");

        //  bufferDividido = strtok(buffer," ");
          //printf("%s\n", &bufferDividido);
        //  printf("%s\n", bufferDividido);





          /*
          bufferDividido = strtok (buffer," ");
          printf("%s\n", bufferDividido);

        /*  if(ordem = 0 && strcmp(bufferDividido, "USER") == 0 && strcmp(bufferDividido, "anonymous") == 0){
                enviarValor(new_socket,"331");
                buffer = pegaValor(new_socket);
                bufferDividido = strtok(buffer, " ");
                ordem = 1;
            }else if(ordem = 1 && strcmp(bufferDividido, "PASS") == 0 && strcmp(bufferDividido, "") == 0 ){
                enviarValor(new_socket, "230");
                buffer = pegaValor(new_socket);
                bufferDividido = strtok (buffer," ");
                ordem = 2;
            }else if(ordem = 2 &&  strcmp(bufferDividido, "PASS") == 0 &&  strcmp(bufferDividido, "") == 0){
                enviarValor(new_socket, "331");
                buffer = pegaValor(new_socket);
                bufferDividido = strtok(buffer, " ");
                ordem = 2;
            }
*/
          variavel = 2;
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

    }

    /*
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
    */

    return 0;
}
