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
#define PORT_DOWNLOAD 9101
#define IP_SERVEUR6 "::1"
#define IP_SERVEUR "127.0.0.1"
#define SIZE_BUF 512
#define BUFFER_SIZE 1024

/*structure représentant l'entete*/
typedef struct e{
    uint16_t val; /*valeur de l'entete avec le codereq sur 5bits et l'id sur 11 bits*/
}entete;

/*structure représentant un message d'inscription*/
typedef struct inscription_message{
    entete entete;
    char pseudo[10];
}inscription;

/*structure représentant une requete*/
typedef struct client_requete{
    entete entete;
    uint16_t num_fil;
    uint16_t num_messages;
    uint8_t* data;
}requete;

/*structure représentant la réponse du serveur*/
typedef struct reponse_serveur{
    entete entete;
    uint16_t num_fil;
    uint16_t num_messages;
}reponse;


typedef struct serv_mess{
    uint16_t num_fil;
    char origine[10];
    char pseudo[10];
    uint8_t datalen;
    char* data;
}serveur_message;


typedef struct udp{
    entete entete;
    uint8_t numbloc;
    char data[512];
}paquet_udp;


typedef struct{
    pthread_mutex_t mutexInscription;
    pthread_mutex_t mutexPosterBillet;
    pthread_mutex_t mutexDemandeNBillet;
    pthread_mutex_t mutexAjoutFichier;
    pthread_mutex_t mutexTelechargement;
}fonctionnalite;

//Méthodes

char *ask_name(); //fonction pour le formatage du pseudo

void recv_erreur(char *err);

int ask_id(); //fonction demandant l'id du client

int ask_fil(); //fonction demandant le fil voulu

char *ask_message(); //fonction demandant le message au client

int ask_nb();

void print_bits(uint16_t n, int k); //fonction affichant les k bits de n

void affiche_reponse(reponse res); 

void affiche_message(serveur_message* mess);

uint16_t new_header(uint8_t code, uint16_t i); //un header bien formaté avec le code et l'id en entrée

void send_inscription(int sock); //fonction d'envoi de l'inscription

reponse recv_reponse(int sock); //fonction pour recevoir une réponse du serveur

void post_message_fil(int sock); //fonction pour poster des messages

void recv_serveur_message(int sock, serveur_message* mess);

void recv_messages_fil(int sock, requete r);

void ask_messages_fil(int sock);

void check_codereq();

void subscribe_to_feed(int sock);

void upload_fichier(int sock);

void download_fichier(int sock);