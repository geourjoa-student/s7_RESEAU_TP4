// Liste des codes commandes utilisables






#define LS 2
#define PUT 8
#define GET 3

#define R_LS 4
#define R_PUT 5
#define R_GET 6

#define AIDE_CLIENT 7




#define QUIT 0


#define TAILLE_BUFFER_NOMBRE 10
#define TAILLE_NOM_FICHIER_MAX 100

#define TAILLE_TAMPON 100



int file_size(char *nomFichier) ;

void file_to_stream(char *nomFichier, char *stream, int size);
void socket_to_file(int socket, int taille_fichier, char* nomFichier);
