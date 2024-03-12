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
 #include "my-pwd.h"
 #include "Tronqueur.h"
 #include "utilitaires.h"
 #include "pipeLine.h"
 #include "redirection.h"


 char* Lastenv="slash";
    int valret=0;

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096


int cd (char* p, char* option){
    char* path=normalize(p);
    int res=0;
    char s[PATH_MAX];
    char newPath[PATH_MAX]="";
    char mots[PATH_MAX]="";

     if (strcmp(path,"..")==0 ){ // si ..
        if(strcmp(option,"-L")==0||strcmp(option,"")==0){ // option -L
         struct stat buf;
         lstat (getenv("PWD"), &buf);
        if (S_ISLNK(buf.st_mode)){ // si c'est un lien
        char h[PATH_MAX];
          strcpy(h,Lastenv); // on recupère le dernier rep de travail
         char * token = strtok(h, "/");
            while( token != NULL ) {
         strcat(mots, "/"); 
         strcpy(mots, token ); 
         token = strtok(NULL, "/");
         }
      int size=strlen(Lastenv)-strlen(mots); // on enleve du string le dernier path (/a/b == /a)
        char *v= malloc((1024)*sizeof(char));
        strcat(v,"");
        strcpy(v,Lastenv);
        v[size-2]='\0'; 
        chdir(v);
        setenv("PWD",v,1); // on change lenvironnement
        free(v);
        return 0;
       
        }else{ // sinon on fait un chdir et on change lenvironnement
           res= chdir(path);
        setenv("PWD", getcwd(s, 100),1);
        }}
        else{
             res=chdir(path);
        setenv("PWD", getcwd(s, 100),1);
        }
    } else{

        if(slash(path)){
        if (path[0]=='/')sprintf(newPath, "%s", path); // si reference depuis la racine on ecrit juste le path
     else sprintf(newPath, "%s/%s", getcwd(s, 100), path); // sino on concatene path au rep courant
    sprintf(Lastenv,"%s",newPath);  // Lastenv devient le rep courant
     if(Lastenv[strlen(Lastenv)-1]!='/') strcat(Lastenv,"/");
        }
    
    if(strcmp(option,"-")==0){ // cd -
        res=chdir(path);
        setenv("PWD", path,1); 
    }  else if(strcmp(path,"")==0){ // cd 
        res=chdir (getenv("HOME"));
        setenv("PWD", getenv("HOME"),1);
    } else if(strcmp(option,"-P")==0){ // option -p
         res=chdir(path); 
        setenv("PWD", getcwd(s, 100),1);
    }
    else if (slash(path)==0){ // si le path contient des slash on se rend dans le repertoire
        char f[PATH_MAX]="n";
        strcpy(f,path);
        char* token;
        char* rest = f;
        while ((token = strtok_r(rest, "/", &rest))){
          if(verifiePoint(path)==1){
              cd(token,""); 
         }else{
            if(dotend(path)) cd(token,"-P"); 
            else cd(token,"-L");
        }
    }
        return 0;
    }
    else if (strcmp(option,"-L")==0 || strcmp(option,"")==0){ // option -L
         struct stat buf={};
        lstat (path, &buf);
        if (S_ISLNK(buf.st_mode)){ // verifie si le repertoire de destination est un lien
            res=chdir(path);
          setenv("PWD", newPath, 1); // on change d'environnement        
        }else{
            res=cd(path,"-P");
        }
    }
    }
    if (res!=0) 
        return 1;

    return 0;
}


int main(){
    char buffer[1024]=""; // va contenir les informations affichées par readline
    char path[PATH_MAX]=""; //contient le dernier repetoire d'ou on part
    char*v=info(".",0);
    strcpy(buffer,v); // ecriture de la première ligne
    rl_outstream = stderr;
    Lastenv=getenv("PWD"); // prends le dernier répertoire de travail
    signal(SIGTERM,SIG_IGN); // on ignore les signaux
    signal(SIGINT,SIG_IGN);

    while (1){ // lecture du buffer
        char* buf = readline(buffer);
        char* str= malloc(1024*sizeof(char));
        char* str2= malloc(1024*sizeof(char)); 
        char* v="";
        int val=0;
        add_history (buf); // ajout dans l'historique

        if (buf==NULL){ // Ctrl D
            exit(valret);
        } else if (sscanf(buf,"exit %d", &val)==1){ // exit avec valeur 
            clear_history();
            free(str);
            free(str2); 
            free (buf); 
            exit(val);
        }else if (strcmp (buf, "exit") == 0 ){  // exit toute seule
            clear_history();
            free(str);
            free(str2); 
            free(buf);
            exit(valret); 
        }else  if (strcmp (buf, "\0") == 0){ // touche Entrer
        }
        else if (strcmp (buf, "true") == 0 ){ // true
            valret=0;
        }
        else if (strcmp (buf, "false") == 0 ){ // false
            valret=1;
        }
        else if(sscanf(buf,"cd %s %s", str,str2)==2){ // cd  a b (2 arguments)
            int t;
          if(strcmp(str,"-L")==0 || strcmp(str,"-P")==0  ){
                strcpy(path,getenv("PWD")); // on stock le chemin courant dans path pour la commande "-"
                t=cd(str2,str);
            }else{
                t=1;
            }
            valret=t;
            if (t==0){
            }else{
                char k[PATH_MAX];
                strcpy(k,"");
                strcat(k,"bash: cd: ");
                strcat(k,str);
                strcat(k,": No such file or directory\n");
                write(2,k,strlen(k)); 
            }
        }else if(strcmp (buf, "cd") == 0){  // commande cd
            strcpy(path,getenv("PWD"));
            int i= cd("","");
            valret=i;

        }else if(sscanf(buf,"cd %s", str)==1){ // cd a (1 argument)
            int t=1;
             if(strcmp(str,"-")==0){
                t=cd(path,"-"); 
            } else{
                strcpy(path,getenv("PWD")); // on stock le chemin courant dans path
                t=cd(str,"-L"); // ensuite on effectue la commande cd normale
            }
            valret=t;
            if (t==0){
            }else{
                char k[PATH_MAX];
                strcpy(k,"");
                strcat(k,"bash: cd: ");
                strcat(k,str);
                strcat(k,": No such file or directory\n");
                write(2,k,strlen(k));    
                }
                
        }else if (isPipeLine(buf)==0){ // si la commane est ue pipeLine
                int status=0;
                pid_t child = fork();
                if (child<0)  perror("fork\n");
                else if(child == 0){
                    pipeExec(buf);
                }else{
                    wait(&status);
                    valret=WEXITSTATUS(status);
                }
                
        }else if(strstr(buf,"<")!=NULL || strstr(buf,">|")!=NULL || strstr(buf,">")!=NULL ||
        strstr(buf,">>")!=NULL || strstr(buf,"2>")!=NULL  || strstr(buf,"2>|")!=NULL  || strstr(buf,"2>>")!=NULL  )
            { // si la commande contient des redirections
                int r= redirection(buf);
                if (r) valret=1;
                else valret=0;

        }else if(strcmp (buf, "pwd") == 0){ // pwd
                    int i= pwd(".",""); 
                    valret=i;
        } else if (sscanf(buf,"pwd %s", str)==1){ // pwd a (1 argument)
                    if(strcmp(str,"-P")==0 || strcmp(str,"-L")==0 ){ // si la commande pwd est suivie de -P ou -L
                        int t= pwd(".",str);
                        valret=t;
                    }else{ printf("option inconnue pour pwd!!\n");
                        valret=1;}
        }
        else{
                    int status=0;
                    char** r= treatStar(buf);
                    if (r[0]==NULL) {
                        freeTable(r);
                        valret=127;
                    }else{
                    if (strcmp(r[0],"exit")==0){
                        int i=0;
                        if(r[1]!=NULL) i=strtol(r[1], NULL, 10);
                        freeTable(r);
                        exit(i);
                    }
                    pid_t child = fork();
                    if (child<0)  perror("fork\n");
                    else if(child == 0){
                            execvp(r[0],r); 
                            exit(127); 
                    } else {
                        wait(&status);
                        valret=WEXITSTATUS(status);
                        freeTable(r);
                    }
                    }
        }
        v=info(".",valret);
        strcpy(buffer,v);
        // libère les variables d'itération
        if(str) free(str);
        if(str2) free(str2);
        if(v) free(v);
        if(buf) free(buf);

    }
    return 0;
}
