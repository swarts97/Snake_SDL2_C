#include "draw.h"

void torol()
{
	// K�perny� let�rl�se
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0x00 );									// Fekete
	SDL_RenderClear( Renderelo );																	// Kit�lti az ablakot az utolj�ra be�ll�tott sz�nnel.
																									// Igaz�b�l a sz�ne teljesen l�nyegtelen, mert mindig fekete h�tteret fogunk ezut�n renderelni.
}

void szamlalosor()
{
	// Sz�rke pontsz�ml�c renderel�se
	SDL_Rect pontszamlec = { 0, 0, KEP_SZEL, CELLA + 1 };
	SDL_SetRenderDrawColor( Renderelo, szurke.r, szurke.g, szurke.b, szurke.a );					// Sz�rke
	SDL_RenderFillRect( Renderelo, &pontszamlec );
}

void hatter()
{
	// Fekete h�tt�r renderel�se
	SDL_Rect teglalap = { 0, CELLA + 1, KEP_SZEL, TERULET };
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0xFF );									// Fekete
	SDL_RenderFillRect( Renderelo, &teglalap );
}

void negyzetracs()
{
	// K�k n�gyzetr�cs renderel�se
	SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0xFF, 0xFF );									// K�k
	// V�zszintes vonalak
	for ( int i = CELLA + 1; i < KEP_MAG; i += CELLA + 1 )
		SDL_RenderDrawLine( Renderelo, 0, i, KEP_SZEL, i );
	// F�gg�leges vonalak
	for ( int i = 0; i < KEP_SZEL; i += CELLA + 1 )
		SDL_RenderDrawLine( Renderelo, i, CELLA + 1, i, KEP_MAG );
}

void alma()
{
	// Alma renderel�se
	SDL_Rect almapoz = { apple.x * (CELLA + 1) + 1, apple.y * (CELLA + 1) + 1, CELLA, CELLA };
	SDL_SetRenderDrawColor( Renderelo, 0xFF, 0x00, 0x00, 0xFF );									// Piros
	SDL_RenderFillRect( Renderelo, &almapoz );
}

void rajzol()
{
	torol();
	hatter();
	szamlalosor();
	//negyzetracs();																				// Tesztel�shez haszn�latos t�bbnyire
	alma();
}