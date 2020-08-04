#include "create.h"

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
	for ( int i = 0; i < CELLA_IN_SOR; i++ )				// El�z� menetben lefoglalt cell�kat fel kell szabad�tani
		for ( int j = 1; j < CELLA_IN_OSZLOP + 1; j++ )		// A 0. sor a sz�ml�l�sor, az els� amit felszabad�t, az az 1. sor, az utols� pedig a 24. sor ( < 25 )
			terulet_foglalt[i][j] = false;
	if ( stealmod )
		kigyocsinal( 2, 6, JOBB, BAL );
	else
		kigyocsinal( 2, 4, JOBB, BAL );
	ujalma();
}