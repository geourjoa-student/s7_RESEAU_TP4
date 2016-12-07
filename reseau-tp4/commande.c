#include <stdio.h>
#include <stdlib.h>
#include "commande.h"

int file_size(char *nomFichier) {
	FILE *parcours = fopen(nomFichier, "r");
	fseek(parcours, 0, SEEK_END);
	int size = ftell(parcours);
	fclose(parcours);

	return size;
}

void file_to_stream(char *nomFichier, char *stream, int size) {
	int i;

	FILE *parcours = fopen(nomFichier, "r");

	for (i = 0; i < size; i++) {
		stream[i] = fgetc(parcours);
	}

	fclose(parcours);

}



void socket_to_file(int socket, int taille_fichier, char* nomFichier) {

	printf("stof\n");

	FILE *f = fopen(nomFichier, "w+");

	char tampon[TAILLE_TAMPON];

	int nb_octets_ecrits = 0;
	int nb_octest_lus = 0;

	while (nb_octets_ecrits != taille_fichier) {
		if (taille_fichier - nb_octets_ecrits > TAILLE_TAMPON)
			nb_octest_lus = h_reads(socket, tampon, TAILLE_TAMPON);
		else
			nb_octest_lus = h_reads(socket, tampon,
					taille_fichier - nb_octets_ecrits);

		printf("hr %d\n", nb_octest_lus);

		int i;
		for (i = 0; i < nb_octest_lus; ++i) {
			fputc(tampon[i], f);
		}

		nb_octets_ecrits += nb_octest_lus;
	}

	fclose(f);

	printf("Télechargement terminé.\n");

}
