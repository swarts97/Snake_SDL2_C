#include "snakeresize.h"

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