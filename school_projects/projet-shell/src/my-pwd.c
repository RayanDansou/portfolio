#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
 #include <fcntl.h>
 #include <errno.h>
 #include "my-pwd.h"



 int pwd(char * c,char* v){ // retourne le chemin vers le repertoire courant
    if(strcmp(v,"-L")==0 ||  strcmp(v,"")==0 ){ // si la commande est suivie de -L ou est seul
    printf("%s\n",getenv("PWD")); // renvoie la variable d'environnement pwd
    return 0;
    }
    else if (strcmp(v,"-P")==0  ){ // si la commande est suivie de -P
        char buf[PATH_MAX];
        if(getcwd(buf, sizeof(buf)) == NULL){ // on recupere le repertoire courant
        printf("erreur: pwd a echoué\n"); 
        }
        printf("%s\n",buf); //renvoie le repertoire courant
        return 0;
    }
    return 1;
}


