#include "editeur.h"
#include "constantes.h"
#include <stdio.h>

void initialiser_terrain_vide(Objet terrain[][NOMBRE_COLONNE])
/*****************************************************
RÔLE: initialiser un terrain vide pour l'editeur
*****************************************************/
{
	int i,j;

	for ( i = 0; i < NOMBRE_LIGNE; i++)
		for ( j = 0; j < NOMBRE_COLONNE; j++)
			terrain[i][j] = MUR;
}


void afficher_menu_editeur(SDL_Surface *ecran, SDL_Surface *spritesheet,Objet element)
/*****************************************************
*****************************************************/
{
	SDL_Rect src, dest;
	src.w = 50;
	src.h = 50;

	dest.w = 50;
	dest.h = 50;

	src.x = 9 * TAILLE_CASE;
	dest.x = LARGEUR_FENETRE-86;

	switch (element)
 	{
 		case MUR:
 			dest.y = 12;
    		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
 		break;

 		case CIBLE:
    		dest.y   += 72;
    		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
 		break;

 		case CAISSE:
    		dest.y   += 132;
    		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
 		break;

 		case PERSO_BAS:
    		dest.y   += 192;
    		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
 		break;

 		case COULOIR:
    		dest.y   += 252;
    		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
 		break;
 	}

    src.w = TAILLE_CASE;
	src.h = TAILLE_CASE;

	dest.w = TAILLE_CASE;
	dest.h = TAILLE_CASE;

   	src.x = MUR * TAILLE_CASE;
	dest.x = LARGEUR_FENETRE-78;
	dest.y = 20;
    SDL_BlitSurface(spritesheet, &src, ecran, &dest);

    src.x = CIBLE * TAILLE_CASE;
    dest.y   += 60;
    SDL_BlitSurface(spritesheet, &src, ecran, &dest);

    src.x = CAISSE * TAILLE_CASE;
    dest.y   += 60;
    SDL_BlitSurface(spritesheet, &src, ecran, &dest);

    src.x = PERSO_BAS * TAILLE_CASE;
    dest.y   += 60;
    SDL_BlitSurface(spritesheet, &src, ecran, &dest);

    src.x = COULOIR * TAILLE_CASE;
    dest.y   += 60;
    SDL_BlitSurface(spritesheet, &src, ecran, &dest);


    src.w = 107;
	src.h = 35;

	dest.w = 107;
	dest.h = 35;

    src.x = 356;
    dest.x   = LARGEUR_FENETRE-113;
    dest.y   += 60;
 	SDL_BlitSurface(spritesheet, &src, ecran, &dest);


 	SDL_Flip(ecran);
}



int menu_clic( SDL_Rect position, int *element, int terrain[][NOMBRE_COLONNE])
/*****************************************************
RÔLE: tester si on a un clic valable
	  actualiser la fenetre
	  modifier la variable element
*****************************************************/

{
	if (clic_okey(position))
    	if (mission_valable(terrain))
    		{
    			enregistrer_mission(terrain);
    			return 2;
    		}

	if (position.x > LARGEUR_FENETRE-78 && position.x < LARGEUR_FENETRE+44)
	{
		if (position.y > 20 && position.y < 20 +34 )
	    	*element = MUR;

		else if (position.y > 80 && position.y < 80 +34 )
	    	*element = CIBLE;

		else if (position.y > 140 && position.y < 140 +34 )
	    	*element = CAISSE;

		else if (position.y > 200 && position.y < 200 +34 )
	    	*element = PERSO_BAS;

	    else if (position.y > 260 && position.y < 260 +34 )
	    	*element = COULOIR;

		else return 0;

		return 1 ;

	}

}





int clic_okey(SDL_Rect position)
/*****************************************************
ROLE: retourner 1 si on a une clic valable "okey"
*****************************************************/
{
	if ((position.x) > LARGEUR_FENETRE-113 && (position.x) < LARGEUR_FENETRE-6 && (position.y) > 320 && (position.y) < 354)
		    return 1;
	return 0 ;

}
int mission_valable(int terrain[][NOMBRE_COLONNE])
/*****************************************************
*****************************************************/
{
	int nbre_objt = 0, nbre_caisse = 0, nbre_perso = 0, i, j;
	for (i = 0; i < NOMBRE_LIGNE; i++)
	{
		for (j = 0; j < NOMBRE_COLONNE; j++)
		{
			switch (terrain[i][j])
			{
				case CIBLE:
				nbre_objt++;
				break;
				case CAISSE:
				nbre_caisse++;
				break;
				case PERSO_BAS:
				case PERSO_HAUT:
				case PERSO_DROITE:
				case PERSO_GAUCHE:
				nbre_perso++;
				break;
			}
		}
	}
	if (nbre_caisse == nbre_objt && nbre_perso == 1)
		return 1 ;
	return 0 ;
}
void enregistrer_mission(int terrain[][NOMBRE_COLONNE])
/*****************************************************
*****************************************************/
{
	int i,j;
    FILE *fichier=NULL;
    fichier = fopen("DATA\\niveau.niv","a");
    if (fichier == NULL)
    {
        fprintf(stderr, "On peut pas ovrire la fichier missions\n");
        exit(-4);
    }

    fputc('\n',fichier);
    for (i=0 ; i < NOMBRE_LIGNE ; i++)
        for (j=0 ; j < NOMBRE_COLONNE ; j++)
            fputc((char)terrain[i][j]+'0',fichier);

    fclose(fichier);

}
int terrain_clic( SDL_Rect position, Objet element,int terrain[][NOMBRE_COLONNE])
/*****************************************************
*****************************************************/
{
	int i,j;
	if ( (position.x) > LARGEUR_FENETRE - MARGE ) return 0;
	j = (position.x)/TAILLE_CASE;
	i = (position.y)/TAILLE_CASE;
	terrain[i][j] = element;

	return 1 ;
}


