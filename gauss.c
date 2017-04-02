#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"saisieM.c"
#include"couleurs_terminal.c"

#define TAILLE_MAX 10

/*********************************************

		PARTIE STRUCTURE DE DONNEE

*********************************************/

typedef struct{
	float *matrice;
	float *solution;
	char **infiniSol;
	char *nomFichier;
	int *varLibre;
	int nbColonne;
	int nbLigne;
}gauss;

typedef struct{
	char *fleche;
	int *menu;
	int nbChoix;
	char **choix;
} menu;

/*********************************************

		FIN PARTIE STRUCTURE DE DONNEE

*********************************************/


/*********************************************

			PARTIE PIVOT DE GAUSS

*********************************************/

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
	free(p->nomFichier);
}

/*
* On va afficher la matrice
*/

void affichage(gauss *p){
	int i;
	int caseTotal=p->nbLigne*p->nbColonne;
	for (i=0; i<caseTotal; i++){
		printf("%f\t", p->matrice[i]);		//On affiche les valeurs du tableau matrice de la structure de donnée
		if((i+1)%p->nbColonne==0)			//Dès qu'on arrive à la fin de la ligne on effectue un retour chariot
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
    fichier = fopen(p->nomFichier, "r");		//Ouverture du fichier en read only
    if(fichier==NULL){
    	printf("Impossible d'atteindre le fichier! Retour au menu principal\n");	//Si le fichier n'est pas accessible alors on sort de la fonction
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
    if(p->nbColonne==0 || p->nbLigne==0){  		//Si le nombre de colonnes et le nombre de ligne est égale à 0 alors sa signifie que le fichier choisit ne respecte pas les normes pour ce programme
    	printf("Le fichier que vous avez choisi n'est pas dans les normes de ce programme. \nRedirection vers le menu principal en cours...");
    	sleep(2);
    	return 0;
    }
    allouTabMat(p);					//Si tout c'est bien passé on va initialiser notre structure de donnée avec l'allocation dynamique de la mémoire
    initMatVec(p, fichier);			//On va ensuite initialiser notre structure de donnée avec les valeurs qui sont dans le fichier .txt
    fclose(fichier);				//Dès qu'on a fini de récupérer toutes les information on va fermer le fichier
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
		return 'f';			//Si le compteur est au maximum c'est à dire si toutes la colonne (col) contient que des zéros alors on retourne 0
	if(p->matrice[col*p->nbColonne+col]==0){  //Si le compteur n'est pas au maximum et que colonne (col) est égale à zéros, sa signifie qu'on doit permuter deux lignes
		for(i=col+1; i<p->nbLigne; i++){
			if(p->matrice[i*p->nbColonne+col]!=0){
				permuteLigne(p, i, col); //On permute la ligne dont la valeur n'est pas nul avec la ligne ou on doit avoir un pivot
				return 'v';	//On retourne si on a trouvé un pivot et qu'on a permuté deux lignes.
			}
		}
	}
	return 'v';
}

/*
* 2ème étape qui consiste à éliminer toutes les valeurs non nulles en dessous du pivot
* NOTE : Fonction trop compliquer à décrire par des phrases. (Meilleur solution : dessin sur papier)
*/

int eliminationGauss(gauss *p, int colonne, int decalage){
	int i, j, decal=decalage*p->nbColonne;
	float elimine=0, a=0, b=0;
	for (i=colonne+1-decalage; i<p->nbLigne; i++){
		if(p->matrice[i*p->nbColonne+colonne]!=0 && p->matrice[colonne*p->nbColonne+colonne-decal]!=0){		//On vérifie que les deux cases ne sont pas nul sinon le programme efectura un 0/0
			elimine=p->matrice[i*p->nbColonne+colonne]/p->matrice[colonne*p->nbColonne+colonne-decal];
			a=p->matrice[i*p->nbColonne+colonne];
			b=p->matrice[colonne*p->nbColonne+colonne-decal];
		}
		else
			return 0;
		for(j=colonne;j<p->nbColonne; j++)
			p->matrice[i*p->nbColonne+j]=p->matrice[i*p->nbColonne+j]-elimine*p->matrice[colonne*p->nbColonne+j-decal];
		printf("L%d = L%d+(%2.2f/%2.2f)*L%d\n", i+1, i+1, a, b, colonne+1-decalage);
	}
	affichage(p);
	return 1;
}

/*
* Fonction boucle qui va effectuer l'élimination tant que l'on peut
*/

void boucle(gauss *p){
	int i, j=0;
	char c;
	for (i=0; i<p->nbColonne-1; i++){
		c=choixPivot(p, i);
		if(c=='v')
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
				compteur++;							//On compte le nombre de 0 dans une ligne sauf la dernière colonne
		}
		if(compteur==p->nbColonne-1 && p->matrice[i*p->nbColonne+p->nbColonne-1]!=0)	//Si la ligne contient que des 0 sauf la dernière valeur alors le système est incompatible
			return 0;		//On retourne 0 si b est différent de 0 et que toutes les autres valeurs de la ligne sont différentes de 0
		compteur=0;
	}
	return 1;				//On retourne 1 si on a pas trouvé de ligne du type 0 0 0 b ou b!=0
}

/*
* Fonction qui va vérifier si on a des variables libres dans notre matrice échelonné. On retourne le nombre de variable libre.
*/

int valLibre(gauss *p){
	int i, j, compteurLigne=0, compteurVar=0, compteurVarNonZero=0;
	for(i=0; i<p->nbLigne; i++){
		for(j=0; j<p->nbColonne; j++){
			if(p->nbColonne>=p->nbLigne){ // On va vérifier les variables libres pour une matrice qui a plus d'inconnus que d'équation.
				if(p->matrice[i*p->nbColonne+j]!=0)
					compteurVarNonZero++;	//On compte les valeurs qui ne sont pas à zéro
			}
			if(p->matrice[i*p->nbColonne+j]==0)
				compteurVar++;			//On incrémente compteurVar si sur la ligne on trouve une valeur qui est égale à 0
		}
		if(compteurVar==p->nbColonne-i){
			compteurLigne++;			//On incrémente compteLigne si toutes les valeurs d'une ligne est égale à 0
			p->varLibre[i-1]++;
		}
		if(p->nbColonne>=p->nbLigne){	//Si on est dans le cas ou on a plus d'inconnues que d'équations
			if(compteurVarNonZero==3 || compteurVarNonZero==2 && p->matrice[(i+1)*p->nbColonne-1]==0){	//Si le compteur de valeur à zéro est à 3 ou à 2 et que la valeur à la dernière colonne est à 0
				p->varLibre[i+1]++;		//Alors on a une variable libre à i+1
				compteurLigne++;		//On incrémente le nombre de variable libre dans le programme
			}
		}
		compteurVar=0;					//On réinitialise les compteurs afin d'éviter d'avoir des valeurs faussées
		compteurVarNonZero=0;
	}
	return compteurLigne;				//On retourne le nombre de variable libre
}

/*
* Fonction solution qui va calculer les solutions du systeme compatible
*/

void solution(gauss *p){
	int i, j, k=p->nbLigne;
	float somme;
	if(k>=p->nbColonne)
		k=p->nbColonne-1;
	p->solution[p->nbColonne-2]=p->matrice[(p->nbColonne*k)-1]/p->matrice[(p->nbColonne*k)-2];	//On rempli déjà X[nbColonne-1]
	for(i=p->nbColonne-3 ; i>=0 ;i--){			
           somme = 0 ;
           for(j=p->nbColonne-2; j>i; j--)
              somme=somme+p->matrice[i*p->nbColonne+j]*p->solution[j] ;		//On fait la somme de toute les valeurs entre le pivot et la dernière colonne en multipliant ces valeur par les solutions trouvé précédemment
           p->solution[i]=(p->matrice[i*p->nbColonne+p->nbColonne-1]-somme)/p->matrice[i*p->nbColonne+i] ;	//Puis on soustrait la valeur de la dernière colonne par la somme du dessus et on divise le tout par la valeur du pivot
    }
    printf("Solutions :\n");
	for (i=0; i<p->nbColonne-1; i++)		//On affiche les solutions du système
		printf("X%d : %f\n", i+1, p->solution[i]);
	printf("\n");
}

/*
* Fonction infiniteSolution qui va afficher les solutions pour le système qui admet une infinité de solution.
*/

void infiniteSolution(gauss *p){
	printf("Il y a une ou plusieurs variables libres qui est/sont :\n");
	int i;
	char c[2];
	for(i=0; i<p->nbColonne-1; i++){
		if(p->varLibre[i]==1){				//Dans le cas d'une infinité de solution nous allons afficher seulement les inconnues qui sonts libres
			printf("X%d\n", i+1);
			p->infiniSol[i][0]='X';
			sprintf(c, "%d", (i));
			p->infiniSol[i][1]=c[0];
			p->solution[i]=0;
		}
	}
}

/*
* Fonction qui va demander à l'utilisateur de choisir un fichier .txt. Il affichera tous les fichiers disponible dans le répertoire courant
* Cette fonction vérifira que le fichier .txt tapé par l'utilisateur existe bien dans le répertoire.
*/

int choixFichier(gauss *p){
	FILE* fich=NULL;
	memset (p->nomFichier, 0, sizeof (p->nomFichier));		//Réinitialisation de la variable p->nomFichier pour être sur qu'il ne contient rien à l'intérieur
	printf("Choisissez un fichier .txt à lire parmi les choix suivant\nNOTE : Si le .txt ne correspond pas au norme pour ce programme, vous serez immédiatement redirigé vers le menu principal.\n\nFichiers disponible dans le répertoire courant : \n");
	system("ls *.txt");				//On affiche sur le terminal tous les fichier qui termine par .txt (Ils ne sont pas forcément tous au norme du programme)
	scanf("%s", p->nomFichier);		//On demande à l'utilisateur de saisir le nom du fichier qu'il souhaite lire et exécuter
	fich=fopen(p->nomFichier, "r");	//On ouvre le fichier grâce au nom que l'utilisateur à rentré
	if(fich==NULL){					//Si le fichier n'est pas accessible
		printf("Impossible d'atteindre le fichier. Vous avez peut-être fait une erreur de frappe lors de la saisie du nom du fichier. Veuillez recommencer ...\n");
		sleep(2);					//Sa veut dire que l'utilisateur a rentré un mauvais nom de fichier ou qu'il a fait une faute de frappe 
		clear_terminal();
		return 0;					//On retourne 0 si c'est le cas
	}
	fclose(fich);
	return 1;						//On retourne 1 si tout c'est bien passé (cad si le fichier .txt existe et non si il est dans les normes du programme)
}

/*
* Fonction lanceGauss qui va lancer l'algorithme de Gauss 
*/

int lanceGauss(){
	gauss p;
	p.nomFichier=malloc(30*sizeof(char));//On alloue dynamiquement de l'espace mémoire à la chaîne de caractère p.nomFichier afin de mettre le nom du fichier à exécuter
	int i=choixFichier(&p);				//On récupère le nom du fichier que l'utilisateur à rentré
	while (i==0)
		i=choixFichier(&p);				//tant que le fichier n'est pas bon on lui redemande de taper le nom d'un fichier valide
	if(recupInfoColLign(&p)==0)			//On récupère les informations du fichier
		return 0;						//Si on a pas réussi à récupérer les données des fichiers ou si le fichier n'est pas dans les normes du programme alors on revient sur le menu principal
	boucle(&p);							//On va effectuer l'étape n°3 de l'algorithme de gauss qui consiste à boucler
	if(systemIncomp(&p)==0){			//On regarde si le incompatible ou non
		printf("Systeme incompatible car il y a une ligne du type 0 0 0 b ou b!=0.\n");  //Si il est incompatible alors on affiche un message et on reviens sur le menu
		return 0;
	}
	if(valLibre(&p)==0)					//On vérifie qu'il n'y a aucunes variables libres
		solution(&p);					//Si il y en a aucune alors on cherche les solutions
	else
		infiniteSolution(&p);			//Sinon on affiche les variables libres du système sans les calculer
	libereTabMat(&p);					//On libère la mémoire alloué durant tout le programme
	return 0;							//On retourne au menu principal
}

/*********************************************

			FIN PARTIE PIVOT DE GAUSS

*********************************************/

/*********************************************

				PARTIE MENU

*********************************************/


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
		p->choix[i]=malloc(15*sizeof(char));
	for (i=0; i<(p->nbChoix); i++)
		p->menu[i]=i+1;
	p->fleche[0]='>';
	p->choix[0]="Pivot de Gauss";
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
	int i;
	initialisationMenu(&p, 3);
	affichageMenu(&p);
	while(i!=3){
		i=lanceMenu(&p);
		switch (i){
			case 1 : 
				lanceGauss();
				break;
			case 2 :
				printf("La fonction Gauss-Jordan est en cours de développement. Merci de patienter.\nSortie de programme en cours ...\n");
				sleep(2);
				return 0;
				break;
			case 3 :
				libereMemoire(&p);
				break;
		} 
	}
	return 0;
}


/*********************************************

				FIN MENU

*********************************************/


int main(){
	menuD();
	return 0;
}