#include "client.h"

/*fonction pour le formatage du pseudo*/
char *ask_name(){
    char *pseudo = malloc(sizeof(char) * 11);
    printf("Entrez votre pseudo : ");
    scanf("%s",pseudo);

    if(strlen(pseudo)>10){
        printf("Pseudo trop long\n");
        exit(4);
    }else if(strlen(pseudo)<10){
        int charmanquant = 10 - strlen(pseudo);
        for(int i=0; i<charmanquant; i++){
            strcat(pseudo,"#");
        }
        pseudo[strlen(pseudo)] = '\0';
        printf("Pseudo : %s\n",pseudo);
    }
    return pseudo;
}

/*fonction pour retirer les # quand on recoi les messages*/
void deformatage(char* str, char c) {
    int len = strlen(str);
    int dst = 0;
    for (int src = 0; src < len; src++) {
        if (str[src] != c) {
            str[dst] = str[src];
            dst++;
        }
    }
    str[dst] = '\0';
}

void recv_erreur(char* err){
    printf("%s\n",err);
    exit(1);
}

/* fonction qui demande au client son id*/
int ask_id(){
    int id=0;
    printf("Entrez votre id : ");
    scanf("%d",&id);
    return id;
}


/* fonction qui demande au client son id*/
int ask_port(){
    int id=0;
    printf("Entrez le port : ");
    scanf("%d",&id);
    return id;
}
/* fonction qui demande au client le fil souhaité*/
int ask_fil(){
    int fil=0;
    printf("Entrez le numéro de fil : ");
    scanf("%d",&fil);
    return fil;
}


/* fonction qui demande au client le message souhaité*/
char *ask_message(){
    char *message = malloc(sizeof(char) * 1024);
    printf("Entrez votre message : ");
    fgets(message, 1024, stdin);
    message[strcspn(message, "\n")] = 0;
    return message;
}

/* fonction qui demande au client le nom du fichier souhaité*/
char *ask_file(){
    char *message = malloc(sizeof(char) * 1024);
    printf("Entrez le nom du fichier : ");
    fgets(message, 1024, stdin);
    message[strcspn(message, "\n")] = 0;
    return message;
}


int ask_nb(){
    int nb=0;
    printf("Entrez le nombre de messages que vous souhaitez recevoir: ");
    scanf("%d",&nb);
    return nb;
}

/*fonction d'affichage d'une reponse*/
void affiche_reponse(reponse res){
    u_int16_t header=ntohs(res.entete.val);
    uint8_t codereq= header & 0x1F;
    uint16_t id= header >> 5;
    printf("\nRéponse Recue: \n");
    printf("code: %d ID: %d\n",codereq,id);
    printf("num: %d num_message: %d\n\n\n\n",ntohs(res.num_fil),ntohs(res.num_messages));
}


/* renvoie un header bien formaté avec le code et l'id en entrée*/
uint16_t new_header(uint8_t code, uint16_t i){
    uint8_t codereq=code;
    uint16_t id=i;  
    uint16_t h = codereq | id << 5;
    return htons(h);
}

/* fonction d'envoi de l'inscription*/
void send_inscription(int sock){
    inscription i;
    memset(&i, 0, sizeof(i));
    i.entete.val=new_header(1,0);
    char* p=ask_name();
    strncpy(i.pseudo, p, 10);
    free(p);
    
    uint16_t i_size = sizeof(i) - sizeof(i.pseudo) + strlen(i.pseudo) + 1;

    // creer un buffer serializé
    char* serialized_i = malloc(i_size);
    if (serialized_i == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour la sérialisation");
        exit(EXIT_FAILURE);
    }

    // Copier les valeurs de la structure inscription vers serialized_i
    memcpy(serialized_i, &(i.entete.val), sizeof(i.entete.val));
    memcpy(serialized_i + sizeof(i.entete.val), &(i.pseudo), strlen(i.pseudo) + 1);

    ssize_t sent = send(sock, serialized_i, i_size, 0);
    if (sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
}

/*fonction pour recevoir une réponse du serveur*/
reponse recv_reponse(int sock){
    reponse res;
    ssize_t received = recv(sock, &res, sizeof(res), 0);
    if (received < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*gestion du codereq 2*/
void post_message_fil(int sock){
    // Récupération des données
    uint16_t id = ask_id();
    printf("vous avez entré l'id : %d\n",id);
    u_int16_t fil = ask_fil();
    printf(" vous avez demandé le fil: %d\n",fil);
    fflush(stdout); 
    getchar(); 
    char* message = ask_message(); 
    printf("votre message : %s\n", message);
    
    // Sérialisation des données
    uint8_t longueur = strlen(message);
    uint8_t* mess = malloc((longueur+1) * sizeof(uint8_t));
    memcpy(mess, message, longueur+1);
    
    requete req;
    memset(&req, 0, sizeof(req));
    req.entete.val = new_header(2, id);
    req.num_fil = htons(fil);
    req.num_messages = htons(1);
    req.data = mess;
    uint16_t req_size = sizeof(req) + longueur + 1 +sizeof(longueur);
    
    char* serialized_req = malloc(req_size * sizeof(char));
    memcpy(serialized_req, &(req.entete.val), sizeof(req.entete.val));
    memcpy(serialized_req + sizeof(req.entete), &(req.num_fil), sizeof(req.num_fil));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil), &(req.num_messages), sizeof(req.num_messages));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages), &(longueur), sizeof(longueur));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages) + sizeof(longueur), req.data, longueur+1);
    int sent = send(sock, serialized_req, req_size, 0);
    if (sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    
    // Libération de la mémoire allouée
    free(serialized_req);
    free(mess);
}

/* fonction pour réceptionner tous les messages envyés par le srveur pour le codereq 3*/
void recv_messages(int sock,int nb){
     char *buffer = malloc(sizeof(char)*1024);
    size_t taille = 1024;
    size_t total_recu = 0;
    size_t recu = 0;

    while ((recu = recv(sock, buffer + total_recu, BUFFER_SIZE, 0)) > 0) {
    total_recu += recu;
    if (total_recu + BUFFER_SIZE > taille) {
        taille *= 2;
        buffer = realloc(buffer, taille);/*on realloc si la taille dépasse*/
        if (!buffer) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }
    }

    if (recu == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    buffer[total_recu] = '\0';/*ajoute un caratère de fin*/

    int index = 0;
    for(int i=0; i<nb; i++){
        // Désérialise les données depuis le buffer
        uint16_t numfil;
        char origine[10];
        char pseudo[10];
        uint8_t datalen;
        char data[4096];
        memcpy(&numfil, buffer + index, sizeof(numfil));
        memcpy(origine, buffer + index + sizeof(numfil), sizeof(origine));
        memcpy(pseudo, buffer + index + sizeof(numfil) + sizeof(origine), sizeof(pseudo));
        memcpy(&datalen, buffer + index + sizeof(numfil) + sizeof(origine) + sizeof(pseudo), sizeof(datalen));
        memcpy(data, buffer + index + sizeof(numfil) + sizeof(origine) + sizeof(pseudo) + sizeof(datalen), datalen);
        data[datalen] = '\0';

        // Incrémente l'index pour passer au prochain message dansle buffer
        index += sizeof(numfil) + sizeof(origine) + sizeof(pseudo)+ sizeof(datalen) + datalen;

        deformatage(origine,'#');
        deformatage(pseudo,'#');
        // Affiche les données
        printf("------------------------------------------------------\n");
        printf("NUMFIL: %d\n", ntohs(numfil));
        printf("Créateur: %.*s \n", 10, origine);
        printf("<<%.*s>>: '%s'\n", 10, pseudo,data);
        //printf("data: %s\n", data);
    }

    // Libère la mémoire allouée pour le buffer
    free(buffer);
}


void recv_messages_fil(int sock,requete r){
    reponse res= recv_reponse(sock);
    affiche_reponse(res);
    if(res.entete.val== r.entete.val){
        if (r.num_fil==0){/*pour le fil=0*/
            printf("Nombre de messages que vous allez recevoir: %d\n\n",ntohs(res.num_messages));
            int nb_total=ntohs(res.num_messages);
            recv_messages(sock,nb_total);
        }else if(res.num_fil== r.num_fil){ /* pour le fil >0*/
            if(r.num_messages > res.num_messages){
                printf("Nombre de messages dans le fil:%d\n\n",ntohs(res.num_messages));
            }
            int nb_dans_fil=ntohs(res.num_messages);
            recv_messages(sock,nb_dans_fil);
        }
    }
}

/*fonction pour demander la liste des n derniers messages*/
void ask_messages_fil(int sock){
    uint16_t id= ask_id();
    printf("ID: %d\n",id);
    u_int16_t fil= ask_fil();
    printf(" FIL: %d\n",fil);
    u_int16_t nb= ask_nb();
    printf(" Nombre de messages à recevoir: %d\n",nb);
    requete req;
    memset(&req,0,sizeof(req));
    req.entete.val=new_header(3,id);
    req.num_fil=htons(fil);
    req.num_messages=htons(nb);
    uint8_t* mess= malloc((2)* sizeof(uint8_t));
    mess[0]=0;
    mess[1]=0;
    req.data=mess;
    int sent = send(sock, &req, sizeof(req) + 2, 0);
    if (sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    recv_messages_fil(sock,req);
}


/*fonction pour s'abonner à un fil*/
void subscribe_to_feed(int sock){
    requete req;
    memset(&req, 0, sizeof(req));
    uint16_t idUser = ask_id();
    printf("id : %d\n", idUser);
    uint16_t nF = ask_fil();
    printf("fil : %d\n", nF);
    req.entete.val = new_header(4, idUser);
    req.num_fil = htons(nF);


    //envoi du message au serveur
    int envoi = send(sock, &req, sizeof(req), 0);
    if(envoi < 0){
        perror("send subscribe");
        exit(4);
    }

    //recevoir la reponse du serveur
    reponse res = recv_reponse(sock);
    //cas erreur
    /*if(res.entete.val != new_header(4, idUser)){
        recv_erreur("Echec abonnement");
        return;
    }*/
    if(res.entete.val != req.entete.val){
        printf("Valeurs dans subscribe\n");
        printf("entete val %d\n", req.entete.val);
        printf("req.num_fil : %d\n", req.num_fil);
        recv_erreur("Echec abonnement");
        return;
    }
    //cas succes donc le client s'abonne
    char addr_multi[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(res.num_fil), addr_multi, INET6_ADDRSTRLEN);
    printf("Adresse multidiffusion : %s\n", addr_multi);
    //inet_ntop(AF_INET6, &(res.num_fil), addr_multi, INET6_ADDRSTRLEN);

    int multiSocket = socket(AF_INET6, SOCK_DGRAM, 0);
    if(multiSocket < 0){
        perror("socket UDP");
        exit(4);
    }
    struct sockaddr_in multiReq;
    memset(&multiReq, 0, sizeof(multiReq));
    multiReq.sin_family = AF_INET6;
    multiReq.sin_port = htons(PORT_SERVEUR);
    multiReq.sin_addr.s_addr = inet_addr(addr_multi);
    int sso = setsockopt(multiSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&multiReq, sizeof(multiReq));
    if(sso<0){
        perror("setsockopt");
        exit(4);
    }

    //reception des messages
    char buffer[MSG_MAX];
    while(1){
        ssize_t nb = recv(sock, buffer, MSG_MAX, 0);
        if(nb < 0){
            perror("recv");
            exit(4);
        }else if(nb == 0){
            break; //serveur ayant fermé la connexion
        }
        buffer[nb] = '\0';
        printf("Message diffusé reçu : %s\n", buffer);

        //traiter les activités en parallèle du client
       /* fonctionnalite mut; //initialisation mutex
        pthread_mutex_init(&mut.mutexInscription, NULL);
        pthread_mutex_init(&mut.mutexPosterBillet, NULL);
        pthread_mutex_init(&mut.mutexDemandeNBillet, NULL);
        pthread_mutex_init(&mut.mutexAjoutFichier, NULL);
        pthread_mutex_init(&mut.mutexTelechargement, NULL);

        pthread_t recvThread, inscripThread, postThread, demandeThread, ajoutThread, telechargementThread;
        pthread_create(&recvThread, NULL, recv_messages_fil, (void *)sock);
        pthread_create(&inscripThread, NULL, send_inscription, (void *)sock);
        pthread_create(&postThread, NULL, post_message_fil, (void *)sock);
        pthread_create(&demandeThread, NULL, ask_messages_fil, (void *)sock);
        pthread_create(&ajoutThread, NULL, AjoutFichier, (void *)sock);
        pthread_create(&telechargementThread, NULL, download_fichier, (void *)sock);

        //patienter jusqua fin des threads
        pthread_join(recvThread, NULL);
        pthread_join(inscripThread, NULL);
        pthread_join(postThread, NULL);
        pthread_join(demandeThread, NULL);
        pthread_join(ajoutThread, NULL);
        pthread_join(telechargementThread, NULL);

        //detruire les mutex
        pthread_mutex_destroy(&mut.mutexInscription);
        pthread_mutex_destroy(&mut.mutexPosterBillet);
        pthread_mutex_destroy(&mut.mutexAjoutFichier);
        pthread_mutex_destroy(&mut.mutexDemandeNBillet);
        pthread_mutex_destroy(&mut.mutexTelechargement);*/


    }
    close(multiSocket);
}

/*fonction pour envoyer les paquets d'un fichier*/
void AjoutFichier(int port, char* nom_fichier,int id){

    /*on configure le port*/
   int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sock == -1){
        perror("socket envoi");
        exit(1);
    }
    
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    inet_pton(AF_INET6, IP_SERVEUR6, &addr.sin6_addr);

   /* Ouverture du fichier à envoyer en paquets */
    int file = open(nom_fichier, O_RDONLY);
    if (file < 0) {
        printf("ouverture fichier.\n");
        exit(1);
    }

    int sent = sendto(sock, nom_fichier, strlen(nom_fichier), 0,(struct sockaddr*)&addr , sizeof(addr));
    if(sent < 0){
        perror(" envoi du nom du fichier ");
        exit(5);
    }

    char buffer[SIZE_BUF+1];
    int num = 0;
    int lu = 0;
    
    /*on lit successivement des paquets de 512 et on les envoie dans la structure*/
    while ((lu = read(file, buffer, SIZE_BUF)) > 0) {
        buffer[lu]=0;
        num++;
        paquet_udp paquet;
        memset(&paquet,0,sizeof(paquet));
        paquet.entete.val=new_header(5,id);
        paquet.numbloc=htons(num);
        strcpy(paquet.data,buffer);
        char* ser_paquet = malloc(sizeof(paquet.entete) + sizeof(paquet.numbloc)+lu);
        memcpy(ser_paquet, &(paquet.entete), sizeof(paquet.entete));
        memcpy(ser_paquet + sizeof(paquet.entete), &(paquet.numbloc), sizeof(paquet.numbloc));
        memcpy(ser_paquet + sizeof(paquet.entete) + sizeof(paquet.numbloc), buffer, lu);
        int sent = sendto(sock, ser_paquet,sizeof(paquet.entete) + sizeof(paquet.numbloc)+lu+1, 0,(struct sockaddr*)&addr , sizeof(addr));
        if(sent < 0){
            perror("send ajout fichier");
            exit(5);
        }

        free(ser_paquet);
    }
    /*si le dernier paquet etat de longueur 512 on en renvoie un dernier vide*/
      if(lu==512){
        num++;
        paquet_udp paquet;
        memset(&paquet,0,sizeof(paquet));
        paquet.entete.val=new_header(5,id);
        paquet.numbloc=htons(num);
        strcpy(paquet.data,"");
        char* ser_paquet = malloc (sizeof(paquet));
        memcpy(ser_paquet, &(paquet.entete.val), sizeof(paquet.entete.val));
        memcpy(ser_paquet + sizeof(paquet.entete), &(paquet.numbloc), sizeof(paquet.numbloc));
        memcpy(ser_paquet + sizeof(paquet.entete)+sizeof(paquet.numbloc) , "", 0);

        int d = sendto(sock, ser_paquet, sizeof(paquet), 0,(struct sockaddr*)&addr , sizeof(addr));
        if(d < 0){
            perror("send ajout fichier");
            exit(5);
        }
        free(ser_paquet);
    }
    close(file);
}

/*fonction d'ajout de fichiers*/
void upload_fichier(int sock){
    uint16_t id= ask_id();
    printf("ID: %d\n",id);
    u_int16_t fil= ask_fil();
    printf(" FIL: %d\n",fil);
    fflush(stdout); 
    getchar(); 
    char* nom_fichier= ask_file(); 
    printf("fichier à ajouter: %s\n",nom_fichier);
    int longueur=(strlen(nom_fichier));
    uint8_t* mess= malloc((longueur+2)* sizeof(uint8_t));
    mess[0]=longueur;
    memcpy(mess+1,nom_fichier,longueur);
    mess[longueur+1]=0;
    requete req;
    memset(&req,0,sizeof(req));
    req.entete.val=new_header(5,id);
    req.num_fil=htons(fil);
    req.num_messages=htons(0);
    req.data=mess;
    int sent = send(sock, &req, sizeof(req) + longueur, 0);
    if (sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    reponse res= recv_reponse(sock);
    affiche_reponse(res);
    close(sock);//on se déconnecte 
    // on envoie les paquets au port fourni
    AjoutFichier(ntohs(res.num_messages),nom_fichier,id);
}

 // fonction pour télécharger un fichier
void download_fichier(int sock){
    uint16_t id= ask_id();
    printf("ID: %d\n",id);
    u_int16_t fil= ask_fil();
    printf(" FIL: %d\n",fil);
    fflush(stdout); 
    getchar(); 
    char* nom_fichier= ask_file(); 
    printf("fichier à télécharger: %s\n",nom_fichier);
     uint8_t longueur = strlen(nom_fichier);
    uint8_t* mess = malloc((longueur+1) * sizeof(uint8_t));
    memcpy(mess, nom_fichier, longueur+1);
    
    requete req;
    memset(&req, 0, sizeof(req));
    req.entete.val = new_header(6, id);
    req.num_fil = htons(fil);
    req.num_messages = htons(PORT_DOWNLOAD);
    req.data = mess;
    uint16_t req_size = sizeof(req) + longueur + 1 +sizeof(longueur);
    
    char* serialized_req = malloc(req_size * sizeof(char));
    memcpy(serialized_req, &(req.entete.val), sizeof(req.entete.val));
    memcpy(serialized_req + sizeof(req.entete), &(req.num_fil), sizeof(req.num_fil));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil), &(req.num_messages), sizeof(req.num_messages));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages), &(longueur), sizeof(longueur));
    memcpy(serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages) + sizeof(longueur), req.data, longueur+1);
    int sent = send(sock, serialized_req, req_size, 0);    if (sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    //serveur accepte sa demande
    reponse res = recv_reponse(sock);
    affiche_reponse(res);
    if(res.entete.val != new_header(6, id)){
        recv_erreur("Echec du téléchargement");
        exit(6);
    }

    //Reception du fichier en UDP
    int sockudp;
    struct sockaddr_in6 addrudp, clientudp;
    socklen_t clientudp_len = sizeof(clientudp);
    
    sockudp = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockudp < 0) {
        perror("socket udp");
        return;
    }
    addrudp.sin6_family = AF_INET6;
    addrudp.sin6_port = htons(PORT_DOWNLOAD);
    addrudp.sin6_addr = in6addr_any;

    if (bind(sockudp, (struct sockaddr*)&addrudp, sizeof(addrudp)) < 0) {
    perror(" bind socket udp");
    return;
    }

    char* r="client_fichiers/";
    char* n=malloc(strlen(r)+strlen(nom_fichier)*sizeof(char));
    strcpy(r,"");
    strcat(n,r);
    strcat(n,nom_fichier);
    printf("Nouveau chemin du fichier %s\n",n);/*on met le fichier dans un répertoire prévu pour*/
        
    char buffer[1024];
    int lu=0;
    int taille=0;

     /*ouverture du fichier a sauvegarde */
    int file = open(n, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file<0) {
        printf("ouverture fichier.\n");
        exit(1);
    }

    /*tant q'uon a des paquets de plus de 512 on ecrit dans la sauvegarde*/
    while ((lu = recvfrom(sockudp, buffer, sizeof(paquet_udp), 0, (struct sockaddr*)&clientudp,&clientudp_len)) >= 512) {
        taille+=lu-4;        
        paquet_udp p;
        memset(&p, 0, sizeof(paquet_udp));
        memcpy(&(p.entete.val), buffer, sizeof(p.entete));
        memcpy(&(p.numbloc), buffer + sizeof(p.entete), sizeof(p.numbloc));
        memcpy(p.data, buffer + sizeof(p.entete) +sizeof(p.numbloc), sizeof(p.data));
        write(file, p.data, strlen(p.data));
    }

    if(lu>4){/*on finit par écrire le dernier paquet sil n'est pas vide*/
    taille+=lu-4;        
    buffer[lu]=0;
    paquet_udp p;
    memset(&p, 0, sizeof(paquet_udp));
    memcpy(&(p.entete.val), buffer, sizeof(p.entete));
    memcpy(&(p.numbloc), buffer + sizeof(p.entete), sizeof(p.numbloc));
    memcpy(p.data, buffer + sizeof(p.entete) +sizeof(p.numbloc),  lu - (sizeof(p.entete) + sizeof(p.numbloc)+1));
    write(file, p.data,  lu - (sizeof(p.entete) + sizeof(p.numbloc)+1));
    }

    printf("Téléchargement terminé\n");

    close(file);
    free(req.data);
    close(sockudp);
}


int main(){
     //créaton de la socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket");
        exit(1);
    }
    
    //préparer IP et port du destinataire (serveur)
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_SERVEUR);
    inet_pton(AF_INET, IP_SERVEUR, &addr.sin_addr);

    //demande de connexion au serveur
    int c = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(c == -1){
        perror("connect");
        exit(2);
    }

    int action=0;
    printf("que souhaitez vous faire? : \n");
    printf("\n 1: {inscription}\n");
    printf(" 2: {poster un message}\n");
    printf(" 3: {demander les n derniers messages d'un fil}\n");
    printf(" 4: {s'abonner à un fil}\n");
    printf(" 5: {ajouter un fichier sur un fil}\n");
    printf(" 6: {télécharger un fichier}\n\n");
    scanf("%d",&action);

    switch (action)
    {
    case 1:
        send_inscription(sock);
        affiche_reponse(recv_reponse(sock));
        break;
    
    case 2:
        post_message_fil(sock);
        affiche_reponse(recv_reponse(sock));
        break;

    case 3:
        ask_messages_fil(sock);
        break;
    
    case 4:
        subscribe_to_feed(sock);
        break;
    
    case 5:
        upload_fichier(sock);
        break;

    case 6:
        download_fichier(sock);
        break;

    default:
        printf("requete inconnue\n");
        break;
    }


    //fermer la socket
    close(sock);

    return 0;
}