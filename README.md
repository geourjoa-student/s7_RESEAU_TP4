# s7_RESEAU_TP4
TP sockets

Les fonctionnaltées attendues (ls,get,put,quit) sont réalisées et fonctionnelles.
Les échanges se font grâce au protocole TCP et le serveur peut traiter plusieurs demandes en parallèle.

#Limitations du programmes :
Dans le cas d'un put ou get ou le fichier est inexistant, le programme peut buguer. Nous n'avons pas eu le temps d'améliorer cet aspect du TP.

#Protocole de test:

-Ouvrir un shell et lancer serveur.
-Ouvrir un second shell et lancer client
-Ouvrir un troisième shell et lancer client (pour vérifier le traitement en parallèle

Le serveur produit un retour console des interactions avec les clients. Le pid du processus du serveur traitant une session client est l'identifiant de la session dans l'affichage.

-Utiliser un shell client pour lancer des commandes.
(un fichier télechargé avec get sera préfixé par 'get\_' afin de ne pas créer de problème avec le fichier original (car serveur et client(s) sont lancés dans le même répertoire)).
(idem pour put où l'on préfixe ici avec 'put\_')

#Exemple de tests :

* shell 1 : ls
* shell 2 : ls
* shell 1 : get client
* shell 1 : put get_client
* shell 2 : get put_get_client
* shell 2 : put get_putget_client
* shell 1 : ls
* shell 2 : ls
* shell 1 : quit
* shell 2 : polytech
* shell 2 : quit

#Problème rencontrés :

Création d'un protocole de communication simple et efficace entre le serveur et un client.



