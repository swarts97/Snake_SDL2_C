#include "menu.h"
#include "scoreboard.h"

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

void menu_start( int *jatekos, bool *stealmod, int *maxpont )
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
		// Ketjatekos - kövmenü
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
		printf( "\nValaszthato jateksebessegek:\n\n1 - nagyon konnyu\n2 - konnyu\n3 - kozepes\n4 - nehez\n5 - nagyon nehez\n\nJatek sebessege: " );
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

void menu( int *speed, int *jatekos, int *maxpont, bool *stealmod, bool *quit )
{
	int gomb = 0;
	fomenu( &gomb );

	switch ( gomb )
	{
		// Jaték - kövmenü
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