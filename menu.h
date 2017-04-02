

#include"saisieM.c"
#include"couleurs_terminal.c"
#include<stdio.h>
#include<stdlib.h>

/*
* Structure d'un menu
* Sa va contenir un tableau qui aura une flèche (quelque part) et pas plus
* Elle va contenir un tableau de numéro d'un choix par exemple 1 pour jouer, 2 pour sauvegarder etc...
* Elle contiendra le nombre de choix possible (pour facilité l'initialisation en faite)
* Et elle contiendra tableau de chaîne de caractère qui contiendra "jouer", "sauvagarder" et ainsi de suite.
*/
typedef struct{
	char *fleche;
	int *menu;
	int nbChoix;
	char **choix;
} menu;

void initialisationMenu(menu *p, int nb);
void libereMemoire(menu *p);
int flecheValide(menu *p, int a);
void affichageMenu(menu *p);
int saisieD();
int getFleche(menu *p);
int lanceMenu (menu *p);
int menuD();