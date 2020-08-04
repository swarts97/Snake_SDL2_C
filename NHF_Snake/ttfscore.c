#include "ttfscore.h"
#include "draw.h"

SDL_Rect Teglalap( int x, int y, SDL_Texture *Tex )
{
	int TexSzel = 0;
	int TexMag = 0;
	SDL_QueryTexture( Tex, NULL, NULL, &TexSzel, &TexMag );
	SDL_Rect teglalap = { x, y, TexSzel, TexMag };												// Textúra méretétõl függõ téglalapot kreál
	return teglalap;
}

void TTF_Free( TTF_Font *Betutipus, SDL_Surface *Felulet, SDL_Texture *Tex )
{
	TTF_CloseFont( Betutipus );
	Betutipus = NULL;
	SDL_FreeSurface( Felulet );
	Felulet = NULL;
	SDL_DestroyTexture( Tex );
	Tex = NULL;
}

void TTF_Renderer( char *szoveg, SDL_Color szovegszin, int x )
{
	// Az adott betûtípusból egy felületet hoz létre, amibõl pedig egy textúrát, valamint meghatározza, hogy milyen x koordinátától kezdõdjön a téglalap
	TTF_Font	*Betutipus = TTF_OpenFont( "arial.ttf", 20 );
	SDL_Surface *Felulet = TTF_RenderText_Solid( Betutipus, szoveg, szovegszin );
	SDL_Texture *Tex = SDL_CreateTextureFromSurface( Renderelo, Felulet );

	// A számlálósorba fog renderelni mindenképp, mert az y koordinátáját 0-nak adtuk meg minden esetben
	SDL_Rect balfel = Teglalap( x, 0, Tex );

	// A létrehozott textúrát a létrehozott kezdési koordinátától függõ helyre rakja
	SDL_RenderCopy( Renderelo, Tex, NULL, &balfel );

	// Renderelés után felszabadítás
	TTF_Free( Betutipus, Felulet, Tex );
}

void maxpont_elerve( char *string, bool *quit, int nyertes )
{
	szamlalosor();													// Számlálósor "törlése" (rárenderel egy szürke csíkot)
	strcpy( string, "NYERT" );
	if ( nyertes == 1 )
		TTF_Renderer( string, vZold, 3 + (CELLA + 1) * 3 );			// Számlálósorban NYERTES kiírása bal oldalon
	if ( nyertes == 2 )
		TTF_Renderer( string, vKek, 3 + 26 * (CELLA + 1) );			// Számlálósorban NYERTES kiírása jobb oldalon
	SDL_RenderPresent( Renderelo );									// E változás megjelenítése
	SDL_Delay( 500 );
	*quit = true;
}