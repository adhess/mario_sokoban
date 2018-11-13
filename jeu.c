#include "jeu.h"
#include "constantes.h"
#include <stdio.h>

int charger_mission(Objet terrain[][NOMBRE_COLONNE], int numero_mission)
{
	FILE* fichier = NULL;
	int i, j, s= 1;
	char c;
	fichier = fopen ("DATA\\niveau.niv", "r");
	if (fichier == NULL)
	{
		for (i=0; i<NOMBRE_LIGNE; ++i)
			for (j=0; j<NOMBRE_COLONNE; ++j)
				terrain[i][j] = MUR;
		return -1;
	}
	//positionner le curseur
	rewind(fichier);
	while (s != numero_mission && ((c = getc(fichier)) != EOF))
		if (c == '\n')
			s++;


	for (i=0; i<NOMBRE_LIGNE; ++i)
		for (j=0; j<NOMBRE_COLONNE; ++j)
		{
				(c = fgetc (fichier));
				if(c!= EOF)
					terrain[i][j] = (Objet) (c - '0');
				else
					terrain[i][j] = COULOIR;
		}

    fclose (fichier);

	return 0;

}

void affiche_terrain(Objet terrain[][NOMBRE_COLONNE], SDL_Surface* ecran, SDL_Surface* spritesheet)
{
	SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
	int i,j;
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = TAILLE_CASE;
	src.h = TAILLE_CASE;

	dest.w = TAILLE_CASE;
	dest.h = TAILLE_CASE;

	for (i = 0; i < NOMBRE_LIGNE; ++i)
		for (j = 0; j < NOMBRE_COLONNE; ++j)
		{
			switch (terrain[i][j])
			{
				case CAISSE:
				case CAISSE_SUR_CIBLE:
				case CIBLE:
				case COULOIR:
				case MUR:
		       	case PERSO_BAS:
		       	case PERSO_DROITE:
		        case PERSO_GAUCHE:
		        case PERSO_HAUT:

		        	src.x = COULOIR * TAILLE_CASE;
		       		dest.y = i * TAILLE_CASE;
		       		dest.x = j * TAILLE_CASE;
		       		SDL_BlitSurface(spritesheet, &src, ecran, &dest);

		       		src.x = terrain[i][j] * TAILLE_CASE;
		       		dest.y = i * TAILLE_CASE;
		       		dest.x = j * TAILLE_CASE;
		       		SDL_BlitSurface(spritesheet, &src, ecran, &dest);
					break;

				case PERSO_BAS_SUR_CIBLE:
				case PERSO_DROITE_SUR_CIBLE:
				case PERSO_GAUCHE_SUR_CIBLE:
				case PERSO_HAUT_SUR_CIBLE:

		       		dest.y = i * TAILLE_CASE;
		       		dest.x = j * TAILLE_CASE;

		       		src.x = COULOIR * TAILLE_CASE;
		       		SDL_BlitSurface(spritesheet, &src, ecran, &dest);

					src.x = CIBLE * TAILLE_CASE;
		       		SDL_BlitSurface(spritesheet, &src, ecran, &dest);

		       		src.x = (terrain[i][j] - 4) * TAILLE_CASE;
		 		    SDL_BlitSurface(spritesheet, &src, ecran, &dest);
		       		break;
			}
		}

	SDL_Flip(ecran);
}

int victoire_trouver(Objet terrain[][NOMBRE_COLONNE])
{
	int h,k;
	for(h=0; h < NOMBRE_LIGNE; ++h)
		for(k=0; k < NOMBRE_COLONNE; ++k)
			if (terrain[h][k] == CIBLE || (PERSO_BAS_SUR_CIBLE <= terrain[h][k] && terrain[h][k] <= PERSO_HAUT_SUR_CIBLE))
				return 0;
	return 1;
}

int deplacer_personnage(Objet terrain[][NOMBRE_COLONNE], Direction direction)
{
	int i, j;
	int bool = 0;

	if (position_personnage(terrain,&i,&j))
	{
		switch(direction)
		{
			case HAUT :
				if (i != 0)
				{
					if(terrain[i-1][j] == COULOIR)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i-1][j] = PERSO_HAUT;
							bool = 1;
						}
					if(terrain[i-1][j] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i-1][j] = PERSO_HAUT_SUR_CIBLE;
							bool = 1;
						}
					if(terrain[i-1][j] == CAISSE && i > 1)
					{
						if(terrain[i-2][j] == COULOIR || terrain[i-2][j] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;
							else {terrain[i][j] = COULOIR;}

							if (terrain[i-2][j] == CIBLE)
							terrain[i-2][j] = CAISSE_SUR_CIBLE;
							else terrain[i-2][j] = CAISSE;
							terrain[i-1][j] = PERSO_HAUT;

							bool = 1;

						}
					}
					if (terrain[i-1][j] == CAISSE_SUR_CIBLE && (terrain[i-2][j] == COULOIR || terrain[i-2][j] == CIBLE))
					{
						if (terrain[i][j] <= PERSO_HAUT_SUR_CIBLE && terrain[i][j] >= PERSO_BAS_SUR_CIBLE)
							terrain[i][j] = CIBLE;
						else terrain[i][j] = COULOIR;

						terrain[i-1][j] = PERSO_GAUCHE_SUR_CIBLE;
						if ( terrain[i-2][j] == COULOIR) terrain[i-2][j] = CAISSE;
						else terrain[i-2][j] = CAISSE_SUR_CIBLE;

						bool = 1;
					}
				}
				break;

			case BAS :
				if (i < NOMBRE_LIGNE)
				{
					if(terrain[i+1][j] == COULOIR)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i+1][j] = PERSO_BAS;

							bool = 1;
						}
					if(terrain[i+1][j] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i+1][j] = PERSO_BAS_SUR_CIBLE;

							bool = 1;
						}
					if(terrain[i+1][j] == CAISSE && i < NOMBRE_LIGNE-1)
					{
						if(terrain[i+2][j] == COULOIR || terrain[i+2][j] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}

							if (terrain[i+2][j] == CIBLE)
							terrain[i+2][j] = CAISSE_SUR_CIBLE;
							else terrain[i+2][j] = CAISSE;
							terrain[i+1][j] = PERSO_BAS;

							bool = 1;
						}
					}
					if (terrain[i+1][j] == CAISSE_SUR_CIBLE && (terrain[i+2][j] == COULOIR || terrain[i+2][j] == CIBLE))
					{
						if (terrain[i][j] <= PERSO_HAUT_SUR_CIBLE && terrain[i][j] >= PERSO_BAS_SUR_CIBLE)
							terrain[i][j] = CIBLE;
						else terrain[i][j] = COULOIR;

						terrain[i+1][j] = PERSO_GAUCHE_SUR_CIBLE;
						if ( terrain[i+2][j] == COULOIR) terrain[i+2][j] = CAISSE;
						else terrain[i+2][j] = CAISSE_SUR_CIBLE;

						bool = 1;
					}
				}
				break;

			case GAUCHE :
				if (j != 0)
				{
					if(terrain[i][j-1] == COULOIR)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i][j-1] = PERSO_GAUCHE;

							bool = 1;
						}
					if(terrain[i][j-1] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i][j-1] = PERSO_GAUCHE_SUR_CIBLE;

							bool = 1;
						}
					if(terrain[i][j-1] == CAISSE && j > 1)
					{
						if(terrain[i][j-2] == COULOIR || terrain[i][j-2] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							if (terrain[i][j-2] == CIBLE)
							terrain[i][j-2] = CAISSE_SUR_CIBLE;
							else terrain[i][j-2] = CAISSE;
							terrain[i][j-1] = PERSO_GAUCHE;

							bool = 1;
						}
					}
					if (terrain[i][j-1] == CAISSE_SUR_CIBLE && (terrain[i][j-2] == COULOIR || terrain[i][j-2] == CIBLE))
					{
						if (terrain[i][j] <= PERSO_HAUT_SUR_CIBLE && terrain[i][j] >= PERSO_BAS_SUR_CIBLE)
							terrain[i][j] = CIBLE;
						else terrain[i][j] = COULOIR;

						terrain[i][j-1] = PERSO_GAUCHE_SUR_CIBLE;
						if ( terrain[i][j-2] == COULOIR) terrain[i][j-2] = CAISSE;
						else terrain[i][j-2] = CAISSE_SUR_CIBLE;

						bool = 1;
					}

				}
				break;

			case DROITE :
				if (i < NOMBRE_COLONNE)
				{
					if(terrain[i][j+1] == COULOIR)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i][j+1] = PERSO_DROITE;

							bool = 1;
						}
					if(terrain[i][j+1] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}
							terrain[i][j+1] = PERSO_DROITE_SUR_CIBLE;

							bool = 1;
						}
					if(terrain[i][j+1] == CAISSE && j < NOMBRE_COLONNE-1)
					{
						if(terrain[i][j+2] == COULOIR  || terrain[i][j+2] == CIBLE)
						{
							if(terrain[i][j] >= PERSO_BAS_SUR_CIBLE   && terrain[i][j] <= PERSO_HAUT_SUR_CIBLE)
								terrain[i][j] = CIBLE;

							else {terrain[i][j] = COULOIR;}

							if (terrain[i][j+2] == CIBLE)
							terrain[i][j+2] = CAISSE_SUR_CIBLE;
							else terrain[i][j+2] = CAISSE;

							terrain[i][j+1] = PERSO_DROITE;

							bool = 1;
						}
					}
					if (terrain[i][j+1] == CAISSE_SUR_CIBLE && (terrain[i][j+2] == COULOIR || terrain[i][j+2] == CIBLE))
					{
						if (terrain[i][j] <= PERSO_HAUT_SUR_CIBLE && terrain[i][j] >= PERSO_BAS_SUR_CIBLE)
							terrain[i][j] = CIBLE;
						else terrain[i][j] = COULOIR;

						terrain[i][j+1] = PERSO_DROITE_SUR_CIBLE;
						if ( terrain[i][j+2] == COULOIR) terrain[i][j+2] = CAISSE;
						else terrain[i][j+2] = CAISSE_SUR_CIBLE;

						bool = 1;
					}

				}
				break;
		}
	}

	return bool;
}


static int position_personnage(Objet terrain[][NOMBRE_COLONNE],int *i, int *j)
{
	int h,k;

	for(h=0; h < NOMBRE_LIGNE; ++h)
		for(k=0; k < NOMBRE_COLONNE; ++k)
			if(terrain[h][k] >= PERSO_BAS   && terrain[h][k] <= PERSO_HAUT_SUR_CIBLE)
			{
				*i = h;
				*j = k;
				return 1 ;
			}
	return 0 ;


}


int nbre_mission()
/*****************************************************
*****************************************************/
{
	int s=1;
	char caractere;
    FILE *fichier=NULL;
    fichier = fopen("DATA\\niveau.niv","r");
    if (fichier == NULL)
    {
        fprintf(stderr, "On peut pas ovrire la fichier missions\n");
        exit(-4);
    }
    rewind(fichier);
    caractere = getc(fichier);
    while (caractere != EOF)
    {
    	if (caractere == '\n')
        		s++;
        caractere = getc(fichier);
    }
    fclose(fichier);
    return s ;
}
void afficher_menu_jeu(SDL_Surface *ecran,SDL_Surface *graphiques,int numero_theme)
{
	SDL_Rect src, dest;
		src.w = 50;
		src.h = 50;

		dest.w = 50;
		dest.h = 50;

		src.x = 306;
		dest.x = LARGEUR_FENETRE-(MARGE + 6)/2-22;

	switch(numero_theme)
	{
		case 1:
			dest.y = (8+34);
		break;

		case 2:
			dest.y = (68+34);
		break;

		case 3:
			dest.y = (128+34);
		break;
	}
	SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter le cadre

	src.w = 100;
	src.h = 34;

	dest.w = 100;
	dest.h = 34;

   	src.x = 599;
	dest.x = LARGEUR_FENETRE-120;
	dest.y = 0;
    SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter le théme


	src.w = 136;
	src.h = 57;

	dest.w = 136;
	dest.h = 57;

   	src.x = 463;
	dest.x = LARGEUR_FENETRE-136;
	dest.y = 450;
    SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter les fleshs

    src.w = 6;
	src.h = 20;

	dest.w = 6;
	dest.h = 20;

   	src.x = 699;
	dest.x = LARGEUR_FENETRE-(MARGE + 6)/2;
	dest.y = 20+34;
    SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter le chiffre 1

    src.w = 13;
	src.h = 20;

	dest.w = 13;
	dest.h = 20;

   	src.x = 705;
	dest.x = LARGEUR_FENETRE-(MARGE + 13)/2;
	dest.y += 60;
    SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter le chiffre 2

    src.w = 13;
	src.h = 20;

	dest.w = 13;
	dest.h = 20;

   	src.x = 718;
	dest.x = LARGEUR_FENETRE-(MARGE + 13)/2;
	dest.y += 60;
    SDL_BlitSurface(graphiques, &src, ecran, &dest);//bliter le chiffre 3
    SDL_Flip(ecran);
}
int changer_mission(SDL_Rect position, int *numero_mission,int nombre_missions)
{
	if (position.x > LARGEUR_FENETRE-136 && position.y > HAUTEUR_FENETRE-57)
	{
		if (position.x > LARGEUR_FENETRE - 68 && position.x < LARGEUR_FENETRE)
		{
			(*numero_mission)++;
			if(*numero_mission == nombre_missions + 1) *numero_mission = 1;
			else if(*numero_mission == 0) *numero_mission = nombre_missions;
			return 1;
		}
		else
		{
			(*numero_mission)--;
			if(*numero_mission == nombre_missions + 1) *numero_mission = 1;
			else if(*numero_mission == 0) *numero_mission = nombre_missions;
			return 1;
		}

	}
	return 0;
}
int changer_theme(SDL_Rect position, int *numero_theme)
{
	if (position.x < 773 && position.x > 723)
	{
		if (position.y > 42 && position.y < 92)
		{
			(*numero_theme) = 1;
			return 1;
		}
		if (position.y > 102 && position.y < 152)
		{
			(*numero_theme) = 2;
			return 1;
		}
		if (position.y > 162 && position.y < 212)
		{
			(*numero_theme) = 3;
			return 1;
		}
	}
	return 0;
}


void afficher_vectoire(SDL_Surface *vectoire, SDL_Surface *ecran)
{
	SDL_Rect position;
	position.x = 0;
    position.y = 0;
    SDL_BlitSurface(vectoire, NULL, ecran, &position);
    SDL_Flip(ecran);
}

int sauvgarder_terrain(Objet terrain[][NOMBRE_COLONNE], Objet sauvgarde[5][NOMBRE_LIGNE][NOMBRE_COLONNE], int *nombre_retour_possible)
{
	int h, i, j;

	for (h = 0; h < 4; ++h)
		for (i = 0; i < NOMBRE_LIGNE; ++i)
			for (j = 0; j < NOMBRE_COLONNE; ++j)
				sauvgarde[h][i][j] = sauvgarde[h+1][i][j];

	for (i = 0; i < NOMBRE_LIGNE; ++i)
		for (j = 0; j < NOMBRE_COLONNE; ++j)
			sauvgarde[4][i][j] = terrain[i][j];

	if (*nombre_retour_possible <= 5) (*nombre_retour_possible) += 1;
}

int retrouver_terrain_precedent(Objet terrain[][NOMBRE_COLONNE], Objet sauvgarde[5][NOMBRE_LIGNE][NOMBRE_COLONNE], int *nombre_retour_possible)
{
	int h, i, j;

	if ((*nombre_retour_possible) > 0)
	{
		for (h = 0; h < 4; ++h)
			for (i = 0; i < NOMBRE_LIGNE; ++i)
				for (j = 0; j < NOMBRE_COLONNE; ++j)
					sauvgarde[4-h][i][j] = sauvgarde[3-h][i][j];

		for (i = 0; i < NOMBRE_LIGNE; ++i)
			for (j = 0; j < NOMBRE_COLONNE; ++j)
				terrain[i][j] = sauvgarde[4][i][j];

		(*nombre_retour_possible) -= 1;
		return 1;
	}
	return 0;
}
