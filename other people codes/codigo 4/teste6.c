#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <ctype.h>
#include <pthread.h>

void *connection_handler(void *);

#define MAX_MSG 1024

/*
 Servidor aguarda por mensagem do cliente, imprime na tela
 e depois envia resposta e finaliza processo
 */

int main(int argc, char* argv[]) {
/*
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;
    //verificando se foi executando o comando corretamente
    if (argc != 3) {
        fprintf(stderr, "use:./server [Porta] [local]\n");
        return -1;
    } else if (!isdigit(*argv[1])) {
        fprintf(stderr, "Argumento invalido '%s'\n", argv[1]);
        fprintf(stderr, "use:./server [Porta] [local]\n");
        return -1;
    }

    mydir = opendir(argv[2]);
    //verificando se o diretorio existe
    if(mydir == NULL ){fprintf(stderr, "Argumento invalido '%s'\n", argv[2]);return -1;}
    char* aux1 = argv[1];

*/
  int portaServidor = atoi(665);
   //variaveis
    int socket_desc, conexao, c, nova_conex;
    struct sockaddr_in servidor, cliente;
    char *mensagem;
    char resposta[MAX_MSG];
    int tamanho, count;

    // para pegar o IP e porta do cliente
    char *cliente_ip;
    int cliente_port;

    //*********************************************************************//
    //      INICIO DO TRATAMENTO DA THREAD, localização e transferência    //
    //      do arquivo.                                                    //
    //*********************************************************************//
    void *connection_handler(void *socket_desc) {
        /*********************************************************/

        /*********comunicao entre cliente/servidor****************/

        // pegando IP e porta do cliente
        cliente_ip = inet_ntoa(cliente.sin_addr);
        cliente_port = ntohs(cliente.sin_port);
        printf("cliente conectou: %s : [ %d ]\n", cliente_ip, cliente_port);

        // lendo dados enviados pelo cliente
        //mensagem 1 recebido nome do arquivo
        if ((tamanho = read(conexao, resposta, MAX_MSG)) < 0) {
            perror("Erro ao receber dados do cliente: ");
            return NULL;
        }
        resposta[tamanho] = '\0';
        printf("O cliente falou: %s\n", resposta);
/*
        char aux_nomeArquivo[MAX_MSG];
        //fazendo copia do nome do arquivo para variavel auxiliar. tal variavel é utilizada para localizar
        // o arquivo no diretorio.
        strncpy(aux_nomeArquivo, resposta, MAX_MSG);
        //printf("ax_nomeArquivo: %s\n", aux_nomeArquivo);



        /*********************************************************/
        if (mydir != NULL) {

            //funcao busca todo o diretorio buscando o arquivo na variavel aux_nomeArquivo
            //struct stat s;
            while ((myfile = readdir(mydir)) != NULL) {

                stat(myfile->d_name, &mystat);

                printf("Arquivo lido: %s, Arquivo procurado: %s\n", myfile->d_name, resposta);
                if (strcmp(myfile->d_name, resposta) == 0) {//arquivo existe
                   closedir(mydir);
                    //Reiniciando variáveis da pesquisa do diretorio para a proxima thread
                    myfile = NULL;
                    mydir = NULL;
                    mydir = opendir(argv[2]);

                    //**************************************//
                    //      INICIO DO PROTOCOLO            //
                    //*************************************//


                    mensagem = "200";
                    //mensagem 2 - enviando confirmação q arquivo existe
                    write(conexao, mensagem, strlen(mensagem));

                    //mensagem 3 - recebendo que arquivo OK do cliente
                    read(conexao, resposta, MAX_MSG);


                    //**************************************//
                    //      FIM DO PROTOCOLO               //
                    //*************************************//

                    //abrindo o arquivo e retirando o tamanho//
                    //fazendo copia do nome do arquivo para variavel auxiliar. tal variavel é utilizada para localizar
                    // o arquivo no diretorio.


                    char localArquivo[1024];
                    strncpy(localArquivo, argv[2], 1024);
                    strcat(localArquivo,aux_nomeArquivo);

                    FILE * f = fopen(localArquivo, "rb");
                    if((fseek(f, 0, SEEK_END))<0){printf("ERRO DURANTE fseek");}
                    int len = (int) ftell(f);
                    mensagem = (char*) len;
                    printf("Tamanho do arquivo: %d\n", len);
                    //convertendo o valor do tamanho do arquivo (int) para ser enviado em uma mensagem no scoket(char)
                    char *p, text[32];
                    int a = len;
                    sprintf(text, "%d", len);
                    mensagem = text;

                    //mensagem 4 - enviando o tamanho do arquivo
                    send(conexao, mensagem, strlen(mensagem), 0);

                    int fd = open(localArquivo, O_RDONLY);
                    off_t offset = 0;
                    int sent_bytes = 0;
                    //localArquivo = NULL;
                    if (fd == -1) {
                        fprintf(stderr, "Error opening file --> %s", strerror(errno));

                        exit(EXIT_FAILURE);
                    }

                    while (((sent_bytes = sendfile(conexao, fd, &offset, BUFSIZ)) > 0)&& (len > 0)) {

                        fprintf(stdout, "1. Servidor enviou %d bytes do arquivo, offset Ã© agora : %d e os dados restantes = %d\n", sent_bytes, (int)offset, len);
                        len -= sent_bytes;
                        fprintf(stdout, "2.Servidor enviou %d bytes do arquivo, offset Ã© agora : %d e os dados restantes = %d\n", sent_bytes, (int)offset, len);
                        if (len <= 0) {
                            break;
                        }
                    }
                    //closedir(mydir);
                    while (1) {
                    }

                }
            }if(myfile==NULL) {
                    //enviando mensagem para o cliente de arquivo nao encontrado.
                    mensagem = "404";//file not found
                    printf("\n//*********************************//\n");
                    printf("Arquivo \"%s\" NÃ£o Existe no diretório: \"%s\"\n",aux_nomeArquivo, argv[2]);
                    //mensagem 2 - enviando confirmação q arquivo existe
                    write(conexao, mensagem, strlen(mensagem));
                    //sempre que termina de pesquisar o diretorio de arquivos a variavel myfile vai para null
                    // entao eh necessario preencher mydir novamente com o argv[2] com o diretorio de pesquisa.
                    //caso contrario novas thread nao acessaram o diretorio passado em argv[2]]
                    mydir = opendir(argv[2]);
                    //
                    while (1) {
                    }
                    close(conexao);
                    //closedir(mydir);

            }
            if (mydir != NULL) {
                closedir(mydir);
                mydir = NULL;
            }
        }

        if (strcmp(resposta, "bye\n") == 0) {
            close(conexao);
            printf("Servidor finalizado...\n");
            return NULL;
        }


    }

    //*********************************************************************//
    //      FIM DO TRATAMENTO DA THREAD, localização e transferencia    //
    //      do arquivo.                                                    //
    //*********************************************************************//



    //************************************************************
    /*********************************************************/
    //Criando um socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Nao foi possivel criar o socket\n");
        return -1;
    }

    //Preparando a struct do socket
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // Obtem IP do S.O.
    servidor.sin_port = htons(portaServidor);

    //Associando o socket a porta e endereco
    if (bind(socket_desc, (struct sockaddr *) &servidor, sizeof (servidor)) < 0) {
        puts("Erro ao fazer bind Tente outra porta\n");
        return -1;
    }
    puts("Bind efetuado com sucesso\n");
    // Ouvindo por conexoes
    listen(socket_desc, 3);
    /*********************************************************/
    //Aceitando e tratando conexoes
    puts("Aguardando por conexoes...");
    c = sizeof (struct sockaddr_in);

    while ((conexao = accept(socket_desc, (struct sockaddr *) &cliente, (socklen_t*) & c))) {
        if (conexao < 0) {
            perror("Erro ao receber conexao\n");
            return -1;
        }
        pthread_t sniffer_thread;
        nova_conex = (int) malloc(1);
        nova_conex = conexao;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) nova_conex) < 0) {
            perror("could not create thread");
            return 1;
        }
        puts("Handler assigned");
    }
    if (nova_conex < 0) {
        perror("accept failed");
        return 1;
    }
}
