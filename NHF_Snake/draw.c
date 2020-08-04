#include "draw.h"

void torol()
{
	// Képernyõ letörlése
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0x00 );									// Fekete
	SDL_RenderClear( Renderelo );																	// Kitölti az ablakot az utoljára beállított színnel.
																									// Igazából a színe teljesen lényegtelen, mert mindig fekete hátteret fogunk ezután renderelni.
}

void szamlalosor()
{
	// Szürke pontszámléc renderelése
	SDL_Rect pontszamlec = { 0, 0, KEP_SZEL, CELLA + 1 };
	SDL_SetRenderDrawColor( Renderelo, szurke.r, szurke.g, szurke.b, szurke.a );					// Szürke
	SDL_RenderFillRect( Renderelo, &pontszamlec );
}

void hatter()
{
	// Fekete háttér renderelése
	SDL_Rect teglalap = { 0, CELLA + 1, KEP_SZEL, TERULET };
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0xFF );									// Fekete
	SDL_RenderFillRect( Renderelo, &teglalap );
}

void negyzetracs()
{
	// Kék négyzetrács renderelése
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0xFF, 0xFF );									// Kék
	// Vízszintes vonalak
	for ( int i = CELLA + 1; i < KEP_MAG; i += CELLA + 1 )
		SDL_RenderDrawLine( Renderelo, 0, i, KEP_SZEL, i );
	// Függõleges vonalak
	for ( int i = 0; i < KEP_SZEL; i += CELLA + 1 )
		SDL_RenderDrawLine( Renderelo, i, CELLA + 1, i, KEP_MAG );
}

void alma()
{
	// Alma renderelése
	SDL_Rect almapoz = { apple.x * (CELLA + 1) + 1, apple.y * (CELLA + 1) + 1, CELLA, CELLA };
	SDL_SetRenderDrawColor( Renderelo, 0xFF, 0x00, 0x00, 0xFF );									// Piros
	SDL_RenderFillRect( Renderelo, &almapoz );
}

void rajzol()
{
	torol();
	hatter();
	szamlalosor();
	//negyzetracs();																				// Teszteléshez használatos többnyire
	alma();
}