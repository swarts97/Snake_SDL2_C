#include "visualcollision.h"
#include "move.h"


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
		SDL_SetRenderDrawColor( Renderelo, vszin.r, vszin.g, vszin.b, vszin.a );											// A teste világosabb
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
		SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );											// A fej rész sötétebb
		SDL_RenderFillRect( Renderelo, &partpoz );
	}
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
	Pont next1 = nextkoord( head1 );										// A fej következõ cellájának meghatározása
	falonat( &next1.x, &next1.y );											// Falon áthaladás esetén is jól határozza meg

	// Egyjátékos mód
	if ( jatekos == 1 )
		// Ha a kígyó következõ mezõje foglalt, valamint ez a mezõ nem a saját farka.
		return (terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y));

	// Kétjátékos mód
	else
	{
		Pont next2 = nextkoord( head2 );									// A fej következõ cellájának meghatározása
		falonat( &next2.x, &next2.y );										// Falon áthaladás esetén is jól határozza meg

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

		/* A következõ feltétel vizsgálata akkor lenne szükséges,
		ha a kígyók fejeinek kezdeti pozícióciója azonos színû lenne, ha befestenénk a játékmezõt úgy mint 1 sakktáblát (de ezt direkt elkerültem).

		// Ha a két kígyó fejeinek következõ mezõje azonos
		if ( next1.x == next2.x && next1.y == next2.y )
		{
		*winner = 0;
		return true;
		}*/
		return false;
	}
}