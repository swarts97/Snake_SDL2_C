#include "create.h"

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

void ujalma()
{
	do
	{
		apple.x = rand() % CELLA_IN_SOR;										// 0-31
		apple.y = rand() % CELLA_IN_OSZLOP + 1;									// 1-24
	} while ( terulet_foglalt[apple.x][apple.y] );
}

void ujrakezd( bool stealmod, bool mozgas_kirajzolva1, bool mozgas_kirajzolva2 )
{
	kigyotorol( farok1 );
	kigyotorol( farok2 );
	for ( int i = 0; i < CELLA_IN_SOR; i++ )				// Elõzõ menetben lefoglalt cellákat fel kell szabadítani
		for ( int j = 1; j < CELLA_IN_OSZLOP + 1; j++ )		// A 0. sor a számlálósor, az elsõ amit felszabadít, az az 1. sor, az utolsó pedig a 24. sor ( < 25 )
			terulet_foglalt[i][j] = false;
	if ( stealmod )
		kigyocsinal( 2, 6, JOBB, BAL );
	else
		kigyocsinal( 2, 4, JOBB, BAL );
	ujalma();
}