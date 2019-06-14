//https://www.vivaolinux.com.br/script/Uso-de-threads-no-C



#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

struct valor{
   int tempo;
   int id;
};

void *espera(void *tmp){
   struct valor *v = (struct valor *) tmp;
   sleep(v->tempo);
   printf("Oi eu sou a thread %d esperei %d segundos antes de executar\n",v->id,v->tempo);
}

int main(){

   pthread_t linhas[10];
   int execute,i;
   struct valor *v;
   srand(time(NULL));
   for (i=0;i<10;i++){
      v = (struct valor *) malloc(sizeof(struct valor *));
      v->tempo = (rand()%10)+2;
      v->id = i;
      printf("Criei a thread <%d> com tempo <%d>\n",i,v->tempo);
      execute = pthread_create(&linhas[i],NULL,espera,(void *)v);
   }
   pthread_exit(NULL);
}
