#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#include "def.h"

extern Rang *rang_olvaso();
extern Rang *rang_ujeredmeny( Rang *, int );
extern void rang_iro( Rang *, bool );
extern void ranglista_free( Rang *);
extern void ranglista( int );

#endif