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

	bool quit = false;							// Kilépés esetén true, amúgy false
	bool mozgas_kirajzolva1 = true;				// Amíg nincs kirajzolva az 1. kígyó mozgása, az nem válthat irányt
	bool mozgas_kirajzolva2 = true;				// Amíg nincs kirajzolva az 2. kígyó mozgása, az nem válthat irányt

	int speed = 3;								// Kígyó(k) sebessége, értéke lehet : 1-tõl 5-ig
	int jatekos = 1;							// Egyjátékos mód / Kétjátékos mód
	int maxpont = 10;							// Kétjátékos mód esetén aki elõbb ennyi menetet nyer, az nyer, viszont legalább 2 ponttal kell nyerni.
	bool stealmod = true;						// Kétjátékos módbeli opció

	int  pont1int = 0;							// Az elsõ kígyó pontszáma
	char pont1str[6];							// 5 darab helyiérték
	int  pont2int = 0;							// A második kígyó pontszáma
	char pont2str[6];							// 5 darab helyiérték

	int szamlalo = 0;							// Számláló a folyamatos mozgás érdekében
	int winner = 0;								// Az adott kör nyertese
	bool noutk = true;							// Pillanatnyilag van-e ütközés? True esetén nincs.

	SDL_Event e;								// SDL esemény kezelõ

	menu( &speed, &jatekos, &maxpont, &stealmod, &quit );

	if ( !init() )
	{
		printf( "Hiba az inicializalas soran.\n" );
	}
	else
	{
		if ( jatekos == 1 )
			kigyocsinal( 1, 4, JOBB, STOP );								// Egyjátékos módban 4 hosszúságú kígyóval kezdünk
		else
		{
			if ( stealmod )
				kigyocsinal( 2, 6, JOBB, BAL );								// Stealmod esetén 6,
			else
				kigyocsinal( 2, 4, JOBB, BAL );								// Normalmod esetén 4 hosszúságú kígyóval kezdünk
		}

		ujalma();

		//Amíg a program fut
		while ( !quit )
		{
			// Soron lévõ események kezelése
			while ( SDL_PollEvent( &e ) != 0 )
			{
				// A felhasználó ki szeretne lépni
				if ( e.type == SDL_QUIT )
				{
					quit = true;
				}
				// A felhasználó egy gombot nyom le
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

						/////////////////////////////////////////////////////// 2 játékos üzemmód ///////////////////////////////////////////////////////

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

						//////////////////////////////////////////////////////////// Kilépés ///////////////////////////////////////////////////////////

					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
					}
				}
			}

			noutk = true;																// Nincs ütközés

			if ( szamlalo == 0 && !utkozes( jatekos, &winner ) && noutk )				// A speed értékétõl függõen bizonyos cikluslefutás után (emiatt lesz folyamatos a mozgás)
			{
				mozgat( &mozgas_kirajzolva1, farok1, head1, 1, jatekos );				// Az aktuális irányba mozgatja a kígyót 1-gyel.
				if ( jatekos == 2 )
				{
					mozgat( &mozgas_kirajzolva2, farok2, head2, 2, jatekos );			// Az aktuális irányba mozgatja a kígyót 1-gyel.
				}
			}

			else if ( szamlalo == 0 && utkozes( jatekos, &winner ) )					// Ha ütközés van
			{
				if ( winner == 1 )
					pont1int++;															// Megnöveli a nyertes pontszámát 1-gyel
				if ( winner == 2 )
					pont2int++;															// Megnöveli a nyertes pontszámát 1-gyel

				if ( jatekos == 2 )
				{
					if ( pont1int >= maxpont && pont1int - pont2int > 1 )	// Ha valaki eléri a maxpontot és legalább 2-vel több pontja van mint a másiknak, nyert és a játéknak vége
						maxpont_elerve( pont1str, &quit, 1 );				// Mindegy, hogy melyik stringet adjuk meg, a lényeg, hogy beleférjen a "NYERT"
					if ( pont2int >= maxpont && pont2int - pont1int > 1 )
						maxpont_elerve( pont2str, &quit, 2 );				// Ez mindkét stringre igaz
				}

				noutk = false;
				SDL_Delay( 1500 );
				if ( jatekos == 1 )
				{
					ranglista( pont1int );												// Eredmény felírása a Dicsõségtáblára
					quit = true;														// A játék ezután véget ér
				}
				else
					ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );		// Kétjátékos mód esetén pedig új kör indul
			}

			if ( alma_eaten( jatekos ) == 1 )											// Ha az elsõ kígyó megette az almát
			{
				ujalma();																// Egy másik alma jelenik meg új helyen
				farok1 = kigyonov( farok1 );											// Valamint nõ egyet a kígyó amelyik megette

				if ( jatekos == 1 )														// Egyjátékos mód esetén
					pont1int += speed * 10;												// Ennyivel nõ a pontunk

				if ( jatekos == 2 && stealmod )											// Kétjátékos stealmod esetén
				{
					if ( kigyohossz( farok2 ) == 1 )									// Ha nullára csökkenne a másik kígyó mérete
					{
						kigyo( farok1, vZold, sZold );									// Elsõ kígyó utolsó lépésének kirajzolása (megeszi az utolsó almát)
						SDL_RenderPresent( Renderelo );									// E változás megjelenítése

						winner = 1;														// Az a kígyó nyer aki megette az utolsó almát
						pont1int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis idõ múltán
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// Új menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 játékos mód esetén tudjuk a másik kígyót csökkenteni
							farok2 = kigyocsokk( farok2 );								// Egyel csökken a második kígyó mérete
				}
			}
			else if ( alma_eaten( jatekos ) == 2 )										// Ha az második kígyó megette az almát
			{
				ujalma();																// Egy másik alma jelenik meg új helyen
				farok2 = kigyonov( farok2 );											// Valamint nõ egyet a kígyó amelyik megette

				if ( jatekos == 2 && stealmod )											// Kétjátékos stealmod esetén
				{
					if ( kigyohossz( farok1 ) == 1 )									// Ha nullára csökkenne a másik kígyó mérete
					{
						kigyo( farok2, vKek, sKek );									// Második kígyó utolsó lépésének kirajzolása (megeszi az utolsó almát)
						SDL_RenderPresent( Renderelo );									// E változás megjelenítése

						winner = 2;														// Az a kígyó nyer aki megette az utolsó almát
						pont2int++;														// Kap 1 pontot
						SDL_Delay( 1500 );												// Kis idõ múltán
						ujrakezd( stealmod, mozgas_kirajzolva1, mozgas_kirajzolva2 );	// Új menet indul
					}
					else
						if ( jatekos == 2 )												// Csak 2 játékos mód esetén tudjuk a másik kígyót csökkenteni
							farok1 = kigyocsokk( farok1 );								// Egyel csökken a második kígyó mérete
				}
			}

			rajzol();																	// Elemek kirajzolása (kivéve a kígyó(ka)t)
			kigyo( farok1, vZold, sZold );												// Elsõ kígyó kirajzolása
			if ( jatekos == 2 )
				kigyo( farok2, vKek, sKek );											// Másik kígyó kirajzolása

			// Akkor, ha még nem érte el a maxpontot, vagypedig ha elérte, maximum 1 a különbség a 2 játékos pontszáma között.
			if ( !quit && pont1int != maxpont || (pont1int == maxpont && abs( pont1int - pont2int ) < 2) )
			{
				szamlaloertek( pont1str, pont1int, jatekos );							// Számlalóérték kiszámítása
				TTF_Renderer( pont1str, vZold, 3 + (CELLA + 1) * 3 );					// Számlálóérték kiírása. Kezdõpoz : 63
			}
			// Akkor, ha még nem érte el a maxpontot, vagypedig ha elérte, maximum 1 a különbség a 2 játékos pontszáma között.
			// 2 játékos mód esetén
			if ( jatekos == 2 && (!quit && pont2int != maxpont || (pont2int == maxpont && abs( pont1int - pont2int ) < 2)) )
			{
				szamlaloertek( pont2str, pont2int, jatekos );							// Másik számlálóérték kiszámítása 2 játékos mód esetén
				TTF_Renderer( pont2str, vKek, 3 + 26 * (CELLA + 1) );					// Másik számlálóérték kiírása. Kezdõpoz : 523
			}

			SDL_RenderPresent( Renderelo );												// Összes változás megjelenítése

			szamlalo = (szamlalo + 1) % ((11 - speed * 2) + 2);							// Számláló növelése 1-gyel és maradékos osztása a speed-tõl függõ konstanssal.

			if ( quit == true )															// Kilépés megtörténése elõtt
			{
				kigyotorol( farok1 );													// Elsõ kígyó láncolt listájának felszabadítása
				if ( jatekos == 2 )
					kigyotorol( farok2 );												// Második kígyó láncolt listájának felszabadítása
			}
		}
	}
	close();																			// Ablak és Renderer megszûntetése, kilépés TTF-bõl, SDL-bõl
	return 0;
}