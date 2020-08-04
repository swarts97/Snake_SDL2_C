#include "startclose.h"

bool init()																			// Inicializáló függvény
{
	bool siker = true;																// Inicializációs flag, a függvény visszatérési értéke
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )											// SDL inicializálása
	{
		printf( "SDL Inicializalasi problema! SDL Error: %s\n", SDL_GetError() );	// Az SDL_GetError mondja majd meg nekünk hogy mi a hiba
		siker = false;
	}
	else
	{
		Ablak = SDL_CreateWindow( "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, KEP_SZEL, KEP_MAG, SDL_WINDOW_SHOWN );
		if ( Ablak == NULL )
		{
			printf( "Az ablakot nem lehet letrehozni. SDL Error: %s\n", SDL_GetError() );
			siker = false;
		}
		else
		{
			Renderelo = SDL_CreateRenderer( Ablak, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if ( Renderelo == NULL )
			{
				printf( "Nem sikerult elinditani a renderert. SDL Error: %s\n", SDL_GetError() );
				siker = false;
			}
			else
			{
				// Renderer szín inicializálás - SDL_RenderClear "ezzel a színnel fog törölni"
				SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0x00 );								// Fekete

				// TTF inicializálás
				if ( TTF_Init() == -1 )
				{
					printf( "A TTF-et nem sikerult inicializalni. TTF Error: %s\n", TTF_GetError() );
					siker = false;
				}
				else
				{
					// Betûtípus
					TTF_Font *Betutipus = TTF_OpenFont( "arial.ttf", 20 );
					if ( Betutipus == NULL )
					{
						printf( "A betutipust nem sikerult beolvasni. TTF Error: %s\n", TTF_GetError() );
						siker = false;
					}
				}
			}
		}
	}
	return siker;
}

void close()											// Bezáró függvény
{
	// Renderer
	SDL_DestroyRenderer( Renderelo );					// Renderelõ törlése
	Renderelo = NULL;

	// Ablak
	SDL_DestroyWindow( Ablak );							// Ablak törlése
	Ablak = NULL;

	// Exit
	TTF_Quit;											// Kilépés a  TTF-bõl
	SDL_Quit;											// Kilépés az SDL-bõl
}