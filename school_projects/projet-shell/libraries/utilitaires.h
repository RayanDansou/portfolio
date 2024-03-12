
int length (char* buf); // renvoie la longueur de la comman
 
int slash(char* n); // renvoie 0 si le n contient le caractère /

int verifiePoint(char* n); // verifie qu il y a .. dans le path (ex: a/b/../c)

int dotend(char* n);   // verifie qu il y a .. a la fin du path (ex: a/b/..)

int pipeLength(char* buf); // renvoie la longueur d'une pipeLine

void freeTable(char** table); // libère un tableau char**

int tableLength(char** table); // renvoie la taille d'un char**

int isPipeLine(char * n); // verifie que la commande est une pipeLine(a | b)

char* normalize(char *s); // normalise un char* (retire les '/' redondants)

char ** commandToTable (char * buf); // prends un commande et la renvoie sous forme de tableau executable par exec

char** concatTables(char** table1, char** table2); // concatène 2 char**

char * tableToString(char** c); // transforme un char** en char*