#include "move.h"

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
	terulet_foglalt[kigyofarok->x][kigyofarok->y] = false;										// Miel�tt tov�bbmozgatjuk a k�gy�t mindig szabadd� kell tenni azt a cell�t, amelyet a farka el fog hagyni
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
	*mozgas_kirajzolva = true;			// Ez az�rt kell, hogy m�g miel�tt kirajzol�dott volna a mozg�s, ne v�lthassunk ism�t ir�nyt
}