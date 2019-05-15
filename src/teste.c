#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **quebrarString (char *str,char **ponteiro){
	char *ptr = strtok(str," ");
	int p = 0;
	while (ptr != NULL){
        ponteiro[p]=ptr;
        p = p + 1;
        ptr = strtok(NULL, " ");
	 }
	  for (int i=0;i<10;i++){
        if(ponteiro!=NULL){
            printf("%s\n",ponteiro[i]);
        }
    }
    
    return ponteiro;
}




int main(){
    char **ponteiro=(char**)malloc(sizeof(char*)*50);;
	char *str= (char*)malloc(sizeof(char)*1024);
	strcpy(str,"strtok needs to be called several times to split a string");
    printf("%s\n",str);
	ponteiro= quebrarString(str,ponteiro);
    printf("To aqui");
    for (int i=0;i<10;i++){
        
            printf("%s\n",ponteiro[i]);
        
    }
	return 0;
}

