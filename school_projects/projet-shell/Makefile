CC = gcc 
CFLAGS= -Wall -g 
INCLUDES= -I./libraries
CCLINK=-lreadline
ALL=slash

.PHONY: clean

all : $(ALL)


slash: src/utilitaires.c src/pipeLine.c  src/redirection.c src/etoileSimple.c src/slash.c src/Tronqueur.c src/my-pwd.c src/external.c 
		$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(CCLINK)
	

clean :
	 rm -rf slash.o 
	 rm -rf utilitaires.o
	 rm -rf slash
	 rm -rf Tronqueur.o
	 rm -rf redirection.o
	 rm -rf pipeLine.o
	 rm -rf etoileSimple.o
	 rm -rf external.o
	 rm -rf my-pwd.o
