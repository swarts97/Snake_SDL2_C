/*#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define KEP_SZEL			640														// 640 pixel sz�les lesz a k�p
#define KEP_MAG				500														// 500 pixel magas lesz a k�p ( sz�ml�l�sor + j�t�kter�let )
#define TERULET				480														// 480 pixel magas maga a j�t�kter�let
#define CELLA				TERULET  / 24 - 1										// 20-1 = 19 pixel lesz egy cella oldala (oszlopelv�laszt� vonallal egy�tt 20)
#define CELLA_IN_SOR		KEP_SZEL / ( CELLA + 1 )								// 32 db cella van 1 sorban		( 0,1, ... , 31 )
#define CELLA_IN_OSZLOP		TERULET  / ( CELLA + 1 )								// 24 db cella van 1 oszlopban  ( 1,2, ... , 24 ) - a 0. sor az sz�ml�l�sor

const SDL_Color vKek = { .r = 0,.g = 0,.b = 255,.a = 255 };
const SDL_Color sKek = { .r = 0,.g = 0,.b = 166,.a = 255 };
const SDL_Color vZold = { .r = 0,.g = 255,.b = 0,.a = 255 };
const SDL_Color sZold = { .r = 0,.g = 166,.b = 0,.a = 255 };
const SDL_Color szurke = { .r = 26,.g = 26,.b = 18,.a = 255 };

//maximalisan megszerezhet� pontsz�m egyj�t�kos m�dban 5 �s sebess�gen (32 * 24 - 4) * 50 = 38 200

SDL_Window		*Ablak		= NULL;													// Az ablak
SDL_Renderer	*Renderelo  = NULL;													// A  renderer

bool init()																			// Inicializ�l� f�ggv�ny
{
	bool siker = true;																// Inicializ�ci�s flag, a f�ggv�ny visszat�r�si �rt�ke
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )											// SDL inicializ�l�sa
	{
		printf( "SDL Inicializalasi problema! SDL Error: %s\n", SDL_GetError() );	// Az SDL_GetError mondja majd meg nek�nk hogy mi a hiba.
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
				// Renderer sz�n inicializ�l�s - SDL_RenderClear "ezzel a sz�nnel t�r�l"
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Rect Teglalap( int x, int y, SDL_Texture *Tex )
{
	int TexSzel = 0;
	int TexMag = 0;
	SDL_QueryTexture( Tex, NULL, NULL, &TexSzel, &TexMag );
	SDL_Rect teglalap = { x, y, TexSzel, TexMag };
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
	// Az adott bet�t�pusb�l egy fel�letet hoz l�tre, amib�l pedig egy text�r�t, valamint meghat�rozza, hogy milyen x koordin�t�t�l kezd�dj�n a t�glalap.
	TTF_Font	*Betutipus	= TTF_OpenFont( "arial.ttf", 20 );
	SDL_Surface *Felulet	= TTF_RenderText_Solid( Betutipus, szoveg, szovegszin );
	SDL_Texture *Tex		= SDL_CreateTextureFromSurface( Renderelo, Felulet );
	SDL_Rect	balfel		= Teglalap( x, 0, Tex );

	// A l�trehozott text�r�t a l�trehozott kezd�si koordin�t�t�l f�gg� helyre rakja.
	SDL_RenderCopy( Renderelo, Tex, NULL, &balfel );

	// Renderel�s ut�n felszabad�t�s
	TTF_Free( Betutipus, Felulet, Tex );
}

void szamlaloertek( char *pontstr, int pontint, int jatekos)
{
	if ( jatekos == 1 )
	{
		if ( pontint < 10 )
			sprintf( pontstr, "0000%d", pontint );
		else if ( pontint < 100 )
			sprintf( pontstr, "000%d", pontint );
		else if ( pontint < 1000 )
			sprintf( pontstr, "00%d", pontint );
		else if ( pontint < 10000 )
			sprintf( pontstr, "0%d", pontint );
		else if ( pontint < 100000 )
			sprintf( pontstr, "%d", pontint );
	}
	else
	{
		if ( pontint < 10 )
			sprintf( pontstr, "  %d  ", pontint );
		else if ( pontint < 100 )
			sprintf( pontstr, "  %d ", pontint );
	}
}


typedef struct Rang
{
	int pontszam;
	struct Rang *kov;
}Rang;

typedef struct Pont
{
	int x;
	int y;
}Pont;

typedef struct Kigyo
{
	int x;
	int y;
	Irany irany;
	struct Kigyo *kov;
}Kigyo;

typedef enum Irany
{
	FEL,
	LE,
	JOBB,
	BAL,
	STOP
}Irany;

Pont apple = { 12, 15 };

Kigyo *head1;
Kigyo *farok1;

Kigyo *head2;
Kigyo *farok2;

bool terulet_foglalt[CELLA_IN_OSZLOP][CELLA_IN_SOR];

// Kezdeti poz�ci�ban l�trehozunk egy 4 hossz�s�g� k�gy�t
void kigyocsinal( int jatekos, int hossz, Irany irany1, Irany irany2 )
{

	int start1 = 33 - (31 - hossz);									// 28-n�l hosszabb k�gy�t ne adjunk meg
																	// start1 % 2 = hossz % 2
	Kigyo *part1 = NULL;
	for ( int i = 0; i < hossz; i++ )
	{
		Kigyo *uj1;
		uj1 = (Kigyo*)malloc( sizeof( Kigyo ) );
		uj1->x = start1;
		uj1->y = 3;
		if ( i + 1 != hossz )										// A l�trehozott k�gy� �ltal letakart mez�k foglaltt� t�tele (kiv�tel a farokmez�)
			terulet_foglalt[uj1->x][uj1->y] = true;
		uj1->kov = part1;
		part1 = uj1;
		if ( i == 0 )
		{
			head1 = part1;											// Ez lesz a k�gy� fej�re mutat� pointer amin kereszt�l vez�relj�k a k�gy� fej�t.
			head1->irany = irany1;									// Alapb�l erre indul majd el a k�gy�
		}
		start1--;
	}
	farok1 = part1;

	if ( jatekos == 2 )
	{
		int start2 = 29 - hossz;									// 28-n�l hosszabb k�gy�t ne adjunk meg
																	// start2 % 2 != hossz % 2 ez�rt start2 % 2 != start1 % 2 valamint p�ratlan sor van a 2 k�gy� k�z�tt
																	// �gy teh�t a sakkt�bla k�l�nb�z� sz�nein lesz a 2 k�gy� feje
		Kigyo *part2 = NULL;
		for ( int i = 0; i < hossz; i++ )
		{
			Kigyo *uj2;
			uj2 = (Kigyo*)malloc( sizeof( Kigyo ) );
			uj2->x = start2;
			uj2->y = 19;
			if ( i + 1 != hossz )									// A l�trehozott k�gy� �ltal letakart mez�k foglaltt� t�tele (kiv�tel a farokmez�)
				terulet_foglalt[uj2->x][uj2->y] = true;
			uj2->kov = part2;
			part2 = uj2;
			if ( i == 0 )
			{
				head2 = part2;										// Ez lesz a k�gy� fej�re mutat� pointer amin kereszt�l vez�relj�k a k�gy� fej�t.
				head2->irany = irany2;								// Alapb�l erre indul majd el a k�gy�
			}
			start2++;
		}
		farok2 = part2;
	}
}

void kigyotorol( Kigyo *kigyofarok )
{
	Kigyo *mozgo = kigyofarok;
	while ( mozgo != NULL )
	{
		Kigyo *tmp = mozgo->kov;
		free( mozgo );
		mozgo = tmp;
	}
}

Kigyo* kigyonov( Kigyo *kigyofarok )
{
	Kigyo *uj;
	uj = (Kigyo*)malloc( sizeof( Kigyo ) );
	uj->x = kigyofarok->x;
	uj->y = kigyofarok->y;
	uj->kov = kigyofarok;
	return uj;
}

Kigyo* kigyocsokk( Kigyo *kigyofarok )
{
	terulet_foglalt[kigyofarok->x][kigyofarok->y] = false;
	Kigyo *ujveg = kigyofarok->kov;
	free( kigyofarok );
	return ujveg;
}

int alma_eaten( int jatekos )
{
	if ( head1->x == apple.x && head1->y == apple.y )							// Ha az els� k�gy� ette meg az alm�t
		return 1;
	if ( jatekos == 2 )
	{
		if ( head2->x == apple.x && head2->y == apple.y )						// Ha a m�sodik k�gy� ette meg az alm�t
			return 2;
	}
	return 0;																	// Ha se az els� se a m�sodik nem ette meg az alm�t
}

void ujalma()
{
	do
	{
		apple.x = rand() % CELLA_IN_SOR;										// 0-31
		apple.y = rand() % CELLA_IN_OSZLOP + 1;									// 1-24
	} while ( terulet_foglalt[apple.x][apple.y] );
}

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
	//negyzetracs();
	alma();
}

int kigyohossz( Kigyo *kigyofarok )
{
	int hossz = 0;
	for ( Kigyo *mozgo = kigyofarok; mozgo != NULL; mozgo = mozgo->kov )
		hossz++;
	return hossz;
}

void kigyo( Kigyo *kigyofarok, SDL_Color vszin, SDL_Color sszin )															// VilagosSzin, SotetSzin
{
	// K�gy� test�nek renderel�se
	for ( Kigyo *mozgo = kigyofarok; mozgo->kov != NULL; mozgo = mozgo->kov )
	{
		SDL_Rect partpoz = { mozgo->x * (CELLA + 1) + 1, mozgo->y * (CELLA + 1) + 1, CELLA, CELLA };
		SDL_SetRenderDrawColor( Renderelo, vszin.r, vszin.g, vszin.b, vszin.a );											// A feje s�t�tebb
		SDL_RenderFillRect( Renderelo, &partpoz );

		// Fej m�s sz�n� renderel�se
		if ( mozgo->kov->kov == NULL )
		{
			SDL_Rect partpoz = { mozgo->kov->x * (CELLA + 1) + 1, mozgo->kov->y * (CELLA + 1) + 1, CELLA, CELLA };
			SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );										// A feje s�t�tebb
			SDL_RenderFillRect( Renderelo, &partpoz );
		}
	}
	if ( kigyohossz( kigyofarok ) == 1 )
	{
		SDL_Rect partpoz = { kigyofarok->x * (CELLA + 1) + 1, kigyofarok->y * (CELLA + 1) + 1, CELLA, CELLA };
		SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );											// A feje s�t�tebb
		SDL_RenderFillRect( Renderelo, &partpoz );
	}
}

void falonat( int *x, int *y )
{
	if ( *y == 0 )
		*y = 24;

	else if ( *y == 25 )
		*y = 1;

	else if ( *x == 32 )
		*x = 0;

	else if ( *x == -1 )
		*x = 31;
}

void mozgat( bool *mozgas_kirajzolva, Kigyo *kigyofarok, Kigyo *kigyofej, int kigyo, int jatekos )
{
	terulet_foglalt[kigyofarok->x][kigyofarok->y] = false;
	// Test r�sz mozgat�sa
	if ( alma_eaten( jatekos ) == kigyo )														// A megadott k�gy� k�vetkez� mozg�sa, amennyiben �pp megette az alm�t
	{
		for ( Kigyo *mozgo = kigyofarok; mozgo->kov->kov != NULL; mozgo = mozgo->kov )
		{
			mozgo->x = mozgo->kov->x;
			mozgo->y = mozgo->kov->y;
		}
	}
	else if ( alma_eaten( jatekos ) != kigyo )													// A megadott k�gy� k�vetkez� mozg�sa, amennyiben �pp nem az alm�t eszi
	{
		for ( Kigyo *mozgo = kigyofarok; mozgo->kov != NULL; mozgo = mozgo->kov )
		{
			mozgo->x = mozgo->kov->x;
			mozgo->y = mozgo->kov->y;
		}
	}
	// Fej r�sz mozgat�sa
	switch ( kigyofej->irany )
	{
	case FEL:
		kigyofej->y--;
		falonat( &kigyofej->x, &kigyofej->y );
		terulet_foglalt[kigyofej->x][kigyofej->y] = true;
		break;
	case LE:
		kigyofej->y++;
		falonat( &kigyofej->x, &kigyofej->y );
		terulet_foglalt[kigyofej->x][kigyofej->y] = true;
		break;
	case JOBB:
		kigyofej->x++;
		falonat( &kigyofej->x, &kigyofej->y );
		terulet_foglalt[kigyofej->x][kigyofej->y] = true;
		break;
	case BAL:
		kigyofej->x--;
		falonat( &kigyofej->x, &kigyofej->y );
		terulet_foglalt[kigyofej->x][kigyofej->y] = true;
		break;
	default:
		break;
	}
	*mozgas_kirajzolva = true;
}

Pont nextkoord( Kigyo *kigyofej )
{
	int changex, changey;
	Pont next;
	switch ( kigyofej->irany )
	{
	case FEL:
		changex = 0;
		changey = -1;
		break;
	case LE:
		changex = 0;
		changey = 1;
		break;
	case JOBB:
		changex = 1;
		changey = 0;
		break;
	case BAL:
		changex = -1;
		changey = 0;
		break;
	default:
		break;
	}
	next.x = kigyofej->x + changex;
	next.y = kigyofej->y + changey;
	return next;
}

bool utkozes( int jatekos, int *winner )
{
	Pont next1 = nextkoord( head1 );
	falonat( &next1.x, &next1.y );

	// Egyj�t�kos m�d
	if ( jatekos == 1 )
		// Ha a k�gy� k�vetkez� mez�je foglalt, valamint ez a mez� nem a saj�t farka.
		return (terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y));

	// K�tj�t�kos m�d
	else
	{
		Pont next2 = nextkoord( head2 );
		falonat( &next2.x, &next2.y );

		// Ha front�lisan �tk�znek az nyer akinek hosszabb volt a k�gy�ja. Egyforma hossz�s�gn�l senki sem kap pontot.
		if ( (next1.x == head2->x && next1.y == head2->y) && (next2.x == head1->x && next2.y == head1->y) )
		{
			if ( kigyohossz( farok1 ) > kigyohossz( farok2 ) )
				*winner = 1;
			else if ( kigyohossz( farok1 ) < kigyohossz( farok2 ) )
				*winner = 2;
			else if ( kigyohossz( farok1 ) == kigyohossz( farok2 ) )
				*winner = 0;
			return true;
		}

		// Ha az els� k�gy� fej�nek k�vetkez� mez�je foglalt, valamint ez a mez� se nem a saj�t farka, se nem a m�sik k�gy� farka.
		if ( terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y) && !(next1.x == farok2->x && next1.y == farok2->y) )
		{
			*winner = 2;
			return true;
		}

		// Ha az m�sodik k�gy� fej�nek k�vetkez� mez�je foglalt, valamint ez a mez� se nem a saj�t farka, se nem a m�sik k�gy� farka.
		if ( terulet_foglalt[next2.x][next2.y] && !(next2.x == farok1->x && next2.y == farok1->y) && !(next2.x == farok2->x && next2.y == farok2->y) )
		{
			*winner = 1;
			return true;
		}

		 A k�vetkez� felt�tel vizsg�lata akkor lenne sz�ks�ges,
		ha a k�gy�k fejeinek kezdeti poz�ci�ci�ja azonos sz�n� lenne, ha befesten�nk a j�t�kmez�t �gy mint 1 sakkt�bl�t (de ezt direkt elker�ltem).
		if ( next1.x == next2.x && next1.y == next2.y )
		{
			*winner = 0;
			return true;
		}
		return false;
	}
}

void ujrakezd( bool stealmod, bool mozgas_kirajzolva1, bool mozgas_kirajzolva2 )
{
	kigyotorol( farok1 );
	kigyotorol( farok2 );
	for ( int i = 0; i < CELLA_IN_SOR; i++ )				// El�z� menetben lefoglalt cell�kat fel kell szabad�tani
		for ( int j = 1; j < CELLA_IN_OSZLOP + 1; j++ )		// A 0. sor a sz�ml�l�sor, az els� amit felszabad�t az 1. sor, az utols� pedig a 24. sor ( < 25 )
			terulet_foglalt[i][j] = false;
	if ( stealmod )
		kigyocsinal( 2, 6, JOBB, BAL );
	else
		kigyocsinal( 2, 4, JOBB, BAL );
	ujalma();
}

void maxpont_elerve( char *string, bool *quit, int nyertes )
{
	szamlalosor();													// Sz�ml�l�sor t�rl�se
	strcpy( string, "NYERT" );
	if ( nyertes == 1 )
		TTF_Renderer( string, vZold, 3 + (CELLA + 1) * 3 );			// Sz�ml�l�sorban NYERTES ki�r�sa
	if ( nyertes == 2 )
		TTF_Renderer( string, vKek, 3 + 26 * (CELLA + 1) );			// Sz�ml�l�sorban NYERTES ki�r�sa
	SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se
	SDL_Delay( 500 );
	*quit = true;
}

Rang *rang_olvaso()
{
	FILE *fp;
	fp = fopen( "ranglista.txt", "r" );
	if ( fp == NULL )
	{
		printf( "Hiba a ranglista.txt f�lj megnyit�sa sor�n." );
		return;
	}
	int adat = 0;
	int szemet = 0;
	Rang *eleje = NULL;
	Rang *mozgo = eleje;
	while ( fscanf( fp, "%d. %d\n", &szemet, &adat ) == 2 )
	{
		if ( eleje == NULL )
		{
			eleje = (Rang*)malloc( sizeof( Rang ) );
			eleje->pontszam = adat;
			eleje->kov = NULL;
			mozgo = eleje;
		}
		else
		{
			while ( mozgo->kov != NULL )
				mozgo = mozgo->kov;

			Rang *uj = (Rang*)malloc( sizeof( Rang ) );
			uj->kov = NULL;
			uj->pontszam = adat;
			mozgo->kov = uj;
		}
	}
	fclose( fp );
	return eleje;
}

Rang* rang_ujeredmeny( Rang *eleje, int pontszam )
{
	if ( eleje == NULL )
	{
		Rang *uj = (Rang*)malloc( sizeof( Rang ) );
		uj->pontszam = pontszam;
		uj->kov = NULL;
		eleje = uj;
	}
	else
	{
		if ( pontszam > eleje->pontszam )
		{
			Rang *uj = (Rang*)malloc( sizeof( Rang ) );
			uj->pontszam = pontszam;
			uj->kov = eleje;
			eleje = uj;
		}
		else
		{
			Rang *mozgo = eleje;
			while ( mozgo->kov != NULL && pontszam < mozgo->kov->pontszam )
				mozgo = mozgo->kov;
			Rang *uj = (Rang*)malloc( sizeof( Rang ) );
			uj->pontszam = pontszam;
			uj->kov = mozgo->kov;
			mozgo->kov = uj;
		}
	}
	return eleje;
}

void rang_iro( Rang *eleje, bool konzol )
{
	Rang *mozgo;
	int i;

	if ( konzol )
	{
		for ( i = 1, mozgo = eleje; mozgo != NULL && i < 11; mozgo = mozgo->kov )
		{
			printf( "%2d. %d\n", i++, mozgo->pontszam );
		}
	}
	else
	{
		FILE *fp;
		fp = fopen( "ranglista.txt", "w" );
		if ( fp == NULL )
		{
			printf( "Hiba a ranglista.txt f�lj megnyit�sa sor�n." );
			return;
		}

		for ( i = 1, mozgo = eleje; mozgo != NULL && i < 11; mozgo = mozgo->kov )
		{
			fprintf( fp, "%2d. %d\n", i++, mozgo->pontszam );
		}
		fclose( fp );
	}
}

void ranglista_free( Rang *eleje )
{
	Rang *tmp = NULL;
	Rang *mozgo = eleje;
	while ( mozgo != NULL )
	{
		mozgo->kov = tmp;
		free( mozgo );
		mozgo = tmp;
	}
}

void ranglista(int pontszam)
{
	Rang *eleje = NULL;
	bool konzol = false;
	eleje = rang_olvaso();
	eleje = rang_ujeredmeny( eleje, pontszam );
	rang_iro( eleje , konzol);
	ranglista_free( eleje );
}

void fomenu( int *gomb )
{
	printf( "1 - Start\n2 - Szint\n3 - Dicsosegtabla\n4 - Kilepes\n" );
	while ( !(*gomb == 1 || *gomb == 2 || *gomb == 3 || *gomb == 4) )
	{
		printf( "\nKerem valasszon az alabbi menupontok kozul egy megfelelo szam beirasaval. " );
		scanf( "%d", gomb );
	}
}

void menu_stealmod( bool *stealmod )
{
	int gomb = 0;
	printf( "\n1 - Normal mod\n2 - Stealmod\n" );
	while ( !(gomb == 1 || gomb == 2 || gomb == 3) )
	{
		printf( "\nKerem valasszon az alabbi menupontok kozul egy megfelelo szam beirasaval. " );
		scanf( "%d", &gomb );
	}

	switch ( gomb )
	{
		// Ketjatekos, no steal - START
	case 1:
		*stealmod = false;
		break;
		// Ketjatekos, steal - START
	case 2:
		*stealmod = true;
		break;
	}
}

void menu_maxpont( int *maxpont )
{
	
	int gomb = 0;
	printf( "\nKerem adjon meg egy maximalis pontszamerteket, mely elerese eseten a jatek veget er: " );
	while ( !(gomb == 1 || gomb == 2 || gomb == 3 || gomb == 4 || gomb == 5 || gomb == 6) )
	{
		printf( "\nValaszthato maxpont ertekek:\n\n1 - 5\n2 - 10\n3 - 15\n4 - 20\n5 - 25\n6 - 30\n\nMaximalis pontszam: " );
		scanf( "%d", &gomb );
	}

	switch ( gomb )
	{
	case 1:
		*maxpont = 5;
		break;
	case 2:
		*maxpont = 10;
		break;
	case 3:
		*maxpont = 15;
		break;
	case 4:
		*maxpont = 20;
		break;
	case 5:
		*maxpont = 25;
		break;	
	case 6:
		*maxpont = 30;
		break;
	}
}

void menu_start(int *jatekos, bool *stealmod, int *maxpont)
{
	int gomb = 0;
	printf( "\n1 - Egyjatekos mod\n2 - Ketjatekos mod\n" );
	while ( !(gomb == 1 || gomb == 2 || gomb == 3) )
	{
		printf( "\nKerem valasszon az alabbi menupontok kozul egy megfelelo szam beirasaval. " );
		scanf( "%d", &gomb );
	}

	switch ( gomb )
	{
		// Egyjatekos - START
	case 1:
		*jatekos = 1;
		break;
		// Ketjatekos - kovmenu
	case 2:
		*jatekos = 2;
		menu_stealmod( stealmod );
		menu_maxpont( maxpont );
		break;
	}
}

void menu_szint( int *speed )
{
	int gomb = 0;
	printf( "\nKerem valassza ki a jatek sebesseget egy megfelelo szam beirasaval, majd visszakerul a fomenube. " );
	while ( !(gomb == 1 || gomb == 2 || gomb == 3 || gomb == 4 || gomb == 5 || gomb == 6) )
	{
		printf( "\nValaszthato jateksebessegek:\n1 - nagyon konnyu\n2 - konnyu\n3 - kozepes\n4 - nehez\n5 - nagyon nehez\n\nJatek sebessege: " );
		scanf( "%d", &gomb );
	}

	switch ( gomb )
	{
	case 1:
		*speed = 1;
		break;
	case 2:
		*speed = 2;
		break;
	case 3:
		*speed = 3;
		break;
	case 4:
		*speed = 4;
		break;
	case 5:
		*speed = 5;
		break;
	}
}

void menu_dicsosegtabla()
{
	printf( "\nDicsosegtabla:\n" );
	Rang *eleje;
	bool konzol = true;
	eleje = rang_olvaso();
	rang_iro( eleje, konzol );
	ranglista_free( eleje );
	printf( "\n" );
}

void menu(int *speed, int *jatekos, int *maxpont, bool *stealmod, bool *quit)
{
	int gomb = 0;
	fomenu( &gomb );
	
	switch ( gomb )
	{
	case 1:
		menu_start( jatekos, stealmod, maxpont );
		break;
	// Szint
	case 2:
		menu_szint( speed );
		menu( speed, jatekos, maxpont, stealmod, quit );
		break;
	// Dics�s�gt�bla
	case 3:
		menu_dicsosegtabla();
		menu( speed, jatekos, maxpont, stealmod, quit );
		break;
	// Kilepes	
	case 4:
		*quit = true;
		break;
	}
}

int main( int argc, char* args[] )
{
	srand( time( NULL ) );

	if ( !init() )
	{
		printf( "Hiba az inicializalas soran.\n" );
	}
	else
	{
		bool quit = false;							// Kil�p�sn�l true, am�gy false
		bool mozgas_kirajzolva1 = true;				// Am�g nincs kirajzolva az 1. k�gy� mozg�sa, az nem v�lthat ir�nyt
		bool mozgas_kirajzolva2 = true;				// Am�g nincs kirajzolva az 2. k�gy� mozg�sa, az nem v�lthat ir�nyt

		int speed = 3;								// K�gy�(k) sebess�ge, �rt�ke lehet : 1-t�l 5-ig
		int jatekos = 1;							// Egyj�t�kos m�d / K�tj�t�kos m�d
		int maxpont = 10;							// K�tj�t�kos m�d eset�n aki el�bb ennyi menetet nyer, az nyer, viszont legal�bb 2 ponttal kell nyerni.
		bool stealmod = true;						// K�tj�t�kos m�dbeli opci�

		int  pont1int = 0;							// Az els� k�gy� pontsz�ma
		char pont1str[6];							// 5 darab helyi�rt�k
		int  pont2int = 0;							// A m�sodik k�gy� pontsz�ma
		char pont2str[6];							// 5 darba helyi�rt�k

		int szamlalo = 0;							// Sz�ml�l� a folyamatos mozg�s �rdek�ben
		int winner = 0;								// Az adott k�r nyertese
		bool noutk = true;							// Pillanatnyilag van e �tk�z�s?

		SDL_Event e;								// "SDL esem�ny kezel�"

		menu( &speed, &jatekos, &maxpont, &stealmod, &quit);

		if ( jatekos == 1 )
			kigyocsinal( 1, 4, JOBB, STOP );
		else
		{
			if ( stealmod )
				kigyocsinal( 2, 6, JOBB, BAL );
			else
				kigyocsinal( 2, 4, JOBB, BAL );
		}

		ujalma();

		//Am�g a program fut
		while ( !quit )
		{
			// Soron l�v� esem�nyek kezel�se
			while ( SDL_PollEvent( &e ) != 0 )
			{
				// A felhaszn�l� ki szeretne l�pni
				if ( e.type == SDL_QUIT )
				{
					quit = true;
				}
				// A felhaszn�l� egy gombot nyom le
				if ( e.type == SDL_KEYDOWN )
				{
					switch ( e.key.keysym.sym )
					{
					case SDLK_w:
						if ( head1->irany != FEL && head1->irany != LE && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = FEL;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_s:
						if ( head1->irany != FEL && head1->irany != LE && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = LE;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_d:
						if ( head1->irany != JOBB && head1->irany != BAL && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = JOBB;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_a:
						if ( head1->irany != JOBB && head1->irany != BAL && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = BAL;
							mozgas_kirajzolva1 = false;
						}
						break;

						/////////////////////////////////////////////////////// 2 j�t�kos �zemm�d ///////////////////////////////////////////////////////

					case SDLK_UP:
						if ( jatekos == 2 && head2->irany != FEL && head2->irany != LE && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = FEL;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_DOWN:
						if ( jatekos == 2 && head2->irany != FEL && head2->irany != LE && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = LE;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_RIGHT:
						if ( jatekos == 2 && head2->irany != JOBB && head2->irany != BAL && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = JOBB;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_LEFT:
						if ( jatekos == 2 && head2->irany != JOBB && head2->irany != BAL && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = BAL;
							mozgas_kirajzolva2 = false;
								}
						break;
						
						//////////////////////////////////////////////////////////// Kil�p�s ///////////////////////////////////////////////////////////

					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
					}
				}
			}

			noutk = true;																// Nincs �tk�z�s

			if ( szamlalo == 0 && !utkozes( jatekos, &winner ) && noutk )				// Minden 5. k�pkocka
			{
				mozgat( &mozgas_kirajzolva1, farok1, head1, 1, jatekos );				// Az aktu�lis ir�nyba mozgatja a k�gy�t 1-gyel.
				if ( jatekos == 2 )
				{
					mozgat( &mozgas_kirajzolva2, farok2, head2, 2, jatekos );			// Az aktu�lis ir�nyba mozgatja a k�gy�t 1-gyel.
				}
			}

			else if ( szamlalo == 0 && utkozes( jatekos, &winner ) )
			{
				if ( winner == 1 )
					pont1int++;
				if ( winner == 2 )
					pont2int++;

				if ( jatekos == 2 )
				{
					if ( pont1int >= maxpont && pont1int - pont2int > 1 )					// Ha valaki el�ri a maxpontot, nyert �s a j�t�knak v�ge
						maxpont_elerve( pont1str, &quit, 1 );								// Mindegy, hogy melyik stringet adjuk meg, a l�nyeg, hogy belef�rjen a "NYERT"
					if ( pont2int >= maxpont && pont2int - pont1int > 1 )
						maxpont_elerve( pont2str, &quit, 2 );								// Ez mindegyikre igaz
				}

				noutk = false;
				SDL_Delay( 1500 );
				if ( jatekos == 1 )
				{
					ranglista( pont1int );
					quit = true;
				}
				else
					ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );
			}

			if ( alma_eaten( jatekos ) == 1 )											// Ha az els� k�gy� megette az alm�t
			{
				ujalma();																// Egy m�sik alma jelenik meg �j helyen
				farok1 = kigyonov( farok1 );											// Valamint n� egyet a k�gy� amelyik megette

				if ( jatekos == 1 )														// Egy j�t�kos m�d eset�n
					pont1int += speed * 10;												// Ennyivel n� a pontunk

				if ( jatekos == 2 && stealmod )											// K�tj�t�kos stealmod eset�n
				{
					if ( kigyohossz( farok2 ) == 1 )									// Ha null�ra cs�kkenne a m�sik k�gy� m�rete
					{
						kigyo( farok1, vZold, sZold );									// Els� k�gy� utols� l�p�s�nek kirajzol�sa (megeszi az utols� alm�t)
						SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se

						winner = 1;														// Az a k�gy� nyer aki megette az utols� alm�t
						pont1int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis id� m�lt�n
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// �j menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 j�t�kos m�d eset�n tudjuk a m�sik k�gy�t cs�kkenteni
							farok2 = kigyocsokk( farok2 );								// Egyel cs�kken a m�sodik k�gy� m�rete
				}
			}
			else if ( alma_eaten( jatekos ) == 2 )										// Ha az m�sodik k�gy� megette az alm�t
			{
				ujalma();																// Egy m�sik alma jelenik meg �j helyen
				farok2 = kigyonov( farok2 );											// Valamint n� egyet a k�gy� amelyik megette

				if ( jatekos == 2 && stealmod )											// K�tj�t�kos stealmod eset�n
				{
					if ( kigyohossz( farok1 ) == 1 )									// Ha null�ra cs�kkenne a m�sik k�gy� m�rete
					{
						kigyo( farok2, vKek, sKek );									// M�sodik k�gy� utols� l�p�s�nek kirajzol�sa (megeszi az utols� alm�t)
						SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se

						winner = 2;														// Az a k�gy� nyer aki megette az utols� alm�t
						pont2int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis id� m�lt�n
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// �j menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 j�t�kos m�d eset�n tudjuk a m�sik k�gy�t cs�kkenteni
							farok1 = kigyocsokk( farok1 );								// Egyel cs�kken a m�sodik k�gy� m�rete
				}
			}

			rajzol();																	// Elemek kirajzol�sa (kiv�ve a k�gy�(ka)t)
			kigyo( farok1, vZold, sZold );												// Els� k�gy� kirajzol�sa
			if ( jatekos == 2 )
				kigyo( farok2, vKek, sKek );											// M�sik k�gy� kirajzol�sa

			// Akkor, ha m�g nem �rte el a maxpontot, vagypedig ha el�rte maximum 1 a k�l�nbs�g a 2 j�t�kos pontsz�ma k�z�tt.
			if ( !quit && pont1int != maxpont || (pont1int == maxpont && abs( pont1int - pont2int ) < 2) )
			{
				szamlaloertek( pont1str, pont1int, jatekos );							// Sz�mlal��rt�k kisz�m�t�sa
				TTF_Renderer( pont1str, vZold, 3 + (CELLA + 1) * 3 );					// Sz�ml�l��rt�k ki�r�sa. Kezd�poz : 63
			}
			// Akkor, ha m�g nem �rte el a maxpontot, vagypedig ha el�rte maximum 1 a k�l�nbs�g a 2 j�t�kos pontsz�ma k�z�tt.
			// 2 j�t�kos m�d eset�n
			if ( jatekos == 2 && (!quit && pont2int != maxpont || (pont2int == maxpont && abs( pont1int - pont2int ) < 2)) )
			{
				szamlaloertek( pont2str, pont2int, jatekos );							// M�sik sz�ml�l��rt�k kisz�m�t�sa 2 j�t�kos m�d eset�n
				TTF_Renderer( pont2str, vKek, 3 + 26 * (CELLA + 1) );					// M�sik sz�ml�l��rt�k ki�r�sa. Kezd�poz : 523
			}

			SDL_RenderPresent( Renderelo );												// �sszes v�ltoz�s megjelen�t�se

			szamlalo = (szamlalo + 1) % ((11 - speed * 2) + 2);							// Sz�ml�l� n�vel�se 1-gyel �s marad�kos oszt�sa a speed-t�l f�gg� konstanssal.
			
			if ( quit == true )
			{
				kigyotorol( farok1 );
				if ( jatekos == 2 )
					kigyotorol( farok2 );
			}
		}
	}
	close();
	return 0;
} */