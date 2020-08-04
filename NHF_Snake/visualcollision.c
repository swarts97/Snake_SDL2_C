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
	// K�gy� test�nek renderel�se
	for ( Kigyo *mozgo = kigyofarok; mozgo->kov != NULL; mozgo = mozgo->kov )
	{
		SDL_Rect partpoz = { mozgo->x * (CELLA + 1) + 1, mozgo->y * (CELLA + 1) + 1, CELLA, CELLA };
		SDL_SetRenderDrawColor( Renderelo, vszin.r, vszin.g, vszin.b, vszin.a );											// A teste vil�gosabb
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
		SDL_SetRenderDrawColor( Renderelo, sszin.r, sszin.g, sszin.b, sszin.a );											// A fej r�sz s�t�tebb
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
	Pont next1 = nextkoord( head1 );										// A fej k�vetkez� cell�j�nak meghat�roz�sa
	falonat( &next1.x, &next1.y );											// Falon �thalad�s eset�n is j�l hat�rozza meg

	// Egyj�t�kos m�d
	if ( jatekos == 1 )
		// Ha a k�gy� k�vetkez� mez�je foglalt, valamint ez a mez� nem a saj�t farka.
		return (terulet_foglalt[next1.x][next1.y] && !(next1.x == farok1->x && next1.y == farok1->y));

	// K�tj�t�kos m�d
	else
	{
		Pont next2 = nextkoord( head2 );									// A fej k�vetkez� cell�j�nak meghat�roz�sa
		falonat( &next2.x, &next2.y );										// Falon �thalad�s eset�n is j�l hat�rozza meg

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

		/* A k�vetkez� felt�tel vizsg�lata akkor lenne sz�ks�ges,
		ha a k�gy�k fejeinek kezdeti poz�ci�ci�ja azonos sz�n� lenne, ha befesten�nk a j�t�kmez�t �gy mint 1 sakkt�bl�t (de ezt direkt elker�ltem).

		// Ha a k�t k�gy� fejeinek k�vetkez� mez�je azonos
		if ( next1.x == next2.x && next1.y == next2.y )
		{
		*winner = 0;
		return true;
		}*/
		return false;
	}
}