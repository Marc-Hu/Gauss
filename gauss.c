#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TAILLE_MAX 10
typedef struct{
	float *matrice;
	float *solution;
	char **infiniSol;
	int *varLibre;
	int nbColonne;
	int nbLigne;
}gauss;

/*
* On va allouer dynamiquement le tableau de float matrice et vecteurs
*/

void allouTabMat(gauss *p){
	int nbCase=p->nbColonne*p->nbLigne, i;
	char *c=malloc(20*sizeof(char));
	p->matrice=malloc(nbCase*sizeof(float));
	p->solution=malloc((p->nbColonne-1)*sizeof(float));
	for(i=0; i<p->nbColonne-1; i++)
		p->solution[i]=0;
	p->varLibre=malloc((p->nbColonne-1)*sizeof(int));
	for(i=0; i<p->nbColonne-1; i++)
		p->varLibre[i]=0;
	p->infiniSol=malloc((p->nbColonne-1)*sizeof(char*));
	for(i=0; i<p->nbColonne-1; i++)
		p->infiniSol[i]=c;
}

/*
* On libère la mémoire allouer auparavant
*/

void libereTabMat(gauss *p){
	free(p->matrice);
	free(p->solution);
	free(p->varLibre);
	free(p->infiniSol);
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

char choixPivot(gauss *p, int col){
	int i, compteur=0;
	for (i=col; i<p->nbLigne; i++){		//Cette boucle permet de savoir si les valeurs de la première colonne sont égales à zéros. Si c'est le cas alors on incrémente le compteur
		if (p->matrice[i*p->nbColonne+col]==0)
			compteur++;
	}
	if (compteur==p->nbLigne-col)
		return 'f';			//Si le compteur est au maximum c'est à dire si toutes la première colonne contient que des zéros alors on retourne 0
	if (p->matrice[col*p->nbColonne+col]!=0)
		return 'v';			//On retourne 1 si le premier terme de la matrice est différent de zéro
	else{
		for(i=col+1; i<p->nbLigne; i++){
			if(p->matrice[i*p->nbColonne]!=0){
				permuteLigne(p, i, 0);
				return 'v';	//On retourne si on a trouvé un pivot et qu'on a permuté deux lignes.
			}
		}
	}
	return 'v';
}

/*
* 2ème étape qui consiste à éliminer toutes les valeurs non nulles en dessous du pivot
*/

void eliminationGauss(gauss *p, int colonne, int decalage){
	int i, j, decal=decalage*p->nbColonne;
	float elimine;
	for (i=colonne+1-decalage; i<p->nbLigne; i++){
		if(p->matrice[i*p->nbColonne+colonne]!=0 && p->matrice[colonne*p->nbColonne+colonne-decal]!=0)
			elimine=p->matrice[i*p->nbColonne+colonne]/p->matrice[colonne*p->nbColonne+colonne-decal];
		else
			elimine=0;
		for(j=colonne;j<p->nbColonne; j++)
			p->matrice[i*p->nbColonne+j]=p->matrice[i*p->nbColonne+j]-elimine*p->matrice[colonne*p->nbColonne+j-decal];
	}
	affichage(p);
}

/*
* Fonction boucle qui va effectuer l'élimination tant que l'on peut
*/

void boucle(gauss *p){
	int i, j=0;
	for (i=0; i<p->nbColonne-1; i++){
		if(choixPivot(p, i)=='v')
			eliminationGauss(p, i, j);
		else
			j++;
	}
}

/*
* Fonction qui verifie que l'on a pas de ligne du type 0 0 0 b ou b!=0
*/

int systemIncomp(gauss *p){
	int i, j, compteur=0;
	for (i=0; i<p->nbLigne; i++){
		for(j=0; j<p->nbColonne-1; j++){
			if(p->matrice[i*p->nbColonne+j]==0)
				compteur++;
		}
		if(compteur==p->nbColonne-1 && p->matrice[i*p->nbColonne+p->nbColonne-1]!=0)
			return 0;		//On retourne 0 si b est différent de 0 et que toutes les autres valeurs de la ligne sont différentes de 0
	}
	return 1;				//On retourne 1 si on a pas trouvé de ligne du type 0 0 0 b ou b!=0
}

/*
* Fonction qui va vérifier si on a des variables libres dans notre matrice échelonné. On retourne le nombre de variable libre.
*/

int valLibre(gauss *p){
	int i, j, compteurLigne=0, compteurVar=0;
	for(i=0; i<p->nbLigne; i++){
		for(j=0; j<p->nbColonne; j++){
			if(p->matrice[i*p->nbColonne+j]==0)
				compteurVar++;			//On incrémente compteurVar si sur la ligne on trouve une valeur qui est égale à 0
		}
		if(compteurVar==p->nbColonne-i){
			compteurLigne++;			//On incrémente compteLigne si toutes les valeurs d'une ligne est égale à 0
			p->varLibre[i-1]++;
		}
		compteurVar=0;
	}
	return compteurLigne;				//On retourne le nombre de variable libre
}

/*
* Fonction solution qui va calculer les solutions du systeme compatible
*/

void solution(gauss *p){
	int i, j;
	float somme;
	p->solution[p->nbColonne-2]=p->matrice[(p->nbColonne*p->nbLigne)-1]/p->matrice[(p->nbColonne*p->nbLigne)-2];	//On rempli déjà X[nbColonne-1]
	for(i=p->nbColonne-3 ; i>=0 ;i--){			
           somme = 0 ;
           for(j=p->nbColonne-2; j>i; j--)
              somme=somme+p->matrice[i*p->nbColonne+j]*p->solution[j] ;
           p->solution[i]=(p->matrice[i*p->nbColonne+p->nbColonne-1]-somme)/p->matrice[i*p->nbColonne+i] ;
    }
    printf("Solutions :\n");
	for (i=0; i<p->nbColonne-1; i++)
		printf("X%d : %f\n", i+1, p->solution[i]);
	printf("\n");
}
/*
int checkLigneVide(gauss *p, int ligne){
	int i, compteur=0;
	for(i=0; i<p->nbColonne; i++){
		if(p->matrice[i*ligne+i]==0)
			compteur++;
	}
	if(compteur==p->nbColonne)
		return 1;
	return 0;
}
*/
/*
* Fonction infiniteSolution qui va afficher les solutions pour le système qui admet une infinité de solution.
*/

void infiniteSolution(gauss *p){
	printf("Il y a une ou plusieurs variables libres qui sont :\n");
	int i;
	char c[2];
	for(i=0; i<p->nbColonne-1; i++){
		if(p->varLibre[i]==1){
			printf("X%d\n", i+1);
			p->infiniSol[i][0]='X';
			sprintf(c, "%d", (i));
			p->infiniSol[i][1]=c[0];
			p->solution[i]=0;
		}
	}
	/*
	for(i=p->nbLigne-1; i>=0; i--){
		if(checkLigneVide(p, i)==0){

		}
	}
	*/
}

int main(){
	gauss p;
	if(recupInfoColLign(&p)==0)
		return 0;
	boucle(&p);
	if(systemIncomp(&p)==0){
		printf("Systeme incompatible car il y a une ligne du type 0 0 0 b ou b!=0.\n");
		return 0;
	}
	if(valLibre(&p)==0)
		solution(&p);
	else
		infiniteSolution(&p);
	libereTabMat(&p);
	return 0;
}