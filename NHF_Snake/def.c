#include "def.h"

Pont apple;															// Alma pozíciója

Kigyo *head1 = NULL;												// Elsõ kígyó fejére mutató pointer
Kigyo *farok1 = NULL;												// Elsõ kígyó farkára mutató pointer

Kigyo *head2 = NULL;												// Második kígyó fejére mutató pointer
Kigyo *farok2 = NULL;												// Második kígyó farkára mutató pointer

const SDL_Color vKek = { .r = 0,.g = 0,.b = 255,.a = 255 };			// Konstans színek, melyeket renderelésnél fogunk használni
const SDL_Color sKek = { .r = 0,.g = 0,.b = 166,.a = 255 };
const SDL_Color vZold = { .r = 0,.g = 255,.b = 0,.a = 255 };
const SDL_Color sZold = { .r = 0,.g = 166,.b = 0,.a = 255 };
const SDL_Color szurke = { .r = 26,.g = 26,.b = 18,.a = 255 };

SDL_Window		*Ablak = NULL;										// Az SDL által használt ablakhoz szükséges
SDL_Renderer	*Renderelo = NULL;									// Az SDL-nek van szüksége erre a rendereléshez