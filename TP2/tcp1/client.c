#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void sendCommand(char *ip, int portno, char *mess)
{
    	int sockfd, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;

    	char buffer[256];

    	//portno = atoi(argv[2]);
	// 1) Création de la socket, INTERNET et TCP

    	sockfd = socket(AF_INET, SOCK_STREAM, 0);

    	if (sockfd < 0) 
	{
		fprintf(stderr,"Impossible d'ouvrir la socket. On arrête tout.\n");
		return;
	}
    
	//server = gethostbyname(argv[1]);
	server = gethostbyname(ip);
    	if (server == NULL) 
	{
        	fprintf(stderr,"Impossible de récupérer les infos sur le serveur\n");
		return;
    	}

	// On donne toutes les infos sur le serveur
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);

	// On se connecte. L'OS local nous trouve un numéro de port, grâce auquel le serveur
	// peut nous renvoyer des réponses

    	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	{
		fprintf(stderr,"Impossible de faire l'appel system connect().\n");
		return;
	}

	//sprintf(buffer,"%s\n",argv[3]);
	sprintf(buffer,"%s\n",mess);
        n = write(sockfd,buffer,strlen(buffer));

	// On ferme la socket

    	close(sockfd);

    	return;
}

int main(int argc, char *argv[]){

	/*
	if (argc<4)
	{
		fprintf(stderr,"./client <server_address> <server_numport> <message>\n");
		fprintf(stderr,"ex: ./client localhost 32000 \"joe\"\n");
		return 1;
	}*/

	sendCommand("134.157.104.70", 32000, "\n\n\n\t ------ Attention ------\nVous avez ete detecter comme faisant partie d une organisation malfaisante.Chacun de vos actes sera enregistre et utilise contre vous si tout acte dangereux est detecte sous votre nom.\n\n\n");
	return 1;
}
























