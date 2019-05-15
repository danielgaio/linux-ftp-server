#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int start_server(int port);
char *pegaValor(int new_socket, char *buffer);
void enviarValor(int new_socket, char *hello);
char **quebrarString (char *str,char *limitador);
