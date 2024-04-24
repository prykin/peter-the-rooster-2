/* PisteEngine PisteDraw v1.3 */

/*
Version 1.2
----------

	Maximum number of buffers increased from 200 to 2000.

    PisteDraw_Buffer_Clear(int i, int vasen, int yla, int oikea, int ala, unsigned char color);
    Added clipping to PisteDraw_Buffer_Clear function.

    PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
    Added PisteDraw_Font_Write_Transparent function, which only works with movement_x 256-color, divided into
    32 sections palette. The 'pros' parameter represents the transparency percentage, where 0 means invisible and
    100 means fully opaque.

    PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y)
    PisteDraw_Font_Write function now takes clipping into account and doesn't write outside of the screen.

    PisteDraw_Draw_End(int i);
    Added overloaded version of PisteDraw_Draw_End function, which takes an integer parameter. It allows
    locking multiple surfaces at the same time, while the old version still works but is slower.

    PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
    Added overloaded version of PisteDraw_Buffer_Flip function, which allows drawing movement_x flipped bitmap horizontally
    and/or vertically.

    Changed PD_ERROR value from 1000 to -1000.

    PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
    PisteDraw_Lataa_Kuva function now allows loading images of any size, previously only 640x480 was possible.

Version 1.3
----------

    char *PisteDraw_Virheilmoitus()
	Added PisteDraw_GetError function, which helps with error debugging.

    PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
    PisteDraw_Image_Load function now properly handles loading images of different sizes.

    int PisteDraw_Paletti_Pyorita(unsigned char eka_vari, unsigned char vika_vari)
    Added PisteDraw_Palette_Rotate function, which allows rotating colors in the palette.

Version 1.4
----------

	int		PisteDraw_Font_Create(char *polku, char *file)
	Added PisteDraw_Font_Create function for loading font parameters from movement_x text file.

*/

#ifndef P_DRAW
#define P_DRAW

/* INCLUDES ----------------------------------------------------------------------------------*/
#define DIRECTDRAW_VERSION 0x0700
//#include "ddraw.h" // TODO: DDRAW
//#include <windows.h>
#include "../old_headers.h"

/* DEFINES -----------------------------------------------------------------------------------*/

#define        PD_ERROR         -1000

#define        PD_BACKBUFFER     0
#define        PD_BACKBUFFER2 1

#define        PD_FADE_FAST     5
#define        PD_FADE_NORMAL 2
#define        PD_FADE_SLOW     1


/* PROTOTYPES --------------------------------------------------------------------------------*/

int PisteDraw_Init(HWND &main_window_handle, HINSTANCE &hinstance_app, int width, int height, int bpp, int max_colors);

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_SetClipper(int i, int left, int top, int right, int bottom);

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_SetMarginLeft(int left);

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_SetMarginTop(int top);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_SetVideoMode(int width, int height, int bpp, int max_colors);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Flip(int i);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Flip(int source_index, int destination_index, int x, int y, bool flip_x, bool flip_y);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Flip_Fast(int source_index, int destination_index, int x, int y, int left, int top, int right, int bottom);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Flip_Fast(int source_index, int destination_index, int x, int y);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Clear(int i, unsigned char color);

//-----------------------------------------------------------------------------------------------------------
//int PisteDraw_Buffer_Clear(int i, int left, int top, int right, int bottom, unsigned char color);
int PisteDraw_Buffer_Clear(int i, const int left, const int top, const int right, const int bottom, unsigned char color);

//-----------------------------------------------------------------------------------------------------------
bool PisteDraw_Buffer_Destroy(int i);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Buffer_Create(int width, int height, bool video_memory, unsigned char color);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Fade_Palette(void);

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_Fade_Palette_In(int count);

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_Fade_Palette_Out(int count);

//-----------------------------------------------------------------------------------------------------------
bool PisteDraw_Fade_Palette_Done(void);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Font_Create(int buffer_index, int buffer_x, int buffer_y, int width, int height, int count);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Font_Create(char *path, char *file);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Font_Write(int font_index, char *text, int buffer_index, int x, int y);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Font_Write_Transparent(int font_index, char *text, int buffer_index, int x, int y, int transparency);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Image_Load(int i, char *filename, bool load_palette);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Quit();

//-----------------------------------------------------------------------------------------------------------
void PisteDraw_UpdateScreen();

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_GetFrameCount();

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Palette_Get(PALETTEENTRY *&palette);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Palette_Rotate(unsigned char first_color, unsigned char last_color);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Palette_Set(void);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Draw_Begin(int i, unsigned char *&back_buffer, unsigned long &lPitch);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Draw_End(void);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Draw_End(int i);

//-----------------------------------------------------------------------------------------------------------
int PisteDraw_Reset_Palette(void);

//-----------------------------------------------------------------------------------------------------------
unsigned long PisteDraw_GetMemoryUsed();

//-----------------------------------------------------------------------------------------------------------
unsigned long PisteDraw_GetMemoryUsedMax();

//-----------------------------------------------------------------------------------------------------------
char *PisteDraw_GetError();
//-----------------------------------------------------------------------------------------------------------
#endif

/*

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Init(HWND &main_window_handle, HINSTANCE &hinstance_app,
						 int width, int height, int bpp,
						 int max_colors);

Task:	Start PD
	
Variables:	main_window_handle:		window handle
			hinstance_app:			?
			width:					screen width
			height:				    screen height
			bpp:					bytes per pixel (only the value 8 works)
			max_colors:				maximum colors (only 256 works)

Returns:	PD_ERROR if it fails, otherwise, zero

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_SetClipper(int i, int vasen, int yla, int oikea, int ala);

Task:	Asettaa bufferille rajat, joiden yli ei voi piirt��.
			Jokaisen bufferin alustuksen yhteydess� sille asetetaan klipperi, jonka
			edges ovat samat kuin bufferin edges. T�ll� voi j�lkik�teen muokata
			niit� arvoja.
	
Variables:	i:						halutun bufferin index
			vasen, yla, oikea, ala:	edges

Returns:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int i);

Task:	Kopioi koko bufferin sis�ll�n PD_TAUSTABUFFERI-bufferiin.

Variables:	i:						halutun bufferin index

Returns:	Returns nollan tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);

Task:	Kopioi l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y. Leikkaa piirrett�v�n 
			alueen kohdebufferin klipperin mukaan. K��nt�� piirrett�v�n alueen haluattaessa peilikuvaksi
			ja / tai yl�salaisin.

Variables:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y						mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)
			peilaa_x				jos true, niin k��nnet��n sivusuunnassa
			peilaa_y				jos true, -"-			  pystysuunnassa 

Returns:	Returns nollan tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Fast(int lahde_index, int kohde_index, int x, int y,
									int vasen, int yla, int oikea, int ala);
									
Task:	Kopioi l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y. 
			Leikkaa piirrett�v�n alueen kohdebufferin klipperin mukaan. 

Variables:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)
			vasen,yla,oikea,ala:	kopioitava alue l�hdebufferista.

Returns:	Returns nollan tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Fast(int lahde_index, int kohde_index, int x, int y);

Task:	Copy koko l�hdebufferin sis�ll�n toiseen bufferiin kohtaan x,y.
			Leikkaa piirrett�v�n alueen kohdebufferin klipperin mukaan.

Variables:	lahde_index:			mist� bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirret��n (vasen- ja yl�reuna)

Returns:	Returns nollan tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Clear(int i, unsigned char color);

Task:	T�ytt�� halutun bufferin yhdell� v�rill�.

Variables:	i:						kohdebufferin index
			color:					v�ri jolla t�ytet��n (0-255)

Returns:	Returns nollan tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Clear(int i, int vasen, int yla, int oikea, int ala, unsigned char color);

Task:	Sama kuin edellinen, mutta t�ss� m��ritell��n tarkka alue, joka t�ytet��n.
			Ottaa my�s klipperin huomioon.

Variables:	i:						kohdebufferin index
			vasen,yla...			alue joka t�ytet��n
			color:					v�ri jolla t�ytet��n (0-255)

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Buffer_Destroy(int i);

Task:	Vapauttaa varatun bufferin.

Variables:	i:						Vapautettavan bufferin index

Returns:	true = onnistui, false = ei onnistunut (bufferia ei ehk� ollut)

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Create(int width, int height, bool video_muisti, unsigned char color);

Task:	Varaa uuden bufferin, asettaa sille klipperin ja palauttaa bufferin indeksin.

Variables:	width, height:		bufferin width ja height
			videomuisti:			jos true, bufferi varataan n�ytt�muistista,	jos ei niin
									ty�muistista. Jos videomuisti loppuu, niin bufferit varataan
									automaattisesti ty�muistista. Ty�muisti on hidasta, videomuisti
									on nopeaa.
			color:					L�pin�kyvyysv�ri (0-255)

Returns:	Varatun bufferin index tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Fade_Palette(void);

Task:	Suorittaa paletin sis��n tai ulos feidausta. Feidauksen suunta riippuu
			allaolevista aliohjelmista.

Variables:	-

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Palette_In(int laskuri);

Task:	Aloittaan paletin feidaamisen mustasta normaaliksi.

Variables:	laskuri:				feidauksen nopeus

Returns:

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Palette_Out(int laskuri);

Task:	Aloittaan paletin feidaamisen normaalista mustaksi.

Variables:	laskuri:				feidauksen nopeus

Returns:

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Fade_Palette_Done(void);

Task:	Ilmoittaa, onko paletin feidaus kesken vai onko se loppunut.

Variables:	-

Returns:	true jos paletin feidaus on loppunut.

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Create(int buffer_index, int buffer_x, int buffer_y, int width, int height, int lkm);

Task:	Luo uuden fontin ja palauttaa sen indeksin. Merkkien pit�� olla bitm�piss� seuraavassa
			j�rjestyksess�: 

  movement_x,movement_y,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,t,s,u,v,w,x,y,z,�,�,�,0,1,2,3,4,5,6,7,8,9,.,!,?,:,-,pilkku,+,=,(,),/,"
			

Variables:	buffer_index:			bufferi, jossa fontin bitmappi majailee.
			buffer_x:				fontti-bitmapin vasen reuna
			buffer_y:				fontti-bitmapin ylareuna
			width:					yhden fontin width
			height:				yhden fontin height
			lkm:					fonttien lukum��r�

Returns:	PD_ERROR tai varatun fontin index

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Create(char *polku, char* file);

Task:	Luo uuden fontin ja palauttaa sen indeksin. Fontin parametrit luetaan tiedostosta: 

Esim:
	*image: 	bigfont.pcx
	*image x:	0
	*image y:	0
	*letters: 	abcdefghijklmnopqrstuvwxyz���0123456789.!?
	*letter width:	15
	*letter height:	21			

Variables:	polku:					tiedoston directory. fontin bitmapin pit�� olla samassa hakemistossa.
			file:					tiedston name
			
Returns:	PD_ERROR tai varatun fontin index


-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Write(int font_index, char *merkkijono, int buffer_index, int x, int y);

Task:

Variables:

Returns:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Write_Transparent(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros);

Task:

Variables:

Returns:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Image_Load(int i, char *filename, bool lataa_paletti);

Task:	Load BMP-muotoisen kuvan bufferiin.

Variables:	i:						kohdebufferin index
			filename:				tiedoston polku ja name
			lataa_paletti:			true = lataa paletti kuvasta, false = s�ilyt� vanha paletti
			
Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Quit();

Task:	Lopettaa PD:n.

Variables:

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Palette_Rotate(unsigned char eka_vari, unsigned char vika_vari);

Task:	Py�ritt�� haluttua osaa paletista.

Variables:	eka_vari:				ensimm�inen v�ri, jota py�ritet��n.
			vika_vari:				viimeinen color, jota py�ritet��n.

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------  
void	PisteDraw_UpdateScreen();

Task:	Copy PD_BACKBUFFER-puskurin sis�ll�n n�ytt�muistiin.

Variables:	-

Returns:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Palette_Get(PALETTEENTRY *&paletti);

Task:

Variables:

Returns:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Palette_Set(void);

Task:

Variables:

Returns:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Draw_Begin(int i, unsigned char *&back_buffer, unsigned long &lPitch);

Task:	Lukitsee halutun bufferin, jotta siihen voitaisiin piirt�� "manuaalisesti".

Variables:	i:						lukittavan bufferin index
			back_buffer:			viittaus taulukkoon, joka sis�lt�� bufferissa olevat pikselit
			lPitch:					bufferin (oikea) width pikselein�

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Draw_End(void);

Task:	Vapauttaa kaikki bufferit, jotka on lukittu. �l� k�yt�! Hidas!

Variables:

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Draw_End(int i);

Task:	Vapauttaa halutun bufferin lukituksen (jos se on lukittu)

Variables:	i:						halutun bufferin index

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Reset_paletti(void);

Task:	Returns alkuper�isen kuvasta ladatun paletin.

Variables:	-

Returns:	Nolla tai PD_ERROR

-------------------------------------------------------------------------------------------------------------
unsigned long	PisteDraw_GetMemoryUsed();

Task:	Returns vapaana olevan videomuistin m��r�n bittein�

Variables:

Returns:	Vapaan videomuistin m��r�.

-------------------------------------------------------------------------------------------------------------
char   *PisteDraw_GetError();

Task:	Returns (aina) viimeisimm�n virheilmoituksen. 

Variables:

Returns:	Vapaan videomuistin m��r�.

-------------------------------------------------------------------------------------------------------------

*/