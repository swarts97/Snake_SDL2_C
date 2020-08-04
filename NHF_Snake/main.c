#include "main.h"

void szamlaloertek( char *pontstr, int pontint, int jatekos )
{
	if ( jatekos == 1 )
	{
		if ( pontint < 10 )
			sprintf( pontstr, "0000%d", pontint );
		else if ( pontint < 100 )
			sprintf( pontstr, "000%d", pontint );
		else if ( pontint < 1000 )
			sprintf( pontstr, "00%d", pontint );
		else if ( pontint < 10000 )
			sprintf( pontstr, "0%d", pontint );
		else if ( pontint < 100000 )
			sprintf( pontstr, "%d", pontint );
	}
	else
	{
		if ( pontint < 10 )
			sprintf( pontstr, "  %d  ", pontint );
		else if ( pontint < 100 )							// 30 a maximum
			sprintf( pontstr, "  %d ", pontint );
	}
}

int main( int argc, char* args[] )
{
	srand( time( NULL ) );

	bool quit = false;							// Kil�p�s eset�n true, am�gy false
	bool mozgas_kirajzolva1 = true;				// Am�g nincs kirajzolva az 1. k�gy� mozg�sa, az nem v�lthat ir�nyt
	bool mozgas_kirajzolva2 = true;				// Am�g nincs kirajzolva az 2. k�gy� mozg�sa, az nem v�lthat ir�nyt

	int speed = 3;								// K�gy�(k) sebess�ge, �rt�ke lehet : 1-t�l 5-ig
	int jatekos = 1;							// Egyj�t�kos m�d / K�tj�t�kos m�d
	int maxpont = 10;							// K�tj�t�kos m�d eset�n aki el�bb ennyi menetet nyer, az nyer, viszont legal�bb 2 ponttal kell nyerni.
	bool stealmod = true;						// K�tj�t�kos m�dbeli opci�

	int  pont1int = 0;							// Az els� k�gy� pontsz�ma
	char pont1str[6];							// 5 darab helyi�rt�k
	int  pont2int = 0;							// A m�sodik k�gy� pontsz�ma
	char pont2str[6];							// 5 darab helyi�rt�k

	int szamlalo = 0;							// Sz�ml�l� a folyamatos mozg�s �rdek�ben
	int winner = 0;								// Az adott k�r nyertese
	bool noutk = true;							// Pillanatnyilag van-e �tk�z�s? True eset�n nincs.

	SDL_Event e;								// SDL esem�ny kezel�

	menu( &speed, &jatekos, &maxpont, &stealmod, &quit );

	if ( !init() )
	{
		printf( "Hiba az inicializalas soran.\n" );
	}
	else
	{
		if ( jatekos == 1 )
			kigyocsinal( 1, 4, JOBB, STOP );								// Egyj�t�kos m�dban 4 hossz�s�g� k�gy�val kezd�nk
		else
		{
			if ( stealmod )
				kigyocsinal( 2, 6, JOBB, BAL );								// Stealmod eset�n 6,
			else
				kigyocsinal( 2, 4, JOBB, BAL );								// Normalmod eset�n 4 hossz�s�g� k�gy�val kezd�nk
		}

		ujalma();

		//Am�g a program fut
		while ( !quit )
		{
			// Soron l�v� esem�nyek kezel�se
			while ( SDL_PollEvent( &e ) != 0 )
			{
				// A felhaszn�l� ki szeretne l�pni
				if ( e.type == SDL_QUIT )
				{
					quit = true;
				}
				// A felhaszn�l� egy gombot nyom le
				if ( e.type == SDL_KEYDOWN )
				{
					switch ( e.key.keysym.sym )
					{
					case SDLK_w:
						if ( head1->irany != FEL && head1->irany != LE && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = FEL;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_s:
						if ( head1->irany != FEL && head1->irany != LE && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = LE;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_d:
						if ( head1->irany != JOBB && head1->irany != BAL && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = JOBB;
							mozgas_kirajzolva1 = false;
						}
						break;
					case SDLK_a:
						if ( head1->irany != JOBB && head1->irany != BAL && mozgas_kirajzolva1 && noutk == true )
						{
							head1->irany = BAL;
							mozgas_kirajzolva1 = false;
						}
						break;

						/////////////////////////////////////////////////////// 2 j�t�kos �zemm�d ///////////////////////////////////////////////////////

					case SDLK_UP:
						if ( jatekos == 2 && head2->irany != FEL && head2->irany != LE && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = FEL;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_DOWN:
						if ( jatekos == 2 && head2->irany != FEL && head2->irany != LE && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = LE;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_RIGHT:
						if ( jatekos == 2 && head2->irany != JOBB && head2->irany != BAL && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = JOBB;
							mozgas_kirajzolva2 = false;
						}
						break;
					case SDLK_LEFT:
						if ( jatekos == 2 && head2->irany != JOBB && head2->irany != BAL && mozgas_kirajzolva2 && noutk == true )
						{
							head2->irany = BAL;
							mozgas_kirajzolva2 = false;
						}
						break;

						//////////////////////////////////////////////////////////// Kil�p�s ///////////////////////////////////////////////////////////

					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
					}
				}
			}

			noutk = true;																// Nincs �tk�z�s

			if ( szamlalo == 0 && !utkozes( jatekos, &winner ) && noutk )				// A speed �rt�k�t�l f�gg�en bizonyos cikluslefut�s ut�n (emiatt lesz folyamatos a mozg�s)
			{
				mozgat( &mozgas_kirajzolva1, farok1, head1, 1, jatekos );				// Az aktu�lis ir�nyba mozgatja a k�gy�t 1-gyel.
				if ( jatekos == 2 )
				{
					mozgat( &mozgas_kirajzolva2, farok2, head2, 2, jatekos );			// Az aktu�lis ir�nyba mozgatja a k�gy�t 1-gyel.
				}
			}

			else if ( szamlalo == 0 && utkozes( jatekos, &winner ) )					// Ha �tk�z�s van
			{
				if ( winner == 1 )
					pont1int++;															// Megn�veli a nyertes pontsz�m�t 1-gyel
				if ( winner == 2 )
					pont2int++;															// Megn�veli a nyertes pontsz�m�t 1-gyel

				if ( jatekos == 2 )
				{
					if ( pont1int >= maxpont && pont1int - pont2int > 1 )	// Ha valaki el�ri a maxpontot �s legal�bb 2-vel t�bb pontja van mint a m�siknak, nyert �s a j�t�knak v�ge
						maxpont_elerve( pont1str, &quit, 1 );				// Mindegy, hogy melyik stringet adjuk meg, a l�nyeg, hogy belef�rjen a "NYERT"
					if ( pont2int >= maxpont && pont2int - pont1int > 1 )
						maxpont_elerve( pont2str, &quit, 2 );				// Ez mindk�t stringre igaz
				}

				noutk = false;
				SDL_Delay( 1500 );
				if ( jatekos == 1 )
				{
					ranglista( pont1int );												// Eredm�ny fel�r�sa a Dics�s�gt�bl�ra
					quit = true;														// A j�t�k ezut�n v�get �r
				}
				else
					ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );		// K�tj�t�kos m�d eset�n pedig �j k�r indul
			}

			if ( alma_eaten( jatekos ) == 1 )											// Ha az els� k�gy� megette az alm�t
			{
				ujalma();																// Egy m�sik alma jelenik meg �j helyen
				farok1 = kigyonov( farok1 );											// Valamint n� egyet a k�gy� amelyik megette

				if ( jatekos == 1 )														// Egyj�t�kos m�d eset�n
					pont1int += speed * 10;												// Ennyivel n� a pontunk

				if ( jatekos == 2 && stealmod )											// K�tj�t�kos stealmod eset�n
				{
					if ( kigyohossz( farok2 ) == 1 )									// Ha null�ra cs�kkenne a m�sik k�gy� m�rete
					{
						kigyo( farok1, vZold, sZold );									// Els� k�gy� utols� l�p�s�nek kirajzol�sa (megeszi az utols� alm�t)
						SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se

						winner = 1;														// Az a k�gy� nyer aki megette az utols� alm�t
						pont1int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis id� m�lt�n
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// �j menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 j�t�kos m�d eset�n tudjuk a m�sik k�gy�t cs�kkenteni
							farok2 = kigyocsokk( farok2 );								// Egyel cs�kken a m�sodik k�gy� m�rete
				}
			}
			else if ( alma_eaten( jatekos ) == 2 )										// Ha az m�sodik k�gy� megette az alm�t
			{
				ujalma();																// Egy m�sik alma jelenik meg �j helyen
				farok2 = kigyonov( farok2 );											// Valamint n� egyet a k�gy� amelyik megette

				if ( jatekos == 2 && stealmod )											// K�tj�t�kos stealmod eset�n
				{
					if ( kigyohossz( farok1 ) == 1 )									// Ha null�ra cs�kkenne a m�sik k�gy� m�rete
					{
						kigyo( farok2, vKek, sKek );									// M�sodik k�gy� utols� l�p�s�nek kirajzol�sa (megeszi az utols� alm�t)
						SDL_RenderPresent( Renderelo );									// E v�ltoz�s megjelen�t�se

						winner = 2;														// Az a k�gy� nyer aki megette az utols� alm�t
						pont2int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis id� m�lt�n
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// �j menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 j�t�kos m�d eset�n tudjuk a m�sik k�gy�t cs�kkenteni
							farok1 = kigyocsokk( farok1 );								// Egyel cs�kken a m�sodik k�gy� m�rete
				}
			}

			rajzol();																	// Elemek kirajzol�sa (kiv�ve a k�gy�(ka)t)
			kigyo( farok1, vZold, sZold );												// Els� k�gy� kirajzol�sa
			if ( jatekos == 2 )
				kigyo( farok2, vKek, sKek );											// M�sik k�gy� kirajzol�sa

			// Akkor, ha m�g nem �rte el a maxpontot, vagypedig ha el�rte, maximum 1 a k�l�nbs�g a 2 j�t�kos pontsz�ma k�z�tt.
			if ( !quit && pont1int != maxpont || (pont1int == maxpont && abs( pont1int - pont2int ) < 2) )
			{
				szamlaloertek( pont1str, pont1int, jatekos );							// Sz�mlal��rt�k kisz�m�t�sa
				TTF_Renderer( pont1str, vZold, 3 + (CELLA + 1) * 3 );					// Sz�ml�l��rt�k ki�r�sa. Kezd�poz : 63
			}
			// Akkor, ha m�g nem �rte el a maxpontot, vagypedig ha el�rte, maximum 1 a k�l�nbs�g a 2 j�t�kos pontsz�ma k�z�tt.
			// 2 j�t�kos m�d eset�n
			if ( jatekos == 2 && (!quit && pont2int != maxpont || (pont2int == maxpont && abs( pont1int - pont2int ) < 2)) )
			{
				szamlaloertek( pont2str, pont2int, jatekos );							// M�sik sz�ml�l��rt�k kisz�m�t�sa 2 j�t�kos m�d eset�n
				TTF_Renderer( pont2str, vKek, 3 + 26 * (CELLA + 1) );					// M�sik sz�ml�l��rt�k ki�r�sa. Kezd�poz : 523
			}

			SDL_RenderPresent( Renderelo );												// �sszes v�ltoz�s megjelen�t�se

			szamlalo = (szamlalo + 1) % ((11 - speed * 2) + 2);							// Sz�ml�l� n�vel�se 1-gyel �s marad�kos oszt�sa a speed-t�l f�gg� konstanssal.

			if ( quit == true )															// Kil�p�s megt�rt�n�se el�tt
			{
				kigyotorol( farok1 );													// Els� k�gy� l�ncolt list�j�nak felszabad�t�sa
				if ( jatekos == 2 )
					kigyotorol( farok2 );												// M�sodik k�gy� l�ncolt list�j�nak felszabad�t�sa
			}
		}
	}
	close();																			// Ablak �s Renderer megsz�ntet�se, kil�p�s TTF-b�l, SDL-b�l
	return 0;
}