#include "ttfscore.h"
#include "draw.h"

SDL_Rect Teglalap( int x, int y, SDL_Texture *Tex )
{
	int TexSzel = 0;
	int TexMag = 0;
	SDL_QueryTexture( Tex, NULL, NULL, &TexSzel, &TexMag );
	SDL_Rect teglalap = { x, y, TexSzel, TexMag };												// Text�ra m�ret�t�l f�gg� t�glalapot kre�l
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
	// Az adott bet�t�pusb�l egy fel�letet hoz l�tre, amib�l pedig egy text�r�t, valamint meghat�rozza, hogy milyen x koordin�t�t�l kezd�dj�n a t�glalap
	TTF_Font	*Betutipus = TTF_OpenFont( "arial.ttf", 20 );
	SDL_Surface *Felulet = TTF_RenderText_Solid( Betutipus, szoveg, szovegszin );
	SDL_Texture *Tex = SDL_CreateTextureFromSurface( Renderelo, Felulet );

	// A sz�ml�l�sorba fog renderelni mindenk�pp, mert az y koordin�t�j�t 0-nak adtuk meg minden esetben
	SDL_Rect balfel = Teglalap( x, 0, Tex );

	// A l�trehozott text�r�t a l�trehozott kezd�si koordin�t�t�l f�gg� helyre rakja
	SDL_RenderCopy( Renderelo, Tex, NULL, &balfel );

	// Renderel�s ut�n felszabad�t�s
	TTF_Free( Betutipus, Felulet, Tex );
}

void maxpont_elerve( char *string, bool *quit, int nyertes )
{
	szamlalosor();													// Sz�ml�l�sor "t�rl�se" (r�renderel egy sz�rke cs�kot)
	strcpy( string, "NYERT" );
	if ( nyertes == 1 )
		TTF_Renderer( string, vZold, 3 + (CELLA + 1) * 3 );			// Sz�ml�l�sorban NYERTES ki�r�sa bal oldalon
	if ( nyertes == 2 )
		TTF_Renderer( string, vKek, 3 + 26 * (CELLA + 1) );			// Sz�ml�l�sorban NYERTES ki�r�sa jobb oldalon
	SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se
	SDL_Delay( 500 );
	*quit = true;
}