#include<stdio.h>
#include<stdlib.h>
#define TAILLE_MAX 10
typedef struct{
	float *matrice;
	int nbColonne;
	int nbLigne;
}gauss;

/*
* On va allouer dynamiquement le tableau de float matrice et vecteurs
*/

void allouTabMat(gauss *p){
	int nbCase=p->nbColonne*p->nbLigne;
	p->matrice=malloc(nbCase*sizeof(float));
}

/*
* On libère la mémoire allouer auparavant
*/

void libereTabMat(gauss *p){
	free(p->matrice);
}

/*
* On va afficher la matrice
*/

void affichage(gauss *p){
	int i;
	int caseTotal=p->nbLigne*p->nbColonne;
	for (i=0; i<caseTotal; i++){
		printf("%f\t", p->matrice[i]);
		if((i+1)%p->nbColonne==0)
			printf("\n");
	}
}

/*
* Cette fonction permet de récupérer les informations dans le text.txt
*/

void initMatVec(gauss *p, FILE* fichier){
	char test[TAILLE_MAX];
	int i, caseTotal=p->nbLigne*p->nbColonne;
	for(i=0; i<caseTotal; i++){
		fgets(test, TAILLE_MAX, fichier);
		p->matrice[i]=atof(test);
	}
	affichage(p);
}

/*
* Récupération des informations nb colonnes et nb lignes dans le fichier text.txt
*/

int recupInfoColLign(gauss *p){
	FILE* fichier = NULL;
	char info[6];
	char nbCol[3];
	char nbLign[3];
	int i, j=0;
    fichier = fopen("text.txt", "r");		//Ouverture du fichier en read only
    if(fichier==NULL){
    	printf("Impossible d'atteindre le fichier! Sortie du programme.\n");	//Si le fichier n'est pas accessible alors on sort de la fonction
    	return 0;
    }
    fgets (info, TAILLE_MAX, fichier);		//On récupère une chaine de caractère de 5 caractères (max 2 caractère pour la colonne et pour la ligne)
    for(i=0; i<6; i++){				//On va mettre dans les différents chaînes de caractères les valeurs.
    	if(info[i]!=' ' && i<2)		//Si on ne rencontre pas d'expace et qu'on a déjà lu 2 caractère on met les caractères dans la chaine de caractère nbCol
    		nbCol[i]=info[i];
    	else if (info[i]==' '){		//Sinon si on rencontre le caractère vide (cad l'espace) on incrémente i pour passer au nombre de ligne et on met le première caractère dans nbLign (on oublie pas d'incrémenter j)
    		i++;
    		nbLign[j]=info[i];
    		j++;
    	}
    	else{
    		nbLign[j]=info[i];		//Sinon si on a passé toutes les conditions on va mettre les caractères dans nbLign
    		j++;
    	}
    }
    p->nbColonne=atoi(nbCol);		//On va convertir grâce à la fonction atoi() qui permet de caster une chaîne de caractère en integer. Et on va l'affecter à notre structure de donnée gauss
    p->nbLigne=atoi(nbLign);		//Même chose pour le nombre de ligne
    allouTabMat(p);
    initMatVec(p, fichier);
    fclose(fichier);
    return 1;
}

int main(){
	gauss p;
	if(recupInfoColLign(&p)==0)
		return 0;
	libereTabMat(&p);
}