#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "struktura.h"

#define KEP_SZEL			640														// 640 pixel sz�les lesz a k�p
#define KEP_MAG				500														// 500 pixel magas lesz a k�p ( sz�ml�l�sor + j�t�kter�let )
#define TERULET				480														// 480 pixel magas maga a j�t�kter�let
#define CELLA				TERULET  / 24 - 1										// 20-1 = 19 pixel lesz egy cella oldala (oszlopelv�laszt� vonallal egy�tt 20)
#define CELLA_IN_SOR		KEP_SZEL / ( CELLA + 1 )								// 32 db cella van 1 sorban		( 0,1, ... , 31 )
#define CELLA_IN_OSZLOP		TERULET  / ( CELLA + 1 )								// 24 db cella van 1 oszlopban  ( 1,2, ... , 24 ) - a 0. sor az sz�ml�l�sor

extern const SDL_Color vKek;
extern const SDL_Color sKek;
extern const SDL_Color vZold;
extern const SDL_Color sZold;
extern const SDL_Color szurke;

extern SDL_Window		*Ablak;
extern SDL_Renderer	*Renderelo;

extern Pont apple;

extern Kigyo *head1;
extern Kigyo *farok1;

extern Kigyo *head2;
extern Kigyo *farok2;

bool terulet_foglalt[CELLA_IN_OSZLOP][CELLA_IN_SOR];

#endif