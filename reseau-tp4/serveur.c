#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include<sys/signal.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "fon.h"     
#include "commande.h"

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"

#define TAILLE_TAMPON 50
#define NB_REQUETE_MAX 1


int file_size(char *nomFichier);
void file_to_stream(char *nomFichier, char* stream, int size);
void serveur_appli(char *service, char* protocole);


int main(int argc, char *argv[]) {

	char *service = SERVICE_DEFAUT; /* numero de service par defaut */
	char *protocole = PROTOCOLE_DEFAUT; /* protocole par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc) {
	case 1:
		printf("defaut service = %s\n", service);
		break;
	case 2:
		service = argv[1];
		break;

	default:
		printf("Usage:\n> ./serveur service \n");
		return EXIT_SUCCESS;
	}

	serveur_appli(service, protocole);

	return 0;
}

/******************************************************************************/
void serveur_appli(char *service, char *protocole) {
	char tampon[TAILLE_TAMPON];

	int session_en_cours = 1;
	FILE *file;
	int socket_session, nb_lus;

	pid_t pid;

	struct sockaddr_in p_adr_socket;
	struct sockaddr_in p_adr_utilisateur;

	// Création socket
	int socket = h_socket(AF_INET, SOCK_STREAM);

	adr_socket(service, "localhost", protocole, &p_adr_socket);

	// Association de la socket
	h_bind(socket, &p_adr_socket);

	//Mise en écoute de la socket
	h_listen(socket, NB_REQUETE_MAX); //

	//Boucle infinie du serveur	
	while (1) {

		//Acceptation de connexion
		socket_session = h_accept(socket, &p_adr_utilisateur);

		if ((pid = fork()) < 0) {
			fprintf(stderr, "Il y a une erreur \n");
			exit(EXIT_FAILURE);
		} else {
			if (pid == 0) {
				//Fils



				h_close(socket);

				while (session_en_cours) {

					int status;
					pid_t pid_fils;

					printf("hr %d\n", h_reads(socket_session, tampon,1)); // lit le char tappé en entrée
					//TODO rajouter des tests sur le nombre d'octets lu

					switch (tampon[0]) {

					case LS:



						switch (pid_fils = fork()) {
						//Fils qui execute sur le serveur
						case 0:


							file = fopen("temp.txt", "w+");
							dup2(fileno(file), STDOUT_FILENO);
							fclose(file);
							char* arg[] = {"ls", NULL};
							execvp(arg[0],arg);
							break;

						default:




							if (waitpid(pid_fils, &status, NULL) == -1) {
								perror("wait pid ls");
								exit(-1);
							} else {


								//Transferer le résultat de ls au client

								char taille_texte[TAILLE_BUFFER_NOMBRE];

								int taille = file_size("temp.txt");
								char *stream = (char*) malloc(taille * sizeof(char));

								file_to_stream("temp.txt", stream, taille);


								sprintf(taille_texte, "%d", taille);


								printf("hw : %d\n",h_writes(socket_session, taille_texte,
								TAILLE_BUFFER_NOMBRE));

								printf("hw : %d\n", h_writes(socket_session, stream, taille));

							}
							break;
						}

						break;
					case PUT:
						break;

					case GET:
						break;

					case QUIT:
						h_close(socket_session);
						break;

					default:
						break;

					}

					printf("Commande traitée \n");


				}

				h_close(socket_session);

				exit(EXIT_SUCCESS);
			} else {
				//Pere
				printf("Connexion d'un client.\n");
				h_close(socket_session);

			}
		}

	}

}



int file_size(char *nomFichier){
	FILE *parcours= fopen(nomFichier, "r");
		fseek(parcours, 0, SEEK_END);
		int size=ftell(parcours);
		fclose(parcours);

		return size;
}

void file_to_stream(char *nomFichier, char *stream, int size) {
	int i;

	FILE *parcours= fopen(nomFichier, "r");

	for (i = 0; i < size; i++) {
		stream[i] = fgetc(parcours);
	}

	fclose(parcours);



}
