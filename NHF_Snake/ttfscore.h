#ifndef TTFSCORE_H_INCLUDED
#define TTFSCORE_H_INCLUDED

#include "def.h"

extern SDL_Rect Teglalap( int, int, SDL_Texture * );
extern void TTF_Free( TTF_Font *, SDL_Surface *, SDL_Texture * );
extern void TTF_Renderer( char *, SDL_Color, int );
extern void maxpont_elerve( char *, bool *, int );

#endif