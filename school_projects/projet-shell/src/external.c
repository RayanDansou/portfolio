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
#include "external.h"
#include "utilitaires.h"
#include "etoileSimple.h"



 char** starInPath (char* buf){
   if(strchr(buf,'*')==NULL) return commandToTable(buf); // si le path n'a pas d'etoile
    char** chemins=getAllPaths(buf); // on fait le traitement
    int newlength=tableLength(chemins)+1;
    char** res = (char**) malloc( newlength* sizeof(char*));
    for(int p=0; p < newlength-1; ++p ){
       res[p] = (char*) malloc(1024*sizeof(char)); 
        strcpy(res[p],"");
    }
   
    for(int k=0;k<newlength-1;k++) {
        strcpy(res[k],chemins[k]);
    }
    freeTable(chemins);
    res[newlength-1]=0; // on met NULL a la fin
    return res;
}

char** treatStar (char* buf){
   if(strchr(buf,'*')==NULL) return commandToTable(buf);// si le path n'a pas d'etoile
    char** c= commandToTable(buf);
    int l=tableLength(c);
    char** res = (char**) malloc(1 * sizeof(char*));
     res[0]=NULL;

    for(int k=0;k<l;k++){
        char** st=starInPath(c[k]); // on traite dans chaque string de la commande ("ls" ensuite "-l" etc)
        char** j=concatTables(res,st); // on concatène
        freeTable(res);
        res=j;
        freeTable(st);
    }

    freeTable(c);
    return res;
}
