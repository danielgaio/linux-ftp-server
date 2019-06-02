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

#define BUFFER_SIZE 1024

int start_server(int port);
void lb(char *buffer);
int create_pasv_listen_socket(int port);
int aceitar_conexao(int socket);
void str_perm(int perm, char *str_perm);