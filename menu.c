#include "menu.h"

/*
* Fonction qui va initialiser un menu 
* Ici on va initialiser un menu avec un nombre de choix possible définit en paramètre d'entré
* On va allouer dynamiquement les différents tableaux (tableau de la flèche, tableau des du numéro des choix et tableau de chaine de caractère)
* On remarque que pour le tableau de chaîne de caractère le sizeof est de type char * car ce sont des chaîne de caractère et non un char normal
* Ensuite on va allouer chaque case ou plutôt ligne du tableau de chaîne de caractère. Si tu veux un tableau de chaine de caractère 
* contient plusieurs lignes et chaque ligne contient une chaine de caractère et c'est indéxé comme pour un tableau, la première ligne
* c'est la chaîne de caractère n°1, la deuxième ligne c'est la chaîne de caractère n°2 etc etc...
* On va aussi initialiser le tableau menu avec 1, 2, 3, 4 etc jusqu'au nombre maximum de choix possible
* Ensuite on va mettre la flèche à la position 0 du menu c'est à dire on va mettre une flèche sur p->menu = 1 donc jouer.
* Et pour finir on va mettre les chaîne de caractère (une par une) sur chaque ligne du tableau de chaîne de caractère (p->choix)
*/

void initialisationMenu(menu *p, int nb){
	p->nbChoix=nb;
	p->menu=malloc((p->nbChoix)*sizeof(int)); 
	p->fleche=malloc((p->nbChoix)*sizeof(char));
	p->choix=malloc((p->nbChoix)*sizeof(char *));
	int i;
	for (i=0; i<(p->nbChoix); i++)
		p->choix[i]=malloc(20*sizeof(char));
	for (i=0; i<(p->nbChoix); i++)
		p->menu[i]=i+1;
	p->fleche[0]='>';
	p->choix[0]="Gauss";
	p->choix[1]="Gauss-Jordan";
	p->choix[2]="Quitter";
}

//Libère la mémoire pour le menu, la flèche, le tableau de chaine de caractère, les chaines de charactère

void libereMemoire(menu *p){
	free(p->menu);
	free(p->fleche);
	free(p->choix);
}

/*
* On va vérifier ici que la flèche se trouve oui ou non à la ligne a
* Donc si la flèche est sur jouer et qu'on cherche à savoir si flecheValide(&p, 0) (0 pour ligne 1) sa va retourner true car 
* la flèche se situer bien sur la ligne 0 donc sur jouer
* sinon sa retourne 0;
*/

int flecheValide(menu *p, int a){
	if (p->fleche[a]=='>')
		return 1;
	return 0;
}

/*
* Fonction qui va afficher le menu
* \33[34;01m    \33[00m  sont les couleurs pour la sélection
* Il va afficher en blanc si la flèche n'est pas devant la sélection 
* Il va afficher en bleu si la flèche pointe la sélection

* Il y a un ptit aide au menu sur les touches à effectuer : HAUT BAS et DROITE
* Il va donc afficher chaque ligne du menu et si c'est la ligne ou la flèche est dessus alors on l'affiche avec de la surbrillance (bleu ici si je me trompe pas)
* sinon on l'affiche normalement, on va afficher le numéro du choix puis le nom du choix (jouer, sauvegarder etc...)
*/

void affichageMenu(menu *p){
	int i; 
	printf("Monter/descendre : flèche haut/bas; selectionner : flèche droite\n");
	for (i=0; i<(p->nbChoix); i++){
		if (flecheValide(p, i))
			printf(" %c \33[34;01m%d  %s\33[00m\n", p->fleche[i], p->menu[i], p->choix[i]);
		else
			printf("     %d  %s\n", p->menu[i], p->choix[i]);
	}
}

// C'est exactement la même fonction que dans le jeu

int saisieD(){
    debutTerminalSansR();
    Key saisieDir = NO_KEY;
    while (saisieDir==NO_KEY){
        saisieDir=lectureFleche();
    }
    switch (saisieDir){ 
        case KEY_DOWN: 
            finTerminalSansR();
            return 0;           
            break;
        case KEY_RIGHT: 
            finTerminalSansR();
            return 1;
            break;
        case KEY_UP: 
            finTerminalSansR();
            return 2;
            break;
        case KEY_LEFT: 
            finTerminalSansR();
            return 3;
            break;
        case KEY_ESCAPE: 
            finTerminalSansR();
            return -1;
            break;
        default :
            finTerminalSansR();
            return -2;
            break;
    }
}

/*
* Fonction qui va retourné la position de la flèche dans flèche
* On va donc retourné l'indice ou se trouve la flèche 
* par exemple si la flèche est sur jouer alors sa retourne 0 sinon si la flèche est sur sauvegarder il retourne 1 etc etc... 
*/

int getFleche(menu *p){
	int i, j=0;
	for (i=0; i<(p->nbChoix); i++){
		if (p->fleche[i]=='>')
			return j;
		j++;
	}
	return 0;
}

/*
* Fonction qui va lancer le menu 
* Si on appuie sur flèche du bas ou du haut alors la flèche dans p->flèche bouge selon la direction
* Si la flèche atteint la limite (haut ou bas) alors la flèche ne bouge plus
* On retourne le menu selectionné (1, 2, 3 ou 4)

* Ici on execute l'affichage du menu, on demande à l'utilisateur de saisir une touche
* tant que c'est pas la flèche DROITE on continue le clear et l'affichage
* Si c'est 0 (flèche du BAS) alors on cherche ou se situe la flèche avec getFleche()
* Ensuite on descend la flèche d'une ligne (on va ajouter un > sur la ligne en dessous et " " sur la ligne ou il y avait la flèche)
* Maintenant si la flèche est déjà tout en bas c'est à dire sur QUITTER alors on fait rien
* C'est la même chose pour le cas de 2 c'est à dire si on fait flèche du HAUT on enlève la flèche sur la ligne ou il y avait la flèche
* et on le mettra sur la ligne juste au dessus et on vérifie bien si on est pas déjà tout en haut sinon on va dépasser et on va perdre la flèche
* Et donc dernier cas si on fait flèche DROITE on va sélectionner un vhoix dans le menu et sa retourne la valeur du choix sélectionné (1 pour jouer etc...)
*/

int lanceMenu (menu *p){
	int saisie, j;
	while (saisie!=1){
		clear_terminal();
		affichageMenu(p);
		saisie=saisieD();
		switch (saisie){
			case 0 : 
				j=getFleche(p);
				if (j!=(p->nbChoix-1)){
					p->fleche[j+1]='>';
					p->fleche[j]=' ';
				}
				break;
			case 2 :
				j=getFleche(p);
				if (j!=0){
					p->fleche[j-1]='>';
					p->fleche[j]=' ';
				}
				break;
			case 1 :
				return getFleche(p)+1;
		}
	}
	return 0;
}

/*
* Fonction menu qui va gérer le menu
* Initialise un menu
* Il va afficher le menu
* Et il va retourné le choix opté par l'utilisateur
* On libère la mémoire
*/

int menuD(){
	menu p;
	initialisationMenu(&p, 3);
	affichageMenu(&p);
	int i = lanceMenu(&p);
	printf("Le menu selectionné est %d.\n", i);
	switch (i){
		case 1 : 
			break;
		case 2 :
			break;
		case 3 :
			libereMemoire(&p);
			return 0;
	} 
	return 0;
}

int main(){
	menuD();
	return 0;
}