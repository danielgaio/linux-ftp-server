#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 1024

int start_server(int port);
void lb(char *buffer);
int serverPorta(int port);
int connectPORT(char  argumento[128]);