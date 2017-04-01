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
	printf("\n\n");
}

/*
* Cette fonction permet de récupérer les informations dans le text.txt
*/

void initMatVec(gauss *p, FILE* fichier){
	char test[TAILLE_MAX];
	int i, caseTotal=p->nbLigne*p->nbColonne;
	for(i=0; i<caseTotal; i++){
		fgets(test, TAILLE_MAX, fichier);		//On récupère la ligne souhaité grâce à fgets en format chaine de caractère
		p->matrice[i]=atof(test);				//On cast le char* en float grâce à la fonction atof() et on l'affecte dans le tableau de la matrice.
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

/*
* Fonction va permuter 2 lignes
*/

void permuteLigne(gauss *p, int ligne1, int ligne2){
	int i, j=ligne1*p->nbColonne, k=ligne2*p->nbColonne;
	float echange;
	for(i=0; i<p->nbColonne; i++){					//Boucle qui va permuter chaque valeur de la case de la ligne 1 à la case de la ligne 2
		echange = p->matrice[j+i];
		p->matrice[j+i]=p->matrice[k+i];
		p->matrice[k+i]=echange;
	}
	printf("l%d <==> l%d\n", ligne1+1, ligne2+1);	//On affiche les lignes qui ont étaient permutées.
	affichage(p);									//On réaffiche la matrice avec les lignes permutées
}

/*
* Fonction qui va choisir un pivot pour la suite
*/

int choixPivot(gauss *p, int col){
	int i, compteur=0;
	for (i=0; i<p->nbLigne; i++){		//Cette boucle permet de savoir si les valeurs de la première colonne sont égales à zéros. Si c'est le cas alors on incrémente le compteur
		if (p->matrice[i*p->nbColonne+col]==0)
			compteur++;
	}
	if (compteur==p->nbLigne)
		return 0;			//Si le compteur est au maximum c'est à dire si toutes la première colonne contient que des zéros alors on retourne 0
	if (p->matrice[col*p->nbColonne+col]!=0)
		return 1;			//On retourne 1 si le premier terme de la matrice est différent de zéro
	else{
		for(i=col+1; i<p->nbLigne; i++){
			if(p->matrice[i*p->nbColonne]!=0){
				permuteLigne(p, i, 0);
				return 2;	//On retourne 2 si on a trouvé un pivot et qu'on a permuté deux lignes.
			}
		}
	}
	return 1;
}

/*
* 2ème étape qui consiste à éliminer toutes les valeurs non nulles en dessous du pivot
*/

void eliminationGauss(gauss *p){
	int i;
	for (i=1; i<p->nbLigne; i++)
		p->matrice[i*p->nbColonne]=p->matrice[i*p->nbColonne]-(p->matrice[i*p->nbColonne]/p->matrice[0])*p->matrice[0];
	affichage(p);
}

int main(){
	gauss p;
	if(recupInfoColLign(&p)==0)
		return 0;
	choixPivot(&p, 0);
	eliminationGauss(&p);
	libereTabMat(&p);
	return 0;
}