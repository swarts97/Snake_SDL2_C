#include "def.h"

Pont apple;															// Alma poz�ci�ja

Kigyo *head1 = NULL;												// Els� k�gy� fej�re mutat� pointer
Kigyo *farok1 = NULL;												// Els� k�gy� fark�ra mutat� pointer

Kigyo *head2 = NULL;												// M�sodik k�gy� fej�re mutat� pointer
Kigyo *farok2 = NULL;												// M�sodik k�gy� fark�ra mutat� pointer

const SDL_Color vKek = { .r = 0,.g = 0,.b = 255,.a = 255 };			// Konstans sz�nek, melyeket renderel�sn�l fogunk haszn�lni
const SDL_Color sKek = { .r = 0,.g = 0,.b = 166,.a = 255 };
const SDL_Color vZold = { .r = 0,.g = 255,.b = 0,.a = 255 };
const SDL_Color sZold = { .r = 0,.g = 166,.b = 0,.a = 255 };
const SDL_Color szurke = { .r = 26,.g = 26,.b = 18,.a = 255 };

SDL_Window		*Ablak = NULL;										// Az SDL �ltal haszn�lt ablakhoz sz�ks�ges
SDL_Renderer	*Renderelo = NULL;									// Az SDL-nek van sz�ks�ge erre a renderel�shez