#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
 #include <fcntl.h>
 #include <errno.h>
 
#include "utilitaires.h"
#include "external.h"
#include "redirection.h"

static int duped[3]={0,1,2};
static int saved[3]={-1,-1,-1};


int redi(char* redir,char* path){
    int fd;

    if (strcmp(redir, ">")==0){
        fd = open(path, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(fd<0) return 1;
        saved[1]=dup(1);
        dup2(fd, 1); 
        close(fd); 
    }
    else if(strcmp(redir, ">|")==0){ 
        fd = open(path,O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR );
        if(fd<0) return 1;
        saved[1]=dup(1);
        dup2(fd, 1);   
        close(fd);    
    }
    else if(strcmp(redir, ">>")==0){
        fd = open(path,O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
        if(fd<0) return 1;
        saved[1]=dup(1);
        dup2(fd, 1);   
        close(fd);        
    }
    else if (strcmp(redir, "2>")==0){
        fd = open(path, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(fd<0) return 1;
        saved[2]=dup(2);
        dup2(fd, 2);   
        close(fd);   
    } 
     else if(strcmp(redir, "2>|")==0){
        fd = open(path,O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
        if(fd<0) return 1;
        saved[2]=dup(2);
        dup2(fd, 2);   
        close(fd);   
    }
    else if(strcmp(redir, "2>>")==0){
        fd = open(path,O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR );
        if(fd<0) return 1;
        saved[2]=dup(2);
        dup2(fd, 2);   
        close(fd);   
    }else  if (strcmp(redir, "<")==0){
        fd = open(path, O_RDONLY, S_IRUSR );
        if(fd<0) return 1;
        saved[0]=dup(0);
        dup2(fd, 0); 
        close(fd); 
    } else return 1;

    return 0;  
}

void afterRedi(){
    for(int i=0;i<3;i++){
    if(saved[i]!=-1)
    dup2(saved[i], duped[i]);
    close(saved[i]); 
    saved[i]=-1;
    }  
}


int redirection(char* path){
    char** commandWithNoStar=treatStar(path); // on traite les etoiles de la commande
    char** command;
    int l=length(path)+1;
    command = (char**) malloc(l * sizeof(char*));
    int i=0;

    for(i=0;i<l-1;i++){
    if (strcmp(commandWithNoStar[i],"<")==0 || strcmp(commandWithNoStar[i],">")==0  || strcmp(commandWithNoStar[i],">|")==0  ||strcmp(commandWithNoStar[i],">>")==0  ||
            strcmp(commandWithNoStar[i],"2>")==0  || strcmp(commandWithNoStar[i],"2>>")==0  || strcmp(commandWithNoStar[i],"2>|")==0  )  {
             break;} // on s'arrete des quil y a une redirection
    command[i] = (char*) malloc((strlen(commandWithNoStar[i])+1)*sizeof(char)); 
    strcpy (command[i],commandWithNoStar[i]);
    }
    command[i]=NULL;

   
    for(int k=i;k<l-2;k+=2){
        int r=redi(commandWithNoStar[k],commandWithNoStar[k+1]); // on effectue les ouvertures
        if(r){
            freeTable(command);
            freeTable(commandWithNoStar);
            return 126;
        }
    }
    if(strcmp(command[0],"exit")==0){ // si la commande est exit on exit 
        freeTable(command);
        freeTable(commandWithNoStar);
        exit(STDIN_FILENO);
    }
    int status=0;
    pid_t child = fork();
     if (child<0)  perror("fork\n");
    else if(child == 0){
        execvp(command[0],command);
        exit(127);
    } 
        wait(&status);
        freeTable(command);
        freeTable(commandWithNoStar);
       
        afterRedi(); // après execution de la commande on netoie stdin /stdout/stdrr
        return WEXITSTATUS(status);


}


