/***************************************************
			          Constantes.c
***************************************************/

# ifndef DEF_CONSTANTES
# define DEF_CONSTANTES

	# define TAILLE_CASE     34

	# define NOMBRE_COLONNE  20
	# define NOMBRE_LIGNE    15

	# define MARGE           136
	# define TAILLE_MENU	 408

	# define LARGEUR_FENETRE TAILLE_CASE * NOMBRE_COLONNE + MARGE
	# define HAUTEUR_FENETRE TAILLE_CASE * NOMBRE_LIGNE

	typedef enum {
		       CAISSE, CAISSE_SUR_CIBLE, CIBLE, COULOIR, MUR,
		       PERSO_BAS, PERSO_DROITE,  PERSO_GAUCHE, PERSO_HAUT,
			   PERSO_BAS_SUR_CIBLE, PERSO_DROITE_SUR_CIBLE, PERSO_GAUCHE_SUR_CIBLE, PERSO_HAUT_SUR_CIBLE
			   }Objet;

	typedef enum {BAS, DROITE, GAUCHE, HAUT,AUCUNE}Direction;
	typedef enum {MENU,JEU,EDITEUR}Etat;

# endif
