CC=gcc #Le compilateur à utiliser
TARGET=client #Nom du fichier à créer
TARGET2= serveur


all:
	$(CC) -o $(TARGET) -Wall -g  -pthread client.c
	$(CC) -o $(TARGET2) -Wall -g -pthread serveur.c
	
distclean:
	rm $(TARGET)
	rm $(TARGET2)