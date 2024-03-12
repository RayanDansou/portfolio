

int getFirstStar(char * s); // renvoie la position de la première etoile dans le path

int containsEnd(char * container, char * s);// vérifie que s est suffixe de container


char * getAfterPos(unsigned long pos, char * s); // renvoie tout ce quil y a après la position pos


char * getBeforePos(unsigned long pos, char * s); // renvoie tout ce quil y a avant la position pos


char *getSearch(int start, const char *path); // renvoie le suffixe recherché (*txt = txt)


char ** getAllPaths(char * path); // renvoie tous les possibles chemins correspond au path
