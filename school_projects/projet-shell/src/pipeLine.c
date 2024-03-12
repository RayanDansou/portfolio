#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utilitaires.h"
#include "pipeLine.h"
#include "redirection.h"




char *** pipeCommands (char* buf){
    int l=pipeLength(buf);
    char ***res = calloc(l, sizeof(char**)); 
    for(int z = 0; z < l; z++) {
    res[z] = calloc(l, sizeof(char*));
    }
    char g[1024]="";
    strcpy(g,buf);
     int i=0;
     char* token;
    char* rest = g;

    while ((token = strtok_r(rest, "|", &rest))){
     char j[1024]="";
    strcpy(j,token);
    char * tok= strtok(j, " ");
    int p=0;
    while( tok != NULL ) {
        res[i][p]=malloc(1024*sizeof(char));
        strcpy(res[i][p],tok);
        tok = strtok(NULL, " ");
      p++;
    }
    res[i][p]=0;
    i++;
    }
    return res;

}


void pipeRedi(char* buf){
    char ***commands = pipeCommands(buf);
  size_t i, n;

    n = pipeLength(buf);

    for (i = 0; i < n-1; i++) {            
        char* str=tableToString(commands[i]);
        int r= redirection(str);
        free(str);
        if(r!=0) exit(127);
    }
    char* str=tableToString(commands[i]);
        int r= redirection(str);
        free(str);
        for(int m=0; m<n;m++) freeTable(commands[m]);
        free(commands);
        exit(r);

}

void pipeExec(char* buf){
    if(strchr(buf,'>')) pipeRedi(buf);
    else{
    char ***commands = pipeCommands(buf);
    size_t i, n;
    int prev_pipe, pfds[2];

    n = pipeLength(buf);
    prev_pipe = STDIN_FILENO; 

    for (i = 0; i < n - 1; i++) {
        pipe(pfds); // on crée un tube

        if (fork() == 0) {
            
            if (prev_pipe != STDIN_FILENO) { // si le dernier tube n'est pas stdin 
                dup2(prev_pipe, STDIN_FILENO); //le dernier tube devient l'entree
                close(prev_pipe);
            }

            dup2(pfds[1], STDOUT_FILENO); // on fait sortir le tube sur stdout
            close(pfds[1]);

            execvp(commands[i][0], commands[i]); // execute le commande
            exit(127);  // cas erreur
       }

        close(prev_pipe);
        close(pfds[1]);
        prev_pipe = pfds[0]; // le dernier tube devient le contenu du tube courant
    }

    if (prev_pipe != STDIN_FILENO) {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }
    
    execvp(commands[i][0], commands[i]);
    exit(127);
    }
}