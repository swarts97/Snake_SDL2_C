#include "scoreboard.h"

Rang *rang_olvaso()
{
	int adat = 0;
	int szemet = 0;														// Beolvasásnál erre nincs szükség
	Rang *eleje = NULL;
	Rang *mozgo = eleje;
	FILE *fp;
	fp = fopen( "ranglista.txt", "rt" );
	if ( fp == NULL )
	{
		return eleje;													// NULL pointer marad
	}
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
		if ( eleje == NULL )
		{
			printf( "\n Jelenleg nem szerepel adat a Dicsosegtablan!\n" );
			return;
		}
		else
		{
			printf( "\n" );
			for ( i = 1, mozgo = eleje; mozgo != NULL && i < 11; mozgo = mozgo->kov )
				printf( "%2d. %d\n", i++, mozgo->pontszam );
		}
	}
	else
	{
		FILE *fp;
		fp = fopen( "ranglista.txt", "wt" );
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

void ranglista( int pontszam )
{
	Rang *eleje = NULL;
	bool konzol = false;
	eleje = rang_olvaso();
	eleje = rang_ujeredmeny( eleje, pontszam );
	rang_iro( eleje, konzol );
	ranglista_free( eleje );
}