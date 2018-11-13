/*****************************************************
                       menu.c
*****************************************************/
#include "constantes.h"
#include <stdio.h>
# include "menu.h"
 

void affiche_menu(SDL_Surface *ecran, SDL_Surface *menu)
/*****************************************************
RÔLE: Centré et afficher le menu
*****************************************************/
{	
	SDL_Rect position;

	position.x=(LARGEUR_FENETRE - TAILLE_MENU)/2;
	position.y=(HAUTEUR_FENETRE - TAILLE_MENU)/2;

	SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));
	SDL_BlitSurface(menu,NULL, ecran, &position);
	SDL_Flip(ecran);
}
