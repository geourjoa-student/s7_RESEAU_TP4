#include <stdio.h>
#include <stdlib.h>
#include <curses.h> 		
#include <sys/signal.h>
#include <sys/wait.h>
#include "fon.h"   		

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define SERVEUR_DEFAUT "localhost"

void client_appli (char *serveur, char *service, char *protocole);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT;
	char *service= SERVICE_DEFAUT; /* no de port) */
	char *protocole= PROTOCOLE_DEFAUT; 

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
		case 1 :		
			  printf("serveur par defaut: %s\n",serveur);
			  printf("service par defaut: %s\n",service);
			  break;
		case 2 :		
			  serveur=argv[1];
			  printf("service par defaut: %s\n",service);
			  break;
		case 3 :		
			  serveur=argv[1];
			  service=argv[2];
			  break;

		default:
			  printf("Usage: \n> ./client ip port \n");
			  return EXIT_SUCCESS;
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	/* protocole le protocole qui sera utilise pour la communication */
	
	client_appli(serveur,service,protocole);

	return EXIT_SUCCESS;
}
void viderbuffer()
{
	int c;
	while((c=getchar()) != EOF && c!='\n');
}
/*****************************************************************************/
void client_appli (char *serveur,char *service,char *protocole)
{
	int i, nb_coups_joues=1;
	
	char nb_erreurs, lettre, jeu_fini = 0,taille_mot; 
  	
	struct sockaddr_in p_adr_distant;
	
	//Création socket
	int socket_local = h_socket(AF_INET,SOCK_STREAM); 
	
	//Initialisation du socket
	adr_socket(service,serveur,protocole,&p_adr_distant);
	
	//Connexion au serveur
	h_connect(socket_local,&p_adr_distant);
	
	h_reads(socket_local,&taille_mot,sizeof(char)); // recupere la taille du mot	
	
	char mot_trouve[taille_mot]; // declaration du tableau du mot_courant
	
	printf("\n********************************\nBienvenue dans le jeu du pendu !\n********************************\n\n");

	printf("Voici le mot actuel : \n");
	for(i=0; i<taille_mot; i++){
		printf(".");
	}

	printf("\n\n");

	while(!jeu_fini)
	{
		do {
			printf("Coup %d : Saisissez une lettre minuscule: ", nb_coups_joues);
			scanf("%c",&lettre);
			viderbuffer();	
		} while ( lettre > 'z' || lettre < 'a');
		
		
		
		h_writes(socket_local,&lettre,sizeof(char)); // Ecriture de la lettre jouée
		h_reads(socket_local,mot_trouve,taille_mot); // Lecture de l'état du mot
		
		h_reads(socket_local,&nb_erreurs,sizeof(char)); // Lecture du nombre d'erreur		
				
		printf("Voici le mot actuel : \n");
		for(i=0;i<taille_mot;i++)
			printf("%c",mot_trouve[i]);
		printf("\n\nVous avez encore droit a %d erreurs\n",nb_erreurs);
		
		h_reads(socket_local,&jeu_fini,sizeof(char)); // Lecture du bouléen d'état fin de partie

		printf("\n\n");

		nb_coups_joues++;
	}
	
	if(nb_erreurs > 0)		
		printf("*** Vous avez gagne en %d coups ! ***\n", nb_coups_joues);
	else		
		printf("Vous avez perdu.\n");

	// fermeture connexion
	h_close(socket_local);
	

 }
	
/*****************************************************************************/

