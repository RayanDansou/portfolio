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

int getFirstStar(char *s)
{ 
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == '*')
            return i;
    }
    return -1;
}

int containsEnd(char *container, char *s){
    if (strlen(container) < strlen(s))
        return 0;
    int r = strlen(container) - 1;
    for (int i = strlen(s) - 1; i >= 0; i--){
        if (container[r] != s[i])
            return 0;
        r--; }
    return 1;
}

char *getAfterPos(unsigned long pos, char *s)
{   size_t len = strlen(s);
    size_t sub_len = len - pos;
    char *sub = malloc(sub_len + 1);
    strncpy(sub, s + pos, sub_len);
    sub[sub_len] = 0;
    return sub;
}

char *getBeforePos(unsigned long pos, char *s)
{
    char *beforeStar = malloc(sizeof(char) * (pos + 1));
    if (pos > 0)
    {
        memcpy(beforeStar, s, pos);
    }
    beforeStar[pos] = 0;
    return beforeStar;
}


char *getSearch(int start, const char *path)
{
    int end = start;
    while (path[end] != '\0' && path[end] != '/')end++;
    char *search = malloc(sizeof(char) * (end - start + 1));
    memcpy(search, path + start, end - start);
    search[end - start] = '\0';
    return search;
}


char **getAllPaths(char *path){
    char **res = malloc(sizeof(char *) * 1);
    res[0] = 0;
    int starpos = getFirstStar(path);
    
    if (starpos == -1){ // si pas d'etoile dans le path
        struct stat st;
        if (lstat(path, &st) == 0){ 
            freeTable(res);
            return commandToTable(path); // renvoie le path avec NULL
        }else{
            return res; 
        }
    }
    char *beforeStar = getBeforePos(starpos, path);
    char *search = getSearch(starpos + 1, path);
    unsigned long afterSize = strlen(beforeStar) + strlen(search) + 1;
    char *afterStar = getAfterPos(afterSize, path);

    DIR *dir = NULL; // on ouvre tout ce quil y a avant l'etoile
    struct dirent *entry;
    if (strlen(beforeStar) == 0){ 
        dir = opendir(".");// si on commence par l'etoile on ouvre le rep courant
    }else{
        dir = opendir(beforeStar); // sinon on ouvre tout ce quil y a avant
    }
    if (dir == NULL){ // si l'ouveture a échouée
        free(beforeStar);
        free(afterStar);
        free(search);
        return res;
    }
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_name[0] == '.') continue;// on ignore les fichiers cachés
        
        if (strlen(afterStar) != 0){ // si après l'etoile il y a encore du traitement a effectuer(*/a.. par exemple)
            if (entry->d_type != DT_DIR && entry->d_type != DT_LNK) continue; 
        }
        if (containsEnd(entry->d_name, search) == 1) { 
            char *newString = malloc(sizeof(char) * (strlen(beforeStar) + strlen(entry->d_name) + strlen(afterStar) + 1));
            sprintf(newString, "%s%s%s", beforeStar, entry->d_name, afterStar);
            char **replace = getAllPaths(newString); // on traite le nouveau path a chaque cas
            char **tmp = concatTables(res, replace);// on concatène au resultat
            freeTable(res);
            res = tmp;
            freeTable(replace);
            free(newString);
        }
    }
    closedir(dir);

    free(beforeStar);
    free(afterStar);
    free(search);

    return res;
}
