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


#define NB_REQUETE_MAX 1
#define FICHIER_TEMPORAIRE ".temp"

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

				h_close(socket);

				while (1) {

					int status;
					pid_t pid_fils;

					//Lecture de la commande reçue.
					h_reads(socket_session, tampon, 1);

					//En cas d'ajout de commande, il faut les ajouter ici.
					switch (tampon[0]) {

					case LS:

						printf("[%d] Commande ls demandée.\n", getpid());

						switch (pid_fils = fork()) {
						//Fils qui execute la commande LS sur le serveur
						case 0:

							//Utilisation d'un fichier temporaire pour récuperer le résultat de LS
							file = fopen(FICHIER_TEMPORAIRE, "w+");
							dup2(fileno(file), STDOUT_FILENO);
							fclose(file);
							char* arg[] = { "ls", NULL };
							execvp(arg[0], arg);
							break;

						default:

							if (waitpid(pid_fils, &status, 0) == -1) {
								perror("wait pid ls");
								exit(-1);
							} else {

								//Transferer le résultat de ls au client

								char taille_texte[TAILLE_BUFFER_NOMBRE];

								int taille = file_size(FICHIER_TEMPORAIRE);
								char *stream = (char*) malloc(
										taille * sizeof(char));

								file_to_stream(FICHIER_TEMPORAIRE, stream, taille);

								// int to char []
								sprintf(taille_texte, "%d", taille);
								h_writes(socket_session, taille_texte,
										TAILLE_BUFFER_NOMBRE);

								h_writes(socket_session, stream,taille);

								remove(FICHIER_TEMPORAIRE);

							}
							break;
						}

						break;

					case PUT:

						printf("[%d] Commande put demandée.\n", getpid());

						char nomFichierARecevoir[TAILLE_NOM_FICHIER_MAX];

						//Lecture du nom du fichier
						h_reads(socket_session, nomFichierARecevoir,TAILLE_NOM_FICHIER_MAX);

						//Lecture de la taille du fichier
						h_reads(socket_session, tampon,	TAILLE_BUFFER_NOMBRE);

						char nouveauNomFichierARecevoir[TAILLE_NOM_FICHIER_MAX];

						strcpy(nouveauNomFichierARecevoir, "put_");
						strcpy(&nouveauNomFichierARecevoir[4],
								nomFichierARecevoir);

						socket_to_file(socket_session, atoi(tampon),
								nouveauNomFichierARecevoir);

						break;

					case GET:

						printf("[%d] Commande get demandée.\n", getpid());

						char nomFichierAEnvoyer[TAILLE_NOM_FICHIER_MAX];

						h_reads(socket_session, nomFichierAEnvoyer,	TAILLE_NOM_FICHIER_MAX);

						char taille_texte[TAILLE_BUFFER_NOMBRE];

						int taille = file_size(nomFichierAEnvoyer);

						char *stream = (char*) malloc(taille * sizeof(char));

						file_to_stream(nomFichierAEnvoyer, stream, taille);

						sprintf(taille_texte, "%d", taille);
						h_writes(socket_session, taille_texte,	TAILLE_BUFFER_NOMBRE);

						h_writes(socket_session, stream, taille);

						break;

					case QUIT:
						session_en_cours = 0;
						h_close(socket_session);
						printf("[%d] Fin traitement session client. \n", getpid());
						exit(EXIT_SUCCESS);

						break;

					default:
						printf("[%d] Commande inconnue reçue.\n", getpid());
						break;

					}


					printf("[%d] Commande traitée.\n", getpid());
				}


			} else {
				//Pere
				printf("[%d] Connexion d'un client.\n", pid);
				h_close(socket_session);

			}
		}

	}

}

