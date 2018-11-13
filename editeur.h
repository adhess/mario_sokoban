#ifndef __DEF_EDITEUR_H__
#define __DEF_EDITEUR_H__

#include <stdlib.h>
#include <SDL.h>
#include "constantes.h"

	void initialiser_terrain_vide(Objet terrain[][NOMBRE_COLONNE]);

	int menu_clic( SDL_Rect position, int *element, int terrain[][NOMBRE_COLONNE]);

	void afficher_menu_editeur(SDL_Surface *ecran, SDL_Surface *spritesheet,Objet element);

	int clic_okey(SDL_Rect position);

	int mission_valable(int terrain[][NOMBRE_COLONNE]);

	void enregistrer_mission(int terrain[][NOMBRE_COLONNE]);

	int terrain_clic( SDL_Rect position, Objet element,int terrain[][NOMBRE_COLONNE]);

	void terrain_clic_droite( SDL_Rect position, int terrain[][NOMBRE_COLONNE], SDL_Surface *ecran,SDL_Surface *vide,int element);

	int supprimer_perso(SDL_Surface ecran,int terrain[][NOMBRE_COLONNE],int i,int j, int VIDE);



#endif
