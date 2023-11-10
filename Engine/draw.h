/* PisteEngine PisteDraw v1.3 */

/*
Versio 1.2
----------

	Maksimim��r� buffereita korotettu 200 => 2000.

	PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);
	Klipperi lis�tty.

	PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
	Lis�tty. Toimii vain 256-v�risess�, 32:n osiin pilkotussa paletissa. 
	pros -parametri on prosentti (0-100) 0= ei n�y, 100 = ei l�pin�kyv�.

	PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y)
	Piirtorutiini ottaa nyt leikkurit huomioon, joten se ei en�� kirjoita ruudun ulkopuolelle.

	PisteDraw_Piirto_Lopeta(int i);
	Lis�tty kuormitettu versio, jossa on int muotoinen parametri. 
	Mahdollistaa useiden pintojen lukitsemisen yht�aikaa. Vanha versio toimii my�s.
	Vanha versio k�y kaikki mahdolliset pinnat l�pi ja tarkistaa onko ne lukittu. Hidasta. �l� k�yt�.

	PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
	Lis�tty kuormitettu versio. 
	Mahdollistaa bittikartan piirron, joka on peilattu sivusuunnassa ja/tai yl�salaisin.

	PD_VIRHE muutettu 1000 => -1000

	PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
	Nyt kaiken kokoisten kuvien lataus on mahdollista. Ennen pystyi lataamaan vain 640x480 kokoisia.

Versio 1.3
----------

	char *PisteDraw_Virheilmoitus() lis�tty. Helpottaa virheen etsint��.

	PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
	Eri kokoisten kuvien lataus on nyt mahdollista (ilman bugeja).

	int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
	Mahdollistaa v�rien py�ritt�misen paletissa.

Versio 1.4
----------

	int		PisteDraw_Font_Uusi(char *polku, char *tiedosto)
	Fontin parametrien lataus tekstitiedostosta.

*/

#ifndef P_DRAW
#define P_DRAW

/* INCLUDES ----------------------------------------------------------------------------------*/
#define DIRECTDRAW_VERSION 0x0700
//#include "ddraw.h" // TODO: DDRAW
//#include <windows.h>

/* DEFINES -----------------------------------------------------------------------------------*/

#define		PD_VIRHE		 -1000

#define		PD_TAUSTABUFFER	 0
#define		PD_TAUSTABUFFER2 1

#define		PD_FADE_NOPEA	 5
#define		PD_FADE_NORMAALI 2
#define		PD_FADE_HIDAS	 1


/* PROTOTYPES --------------------------------------------------------------------------------*/

int		PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
						 int leveys, int korkeus, int bpp,
						 int max_colors);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Marginaali_Vasen(int vasen);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Marginaali_Yla(int yla);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Aseta_Videomode(int leveys, int korkeus, int bpp, int max_colors);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
									int vasen, int yla, int oikea, int ala);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
//int		PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);
int		PisteDraw_Buffer_Tayta(int i, const int vasen, const int yla, const int oikea, const int ala, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
bool	PisteDraw_Buffer_Tuhoa(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Fade_Paletti(void);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_In(int laskuri);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_Out(int laskuri);
//-----------------------------------------------------------------------------------------------------------
bool	PisteDraw_Fade_Paletti_Valmis(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(char *polku, char *tiedosto);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, 
										   int x, int y, int pros);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Lopeta();
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Paivita_Naytto();
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Palauta_Pinnat();
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Get(PALETTEENTRY *&paletti);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Set(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Reset_Paletti(void);
//-----------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia();
//-----------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia_Max();
//-----------------------------------------------------------------------------------------------------------
char   *PisteDraw_Virheilmoitus();
//-----------------------------------------------------------------------------------------------------------
#endif

/*

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
						 int leveys, int korkeus, int bpp,
						 int max_colors);

Teht�v�:	Alustaa PD:n. 
	
Muuttujat:	main_window_handle:		ikkunan kahva
			hinstance_app:			?
			leveys:					ruudun leveys
			korkeus:				ruudun korkeus
			bpp:					bytes per bixel (ainoastaan arvo 8 toimii)
			max_colors:				max v�rej� (vain 256 toimii)

Palauttaa:	PD_VIHE, jos ep�onnistuu, muuten nolla

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala);

Teht�v�:	Asettaa bufferille rajat, joiden yli ei voi piirt��.
			Jokaisen bufferin alustuksen yhteydess� sille asetetaan klipperi, jonka
			reunat ovat samat kuin bufferin reunat. T�ll� voi j�lkik�teen muokata
			niit� arvoja.
	
Muuttujat:	i:						halutun bufferin indeksi
			vasen, yla, oikea, ala:	reunat

Palauttaa:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int i);

Teht�v�:	Kopioi koko bufferin sis�ll�n PD_TAUSTABUFFERI-bufferiin.

Muuttujat:	i:						halutun bufferin indeksi

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);

Teht�v�:	Kopioi l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y. Leikkaa piirrett�v�n 
			alueen kohdebufferin klipperin mukaan. K��nt�� piirrett�v�n alueen haluattaessa peilikuvaksi
			ja / tai yl�salaisin.

Muuttujat:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y						mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)
			peilaa_x				jos true, niin k��nnet��n sivusuunnassa
			peilaa_y				jos true, -"-			  pystysuunnassa 

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
									int vasen, int yla, int oikea, int ala);
									
Teht�v�:	Kopioi l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y. 
			Leikkaa piirrett�v�n alueen kohdebufferin klipperin mukaan. 

Muuttujat:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)
			vasen,yla,oikea,ala:	kopioitava alue l�hdebufferista.

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y);

Teht�v�:	Kopioi koko l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y. 
			Leikkaa piirrett�v�n alueen kohdebufferin klipperin mukaan.

Muuttujat:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, UCHAR color);

Teht�v�:	T�ytt�� halutun bufferin yhdell� v�rill�.

Muuttujat:	i:						kohdebufferin indeksi
			color:					v�ri jolla t�ytet��n (0-255)

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);

Teht�v�:	Sama kuin edellinen, mutta t�ss� m��ritell��n tarkka alue, joka t�ytet��n.
			Ottaa my�s klipperin huomioon.

Muuttujat:	i:						kohdebufferin indeksi
			vasen,yla...			alue joka t�ytet��n
			color:					v�ri jolla t�ytet��n (0-255)

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Buffer_Tuhoa(int i);

Teht�v�:	Vapauttaa varatun bufferin.

Muuttujat:	i:						Vapautettavan bufferin indeksi

Palauttaa:	true = onnistui, false = ei onnistunut (bufferia ei ehk� ollut)

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color);

Teht�v�:	Varaa uuden bufferin, asettaa sille klipperin ja palauttaa bufferin indeksin.

Muuttujat:	leveys, korkeus:		bufferin leveys ja korkeus
			videomuisti:			jos true, bufferi varataan n�ytt�muistista,	jos ei niin
									ty�muistista. Jos videomuisti loppuu, niin bufferit varataan
									automaattisesti ty�muistista. Ty�muisti on hidasta, videomuisti
									on nopeaa.
			color:					L�pin�kyvyysv�ri (0-255)

Palauttaa:	Varatun bufferin indeksi tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Fade_Paletti(void);

Teht�v�:	Suorittaa paletin sis��n tai ulos feidausta. Feidauksen suunta riippuu
			allaolevista aliohjelmista.

Muuttujat:	-

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_In(int laskuri);

Teht�v�:	Aloittaan paletin feidaamisen mustasta normaaliksi.

Muuttujat:	laskuri:				feidauksen nopeus

Palauttaa:

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_Out(int laskuri);

Teht�v�:	Aloittaan paletin feidaamisen normaalista mustaksi.

Muuttujat:	laskuri:				feidauksen nopeus

Palauttaa:

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Fade_Paletti_Valmis(void);

Teht�v�:	Ilmoittaa, onko paletin feidaus kesken vai onko se loppunut.

Muuttujat:	-

Palauttaa:	true jos paletin feidaus on loppunut.

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);

Teht�v�:	Luo uuden fontin ja palauttaa sen indeksin. Merkkien pit�� olla bitm�piss� seuraavassa
			j�rjestyksess�: 

  a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,t,s,u,v,w,x,y,z,�,�,�,0,1,2,3,4,5,6,7,8,9,.,!,?,:,-,pilkku,+,=,(,),/,"
			

Muuttujat:	buffer_index:			bufferi, jossa fontin bitmappi majailee.
			buffer_x:				fontti-bitmapin vasen reuna
			buffer_y:				fontti-bitmapin ylareuna
			leveys:					yhden fontin leveys
			korkeus:				yhden fontin korkeus
			lkm:					fonttien lukum��r�

Palauttaa:	PD_VIRHE tai varatun fontin indeksi

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(char *polku, char* file);

Teht�v�:	Luo uuden fontin ja palauttaa sen indeksin. Fontin parametrit luetaan tiedostosta: 

Esim:
	*image: 	bigfont.pcx
	*image x:	0
	*image y:	0
	*letters: 	abcdefghijklmnopqrstuvwxyz���0123456789.!?
	*letter width:	15
	*letter height:	21			

Muuttujat:	polku:					tiedoston hakemisto. fontin bitmapin pit�� olla samassa hakemistossa.
			file:					tiedston nimi
			
Palauttaa:	PD_VIRHE tai varatun fontin indeksi


-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);

Teht�v�:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros);

Teht�v�:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);

Teht�v�:	Lataa BMP-muotoisen kuvan bufferiin.

Muuttujat:	i:						kohdebufferin indeksi
			filename:				tiedoston polku ja nimi
			lataa_paletti:			true = lataa paletti kuvasta, false = s�ilyt� vanha paletti
			
Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Lopeta();

Teht�v�:	Lopettaa PD:n.

Muuttujat:

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari);

Teht�v�:	Py�ritt�� haluttua osaa paletista.

Muuttujat:	eka_vari:				ensimm�inen v�ri, jota py�ritet��n.
			vika_vari:				viimeinen vari, jota py�ritet��n.

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------  
void	PisteDraw_Paivita_Naytto();

Teht�v�:	Kopioi PD_TAUSTABUFFER-puskurin sis�ll�n n�ytt�muistiin.

Muuttujat:	-

Palauttaa:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Get(PALETTEENTRY *&paletti);

Teht�v�:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Set(void);

Teht�v�:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch);

Teht�v�:	Lukitsee halutun bufferin, jotta siihen voitaisiin piirt�� "manuaalisesti".

Muuttujat:	i:						lukittavan bufferin indeksi
			back_buffer:			viittaus taulukkoon, joka sis�lt�� bufferissa olevat pikselit
			lPitch:					bufferin (oikea) leveys pikselein�

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(void);

Teht�v�:	Vapauttaa kaikki bufferit, jotka on lukittu. �l� k�yt�! Hidas!

Muuttujat:

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(int i);

Teht�v�:	Vapauttaa halutun bufferin lukituksen (jos se on lukittu)

Muuttujat:	i:						halutun bufferin indeksi

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Reset_paletti(void);

Teht�v�:	Palauttaa alkuper�isen kuvasta ladatun paletin.

Muuttujat:	-

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia();

Teht�v�:	Palauttaa vapaana olevan videomuistin m��r�n bittein�

Muuttujat:

Palauttaa:	Vapaan videomuistin m��r�.

-------------------------------------------------------------------------------------------------------------
char   *PisteDraw_Virheilmoitus();

Teht�v�:	Palauttaa (aina) viimeisimm�n virheilmoituksen. 

Muuttujat:

Palauttaa:	Vapaan videomuistin m��r�.

-------------------------------------------------------------------------------------------------------------

*/