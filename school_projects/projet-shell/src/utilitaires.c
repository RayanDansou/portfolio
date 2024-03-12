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
 #include "utilitaires.h"



 int length (char* buf){
    char g[1024]="";
    strcpy(g,buf);
    char * token = strtok(g, " ");
     int i=0;
    while( token != NULL ) {
      token = strtok(NULL, " ");
      i++;
      }
      return i;
    }

int slash(char* n){
    int res=1;
    for(int i=1; i<(strlen(n)-1);i++){
        if(n[i]=='/') return 0;
    }
    return res;
}

int verifiePoint(char* n){
    int res=1;
    for(int i=1; i<(strlen(n)-1);i++){
        if(n[i-1]=='/' && n[i]=='.' && n[i+1]=='.' ) res= 0;
    }
    return res;
}

int dotend(char* n){
    int res=1;
    for(int i=0; i<(strlen(n)-3);i++){
        if(n[i]=='.' && n[i+1]=='.') return 1;
    }
    if(n[strlen(n)-1]=='.'&& n[strlen(n)-2]=='.') return 0;
    return res;
}

int pipeLength(char* buf ){
   char g[1024]="";
    strcpy(g,buf);
    char * token =malloc(1024*sizeof(char));
    token= strtok(g, "|");
     int i=0;
    while( token != NULL ) {
      token = strtok(NULL, "|");
      i++;
      }
    free(token);
      return i;
}

void freeTable(char** table){
     for (int i = 0; *(table+i) != NULL; i++) {
        free(*(table+i));
    }
    free(table);
}

int tableLength(char** table){
   int res=0;
    for (int i = 0; *(table+i) != NULL; i++) {
        res++;
      }
    return res;
}

int isPipeLine(char * n){
    if(strlen(n)<3) return 1;
  int res=1;
    for(int i=0; i<(strlen(n)-3);i++){
        if(n[i]!='>' && n[i+1]=='|') return 0;
    }
    return res;
}

char* normalize(char *s){
    if(strlen(s)==0) return s;
    char *src = s;
    char *dest = s;
    while ((*dest = *src)) {
        if (*dest == '/') {
            while (*src=='/') {
                src++;
            }
            dest++;
        }else {
            src++;
            dest++;
        }
    }
    return s;
}

char ** commandToTable (char * buf){
    char g[1024]="";
    strcpy(g,buf);
    int l=length(buf)+1;
    char ** line = (char**) malloc(l * sizeof(char*));
    int p=0;
    for(p=0; p < l-1 ; ++p ){
       line[p] = (char*) malloc(1024*sizeof(char)); 
    }
     char * token =strtok(g, " ");
     int i=0;
    while( token != NULL ) {
    strcpy(line[i], normalize(token)); 
      i++;
      token = strtok(NULL, " ");
      }
    line[i]=0; 
    return line;
}

char** concatTables(char** table1, char** table2) {
    int len1 = tableLength(table1);
    int len2=tableLength(table2);
    char** newTable = malloc(sizeof(char*) * (len1 + len2 + 1));
    if(len1 + len2 == 0){
        *newTable = NULL;
        return newTable;
    }
    int i;
    if(len1 != 0) {
        for (i = 0; i < len1; i++) {
            newTable[i] = malloc(sizeof(char) * (strlen(table1[i]) + 1));
            strcpy(newTable[i], table1[i]);
        }
        if(len2 != 0) {
            for (int j = 0; j < len2; j++) {
                newTable[i] = malloc(sizeof(char) * (strlen(table2[j]) + 1));
                strcpy(newTable[i], table2[j]);
                i++;
            }
        }
    }else if(len2 != 0){
        for (i = 0; i < len2; i++) {
            newTable[i] = malloc(sizeof(char) * (strlen(table2[i]) + 1));
            strcpy(newTable[i], table2[i]);
        }
    }
    newTable[i] = NULL;
    return newTable;
}

char * tableToString(char** c){
    int l=tableLength(c);
    char * res= malloc(l * sizeof(char));
    strcpy(res,"");
    for(int i=0;i<l;i++){
        strcat(res,c[i]);
        strcat(res," ");
    }

    return res;
}