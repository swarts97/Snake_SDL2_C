#include "startclose.h"

bool init()																			// Inicializ�l� f�ggv�ny
{
	bool siker = true;																// Inicializ�ci�s flag, a f�ggv�ny visszat�r�si �rt�ke
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )											// SDL inicializ�l�sa
	{
		printf( "SDL Inicializalasi problema! SDL Error: %s\n", SDL_GetError() );	// Az SDL_GetError mondja majd meg nek�nk hogy mi a hiba
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
				// Renderer sz�n inicializ�l�s - SDL_RenderClear "ezzel a sz�nnel fog t�r�lni"
				SDL_SetRenderDrawColor( Renderelo, 0x00, 0x00, 0x00, 0x00 );								// Fekete

				// TTF inicializ�l�s
				if ( TTF_Init() == -1 )
				{
					printf( "A TTF-et nem sikerult inicializalni. TTF Error: %s\n", TTF_GetError() );
					siker = false;
				}
				else
				{
					// Bet�t�pus
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

void close()											// Bez�r� f�ggv�ny
{
	// Renderer
	SDL_DestroyRenderer( Renderelo );					// Renderel� t�rl�se
	Renderelo = NULL;

	// Ablak
	SDL_DestroyWindow( Ablak );							// Ablak t�rl�se
	Ablak = NULL;

	// Exit
	TTF_Quit;											// Kil�p�s a  TTF-b�l
	SDL_Quit;											// Kil�p�s az SDL-b�l
}