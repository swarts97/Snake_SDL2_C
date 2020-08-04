/*#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define KEP_SZEL			640														// 640 pixel széles lesz a kép
#define KEP_MAG				500														// 500 pixel magas lesz a kép ( számlálósor + játékterület )
#define TERULET				480														// 480 pixel magas maga a játékterület
#define CELLA				TERULET  / 24 - 1										// 20-1 = 19 pixel lesz egy cella oldala (oszlopelválasztó vonallal együtt 20)
#define CELLA_IN_SOR		KEP_SZEL / ( CELLA + 1 )								// 32 db cella van 1 sorban		( 0,1, ... , 31 )
#define CELLA_IN_OSZLOP		TERULET  / ( CELLA + 1 )								// 24 db cella van 1 oszlopban  ( 1,2, ... , 24 ) - a 0. sor az számlálósor

const SDL_Color vKek = { .r = 0,.g = 0,.b = 255,.a = 255 };
const SDL_Color sKek = { .r = 0,.g = 0,.b = 166,.a = 255 };
const SDL_Color vZold = { .r = 0,.g = 255,.b = 0,.a = 255 };
const SDL_Color sZold = { .r = 0,.g = 166,.b = 0,.a = 255 };
const SDL_Color szurke = { .r = 26,.g = 26,.b = 18,.a = 255 };

//maximalisan megszerezhetõ pontszám egyjátékos módban 5 ös sebességen (32 * 24 - 4) * 50 = 38 200

SDL_Window		*Ablak		= NULL;													// Az ablak
SDL_Renderer	*Renderelo  = NULL;													// A  renderer

bool init()																			// Inicializáló függvény
{
	bool siker = true;																// Inicializációs flag, a függvény visszatérési értéke
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )											// SDL inicializálása
	{
		printf( "SDL Inicializalasi problema! SDL Error: %s\n", SDL_GetError() );	// Az SDL_GetError mondja majd meg nekünk hogy mi a hiba.
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
				// Renderer szín inicializálás - SDL_RenderClear "ezzel a színnel töröl"
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
	// Az adott betûtípusból egy felületet hoz létre, amibõl pedig egy textúrát, valamint meghatározza, hogy milyen x koordinátától kezdõdjön a téglalap.
	TTF_Font	*Betutipus	= TTF_OpenFont( "arial.ttf", 20 );
	SDL_Surface *Felulet	= TTF_RenderText_Solid( Betutipus, szoveg, szovegszin );
	SDL_Texture *Tex		= SDL_CreateTextureFromSurface( Renderelo, Felulet );
	SDL_Rect	balfel		= Teglalap( x, 0, Tex );

	// A létrehozott textúrát a létrehozott kezdési koordinátától függõ helyre rakja.
	SDL_RenderCopy( Renderelo, Tex, NULL, &balfel );

	// Renderelés után felszabadítás
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

// Kezdeti pozícióban létrehozunk egy 4 hosszúságú kígyót
void kigyocsinal( int jatekos, int hossz, Irany irany1, Irany irany2 )
{

	int start1 = 33 - (31 - hossz);									// 28-nál hosszabb kígyót ne adjunk meg
																	// start1 % 2 = hossz % 2
	Kigyo *part1 = NULL;
	for ( int i = 0; i < hossz; i++ )
	{
		Kigyo *uj1;
		uj1 = (Kigyo*)malloc( sizeof( Kigyo ) );
		uj1->x = start1;
		uj1->y = 3;
		if ( i + 1 != hossz )										// A létrehozott kígyó által letakart mezõk foglalttá tétele (kivétel a farokmezõ)
			terulet_foglalt[uj1->x][uj1->y] = true;
		uj1->kov = part1;
		part1 = uj1;
		if ( i == 0 )
		{
			head1 = part1;											// Ez lesz a kígyó fejére mutató pointer amin keresztül vezéreljük a kígyó fejét.
			head1->irany = irany1;									// Alapból erre indul majd el a kígyó
		}
		start1--;
	}
	farok1 = part1;

	if ( jatekos == 2 )
	{
		int start2 = 29 - hossz;									// 28-nál hosszabb kígyót ne adjunk meg
																	// start2 % 2 != hossz % 2 ezért start2 % 2 != start1 % 2 valamint páratlan sor van a 2 kígyó között
																	// így tehát a sakktábla különbözõ színein lesz a 2 kígyó feje
		Kigyo *part2 = NULL;
		for ( int i = 0; i < hossz; i++ )
		{
			Kigyo *uj2;
			uj2 = (Kigyo*)malloc( sizeof( Kigyo ) );
			uj2->x = start2;
			uj2->y = 19;
			if ( i + 1 != hossz )									// A létrehozott kígyó által letakart mezõk foglalttá tétele (kivétel a farokmezõ)
				terulet_foglalt[uj2->x][uj2->y] = true;
			uj2->kov = part2;
			part2 = uj2;
			if ( i == 0 )
			{
				head2 = part2;										// Ez lesz a kígyó fejére mutató pointer amin keresztül vezéreljük a kígyó fejét.
				head2->irany = irany2;								// Alapból erre indul majd el a kígyó
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
	if ( head1->x == apple.x && head1->y == apple.y )							// Ha az elsõ kígyó ette meg az almát
		return 1;
	if ( jatekos == 2 )
	{
		if ( head2->x == apple.x && head2->y == apple.y )						// Ha a második kígyó ette meg az almát
			return 2;
	}
	return 0;																	// Ha se az elsõ se a második nem ette meg az almát
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
	// Kígyó testének renderelése
	for ( Kigyo *mozgo = kigyofarok; mozgo->kov != NULL; mozgo = mozgo->kov )
	{
		SDL_Rect partpoz = { mozgo->x * (CELLA + 1) + 1, mozgo->y * (CELLA + 1) + 1, CELLA, CELLA };
		SDL_SetRenderDrawColor( Renderelo, vszin.r, vszin.g, vszin.b, vszin.a );											// A feje sötétebb
		SDL_RenderFillRect( Renderelo, &partpoz );

		// Fej más színû renderelése
		if ( mozgo->kov->kov == NULL )
		{
			SDL_Rect partpoz = { mozgo->kov->x * (CELLA + 1) + 1, mozgo->kov->y * (CELLA + 1) + 1, CELLA, CELLA };
			SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );										// A feje sötétebb
			SDL_RenderFillRect( Renderelo, &partpoz );
		}
	}
	if ( kigyohossz( kigyofarok ) == 1 )
	{
		SDL_Rect partpoz = { kigyofarok->x * (CELLA + 1) + 1, kigyofarok->y * (CELLA + 1) + 1, CELLA, CELLA };
		SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );											// A feje sötétebb
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
	// Test rész mozgatása
	if ( alma_eaten( jatekos ) == kigyo )														// A megadott kígyó következõ mozgása, amennyiben épp megette az almát
	{
		for ( Kigyo *mozgo = kigyofarok; mozgo->kov->kov != NULL; mozgo = mozgo->kov )
		{
			mozgo->x = mozgo->kov->x;
			mozgo->y = mozgo->kov->y;
		}
	}
	else if ( alma_eaten( jatekos ) != kigyo )													// A megadott kígyó következõ mozgása, amennyiben épp nem az almát eszi
	{
		for ( Kigyo *mozgo = kigyofarok; mozgo->kov != NULL; mozgo = mozgo->kov )
		{
			mozgo->x = mozgo->kov->x;
			mozgo->y = mozgo->kov->y;
		}
	}
	// Fej rész mozgatása
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

	// Egyjátékos mód
	if ( jatekos == 1 )
		// Ha a kígyó következõ mezõje foglalt, valamint ez a mezõ nem a saját farka.
		return (terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y));

	// Kétjátékos mód
	else
	{
		Pont next2 = nextkoord( head2 );
		falonat( &next2.x, &next2.y );

		// Ha frontálisan ütköznek az nyer akinek hosszabb volt a kígyója. Egyforma hosszúságnál senki sem kap pontot.
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

		// Ha az elsõ kígyó fejének következõ mezõje foglalt, valamint ez a mezõ se nem a saját farka, se nem a másik kígyó farka.
		if ( terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y) && !(next1.x == farok2->x && next1.y == farok2->y) )
		{
			*winner = 2;
			return true;
		}

		// Ha az második kígyó fejének következõ mezõje foglalt, valamint ez a mezõ se nem a saját farka, se nem a másik kígyó farka.
		if ( terulet_foglalt[next2.x][next2.y] && !(next2.x == farok1->x && next2.y == farok1->y) && !(next2.x == farok2->x && next2.y == farok2->y) )
		{
			*winner = 1;
			return true;
		}

		 A következõ feltétel vizsgálata akkor lenne szükséges,
		ha a kígyók fejeinek kezdeti pozícióciója azonos színû lenne, ha befestenénk a játékmezõt úgy mint 1 sakktáblát (de ezt direkt elkerültem).
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
	for ( int i = 0; i < CELLA_IN_SOR; i++ )				// Elõzõ menetben lefoglalt cellákat fel kell szabadítani
		for ( int j = 1; j < CELLA_IN_OSZLOP + 1; j++ )		// A 0. sor a számlálósor, az elsõ amit felszabadít az 1. sor, az utolsó pedig a 24. sor ( < 25 )
			terulet_foglalt[i][j] = false;
	if ( stealmod )
		kigyocsinal( 2, 6, JOBB, BAL );
	else
		kigyocsinal( 2, 4, JOBB, BAL );
	ujalma();
}

void maxpont_elerve( char *string, bool *quit, int nyertes )
{
	szamlalosor();													// Számlálósor törlése
	strcpy( string, "NYERT" );
	if ( nyertes == 1 )
		TTF_Renderer( string, vZold, 3 + (CELLA + 1) * 3 );			// Számlálósorban NYERTES kiírása
	if ( nyertes == 2 )
		TTF_Renderer( string, vKek, 3 + 26 * (CELLA + 1) );			// Számlálósorban NYERTES kiírása
	SDL_RenderPresent( Renderelo );									// E változás megjelenítése
	SDL_Delay( 500 );
	*quit = true;
}

Rang *rang_olvaso()
{
	FILE *fp;
	fp = fopen( "ranglista.txt", "r" );
	if ( fp == NULL )
	{
		printf( "Hiba a ranglista.txt fálj megnyitása során." );
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
			printf( "Hiba a ranglista.txt fálj megnyitása során." );
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
	// Dicsõségtábla
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
		bool quit = false;							// Kilépésnél true, amúgy false
		bool mozgas_kirajzolva1 = true;				// Amíg nincs kirajzolva az 1. kígyó mozgása, az nem válthat irányt
		bool mozgas_kirajzolva2 = true;				// Amíg nincs kirajzolva az 2. kígyó mozgása, az nem válthat irányt

		int speed = 3;								// Kígyó(k) sebessége, értéke lehet : 1-tõl 5-ig
		int jatekos = 1;							// Egyjátékos mód / Kétjátékos mód
		int maxpont = 10;							// Kétjátékos mód esetén aki elõbb ennyi menetet nyer, az nyer, viszont legalább 2 ponttal kell nyerni.
		bool stealmod = true;						// Kétjátékos módbeli opció

		int  pont1int = 0;							// Az elsõ kígyó pontszáma
		char pont1str[6];							// 5 darab helyiérték
		int  pont2int = 0;							// A második kígyó pontszáma
		char pont2str[6];							// 5 darba helyiérték

		int szamlalo = 0;							// Számláló a folyamatos mozgás érdekében
		int winner = 0;								// Az adott kör nyertese
		bool noutk = true;							// Pillanatnyilag van e ütközés?

		SDL_Event e;								// "SDL esemény kezelõ"

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

		//Amíg a program fut
		while ( !quit )
		{
			// Soron lévõ események kezelése
			while ( SDL_PollEvent( &e ) != 0 )
			{
				// A felhasználó ki szeretne lépni
				if ( e.type == SDL_QUIT )
				{
					quit = true;
				}
				// A felhasználó egy gombot nyom le
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

						/////////////////////////////////////////////////////// 2 játékos üzemmód ///////////////////////////////////////////////////////

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
						
						//////////////////////////////////////////////////////////// Kilépés ///////////////////////////////////////////////////////////

					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
					}
				}
			}

			noutk = true;																// Nincs ütközés

			if ( szamlalo == 0 && !utkozes( jatekos, &winner ) && noutk )				// Minden 5. képkocka
			{
				mozgat( &mozgas_kirajzolva1, farok1, head1, 1, jatekos );				// Az aktuális irányba mozgatja a kígyót 1-gyel.
				if ( jatekos == 2 )
				{
					mozgat( &mozgas_kirajzolva2, farok2, head2, 2, jatekos );			// Az aktuális irányba mozgatja a kígyót 1-gyel.
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
					if ( pont1int >= maxpont && pont1int - pont2int > 1 )					// Ha valaki eléri a maxpontot, nyert és a játéknak vége
						maxpont_elerve( pont1str, &quit, 1 );								// Mindegy, hogy melyik stringet adjuk meg, a lényeg, hogy beleférjen a "NYERT"
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

			if ( alma_eaten( jatekos ) == 1 )											// Ha az elsõ kígyó megette az almát
			{
				ujalma();																// Egy másik alma jelenik meg új helyen
				farok1 = kigyonov( farok1 );											// Valamint nõ egyet a kígyó amelyik megette

				if ( jatekos == 1 )														// Egy játékos mód esetén
					pont1int += speed * 10;												// Ennyivel nõ a pontunk

				if ( jatekos == 2 && stealmod )											// Kétjátékos stealmod esetén
				{
					if ( kigyohossz( farok2 ) == 1 )									// Ha nullára csökkenne a másik kígyó mérete
					{
						kigyo( farok1, vZold, sZold );									// Elsõ kígyó utolsó lépésének kirajzolása (megeszi az utolsó almát)
						SDL_RenderPresent( Renderelo );									// E változás megjelenítése

						winner = 1;														// Az a kígyó nyer aki megette az utolsó almát
						pont1int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis idõ múltán
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// Új menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 játékos mód esetén tudjuk a másik kígyót csökkenteni
							farok2 = kigyocsokk( farok2 );								// Egyel csökken a második kígyó mérete
				}
			}
			else if ( alma_eaten( jatekos ) == 2 )										// Ha az második kígyó megette az almát
			{
				ujalma();																// Egy másik alma jelenik meg új helyen
				farok2 = kigyonov( farok2 );											// Valamint nõ egyet a kígyó amelyik megette

				if ( jatekos == 2 && stealmod )											// Kétjátékos stealmod esetén
				{
					if ( kigyohossz( farok1 ) == 1 )									// Ha nullára csökkenne a másik kígyó mérete
					{
						kigyo( farok2, vKek, sKek );									// Második kígyó utolsó lépésének kirajzolása (megeszi az utolsó almát)
						SDL_RenderPresent( Renderelo );									// E változás megjelenítése

						winner = 2;														// Az a kígyó nyer aki megette az utolsó almát
						pont2int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis idõ múltán
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// Új menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 játékos mód esetén tudjuk a másik kígyót csökkenteni
							farok1 = kigyocsokk( farok1 );								// Egyel csökken a második kígyó mérete
				}
			}

			rajzol();																	// Elemek kirajzolása (kivéve a kígyó(ka)t)
			kigyo( farok1, vZold, sZold );												// Elsõ kígyó kirajzolása
			if ( jatekos == 2 )
				kigyo( farok2, vKek, sKek );											// Másik kígyó kirajzolása

			// Akkor, ha még nem érte el a maxpontot, vagypedig ha elérte maximum 1 a különbség a 2 játékos pontszáma között.
			if ( !quit && pont1int != maxpont || (pont1int == maxpont && abs( pont1int - pont2int ) < 2) )
			{
				szamlaloertek( pont1str, pont1int, jatekos );							// Számlalóérték kiszámítása
				TTF_Renderer( pont1str, vZold, 3 + (CELLA + 1) * 3 );					// Számlálóérték kiírása. Kezdõpoz : 63
			}
			// Akkor, ha még nem érte el a maxpontot, vagypedig ha elérte maximum 1 a különbség a 2 játékos pontszáma között.
			// 2 játékos mód esetén
			if ( jatekos == 2 && (!quit && pont2int != maxpont || (pont2int == maxpont && abs( pont1int - pont2int ) < 2)) )
			{
				szamlaloertek( pont2str, pont2int, jatekos );							// Másik számlálóérték kiszámítása 2 játékos mód esetén
				TTF_Renderer( pont2str, vKek, 3 + 26 * (CELLA + 1) );					// Másik számlálóérték kiírása. Kezdõpoz : 523
			}

			SDL_RenderPresent( Renderelo );												// Összes változás megjelenítése

			szamlalo = (szamlalo + 1) % ((11 - speed * 2) + 2);							// Számláló növelése 1-gyel és maradékos osztása a speed-tõl függõ konstanssal.
			
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