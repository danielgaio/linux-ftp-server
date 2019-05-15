#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <bits/stdc++.h>
//#include <boost/algorithm/string.hpp>
int start_server();
char *pegaValor(int new_socket, char *buffer);
void enviarValor(int new_socket, char *hello);
char* quebrarString (char *str);
int qtd_espacos(char *str);
