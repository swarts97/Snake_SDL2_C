#include "move.h"

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
	terulet_foglalt[kigyofarok->x][kigyofarok->y] = false;										// Mielõtt továbbmozgatjuk a kígyót mindig szabaddá kell tenni azt a cellát, amelyet a farka el fog hagyni
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
	*mozgas_kirajzolva = true;			// Ez azért kell, hogy még mielõtt kirajzolódott volna a mozgás, ne válthassunk ismét irányt
}