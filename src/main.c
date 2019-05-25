// gcc main.c header.h server.c -o main

#include "header.h"

int main(){
  printf("Server is runnig...\n");

  int server_socket = start_server(9923);

  if(server_socket == 0){
    perror("Erro ao atender cliente!\n");
    exit(EXIT_FAILURE);
  }else{
    printf("Servidor atendeu cliente com sucesso!\n");
  }

  return 0;
}
