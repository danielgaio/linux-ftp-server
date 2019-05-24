#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 1024

int start_server(int port);
char *pegaValor(int new_socket, char *buffer);
void enviarValor(int new_socket, char *hello);
char **quebrarString (char *str,char *limitador);