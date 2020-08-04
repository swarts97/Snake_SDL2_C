#ifndef STRUKTURA_H_INCLUDED
#define STRUKTURA_H_INCLUDED

typedef enum Irany
{
	FEL,
	LE,
	JOBB,
	BAL,
	STOP
}Irany;

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

typedef struct Rang
{
	int pontszam;
	struct Rang *kov;
}Rang;

#endif