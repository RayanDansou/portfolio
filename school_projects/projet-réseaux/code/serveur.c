#include "serveur.h"

Client clients[MAX_CLIENTS];
int num_clients = 0;

Fil fils[MAX_CLIENTS];
int num_fils = 0;


//Fonction pour bien formater le header
uint16_t new_header(int id,uint8_t code){
    uint8_t codereq=code;
    uint16_t i=id;  
    uint16_t h = codereq | i << 5;
    return htons(h);
}


/*cette fonction change un chemin vers un fichier en juste la nom du fichier*/
char* get_nom_fichier(char* fic) {
     char* chr = strrchr(fic, '/');
    if (chr != NULL) {
        return chr + 1;
    } else {
        return fic;
    }
}

/*fonction de traitement des inscriptions*/
void treat_inscription(int sockclient, inscription r) {
    char* pseudo=r.pseudo;
    if (strlen(pseudo) < 10) {/* si le pseudo trop court*/
        treat_error(sockclient);
        return;
    }

    /* on augmente le combre de clients et on insère les informations recues*/
    int id = num_clients + 1;
    strcpy(clients[id].pseudo, pseudo);
    clients[id].id = id;
    clients[id].pseudo[10]=0;
    num_clients++;
    /*on affcihe et on envoie la reponse au client*/
    printf("\nInformations du nouveau Client\nID: %d  PSEUDO:%s \n",clients[id].id ,clients[id].pseudo);
    reponse res;
    res.entete.val=new_header(num_clients,1);
    res.num_fil=htons(0);
    res.num_messages=htons(0);
    send(sockclient,&res,sizeof(res),0);

}

/*fonction pour la gestion des erreurs*/
void treat_error(int sockclient){
    reponse res;
    res.entete.val = new_header(31, 0);
    res.num_fil=htons(0);
    res.num_messages=htons(0);
    ssize_t nb = send(sockclient, &res, sizeof(res), 0);
    if(nb < 0){
        perror("send");
        exit(1);
    }
}


/*cette fonction affiche les informations d'un fil et son contenu*/
void affiche_fil(int k) {
    if (k < 0 || k > num_fils) {
        printf("fil avec l'ID %d introuvable.\n", k);
        return;
    }

    Fil fil = fils[k];
    printf("\n\nfil ID: %d\n", fil.id_fil);
    printf("createur du fil: %s\n", fil.nom_fil);
    printf("Nombre de messages dans le fil: %d\n", fil.num_messages);

    printf("Messages:\n");
    for (int i = 0; i < fil.num_messages; i++) {
        Message message = fil.messages[i];
        printf("\n-------------------------\n");
        printf("originee: %s\n", fil.nom_fil);
        printf("<<%s>>: '%s'\n\n" ,message.pseudo, message.data);
    }
}


/*fonction pour traiter le post de message*/
void treat_post(int sockclient,entete e,requete r){

    u_int16_t header=ntohs(r.entete.val);
    uint16_t id= header >> 5;
    u_int16_t n= ntohs(r.num_fil);
   
    if(n<0 || n>num_fils || id<=0 || id>num_clients){
        printf("ERREUR: numfil ou id inexistant\n");
        treat_error(sockclient);
        return;

    }

    if (n == 0) { //, si le client demande un nouveau fil on incremente juste le nombre de fils
        num_fils++;// et on y ajoute le message
        n = num_fils;
        fils[n].id_fil = n;
        strcpy(fils[n].nom_fil,clients[id].pseudo);
        strcpy(fils[n].messages[0].pseudo, clients[id].pseudo);
        fils[n].messages[0].pseudo[10]=0;
        fils[n].nom_fil[10]=0;
        strcpy(fils[n].messages[0].data, (char*)r.data);
        fils[n].num_messages = 1;
        printf("%s\n", clients[id].pseudo);
    } else {// sinon on ajoute le message au fil existant
        int num_messages = fils[n].num_messages;
        strcpy(fils[n].messages[num_messages].pseudo, clients[id].pseudo);
        strcpy(fils[n].messages[num_messages].data,(char*) r.data);
        fils[n].num_messages++;
    }
   
    affiche_fil(num_fils);
    // on enoie la reponse au serveur
    reponse res;
    res.entete.val=new_header(id,2);
    res.num_fil=htons(num_fils);
    res.num_messages=htons(0);
    send(sockclient,&res,sizeof(res),0);

}


/*fontion pour la gestion de la requete 2*/
void send_last_messages(int sockclient, int k, int n) {
    if (k < 0 || k > num_fils) {
        printf("fil avec l'ID %d inexistant.\n", k);
        treat_error(sockclient);
        return;
    }

    /*on affiche les informations du fil visé*/
    Fil fil = fils[k];
    printf("\n\nfil ID: %d\n", fil.id_fil);
    printf("Createur du fil: %s\n", fil.nom_fil);
    printf("Nombre de messages: %d\n\n\n", fil.num_messages);

    int start_index = (fil.num_messages >= n) ? fil.num_messages - n : 0;/*on cherche l'index du message ou commencer*/
    for (int i = start_index; i < fil.num_messages; i++) {
        Message message = fil.messages[i];
        serveur_message serv_message;
        serv_message.num_fil = htons(fil.id_fil);
        strncpy(serv_message.origine, fil.nom_fil, sizeof(serv_message.origine) - 1);
        strncpy(serv_message.pseudo, message.pseudo, sizeof(serv_message.pseudo) - 1);
        serv_message.datalen = strlen(message.data);
        serv_message.data = message.data;

        uint16_t num_fil_network = serv_message.num_fil;
        uint8_t datalen_network = serv_message.datalen;

        size_t message_size = sizeof(num_fil_network) + sizeof(serv_message.origine) +
                              sizeof(serv_message.pseudo) + sizeof(datalen_network) +
                              datalen_network;

        char* buffer = (char*)malloc(message_size);
        if (buffer == NULL) {
            printf("Erreur lors de l'allocation du buffer.\n");
            return;
        }
        /*on serialise chaque message*/
        char* ptr = buffer;
        memcpy(ptr, &num_fil_network, sizeof(num_fil_network));
        ptr += sizeof(num_fil_network);
        memcpy(ptr, serv_message.origine, sizeof(serv_message.origine));
        ptr += sizeof(serv_message.origine);
        memcpy(ptr, serv_message.pseudo, sizeof(serv_message.pseudo));
        ptr += sizeof(serv_message.pseudo);
        memcpy(ptr, &datalen_network, sizeof(datalen_network));
        ptr += sizeof(datalen_network);
        memcpy(ptr, serv_message.data, datalen_network);

        send(sockclient, buffer, message_size, 0);

        free(buffer);
    }
}



/*fonction pour traiter les demandes de derniers messages*/
void treat_ask(int sockclient,requete r){
    u_int16_t header=ntohs(r.entete.val);
    uint16_t id= header >> 5;
    u_int16_t n= ntohs(r.num_fil);
    printf("numfil souhaité: %d\n",n);
    u_int16_t nb= ntohs(r.num_messages);
    printf("nb messages souhaités: %d\n",nb);

    if(n<0 || n>num_fils || id<=0 || id>num_clients){
        printf("ERREUR: numfil ou id inexistant\n");
        treat_error(sockclient);
        return;
    }

    int req_nb;

    if(n>0){ /*si le fil est positif */
        int num_messages = fils[n].num_messages;
        if(nb>num_messages){
            req_nb=num_messages;
        }else{
            req_nb=nb;
        }
        reponse res;
        res.entete.val=new_header(id,3);
        res.num_fil=htons(n);
        res.num_messages=htons(req_nb);
        send(sockclient,&res,sizeof(res),0);
        send_last_messages(sockclient,n,req_nb);
    }else{/*si numfil est 0 on fait pourtous les fils le nombe demandé*/
        int req_num=num_fils;
        int num_messages=0;
        for(int i=1;i<=num_fils;i++) num_messages+= fils[i].num_messages;
         printf("num %d\n",num_messages);
        if(nb>num_messages){
            req_nb=num_messages;
        }else{
            req_nb=nb;
        }
        reponse res;
        res.entete.val=new_header(id,3);
        res.num_fil=htons(req_num);
        res.num_messages=htons(req_nb);
        send(sockclient,&res,sizeof(res),0);
        int k=0;
        for(int n=0;n<= num_fils;n++){/* on envoie les messages jusq'a atteindre le nombre max/demandé*/
        if(k<req_nb){
        send_last_messages(sockclient,n,fils[n].num_messages);
        k+=fils[n].num_messages;
        }else{break;}
        }

    }

}


/*fonction pour traiter les abonnements*/
void treat_subscribe(int sockclient, requete res){
    uint16_t header = ntohs(res.entete.val);
    uint16_t numFil = ntohs(res.num_fil);
    printf("abonnement au fil %d\n", numFil);

    if(numFil < 0){
        treat_error(sockclient);
        return;
    }

    int multiSocket = socket(AF_INET6, SOCK_DGRAM, 0);
    if(multiSocket < 0){
        treat_error(sockclient);
    }

    //adresse de multidiffusion
    struct sockaddr_in6 multiReq;
    memset(&multiReq, 0, sizeof(multiReq));
    multiReq.sin6_family = AF_INET6;
    multiReq.sin6_port = htons(PORT_SERVEUR);
    inet_pton(AF_INET6, multiReq.sin6_addr.s6_addr, &multiReq.sin6_addr);

    int sso = setsockopt(multiSocket, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&multiReq, sizeof(multiReq));
    if(sso < 0){
        treat_error(sockclient);
    }

    reponse rep;
    rep.entete = res.entete;
    rep.num_fil = res.num_fil;
    rep.num_messages = res.num_messages;
    /*rep.entete.val = new_header(id, 4);
    rep.num_fil = htons(numFil);
    rep.num_messages = res.num_messages;*/
    printf("Valeurs dans treat \n");
    printf("header : %d\n", header);
    printf("numFil : %d\n", numFil);

    send(sockclient, &rep, sizeof(rep), 0);

}



/*fonction de reception des paquets pour l'ajout de fichiers*/
void recv_dataudp(char* nom_fichier, int sockudp,int id, int numfil,struct sockaddr* clientudp,socklen_t clientudp_len){
    char* r="serveur_fichiers/";
    char* n=malloc(strlen(r)+strlen(nom_fichier)*sizeof(char));
    strcat(n,r);
    strcat(n,nom_fichier);
    printf("Nouveau chemin du fichier %s\n",n);/*on met le fichier dans un répertoire prévu pouer*/
        
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
    while ((lu = recvfrom(sockudp, buffer, sizeof(paquet_udp), 0, clientudp, &clientudp_len)) >= 512) {
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
    close(file);


    /*on ajoute un  billet contenant le nom du fichier et sa taille dans le fil souhaité*/
    char* q = malloc(strlen(nom_fichier) + taille + 1);
    strcpy(q, nom_fichier);
    sprintf(q + strlen(q), " %d", taille);
    if (numfil == 0) {/*si numfil est 0 on met sur un nouveau fil*/
    num_fils++;
    int n = num_fils;
    fils[n].id_fil = n;
    strcpy(fils[n].nom_fil,clients[id].pseudo);
    strcpy(fils[n].messages[0].pseudo, clients[id].pseudo);
    fils[n].messages[0].pseudo[10]=0;
    fils[n].nom_fil[10]=0;
    strcpy(fils[n].messages[0].data,q);
    fils[n].num_messages = 1;
    } else {/* sinon on le mets sur le num demandé*/
        int num_messages = fils[numfil].num_messages;
        strcpy(fils[numfil].messages[num_messages].pseudo, clients[id].pseudo);
        strcpy(fils[numfil].messages[num_messages].data,q);
        fils[numfil].num_messages++;
    }
   
   affiche_fil(num_fils);

}

/*fonction pour traiter les ajout de fichier*/
void treat_upload(int sockclient,requete r){
    u_int16_t header=ntohs(r.entete.val);
    uint16_t id= header >> 5;
    u_int16_t n= ntohs(r.num_fil);
    printf("numfil souhaité: %d\n",n);
    u_int16_t nb= ntohs(r.num_messages);
    printf("nb messages souhaités: %d\n",nb);

    if(n<0 || n>num_fils || id<=0 || id>num_clients){
        printf("ERREUR: numfil ou id inexistant\n");
        treat_error(sockclient);
        return;
    }
    /* on prépare la reponse a envoyer au serveur*/
    reponse res;
    res.entete.val=r.entete.val;
    res.num_fil=r.num_fil;
    res.num_messages=htons(PORT_UPLOAD);

    /*on prépare le port d'envoi*/
    int sockudp;
    struct sockaddr_in6 addrudp, clientudp;
    socklen_t clientudp_len = sizeof(clientudp);
    
    sockudp = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockudp < 0) {
        perror("socket udp");
        return;
    }
    addrudp.sin6_family = AF_INET6;
    addrudp.sin6_port = htons(PORT_UPLOAD);
    addrudp.sin6_addr = in6addr_any;
    
    /*le port doit pouvoir etre utilisé en parallèle*/
    int optval = 1;
    if (setsockopt(sockudp, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        exit(2);
    }
    
    if (bind(sockudp, (struct sockaddr*)&addrudp, sizeof(addrudp)) < 0) {
        perror(" bind socket udp");
        return;
    }

  
    /*on envoie la reponse et on passe a la reception du fichier*/
    send(sockclient,&res,sizeof(res),0);
    close(sockclient);

    char buffer[1024];
    int re = recvfrom(sockudp, buffer, sizeof(buffer), 0,(struct sockaddr*)&clientudp , &clientudp_len);
    buffer[re]=0;
    printf("chemin du fichier à ajouter : %s\n", buffer);
    printf("Nom du fichier à ajouter : %s\n", get_nom_fichier(buffer));
    char* buf= get_nom_fichier(buffer);
    recv_dataudp(buf,sockudp,id,ntohs(r.num_fil),(struct sockaddr*)&clientudp,clientudp_len);
}


/*fonction pour traiter les téléchargements*/
void treat_download(int sockclient,requete r){
    char* a="serveur_fichiers/";
    char* new=malloc((strlen(a)+strlen((char*)r.data)+1)*sizeof(char));
    strcpy(new,"");
    strcat(new,a);
    strcat(new,(char*)r.data);
    u_int16_t header=ntohs(r.entete.val);
    uint16_t id= header >> 5;
    u_int16_t n= ntohs(r.num_fil);
    printf("numfil souhaité: %d\n",n);
    u_int16_t nb= ntohs(r.num_messages);
    printf("nb messages souhaités: %d\n",nb);

    if(n<0 || n>num_fils || id<=0 || id>num_clients){
        printf("ERREUR: numfil ou id inexistant\n");
        treat_error(sockclient);
        return;
    }

    Fil f= fils[n];
    int contient=0;
    for(int i=0; i<f.num_messages;i++){/* on regarde si le fichier demandé apparait effectivement dans le fil*/
        if(strstr(f.messages[i].data,(char*)r.data)!=NULL){
            contient=1;
        }
    }

    if(contient==0){
        printf("Le fichier n'existe pas dans le fil %d\n",n);
        treat_error(sockclient);
        exit(1);
    }

    reponse res;/* on revoie une reponse positive avant le début de l'envoi des paquets*/
    res.entete.val=r.entete.val;
    res.num_fil=r.num_fil;
    res.num_messages=r.num_messages;

    send(sockclient,&res,sizeof(res),0);
    close(sockclient);

    int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sock == -1){
        perror("socket envoi");
        exit(1);
    }
    
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    int b=ntohs(r.num_messages);
    addr.sin6_port = htons(b);
    inet_pton(AF_INET6, IP_SERVEUR, &addr.sin6_addr);


    printf("%s\n",new);
    int file = open(new, O_RDONLY);
    if (file < 0) {
        printf("ouverture fichier.\n");
        exit(1);
    }

    char buffer[512+1];
    int num = 0;
    int lu = 0;
    
    /*on lit successivement des paquets de 512 et on les envoie dans la structure*/
    while ((lu = read(file, buffer, 512)) > 0) {
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
            perror("send dwonload fichier");
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


/*cette fonction déserialize une requete*/
requete deserialize_requete(const char* serialized_req) {
    requete req;
    memcpy(&(req.entete.val), serialized_req, sizeof(req.entete.val));
    memcpy(&(req.num_fil), serialized_req + sizeof(req.entete), sizeof(req.num_fil));
    memcpy(&(req.num_messages), serialized_req + sizeof(req.entete) + sizeof(req.num_fil), sizeof(req.num_messages));
    uint8_t longueur;
    memcpy(&longueur, serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages), sizeof(uint8_t));
    req.data = malloc((longueur+1) * sizeof(uint8_t));
    memcpy(req.data, serialized_req + sizeof(req.entete) + sizeof(req.num_fil) + sizeof(req.num_messages) + sizeof(uint8_t), (longueur + 1));
    return req;
}

/* cette fonction déserialise une structure inscription*/
inscription deserialize_inscription(const char* serialized_i){
    inscription i;
    memset(&i, 0, sizeof(i));
    memcpy(&(i.entete.val), serialized_i, sizeof(i.entete.val));
    memcpy(&(i.pseudo), serialized_i + sizeof(i.entete.val), sizeof(i.pseudo));

    return i;
}

/*fonction de traitement des requetes*/
void treat_request(void* arg){
    int sockclient = (intptr_t)arg;
    char buffer[BUFFER_SIZE];
    size_t received_bytes = 0;

    received_bytes = recv(sockclient, buffer, BUFFER_SIZE, 0);
    if(received_bytes==0){
        perror("reception");
        exit(1);
    }

    buffer[received_bytes] = '\0';
    
    requete req= deserialize_requete(buffer);// on déserialise la requete
    u_int16_t header=ntohs(req.entete.val);
    uint8_t codereq= header & 0x1F;
    inscription i;
    if(codereq==1){
     i= deserialize_inscription(buffer);
    }

    if(codereq==1){
        printf("\nCODEREQ recu: 1 {inscription}\n");
        treat_inscription(sockclient,i);
    
    }else{ 
        switch(codereq){
            case 2:
                printf("\nCODEREQ recu: 2 {poster un message}\n");
                treat_post(sockclient,req.entete,req);
                break;
            case 3:
                printf("\nCODEREQ recu: 3 {demande de derniers messages}\n");
                treat_ask(sockclient,req);
                break;
            case 4:
                printf("\nCODEREQ recu: 4 {abonnement}\n");
                treat_subscribe(sockclient, req);
                break;
            case 5:
                 printf("\nCODEREQ recu: 5 {ajout de fichier}\n");
                treat_upload(sockclient,req);
                break;
            case 6:
                 printf("\nCODEREQ recu: 6 {téléchargement}\n");
                treat_download(sockclient,req);
                break;
            default:
                printf("\nCODEREQ recu: %d {requte inconnue}\n",codereq);
                treat_error(sockclient);
                break;
        }

    }
        close(sockclient);

    
}



int main(){
    //création de la socket
    int sock = socket(PF_INET6, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket");
        exit(1);
    }

    //préparer l'adresse avec port d'écoute du serveur
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT_SERVEUR);
    addr.sin6_addr = in6addr_any;


    int ok = 0;
    int r = setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &ok, sizeof(ok));
    if (r < 0) perror("erreur connexion IPv4 impossible");

    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        exit(2);
    }

    //lier la socket serveur au port d'écoute
    int b = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(b<0){
        perror("bind");
        exit(2);
    }

    //préparer la socket serveur à recevoir des connexions
    int l = listen(sock, 0);
    if(l < 0){
        perror("listen");
        exit(2);
    }

 while (1) {
    struct sockaddr_in6 addrclient;
    memset(&addrclient, 0, sizeof(addrclient));
    socklen_t addrlen = sizeof(addrclient);
    int sockclient = accept(sock, (struct sockaddr *)&addrclient, &addrlen);
    if(sockclient == -1){
        perror("probleme socket client");
        exit(1);
    }

    printf("\n\nNouvelle Connexion entrante..\n\n");

    // on cree un thread par requete pour la gestion
    pthread_t fil;
    pthread_create(&fil, NULL, treat_request, (void*)(intptr_t)sockclient);
    pthread_detach(fil);

    }

    
    //fermer la socket serveur
    close(sock);
}