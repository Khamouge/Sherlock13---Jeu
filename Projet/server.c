




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#include <netdb.h>
#include <arpa/inet.h>

struct _client
{
        char ipAddress[40];
        int port;
        char name[40];
} tcpClients[4];
int nbClients;
int fsmServer;
int deck[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
int tableCartes[4][8];
char *nomcartes[]=
{"Sebastian Moran", "irene Adler", "inspector Lestrade",
  "inspector Gregson", "inspector Baynes", "inspector Bradstreet",
  "inspector Hopkins", "Sherlock Holmes", "John Watson", "Mycroft Holmes",
  "Mrs. Hudson", "Mary Morstan", "James Moriarty"};
int joueurCourant;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void melangerDeck()
{
        int i;
        int index1,index2,tmp;

        for (i=0;i<1000;i++)
        {
                index1=rand()%13;
                index2=rand()%13;

                tmp=deck[index1];
                deck[index1]=deck[index2];
                deck[index2]=tmp;
        }
}

void createTable()
{
	// Le joueur 0 possede les cartes d'indice 0,1,2
	// Le joueur 1 possede les cartes d'indice 3,4,5 
	// Le joueur 2 possede les cartes d'indice 6,7,8 
	// Le joueur 3 possede les cartes d'indice 9,10,11 
	// Le coupable est la carte d'indice 12
	int i,j,c;

	for (i=0;i<4;i++)
		for (j=0;j<8;j++)
			tableCartes[i][j]=0;

	for (i=0;i<4;i++)
	{
		for (j=0;j<3;j++)
		{
			c=deck[i*3+j];
			switch (c)
			{
				case 0: // Sebastian Moran
					tableCartes[i][7]++;
					tableCartes[i][2]++;
					break;
				case 1: // Irene Adler
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					tableCartes[i][5]++;
					break;
				case 2: // Inspector Lestrade
					tableCartes[i][3]++;
					tableCartes[i][6]++;
					tableCartes[i][4]++;
					break;
				case 3: // Inspector Gregson 
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					tableCartes[i][4]++;
					break;
				case 4: // Inspector Baynes 
					tableCartes[i][3]++;
					tableCartes[i][1]++;
					break;
				case 5: // Inspector Bradstreet 
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					break;
				case 6: // Inspector Hopkins 
					tableCartes[i][3]++;
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					break;
				case 7: // Sherlock Holmes 
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][2]++;
					break;
				case 8: // John Watson 
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					tableCartes[i][2]++;
					break;
				case 9: // Mycroft Holmes 
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][4]++;
					break;
				case 10: // Mrs. Hudson 
					tableCartes[i][0]++;
					tableCartes[i][5]++;
					break;
				case 11: // Mary Morstan 
					tableCartes[i][4]++;
					tableCartes[i][5]++;
					break;
				case 12: // James Moriarty 
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					break;
			}
		}
	}
} 

void printDeck()
{
        int i,j;

        for (i=0;i<13;i++)
                printf("%d %s\n",deck[i],nomcartes[deck[i]]);

	for (i=0;i<4;i++)
	{
		for (j=0;j<8;j++)
			printf("%2.2d ",tableCartes[i][j]);
		puts("");
	}
}

void printClients()
{
        int i;

        for (i=0;i<nbClients;i++)
                printf("%d: %s %5.5d %s\n",i,tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        tcpClients[i].name);
}

int findClientByName(char *name)
{
        int i;

        for (i=0;i<nbClients;i++)
                if (strcmp(tcpClients[i].name,name)==0)
                        return i;
        return -1;
}

void sendMessageToClient(char *clientip,int clientport,char *mess)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(clientip);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(clientport);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
                printf("ERROR connecting\n");
                exit(1);
        }

        sprintf(buffer,"%s\n",mess);
        n = write(sockfd,buffer,strlen(buffer));

    close(sockfd);
}

void broadcastMessage(char *mess)
{
        int i;

        for (i=0;i<nbClients;i++)
                sendMessageToClient(tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        mess);
}

int main(int argc, char *argv[])
{
	 // On commence par initialiser le générateur de nombre pseudo-aléatoires.
     srand( time( NULL ) );
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	 int i;
	 int tabPassif[3];
	 for(int w = 0 ; w <= 3 ; w++){
	 	tabPassif[w] = -1;
	 }

        char com;
        char clientIpAddress[256], clientName[256];
        int clientPort;
        int id;
        char reply[256];


     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

	printDeck();
	melangerDeck();
	createTable();
	printDeck();
	joueurCourant=0;
	int encoreActif = 1; // Pour savoir si tous les joueurs sont passifs ou non

	for (i=0;i<4;i++)
	{
        	strcpy(tcpClients[i].ipAddress,"localhost");
        	tcpClients[i].port=-1;
        	strcpy(tcpClients[i].name,"-");
	}

     while (1)
     {    
     	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     	if (newsockfd < 0) 
          	error("ERROR on accept");

     	bzero(buffer,256);
     	n = read(newsockfd,buffer,255);
     	if (n < 0) 
		error("ERROR reading from socket");

        printf("Received packet from %s:%d\nData: [%s]\n\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);

        if (fsmServer==0)
        {
        	switch (buffer[0])
        	{
                	case 'C':
                        	sscanf(buffer,"%c %s %d %s", &com, clientIpAddress, &clientPort, clientName);
                        	printf("COM=%c ipAddress=%s port=%d name=%s\n",com, clientIpAddress, clientPort, clientName);

                        	// fsmServer==0 alors j'attends les connexions de tous les joueurs
                                strcpy(tcpClients[nbClients].ipAddress,clientIpAddress);
                                tcpClients[nbClients].port=clientPort;
                                strcpy(tcpClients[nbClients].name,clientName);
                                nbClients++;

                                printClients();

				// rechercher l'id du joueur qui vient de se connecter

                                id=findClientByName(clientName);
                                printf("id=%d\n",id);

				// lui envoyer un message personnel pour lui communiquer son id

                                sprintf(reply,"I %d",id);
                                sendMessageToClient(tcpClients[id].ipAddress,
                                       tcpClients[id].port,
                                       reply);

				// Envoyer un message broadcast pour communiquer a tout le monde la liste des joueurs actuellement
				// connectes

                                sprintf(reply,"L %s %s %s %s", tcpClients[0].name, tcpClients[1].name, tcpClients[2].name, tcpClients[3].name);
                                broadcastMessage(reply);

				// Si le nombre de joueurs atteint 4, alors on peut lancer le jeu

                                if (nbClients==4)
								{
								// On envoie ses cartes au joueur 0, ainsi que la ligne qui lui correspond dans tableCartes
								// RAJOUTER LE CODE ICI
								sprintf(reply, "D %d %d %d", deck[0], deck[1], deck[2]); // ENvoi de cartes vers J1 avec D n1 n2 n3
								sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port, reply); // On envoit un message au J1 -> tcpClients[0]
								/* Envoi une indicztion sur le nombre de type que le joueur possède, par exemple
								1 0 0 1 ... signifie que le joueur possède 1 objet avec le type de la 1ère colonne, 0 objet avec le type de la 2ème colonne...*/
								for(int j = 0 ; j < 8 ; j++)
								{
											sprintf(reply, "V 0 %d %d", j, tableCartes[0][j]);
											sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port, reply);
								}

								// On envoie ses cartes au joueur 1, ainsi que la ligne qui lui correspond dans tableCartes
								// RAJOUTER LE CODE ICI
								sprintf(reply, "D %d %d %d", deck[3], deck[4], deck[5]); // Envoi de cartes vers J1 avec D n1 n2 n3 - Mais différentes du J1
								sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port, reply); // On envoit un message au J2 -> tcpClients[1]
								for(int j = 0 ; j < 8 ; j++) // Voir explication de la boucle du J1 (tcpClients[0])
								{
											sprintf(reply, "V 1 %d %d", j, tableCartes[1][j]);
											sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port, reply);
								}

								// On envoie ses cartes au joueur 2, ainsi que la ligne qui lui correspond dans tableCartes
								// RAJOUTER LE CODE ICI
								sprintf(reply, "D %d %d %d", deck[6], deck[7], deck[8]); // Envoi de cartes vers J1 avec D n1 n2 n3 - Mais différentes du J1 et du J2
								sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port, reply); // On envoit un message au J3 -> tcpClients[2]
								for(int j = 0 ; j < 8 ; j++) // Voir explication de la boucle du J1 (tcpClients[0])
								{
											sprintf(reply, "V 2 %d %d", j, tableCartes[2][j]);
											sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port, reply);
								}

								// On envoie ses cartes au joueur 3, ainsi que la ligne qui lui correspond dans tableCartes
								// RAJOUTER LE CODE ICI
								sprintf(reply, "D %d %d %d", deck[9], deck[10], deck[11]); // Envoi de cartes vers J1 avec D n1 n2 n3 - Mais différentes du J1, du J2 et du J3
								sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port, reply); // On envoit un message au J4 -> tcpClients[3]
								for(int j = 0 ; j < 8 ; j++) // Voir explication de la boucle du J1 (tcpClients[0])
								{
											sprintf(reply, "V 3 %d %d", j, tableCartes[3][j]);
											sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port, reply);
								}

								// On envoie enfin un message à tout le monde pour définir qui est le joueur courant=0
								// RAJOUTER LE CODE ICI
								sprintf(reply, "M %d", joueurCourant); // Ajout
								broadcastMessage(reply); // Ajout

                                fsmServer=1;
				}
				break;
                }
	}
	else if (fsmServer==1)
	{
		for(int w = 0 ; w <= 3 ; w++){ // Vérification que le joueur courant n'est pas passif
			if(joueurCourant == tabPassif[w]){
				sprintf(reply, "J%d est passif, il passe son tour !", joueurCourant + 1);
				broadcastMessage(reply);
				joueurCourant++;
			}
		}
		switch (buffer[0])
		{
                case 'G':
				// RAJOUTER DU CODE ICI
				/*G pour guilty -> coupable
				On accuse un personnage d'être le coupable, si c'est vrai, on gagne et la partie se finit sinon on devient un joueur passif*/
				int joueurCourant, personnageAccuse;
				sscanf(buffer, "G %d %d", &joueurCourant, &personnageAccuse);

				if(personnageAccuse == deck[12]){ // deck[12] correspond au coupable
					sprintf(reply, "J%d a trouve l identite du coupable, il gagne la partie !", joueurCourant + 1);
					broadcastMessage(reply); // Informer tout le monde que le joueur a gagné
					fsmServer = 2; // Fin de la partie
					sprintf(reply, "FIN DE LA PARTIE");
					broadcastMessage(reply); // Informer tout le monde que la partie est terminée
					close(newsockfd);
					close(sockfd);
					return 0;
					break;
				}
				else{

					sprintf(reply, "J%d n'est pas un bon detective... Il devient passif pour la fin de la partie !", joueurCourant + 1); // P pour passif
					broadcastMessage(reply); // Informer tout le monde que le joueur devient passif
					encoreActif = 0;
					for(int w = 0 ; w <= 3 ; w++){
						if(tabPassif[w] == -1){
						encoreActif = 1;
						tabPassif[w] = joueurCourant;
						}
					}
					if(!encoreActif){
						sprintf(reply, "Tous les joueurs sont passifs, la partie est terminée !");
						broadcastMessage(reply); // On informe tout le monde que tous les joueurs sont passifs
						sprintf(reply, "FIN DE LA PARTIE");
						broadcastMessage(reply); // Informer tout le monde que la partie est terminée
						close(newsockfd);
						close(sockfd);
						return 0;
					}
				}

				break;

                case 'O':
				// RAJOUTER DU CODE ICI
				/*O pour object -> objet
				Demander un symbole à tout le monde
				-> Le serveur répond par le nombre de joueurs ayant ce symbole et on renvoie la valeur 100
				si un joueur possède au moins une carte avec ce symbole*/
				int symbole, joueursAvecSymbole = 0;
				sscanf(buffer, "O %d", &symbole); // Lire le symbole demandé
				for(int i = 0 ; i < 4 ; i++){ // Parcourir les joueurs
					if(tableCartes[i][symbole] > 0) { // Vérifier si le joueur possède ce symbole
						joueursAvecSymbole++;
					}
				}
				if(joueursAvecSymbole > 0){
					sprintf(reply, "R %d 100", joueursAvecSymbole); // R pour réponse, 100 si au moins un joueur possède le symbole
				}
				else{
					sprintf(reply, "R %d 0", joueursAvecSymbole); // 0 si aucun joueur ne possède le symbole
				}
				sendMessageToClient(tcpClients[joueurCourant].ipAddress, tcpClients[joueurCourant].port, reply); // Répondre au joueur courant

				break;

				case 'S':
				// RAJOUTER DU CODE ICI
				/*S pour symbol -> symbole
				Demander à un joueur combien de cartes avec un symbole en particulier il possède*/
				int joueurDemande, symboleDemande, nbCartes;
				sscanf(buffer, "S %d %d", &joueurDemande, &symboleDemande); // Lire le joueur et le symbole demandés
				nbCartes = tableCartes[joueurDemande][symboleDemande]; // Obtenir le nombre de cartes pour ce joueur et ce symbole
				sprintf(reply, "R %d", nbCartes); // R pour réponse, suivi du nombre de cartes
				sendMessageToClient(tcpClients[joueurCourant].ipAddress, tcpClients[joueurCourant].port, reply); // Répondre au joueur courant
				
				break;
                default:
                break;
		}
		if(joueurCourant==3 && encoreActif==1) // On passe au joueur suivant (mise à jour)
		{
			joueurCourant=0;
		}
		else{
			joueurCourant++;
		}
		sprintf(reply, "M %d", joueurCourant); // Ajout
		broadcastMessage(reply); // Ajout
    }
	else{
		sprintf(reply, "FIN DE LA PARTIE");
		broadcastMessage(reply); // Informer tout le monde que la partie est terminée
		close(newsockfd);
		close(sockfd);
		return 0;
	}
    close(newsockfd);
    }
    close(sockfd);
    return 0; 
}
