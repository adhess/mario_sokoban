#ifndef __DEF_JEU_H__
#define __DEF_JEU_H__

#include <stdlib.h>
#include <SDL.h>
#include "constantes.h"

	//charger la mission retourne 0 si tout va bien 1 sinon
	int charger_mission(Objet terrain[][NOMBRE_COLONNE], int numero_mission);

	void affiche_terrain(Objet terrain[][NOMBRE_COLONNE], SDL_Surface* ecran, SDL_Surface* spritesheet);

	int victoire_trouver(Objet terrain[][NOMBRE_COLONNE]);

	int deplacer_personnage(Objet terrain[][NOMBRE_COLONNE], Direction direction);

	static int position_personnage(Objet terrain[][NOMBRE_COLONNE],int *i, int *j);

	int nbre_mission();

	void afficher_menu_jeu(SDL_Surface *ecran,SDL_Surface *graphiques, int numero_theme);

	int changer_mission(SDL_Rect position, int *numero_mission,int nombre_missions);

	int changer_theme(SDL_Rect position, int *numero_theme);

	void afficher_vectoire(SDL_Surface *vectoire, SDL_Surface *ecran);

	int retrouver_terrain_precedent(Objet terrain[][NOMBRE_COLONNE],Objet sauvgarde[5][NOMBRE_LIGNE][NOMBRE_COLONNE], int *nombre_retour_possible);

	int sauvgarder_terrain(Objet terrain[][NOMBRE_COLONNE],Objet sauvgarde[][NOMBRE_LIGNE][NOMBRE_COLONNE], int *nombre_retour_possible);


#endif
