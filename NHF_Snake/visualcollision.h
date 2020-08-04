#ifndef VISUALCOLLISION_H_INCLUDED
#define VISUALCOLLISION_H_INCLUDED

#include "def.h"

extern int kigyohossz( Kigyo * );
extern void kigyo( Kigyo *, SDL_Color, SDL_Color );
extern Pont nextkoord( Kigyo * );
extern bool utkozes( int, int * );

#endif