#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <arpa/inet.h>

int traitement(int fd, char *mess){

	// On commence à écouter sur la socket. Le 5 est le nombre max
	// de connexions pendantes

		int newfd;
		int n;
		char buffer[256];
		socklen_t clilen;
		struct sockaddr_in cli_addr;
     	listen(fd,5);
     	while (1)
     	{    
     		newfd = accept(fd, (struct sockaddr *) &cli_addr, &clilen);
     		if (newfd < 0) 
		{
         		fprintf(stderr,"Impossible de faire l'appel système accept().\n");
			return 1;
		}

     		bzero(buffer,256);
     		n = read(newfd,buffer,255);
     		if (n < 0) 
		{
         		fprintf(stderr,"Impossible de faire l'appel système read().\n");
			return 1;
		}

        	printf("Received packet from %s:%d\nData: [%s]\n\n",
                	inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);

     		close(newfd);
     	}

     	close(fd);
     	return 0; 
}

void serveurTcp(void)
{
     	int sockfd, portno;
     	struct sockaddr_in serv_addr;
     	int n;

	// 1) on crée la socket, SOCK_STREAM signifie TCP
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0) 
	{
         	fprintf(stderr,"Impossible d'ouvrir la socket.\n");
		return;
	}

	// 2) on réclame au noyau l'utilisation du port passé en paramètre
	// INADDR_ANY dit que la socket va être affectée à toutes les interfaces locales

     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	//portno = atoi(argv[1]);
     	portno = 32000;		// A changer hein
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
     	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
         	fprintf(stderr,"Impossible de faire l'appel système bind().\n");
		return;
	}
	
	traitement(sockfd, "Connexion");
	return;

}


int main(int argc, char *argv[]){

	/*
	if (argc < 2) 
	{
         	fprintf(stderr,"./server <numport>\n");
         	fprintf(stderr,"ex: ./server 32000\n");
		return 1;
    	}*/
    	
    serveurTcp();

}

















