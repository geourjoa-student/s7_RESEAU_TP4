
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include<sys/signal.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "fon.h"     

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"

#define TAILLE_TAMPON 50
#define NB_REQUETE_MAX 1


void serveur_appli (char *service, char* protocole);   

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  break;
 	case 2:
		  service=argv[1];
		  break;

   	default :
		  printf("Usage:\n> ./serveur service \n");
		  return EXIT_SUCCESS;
 	}

	
	serveur_appli(service, protocole);
	
	return 0;
}
// renvoie 1 si la partie est fini , 0 sinon
char fin_partie(char *mot, int taille_mot,int nb_erreur)
{
	if (nb_erreur ==0)
		return 1;
	int i =0;
	for (i =0;i<taille_mot;i++)
	{
		if (mot[i]<'a' || mot[i] >'z')
			return 0;
	}
	
	return 1;
}
// verifie si le char c a déja été joué ou non
int verif_lettre(int *lettre, char c)
{
	int i;
	i = c - 'a'; 
	return lettre[i];
}
void maj_lettre(int *lettre, char c)
{
	int i;
	i = c - 'a'; // a check
	lettre[i]==1;
}
//verifie que la lettre est dans le mot
int est_lettre_mot (char *mot ,int taille_mot, char c)
{
	int i=0;
	for(i=0;i<taille_mot;i++)
	{
		if (mot[i]==c)
			return 1;
	}
	
	return 0;
}
void apparaitre_lettre_mot(char *mot, char *mot_trouve, int taille_mot, char c)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		if(mot[i]==c)
			mot_trouve[i]=c;
	}
}

void init(char *mot , int taille_mot, char value)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		mot[i]=value;
	}
}
void init_l(int *lettre , int taille_mot, int value)
{
	int i;
	for (i=0;i<taille_mot;i++)
	{
		lettre[i]=value;
	}
}
/******************************************************************************/	
void serveur_appli(char *service, char *protocole)
{
	int lettres[26]; // lettres[i] vaut 0 si la ieme lettre n'a pas été jouée
	
	char nb_coups = 5, taille_mot=7 , mot[]="reseaux", mot_trouve[taille_mot], partie_finie = 0,tampon[TAILLE_TAMPON];
	char temp[4];
	int i;
	int file;
	int socket_pendu,nb_lus; 

	pid_t pid; 
		
	struct sockaddr_in p_adr_socket; 
	struct sockaddr_in p_adr_utilisateur;
	
	init(mot_trouve,taille_mot,'.');
	
	init_l(lettres,26,0);
	
	// Création socket
	int socket= h_socket(AF_INET,SOCK_STREAM); 

	adr_socket(service,"localhost",protocole,&p_adr_socket); 
	
	// Association de la socket
	h_bind (socket,&p_adr_socket); 
	
	//Mise en écoute de la socket
	h_listen(socket,NB_REQUETE_MAX); // 
	
	//Boucle infinie du serveur	
	while(1){


		//Acceptation de connexion
		socket_pendu=h_accept(socket,&p_adr_utilisateur); 

		if ((pid = fork())<0){ 
			fprintf (stderr,"Il y a une erreur \n"); 
			exit(EXIT_FAILURE); 
		} 
		else {
			if (pid == 0){ 
				//Fils

		
				h_close(socket);


				h_writes(socket_pendu,&taille_mot,sizeof(char)); // envoie la taille du mot au client

				while (!partie_finie)
				{
					nb_lus = h_reads(socket_pendu,tampon,sizeof(char[TAILLE_TAMPON]));// lit le char tappé en entrée
					
					tampon[nb_lus] = '\0';
					
					for(i=0;i<3;i++) {
						temp[i] = tampon[i];
					}
					temp[3] = '\0';

					if(strcmp(tampon,"ls") == 0){
						pid_t pid_fils;
						int status;
						switch (pid_fils = fork()){
							//Fils qui execute sur le serveur
							case 0: 
								
								file= open ("temp.txt", O_WRONLY);
								dup2(file, STDOUT_FILENO);
								execvp("ls","ls", NULL);
							default: 
								if(waitpid(pid_fils, &status)==-1){
									perror("wait pid ");
									exit(-1);
								} else {
									//Transferer le résultat de ls au client
									
								}
							break;
					}
				
					else if(strcmp(temp,"put") == 0){
						char temp2[nb_lus - 4];    // ici on crée une var temp qui va contenir le nom du fichier
						for(i=0;i<nb_lus-4-1;i++){
							temp2[i] = *tampon[i+4]; 
						}
						FILE * fp = fopen (temp2, "w");
						
						char temp3 = //read(1 caractere du fichier sur le client);
						 	while (temp3 != EOF){
						 		fputc(temp3,fp);
						 	}
					}

					else if(strcmp(temp,"get") == 0){
						char temp2[nb_lus - 4];    // ici on crée une var temp qui va contenir le nom du fichier
						for(i=0;i<nb_lus-4-1;i++){
							temp2[i] = *tampon[i+4]; 
						}
						 FILE * fp = fopen (temp2, "r");
						 if(fp == NULL){
						 	//h_write("erreur fichier introuvable sur le serveur");
						 }
						 else{
						 	char temp3 = fgetc( FILE * fp );
						 	while (temp3 != EOF){
						 		//h_write(temp3);
						 		emp3 = fgetc( FILE * fp );
						 		Q ²
						 	}
						 	//h_write("fin du fichier")
						 }
					}

					/*else {
					}*/
						
					/*if(!(verif_lettre(lettres,tampon[0]))) // La lettre n'a pas été donnée
					{
						maj_lettre(lettres,tampon[0]);
						if(est_lettre_mot(mot,taille_mot,tampon[0]))// La lettre est dans le mot
							apparaitre_lettre_mot(mot,mot_trouve,taille_mot,tampon[0]); // MAJ de la lettre dans le mot_trouve
			
						else
							nb_coups --; 
					}
					else 
						nb_coups --;
		
		
					partie_finie = fin_partie(mot_trouve,taille_mot,nb_coups);

					h_writes(socket_pendu,mot_trouve,taille_mot);// envoie l'état de mot_trouve actuel
					
					h_writes(socket_pendu,&nb_coups,sizeof(char)); // envoie le nombre d'erreurs restantes
		
					h_writes(socket_pendu,&partie_finie,sizeof(char));	// notifie l'état de la partie*/
				}
			


				h_close(socket_pendu);

				exit(EXIT_SUCCESS); 
			} else {
				//Pere
				printf("Connexion d'un client.\n");
				h_close(socket_pendu);
			
			}
		}

	}

	
			
	
	
}



void file_to_stream(FILE *fl,char* stream, int* size)
{
	char c;
	int i=0;
	int nbcar;
	file *parcours=fl;
	
	while(fgetc(parcours)!=EOF){
		nbcar++;
	}
	
	stream=malloc(nbcar);
	
	for(i=0;i<nbcar;i++){
		stream[i]=fgetc(fl);
	}
	
	*size=nbcar;

}

/******************************************************************************/	
