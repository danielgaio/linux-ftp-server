
#include "header.h"
#include <errno.h>
#define _GNU_SOURCE
#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#ifndef sendfile
#define BUF_SIZE 8192

// ====================== ENVIO ARQUIVO ========================
// função copiada de outro projeto

ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count){
        off_t orig;
        char buf[BUF_SIZE];
        size_t toRead, numRead, numSent, totSent;

        if (offset != NULL) {
                /* Save current file offset and set offset to value in '*offset' */
                orig = lseek(in_fd, 0, SEEK_CUR);
                if (orig == -1)
                        return -1;
                if (lseek(in_fd, *offset, SEEK_SET) == -1)
                        return -1;
        }

        totSent = 0;
        while (count > 0) {
                toRead = (count < BUF_SIZE) ? count : BUF_SIZE;

                numRead = read(in_fd, buf, toRead);

                if (numRead == -1)
                        return -1;
                if (numRead == 0)
                        break;          /* EOF */

                numSent = write(out_fd, buf, numRead);

                if (numSent == -1)
                        return -1;
                if (numSent == 0) {       /* Should never happen */
                        perror("sendfile: write() transferred 0 bytes");
                        exit(-1);
                }

                count -= numSent;
                totSent += numSent;
        }

        if (offset != NULL) {
                /* Return updated file offset in '*offset', and reset the file offset
                   to the value it had when we were called. */
                *offset = lseek(in_fd, 0, SEEK_CUR);

                if (*offset == -1)
                        return -1;
                if (lseek(in_fd, orig, SEEK_SET) == -1)
                        return -1;
        }
        return totSent;
}
#endif
// ====================== ENVIO ARQUIVO ========================
//=========================Comandos ==========================
void comados(int server_connection_socket){
        if (FD_ISSET( server_connection_socket, &readfds))
        {
                // server_connection_socket é o que vai atender
                int server_listen_socket, server_connection_socket;

                //varias clientes
                int client_socket[30],  max_clients = 30,   activity, i, valread, sd;
                int max_sd;
                //set of socket descriptors
                fd_set readfds;

                int pasv_listen_socket, data_transfer_socket;
                struct sockaddr_in address;
                int addrlen = sizeof(address);
                char buffer_entrada[BUFFER_SIZE], buffer_saida[BUFFER_SIZE];
                char comando[8], argumento[128];
                int port_or_pasv; // flag para o modo de execução, port = 0, pasv = 1


                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read(server_connection_socket, buffer_entrada, BUFFER_SIZE)) == 0)
                {
                        //Somebody disconnected , get his details and print
                        getpeername(server_connection_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                        printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                        //Close the socket and mark as 0 in list for reuse
                        close( server_connection_socket );
                        client_socket[i] = 0;
                }

                //Echo back the message that came in
                else
                {
                        sscanf(buffer_entrada,"%s %s", comando, argumento);
                        printf("Comando: %s - Argumento: %s\n", comando, argumento);

                        //=================================== PORT ====================================
                        if(strcmp (comando, "PORT") == 0) {

                                port_or_pasv = 0; // setando flag para modo port
                                printf("Modo: PORT\n");

                                int ip[3], port[2];
                                sscanf(argumento, "%i,%i,%i,%i,%i,%i", &ip[0], &ip[1], &ip[2], &ip[3], &port[0], &port[1]);
                                int porta = port[0]*256+port[1];
                                char ip_char[16];
                                sprintf(ip_char, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);

                                printf("IP do cliente: %s, Porta: %i\n", ip_char, porta);

                                lb(buffer_saida);
                                strcat(buffer_saida, "200 PORT command successful.\n");
                                printf("buffer_saida: %s\n", buffer_saida);
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));

                                // Agora fazer uma conexão no cliente
                                struct sockaddr_in endereco_cliente;

                                endereco_cliente.sin_family = AF_INET;
                                endereco_cliente.sin_port = htons(porta);
                                endereco_cliente.sin_addr.s_addr = inet_addr(ip_char);

                                if ((data_transfer_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                                        printf("Erro ao criar o socket para executar o comando PORT \n");
                                        return -1;
                                }else{
                                        printf("Socket para transferir dados em modo PORT OK\n");
                                }

                                connect(data_transfer_socket, (struct sockaddr *)&endereco_cliente, sizeof(endereco_cliente));
                                printf("Conectado ao cliente\n");
                                //=================================== PORT ====================================

                                //=================================== PASV ====================================
                        }else if(strcmp (comando, "PASV") == 0) {

                                port_or_pasv = 1;
                                printf("Modo: PASV\n");
                                int ip[4];
                                // gerar porta, p1, p2 é a porta de acordo com "p1*256+p2", 0xff= 255
                                int p1 = 128 + (rand() % 64), p2 = rand() % 0xff; // gera porta aleatoria
                                printf("PASV porta gerada: %i\n", 256*p1+p2);

                                pasv_listen_socket = create_pasv_listen_socket(256*p1+p2); // vai passar um valor de porta calculado
                                printf("Socket de escuta PASV criado: %i\n", pasv_listen_socket);

                                // Resposta
                                lb(buffer_saida);
                                sprintf(buffer_saida, "227 Entrando no modo passivo (%d,%d,%d,%d,%d,%d)\n", ip[0], ip[1], ip[2], ip[3], p1, p2);
                                printf("buffer_saida: %s", buffer_saida);
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));

                                lb(buffer_saida);
                                sprintf(buffer_saida, "150 Estou abrindo o modo ASCII para conexao de dados\n");
                                printf("buffer_saida: %s", buffer_saida);
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                //=================================== PASV ====================================

                                //=================================== LIST ====================================
                        }else if(strcmp(comando, "LIST") == 0) {

                                printf("Vamos executar o comando LIST\n");

                                char endereco_diretorio_atual[BUFFER_SIZE];
                                char buffer_tempo[80];
                                struct dirent *entrada;
                                struct stat status_buffer;
                                time_t tempo_bruto;
                                struct tm *tempo;
                                getcwd(endereco_diretorio_atual, BUFFER_SIZE);
                                DIR *pointer = opendir(endereco_diretorio_atual);

                                if(port_or_pasv == 0) {
                                        printf("LIST: modo PORT reconhecido\n");
                                        lb(buffer_saida);
                                        sprintf(buffer_saida, "150 Estou abrindo o modo ASCII para conexao de dados\n");
                                        printf("buffer_saida: %s", buffer_saida);
                                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                }else{
                                        data_transfer_socket = aceitar_conexao(pasv_listen_socket);
                                        printf("LIST: modo PASV reconhecido\n");
                                }

                                while(entrada = readdir(pointer)) {
                                        if(stat(entrada->d_name, &status_buffer) == -1) {
                                                fprintf(stderr, "FTP: Erro ao ler status de arquivo...\n");
                                        }else{
                                                char *perms = malloc(9);
                                                memset(perms, 0, 9);

                                                tempo_bruto = status_buffer.st_mtime;
                                                tempo = localtime(&tempo_bruto);
                                                strftime(buffer_tempo, 80, "%b %d %H:%M", tempo);
                                                str_perm((status_buffer.st_mode & ALLPERMS), perms);
                                                dprintf(data_transfer_socket,
                                                        "%c%s %5ld %4d %4d %8ld %s %s\r\n",
                                                        (entrada->d_type == DT_DIR) ? 'd' : '-',
                                                        perms, status_buffer.st_nlink,
                                                        status_buffer.st_uid,
                                                        status_buffer.st_gid,
                                                        status_buffer.st_size,
                                                        buffer_tempo,
                                                        entrada->d_name);
                                        }
                                }

                                lb(buffer_saida);
                                sprintf(buffer_saida, "226 Lista de diretorios enviada\n");
                                printf("buffer_saida: %s", buffer_saida);
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));

                                close(data_transfer_socket);
                                close(pasv_listen_socket);
                                //=================================== LIST ====================================

                                //=================================== RETR ====================================
                        }else if(strcmp(comando, "RETR") == 0) {
                                printf("Iniciando execucao de RETR\n");
                                // data_transfer_socket será usado para conexão
                                int fd_arquivo_transferencia;
                                struct stat status_buffer;
                                off_t offset_arquivo = 0;
                                int total_enviado = 0;

                                if(access(argumento, R_OK) == 0 && (fd_arquivo_transferencia = open(argumento, O_RDONLY))) { // checagem de permissão e abertura de arquivo
                                        fstat(fd_arquivo_transferencia, &status_buffer);

                                        if(port_or_pasv == 0) {
                                                printf("RETR: modo PORT reconhecido\n");
                                                lb(buffer_saida);
                                                sprintf(buffer_saida, "150 Estou abrindo o modo ASCII para conexao de dados\n");
                                                printf("buffer_saida: %s", buffer_saida);
                                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                        }else{ // PASV mode
                                                data_transfer_socket = aceitar_conexao(pasv_listen_socket);
                                                printf("RETR: modo PASV reconhecido\n");
                                                close(pasv_listen_socket);
                                        }
                                        lb(buffer_saida);
                                        sprintf(buffer_saida, "125 vou comesar a transferir \n");
                                        printf("buffer_saida: %s", buffer_saida);
                                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));


                                        if(total_enviado = sendfile(data_transfer_socket, fd_arquivo_transferencia, &offset_arquivo, status_buffer.st_size)) {

                                                if(total_enviado != status_buffer.st_size) {
                                                        perror("ftp_retr:sendfile");
                                                        exit(EXIT_SUCCESS);
                                                }

                                                lb(buffer_saida);
                                                sprintf(buffer_saida, "226 Arquivo enviado com sucesso\n");
                                                printf("buffer_saida: %s", buffer_saida);
                                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                        }else{
                                                lb(buffer_saida);
                                                sprintf(buffer_saida, "550 Ocorreu uma falha na leitura do arquivo\n");
                                                printf("buffer_saida: %s", buffer_saida);
                                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                        }
                                }else{
                                        lb(buffer_saida);
                                        sprintf(buffer_saida, "550 Ocorreu uma falha ao baixar o arquivo\n");
                                        printf("buffer_saida: %s", buffer_saida);
                                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                }

                                close(fd_arquivo_transferencia);
                                close(data_transfer_socket);
                                close(pasv_listen_socket);

                                //=================================== RETR ====================================


                                //=================================== STOR ====================================
                        }else if(strcmp(comando, "STOR") == 0) {

                                printf(argumento);
                                printf("Iniciando execucao de STOR\n");
                                int connection, fd;
                                off_t offset = 0;
                                int pipefd[2];
                                int res = 1;
                                const int buff_size = 8192;

                                if(port_or_pasv == 0) {
                                        printf("LIST: modo PORT reconhecido\n");
                                        lb(buffer_saida);
                                        sprintf(buffer_saida, "150 Estou abrindo o modo ASCII para conexao de dados\n");
                                        printf("buffer_saida: %s", buffer_saida);
                                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                }else{
                                        data_transfer_socket = aceitar_conexao(pasv_listen_socket);
                                        printf("LIST: modo PASV reconhecido\n");
                                }
                                if(pipe(pipefd)!=-1) {
                                        FILE *pont_arq; // cria variável ponteiro para o arquivo
                                        lb(buffer_saida);
                                        sprintf(buffer_saida, "125 Tranferencia de dados \n");
                                        printf("buffer_saida: %s", buffer_saida);
                                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                        pont_arq = fopen(argumento, "w");
                                        if(pont_arq == NULL)
                                        {
                                                printf("Erro na abertura do arquivo!");
                                                return 1;
                                        }
                                        fd = fileno(pont_arq);
                                        while ((res = splice(data_transfer_socket, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0) {
                                                splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
                                        }
                                        if(res==-1) {
                                                printf("Erro na abertura do arquivo!");
                                        }else{
                                                lb(buffer_saida);
                                                sprintf(buffer_saida, "226 File send OK.\n");
                                                printf("buffer_saida: %s", buffer_saida);
                                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                        }
                                        close(connection);
                                        close(fd);

                                        lb(buffer_entrada);
                                        //read(data_transfer_socket, buffer_entrada, BUFFER_SIZE);
                                        //usando fprintf para armazenar a string no arquivo
                                        //fprintf(pont_arq, "%s", buffer_entrada);
                                        fclose(pont_arq);
                                        close(data_transfer_socket);
                                        close(pasv_listen_socket);
                                        //lb(buffer_entrada);
                                        // read(data_transfer_socket, buffer_entrada, BUFFER_SIZE);
                                        //write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                }
                                //=================================== STOR ====================================

                                //=================================== TYPE ====================================
                        }else if(strcmp(comando, "TYPE") == 0) {
                                printf("Iniciando execucao de TYPE\n");

                                if(strcmp(argumento, "I") == 0) {
                                        printf("Tipo de transferência: Image\n");
                                }

                                lb(buffer_saida);
                                sprintf(buffer_saida, "200 Alterando para o modo de transferencia binaria\n");
                                printf("buffer_saida: %s", buffer_saida);
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));

                                //=================================== TYPE ====================================
                        }else if(strcmp(comando, "QUIT") == 0) {
                                printf("Encerrando conexao...\n");
                                lb(buffer_saida);
                                sprintf(buffer_saida, "221 Tchau tchau do servidor\n");
                                write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                                printf("Mensagem enviada ao cliente: %s", buffer_saida);
                                close(server_connection_socket);
                                client_socket[i] = 0;
                                //exit(0);
                        }
                        lb(comando);
                        lb(argumento);
                        lb(buffer_entrada);
                }
        }
}


int start_server(int port) {

        // server_connection_socket é o que vai atender
        int server_listen_socket, server_connection_socket;

        //varias clientes
        int client_socket[30],  max_clients = 30,   activity, i, valread, sd;
        int max_sd;
        //set of socket descriptors
        fd_set readfds;


        //threads
        pthread_t linhas[30];


        int pasv_listen_socket, data_transfer_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        char buffer_entrada[BUFFER_SIZE], buffer_saida[BUFFER_SIZE];
        char comando[8], argumento[128];
        int port_or_pasv; // flag para o modo de execução, port = 0, pasv = 1


///Iniciando  a lista de multiplos crientes
        for (i = 0; i < max_clients; i++)
        {
                client_socket[i] = 0;
        }


        // Creating socket file descriptor
        // socket - create an endpoint for communication
        // int socket(int domain, int type, int protocol);
        // AF_INET             IPv4 Internet protocols
        // SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based
        //                     byte  streams.  An out-of-band data transmission mecha‐
        //                     nism may be supported.
        if ((server_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("A criacao do socket de escuta falhou\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Socket de escuta criado\n");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        // The htons() function converts the unsigned short integer hostshort from
        //     host byte order to network byte order.
        address.sin_port = htons(port );

        // int bind(int sockfd, const struct sockaddr *addr,
        //              socklen_t addrlen);
        if (bind(server_listen_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
                perror("bind failed\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Bind executado com sucesso\n");
        }

        // int listen(int sockfd, int backlog);
        /*The  sockfd  argument  is  a file descriptor that refers to a socket of
           type SOCK_STREAM or SOCK_SEQPACKET.

           The backlog argument defines the maximum length to which the  queue  of
           pending  connections  for sockfd may grow.  If a connection request ar‐
           rives when the queue is full, the client may receive an error  with  an
           indication  of ECONNREFUSED or, if the underlying protocol supports re‐
           transmission, the request may be ignored so that a later  reattempt  at
           connection succeeds.*/
        if (listen(server_listen_socket, 5) == -1) {
                perror("listen\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Comando Listen() executado com sucesso\n");
        }

        while(1) {
                char msg_bem_vindo[BUFFER_SIZE] = {0};

                //clear the socket set
                FD_ZERO(&readfds);
                //add master socket to set
                FD_SET(server_listen_socket, &readfds);
                max_sd = server_listen_socket;
                //add child sockets to set
                for ( i = 0; i < max_clients; i++)
                {
                        //socket descriptor
                        sd = client_socket[i];
                        //if valid socket descriptor then add to read list
                        if(sd > 0)
                                FD_SET( sd, &readfds);

                        //highest file descriptor number, need it for the select function
                        if(sd > max_sd)
                                max_sd = sd;
                }
                //wait for an activity on one of the sockets , timeout is NULL ,
                //so wait indefinitely
                activity = select( max_sd + 1, &readfds, NULL, NULL, NULL);

                if ((activity < 0) && (errno!=EINTR)) {
                        printf("select error");
                }
                if (FD_ISSET(server_listen_socket, &readfds)) {
                        if ((server_connection_socket = accept(server_listen_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
                                perror("accept\n");
                                exit(EXIT_FAILURE);
                        }else{
                                printf("Comando accept() executado\n");
                        }
                        //inform user of socket number - used in send and receive commands
                        printf("Novo , socket fd is %d , ip is : %s , port : %d \n", server_connection_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                        //send new connection greeting message
                        strcat(msg_bem_vindo, "220 Bem vindo ao servidor FTP\n");
                        if(write(server_connection_socket, msg_bem_vindo, strlen(msg_bem_vindo))) {
                                printf("Mensagem de boas vindas enviada\n");
                        }
                        // =================== Inicialização conexão =====================

                        read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
                        printf("Msg do cliente: %s", buffer_entrada);

                        lb(buffer_saida);
                        strcat(buffer_saida, "331 Nome de usuario okay, preciso da senha\n");
                        printf("buffer_saida: %s", buffer_saida);
                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                        printf("Pedido de senha enviado\n");

                        lb(buffer_entrada);
                        read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
                        printf("Senha do usuario: %s", buffer_entrada);

                        lb(buffer_saida);
                        strcat(buffer_saida, "230 Usuario logado\n");
                        printf("buffer_saida: %s", buffer_saida);
                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));

                        lb(buffer_entrada);
                        read(server_connection_socket, buffer_entrada, BUFFER_SIZE);
                        printf("Proximo comando: %s\n", buffer_entrada);

                        lb(buffer_saida);
                        strcat(buffer_saida, "215 UNIX Type: L8\n");
                        printf("buffer_saida: %s", buffer_saida);
                        write(server_connection_socket, buffer_saida, strlen(buffer_saida));
                        // =================== Inicialização conexão =====================

                        puts("Welcome message sent successfully");

                        //add new socket to array of sockets
                        for (i = 0; i < max_clients; i++)
                        {
                                //if position is empty
                                if( client_socket[i] == 0 )
                                {
                                        client_socket[i] = server_connection_socket;
                                        printf("Adding to list of sockets as %d\n", i);

                                        break;
                                }
                        }
                }
                //else its some IO operation on some other socket
                printf("Adentrando ao loop\n");
                lb(buffer_entrada);
                for (i = 0; i < max_clients; i++)
                {
                        server_connection_socket = client_socket[i];
                        pthread_create(&linhas[i],NULL,comados,server_connection_socket);
                }
        }
        return server_connection_socket;
}

// metodo copiado, é usado no LIST
void str_perm(int perm, char *str_perm){
        int curperm = 0;
        int flag = 0;
        int read, write, exec;

        /* Flags buffer */
        char fbuff[4];

        read = write = exec = 0;

        int i;
        for(i = 6; i >= 0; i -= 3) {
                /* Explode permissions of user, group, others; starting with users */
                curperm = ((perm & ALLPERMS) >> i ) & 0x7;

                memset(fbuff,0,3);
                /* Check rwx flags for each*/
                read = (curperm >> 2) & 0x1;
                write = (curperm >> 1) & 0x1;
                exec = (curperm >> 0) & 0x1;

                sprintf(fbuff, "%c%c%c", read ? 'r' : '-', write ? 'w' : '-', exec ? 'x' : '-');
                strcat(str_perm, fbuff);

        }
}

int aceitar_conexao(int socket){
        struct sockaddr_in endereco_cliente;
        int addrlen = sizeof(endereco_cliente);
        return accept(socket, (struct sockaddr*) &endereco_cliente, &addrlen);
}

// limpar buffer de entrada ou saida
void lb(char *buffer){
        memset(buffer, 0, sizeof buffer);
}

//PASV  dados
int create_pasv_listen_socket(int port){

        int pasv_listen_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( port );

        if ((pasv_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("A criacao do socket de escuta pasv falhou\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Socket de escuta em modo PASV criado\n");
        }

        if (bind(pasv_listen_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
                perror("bind failed\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Bind executado com sucesso\n");
        }

        if (listen(pasv_listen_socket, 3) == -1) {
                perror("listen\n");
                exit(EXIT_FAILURE);
        }else{
                printf("Comando Listen() executado com sucesso\n");
        }

        // retornar o socket escutando, fazer os aceites no if
        return pasv_listen_socket;
}


/*
   COMANDOS:
   USER <SP> <username> <CRLF> ok
   PASS <SP> <password> <CRLF> ok
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
