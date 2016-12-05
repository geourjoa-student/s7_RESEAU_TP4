#include <stdio.h>
#include <stdlib.h>
#include <curses.h> 		
#include <sys/signal.h>
#include <sys/wait.h>
#include "fon.h"   		

#include "commande.h"

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define SERVEUR_DEFAUT "localhost"

#define TAILLE_MAX_COMMANDE 10
#define TAILLE_TAMPON 100

void socket_to_file(int socket, int taille_fichier, char* nomFichier);
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

/*****************************************************************************/
void client_appli (char *serveur,char *service,char *protocole)
{
	char code_commande;
	
	char commande[TAILLE_MAX_COMMANDE];
	char tampon[TAILLE_TAMPON];

	char session_fini = 0;
  	
	struct sockaddr_in p_adr_distant;
	
	//Cr�ation socket
	int socket_local = h_socket(AF_INET,SOCK_STREAM); 
	
	//Initialisation du socket
	adr_socket(service,serveur,protocole,&p_adr_distant);
	
	//Connexion au serveur
	h_connect(socket_local,&p_adr_distant);
	
	

	while(!session_fini)
	{
		printf("Serveur $ ");
		scanf("%s",&commande);

		//strcpy(commande,"ls");

		if(strcmp(commande, "ls")==0){
			code_commande=LS;
		} else {

			if(strcmp(commande, "quit")==0){
				code_commande=QUIT;
			} else
			//défault
			code_commande = AIDE_CLIENT;
		}
		
		//TODO autre commande

		switch (code_commande) {
			case LS:


				printf("hw %d\n", h_writes(socket_local,&code_commande,1));

				printf("hr %d\n",h_reads(socket_local, tampon, TAILLE_BUFFER_NOMBRE));


				tampon[TAILLE_BUFFER_NOMBRE]='\0';

				int taille_fichier = atoi(tampon);



				socket_to_file(socket_local,taille_fichier,"ls.txt" );


				break;

			case QUIT :
				session_fini=1;
				break;

			default:
				printf("Erreur sur la saisie de la commande \n" );
				break;
		}


	}
	


	// fermeture connexion
	h_writes(socket_local, QUIT, 1);

	h_close(socket_local);
	

 }

void socket_to_file(int socket, int taille_fichier, char* nomFichier){

	printf("stof\n");

	FILE *f = fopen(nomFichier, "w+");

	char tampon[TAILLE_TAMPON];

	int nb_octets_ecrits=0;
	int nb_octest_lus=0;

	while(nb_octets_ecrits!=taille_fichier){
		if(taille_fichier-nb_octest_lus>TAILLE_TAMPON)
			nb_octest_lus=h_reads(socket, tampon, TAILLE_TAMPON);
		else
			nb_octest_lus=h_reads(socket, tampon, taille_fichier-nb_octest_lus);

		printf("hr %d\n", nb_octest_lus);
		//printf("tampon %s\n", tampon);

		int i;
		for (i = 0; i < nb_octest_lus; ++i) {
			printf("%c", tampon[i]);
			fputc(tampon[i], f);
		}

		nb_octets_ecrits+=nb_octest_lus;
	}

	fputc(EOF, f);

	fclose(f);

}
	
/*****************************************************************************/

