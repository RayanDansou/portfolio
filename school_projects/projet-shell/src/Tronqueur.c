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
 #include "Tronqueur.h"




 char* tronc(char *str) { // fonction de troncage pour respecter la longueur du prompt (30 caractères en comptant $ et [0])
    if(strlen(str)<=25){ // le cas ou la longueur du chemin n'est pas a tronquer
        char* res=malloc(25*sizeof(char));
        memcpy(res,str,strlen(str)); // copie de str dans res
        res[strlen(str)]='\0'; // fin de chaine
        return res;
    }else{ 
        char* res=malloc(26*sizeof(char));
        int i=strlen(str)-22; // valeur qui va permettre de savoir a quel moment on commence a recopier str
        memcpy(res+3,str+i,22); // copie des 22 derniers caracteres de str dans res
        res[0]='.'; // rajout en tete de res le tronquage (...str)
        res[1]='.'; 
        res[2]='.'; 
        res[25]='\0'; // fin de chaine
        return res;
    }
}


char* tronc2(char *str) { // fonction de troncage pour respecter la longueur du prompt (30 caractères en comptant $ et [0])
    if(strlen(str)<=24){ // le cas ou la longueur du chemin n'est pas a tronquer
        char* res=malloc(24*sizeof(char));
        memcpy(res,str,strlen(str)); // copie de str dans res
        res[strlen(str)]='\0'; // fin de chaine
        return res;
    }else{ 
        char* res=malloc(24*sizeof(char));
        int i=strlen(str)-20; // valeur qui va permettre de savoir a quel moment on commence a recopier str
        memcpy(res+3,str+i,20); // copie des 20 derniers caracteres de str dans res
        res[0]='.'; // rajout en tete de res le tronquage (...str)
        res[1]='.'; 
        res[2]='.'; 
        res[23]='\0'; // fin de chaine
        return res;
    }
}


char* info(char * c,  int k){ // fonction retournant le chemin courant affiché dans le prompt sous le bon format
    struct stat st;
    lstat (getenv("PWD"), &st);
    if (S_ISLNK(st.st_mode)){ // on verifie si le fichier courant est un lien symbolique 
        char* res=malloc((30+strlen((getenv("PWD")))*sizeof(char))); // on affiche la variable d'environement pwd
        if(res==NULL) free(res);
        char* t=tronc(getenv("PWD"));
        if(k==0) sprintf(res,"\001\033[32m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
        else if(k==1) sprintf(res,"\001\033[91m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
        else {
            free(t);
            free(res);
            t=tronc2(getenv("PWD"));
            res=malloc((30+strlen(t))*sizeof(char));
            sprintf(res,"\001\033[91m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
            }   
        free(t);
        return res; 
    } 
    char buf[PATH_MAX];
    if(getcwd(buf, sizeof(buf)) == NULL){ // on recupere le repertoire courant
        printf("erreur: pwd a echoué\n"); 
    }
    char* t=tronc(buf);
    char* res=malloc(((55)*sizeof(char)));
    if(k==0) sprintf(res,"\001\033[32m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
    else if(k==1) sprintf(res,"\001\033[91m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
        else {
            free(t);
            free(res);
            t=tronc2(buf);
            res=malloc((30+strlen(t))*sizeof(char));
            sprintf(res,"\001\033[91m\002[%d]\001\033[34m\002%s\001\033[00m\002$ ",k,t);
            }
    free(t);
    return res; // renvoi le repertoire courant 
}

