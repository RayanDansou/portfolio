#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MSG_MAX 1024
#define PORT_SERVEUR 1234
#define IP_SERVEUR "127.0.0.1"
/* on défini un numbre maximum d'abonnés et de messages pouvant etre dans un fil */
#define MAX_MESSAGES 200
#define MAX_ABONNES 100 
#define MAX_CLIENTS 4096 
#define PORT_UPLOAD 5678 
#define FILES_DEST "serveur_fichiers/"
#define MAX_FILS 200 
#define BUFFER_SIZE 1024





typedef struct e{
    uint16_t val; /*valeur de l'entete avec le codereq sur 5bits et l'id sur 11 bits*/
}entete;

/*structure représentant un message d'inscription*/
typedef struct inscription_message{
    entete entete;
    char pseudo[10];
}inscription;


typedef struct reponse_serveur{
    entete entete;
    uint16_t num_fil;
    uint16_t num_messages;
}reponse;


typedef struct client_requete{
    entete entete;
    uint16_t num_fil;
    uint16_t num_messages;
    uint8_t* data;
}requete;

typedef struct udp{
    entete entete;
    uint8_t numbloc;
    char data[512];
}paquet_udp;


typedef struct serv_mess{
    uint16_t num_fil;
    char origine[10];
    char pseudo[10];
    uint8_t datalen;
    char* data;
}serveur_message;

typedef struct {
    int id;
    char pseudo[11];
} Client;


typedef struct {
    char pseudo[11];
    char data[BUFFER_SIZE];
} Message;

typedef struct {
    int id_fil;
    char nom_fil[11];
    int num_messages;
    Message messages[MAX_MESSAGES];
} Fil;



uint16_t new_header(int id,uint8_t code);

void treat_inscription(int sockclient, inscription r);

void treat_error(int sockclient);

void affiche_fil(int k);

void treat_post(int sockclient,entete e,requete r);

void send_last_messages(int sockclient, int k, int n);

void treat_ask(int sockclient,requete r);

void treat_subscribe(int sockclient, requete res);

void treat_upload(int sockclient,requete r);

void treat_download(int sockclient,requete r);

requete deserialize_requete(const char* serialized_req);

inscription deserialize_inscription(const char* serialized_i);

void treat_request(void* arg);

