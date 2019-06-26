#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
struct sockaddr_in address;

struct dadosConecao{
  int socketCliente;
  int pasv_listen_socket, data_transfer_socket;    //socket dados
  int port_or_pasv;    // flag para o modo de execução, port = 0, pasv = 1
};

void lb(char *buffer);
int create_pasv_listen_socket(int port);
int aceitar_conexao(int socket);
void str_perm(int perm, char *str_perm);
void comandos(void * dclientes);

int start_server(int port);
