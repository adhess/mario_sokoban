#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_Image.h>
#include <time.h>
#include "constantes.h"
#include "jeu.h"
#include "menu.h"
#include "editeur.h"


int main(int argc, char *argv[])
{
	SDL_Surface* ecran = NULL;
	SDL_Surface *graphiques = NULL, *menu = NULL, *vectoire = NULL;
    SDL_Rect position;
	int continuer = 1;
    int nombre_missions ,numero_mission = 1;
    int numero_theme = 1;
    int nombre_retour_possible = 0;
    SDL_Event event;
    Objet terrain [NOMBRE_LIGNE][NOMBRE_COLONNE];
    Objet sauvgarde[5][NOMBRE_LIGNE][NOMBRE_COLONNE];
    Objet element = MUR;
    Direction direction = AUCUNE;
    Etat etatDuJeu = MENU;



	if (SDL_Init(SDL_INIT_VIDEO) == -1) //tester l'initialisation video
    {
        fprintf(stderr, "ERREUR de l'initialisation video %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(!ecran)
    {
    	fprintf(stderr, "ERREUR impossible de créer la fenêtre SDL :(\n");
    	exit(EXIT_FAILURE);
    }


    menu = IMG_Load("DATA\\menu.png");
    if (!menu)
    {
        fprintf(stderr, "ERREUR fichier non trouver menu.png\n");
        exit(EXIT_FAILURE);
    }


    graphiques = IMG_Load("DATA\\spritesheet1.png");
    if (!graphiques)
    {
    	fprintf(stderr, "ERREUR fichier non trouver spritesheet.png\n");
    	exit(EXIT_FAILURE);
    }


    vectoire = IMG_Load("DATA\\vectoire.png");
    if (!vectoire)
    {
        fprintf(stderr, "ERREUR fichier non trouver vectoire.jpg\n");
        exit(EXIT_FAILURE);
    }

    nombre_missions = nbre_mission();
    affiche_menu(ecran,menu);
    SDL_EnableKeyRepeat(100, 50);



    while(continuer)
    {

    	SDL_WaitEvent(&event);
    	switch(event.type)
    	{
    		case SDL_QUIT :
    			continuer = 0;
    			break;
    		case SDL_KEYDOWN :
                switch (etatDuJeu)
                {
                    case MENU:

                        if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP1)
                            {
                                etatDuJeu = JEU;
                                charger_mission(terrain, numero_mission);
                                affiche_terrain(terrain,ecran, graphiques);
                                afficher_menu_jeu(ecran,graphiques,numero_theme);
                                nombre_retour_possible = 0;

                            }

                        if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP2)
                            {
                                etatDuJeu = EDITEUR;
                                initialiser_terrain_vide(terrain);
                                affiche_terrain(terrain,ecran, graphiques);
                                afficher_menu_editeur(ecran, graphiques,element);
                            }

                    break;


                    case JEU:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            {
                                etatDuJeu = MENU;
                                affiche_menu(ecran,menu);
                            }

            			switch(event.key.keysym.sym)
            			{
                            case SDLK_ESCAPE:
                                etatDuJeu = MENU;
                                affiche_menu(ecran,menu);
                            break;

            				case SDLK_UP:
        						direction = HAUT;
        					break;

        					case SDLK_DOWN:
                                direction = BAS;
        					break;

        					case SDLK_LEFT:
                                direction = GAUCHE;
        					break;

        					case SDLK_RIGHT:
                                direction = DROITE;
        					break;

                            case SDLK_BACKSPACE:


                               if (retrouver_terrain_precedent(terrain, sauvgarde, &nombre_retour_possible))
                                {
                                    affiche_terrain(terrain,ecran, graphiques);
                                    afficher_menu_jeu(ecran,graphiques,numero_theme);
                                }


                            break;
                        }
                        if (deplacer_personnage(terrain, direction))
            			{
                            sauvgarder_terrain(terrain, sauvgarde, &nombre_retour_possible);
                            affiche_terrain(terrain,ecran, graphiques);
                            afficher_menu_jeu(ecran,graphiques,numero_theme);
                            direction = AUCUNE;

                            if (victoire_trouver(terrain))
                            {
                                afficher_vectoire(vectoire, ecran);
                                SDL_Delay(1500);
                                numero_mission++;
                                if (numero_mission > nombre_missions)   numero_mission = 1;
                                charger_mission(terrain, numero_mission);
                                affiche_terrain(terrain,ecran, graphiques);
                                afficher_menu_jeu(ecran,graphiques,numero_theme);
                                nombre_retour_possible = 0;
                            }
                        }
                    break;

                    case EDITEUR:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            {
                                etatDuJeu = MENU;
                                affiche_menu(ecran,menu);
                            }
                    break;
                }
            break;


            case SDL_MOUSEBUTTONUP :
            switch (event.button.button)
            {

                case SDL_BUTTON_LEFT :
                    position.x = event.button.x;
                    position.y = event.button.y;
                    switch (etatDuJeu)
                    {
                        case EDITEUR:
                        if(terrain_clic(position, element, terrain))
                        {
                            affiche_terrain(terrain,ecran, graphiques);
                            afficher_menu_editeur(ecran, graphiques, element);
                        }

                        else switch(menu_clic( position, &element, terrain))
                            {
                                case 1:
                                affiche_terrain(terrain,ecran, graphiques);
                                afficher_menu_editeur(ecran, graphiques, element);
                                break;

                                case 2:
                                etatDuJeu = MENU;
                                affiche_menu(ecran,menu);
                                nombre_missions ++;
                                break;
                            }

                        break;
                        case JEU :
                        if(changer_mission(position, &numero_mission, nombre_missions))
                        {
                            charger_mission(terrain, numero_mission);
                            affiche_terrain(terrain,ecran, graphiques);
                            afficher_menu_jeu(ecran,graphiques,numero_theme);
                            nombre_retour_possible = 0;
                        }
                        if(changer_theme(position,&numero_theme))
                        {
                            switch(numero_theme)
                            {
                                case 1:
                                     graphiques = IMG_Load("DATA\\spritesheet1.png");
                                break;
                                case 2:
                                     graphiques = IMG_Load("DATA\\spritesheet2.png");
                                break;
                                case 3:
                                     graphiques = IMG_Load("DATA\\spritesheet3.png");
                                break;
                            }
                            affiche_terrain(terrain,ecran, graphiques);
                            afficher_menu_jeu(ecran,graphiques,numero_theme);
                        }
                        break;
                    }
                break;
            }


    	}

    }

    SDL_FreeSurface(vectoire);
    SDL_FreeSurface(graphiques);
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(menu);
    SDL_Quit();


	return EXIT_SUCCESS;
}
