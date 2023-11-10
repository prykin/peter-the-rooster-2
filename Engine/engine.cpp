/* GAME.CPP		PisteEngine Esimerkki-ohjelma v0.1 by Janne Kivilahti 27.12.2001
				Ohjelman toiminta:
				Ruudulla on taustakuva, joka vierii vasemmalta oikealle.
				Ruudulla leijuu pallo, jota voi ohjata hiirell� tai peliohjaimella.
				A-n�pp�imest� ruutu feidaa mustaksi ja S-n�pp�imest� takaisin.
				Ohjelman suoritus loppuu ESC:st�, hiiren oikeasta napista tai peliohjaimen napista 2.
				
				T�ll� hetkell� PisteEnginest� on valmiina:
				PisteInput = Hiiren, n�pp�imist�n ja peliohjainten hallinta.
				PisteDraw  = Ruudulle piirtely. Sis�lt�� my�s PisteFontin.
				PisteWait  = Tahdistus (peli py�rii kaikissa koneissa samaa nopeutta).

				Ikkuna-s�l�n piilottaminen ei ollutkaan niin yksinkertaista kuin kuvittelin,
				joten kaikki ikkunan alustamiseen yms. liittyv� on "piilotettu" ihan koodin
				pohjalle.

				En ole kommentoinut noita Piste-koodeja juuri mitenk��n, mutta t�ss� esimerkiss�
				on aika paljon selityksi�. Koeta saada niist� jotakin irti :)

				Rajoituksia:
				
			-	Vapaassa piirrossa ei voi olla kuin yksi pinta lukittuna kerrallaan.
			-	Kun jokin pinta on lukittuna vapaassa piirrossa, 
				et voi k�ytt�� PisteDraw_Font_Kirjoita()-metodia.
			-	�l� flippaa samasta pinnasta samaan pintaan.

*/


/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN  
#define INITGUID 

/* INCLUDES -----------------------------------------------------------------------------------*/

//#include <windows.h>
//#include <windowsx.h>
//#include <mmsystem.h>
#include <math.h>
#include "input.h"
#include "draw.h"
#include "sound.h"
#include "wait.h"

/* TYYPPIM��RITTELYT ---------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

/* M��RITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define	GAME_NAME		  "ANY GAME"

const int SCREEN_WIDTH			= 800;	// �l� koske! Muut arvot eiv�t (viel�) toimi!
const int SCREEN_HEIGHT			= 600;	// �l� koske!
const int SCREEN_BPP			= 8;	// �l� koske!
const int MAX_COLORS_PALETTE	= 256;	// �l� koske!

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND      ikkunan_kahva			= NULL; // p��ikkunan kahva
HINSTANCE hinstance_app			= NULL; // hinstance?
HDC       global_dc				= NULL; // global dc?

bool DirectX_virhe				= false;// jos t�m� muuttuu todeksi niin ohjelma lopetetaan
char DirectX_virhe_viesti[2]	= " ";  // ei viel� (kunnolla) k�yt�ss�

bool window_closed				= false;// onko ikkuna kiinni

int	kuvabufferi1	= NULL;				// indeksi PisteDraw:n kuvabufferitaulukkoon
int	kuvabufferi2	= NULL;				// indeksi PisteDraw:n kuvabufferitaulukkoon				

int	fontti1,							// indeksi PisteDraw:n fonttitaulukkoon
	fontti2;							// indeksi PisteDraw:n fonttitaulukkoon

int aani1;								// indeksi PisteSoundin ��nitaulukkoon
int aani2;								// indeksi PisteSoundin ��nitaulukkoon

int pallo_x = 320;
int pallo_y = 240;
int map_x = 0;

int key_delay = 0;

/* PELI ---------------------------------------------------------------------------------------*/

int Game_Init(void)
{

	// Alustetaan PisteDraw - Ruudulle piirtely
	if ((PisteSound_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 1, 22050, 8)) == PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}	
	
	// Alustetaan PisteInput - Hiiren ja n�pp�imist�n hallinta
	if ((PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app)) == PI_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Alustetaan PisteDraw - Ruudulle piirtely
	if ((PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
								  SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Luodaan ensimm�inen kuvabufferi:
	// SCREEN_WIDTH ja SCREEN_HEIGHT: koko ruudun korkeus ja leveys (melko itsest��nselv��?)
	// true: Bufferi sijaitsee videomuistissa (VRAM). Jos systeemimuistissa niin false.
	// 255: L�pin�kyvyysv�ri.
	// Metodi palauttaa integerin, joka on indeksiavain bufferitaulukkoon. Eli ota avain talteen. 
	if ((kuvabufferi1 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Tehd��n toinen samanlainen
	if ((kuvabufferi2 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255)) == PD_VIRHE)
	{
		DirectX_virhe = true;	
		return 1;
	}


	// Ladataan kuva ensimm�iseen bufferiin. T�ss� tapauksessa taustakuva.  
	// kuvabufferi1: indeksi PisteDraw:n taulukkoon, jossa on kuvabuffereita.
	// "wormz.bmp": Kuva joka ladataan.
	// true: Ladataan ja m��r�t��n paletti. Jos ei ladata palettia, niin false
	
	if (PisteDraw_Lataa_Kuva(kuvabufferi1,"wormz.bmp", true) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}


	// Ladataan toinen kuva tokaan bufferiin. Ei ladata palettia. Kuvassa on spritej� yms...
	if (PisteDraw_Lataa_Kuva(kuvabufferi2,"wormz2.bmp", false) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}


	// Luodaan uusi fontti. V�h�n kinkkisempi juttu: Joutuu laskeskelemaan pikseleit� kuvassa.
	// kuvabufferi2: Bufferi, josta fontin grafiikat otetaan.
	// 1,422: X- ja Y-kordinaatit josta kohtaa bufferista fontin grafiikat alkavat. Vasen- ja yl�kulma.
	// 14,14: Yhden kirjaimen leveys ja korkeus. 
	// 29: Kuinka monta eri kirjainta t�ss� fontissa on.
	if ((fontti1 = PisteDraw_Font_Uusi(kuvabufferi2,1,422,14,14,29)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	// Ja sama juttu...
	if ((fontti2 = PisteDraw_Font_Uusi(kuvabufferi2,1,467,6,12,46)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	// Asetetaan taustabufferille reunat joiden yli ei voi piirt��. PD_TAUSTABUFFER viittaa
	// siihen kuvabufferiin, joka flipataan n�yt�lle kerran framessa. Jos halutaan asettaa
	// reunat jollekin muulle kuvabufferille, k�ytet��n indeksiavainta: esim. kuvabufferi1.
	// HUOM.! Jokaiselle PisteDraw_Buffer_Uusi() metodilla luodulle kuvabufferille asetetaan
	// automaattisesti reunat: ruudun leveys ja korkeus. Mutta t�ll� siis voidaan erikseen
	// asettaa ne.
	// PD_TAUSTABUFFER: kuvabufferi, jolle reunat asetetaan.
	// 20,20,620,460: Vasen reuna, yl�reuna, oikea reuna, alareuna.
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,20,20,620,460);


	if ((aani1 = PisteSound_SFX_Uusi("sfx1.wav"))==PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;		
	}

	if ((aani2 = PisteSound_SFX_Uusi("sfx2.wav"))==PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;		
	}	

	// Toimii kuin TickCount (tai itseasiassa on se). Aloitetaan ajanotto ekan kerran.
	PisteWait_Start();


	
/*	Tai koko homma lyhyesti ilman virhetarkistuksia ja h�pin�it�...

	PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app);
	PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
	 			     SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE);
	kuvabufferi1 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	kuvabufferi2 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	PisteDraw_Lataa_Kuva(kuvabufferi1,"wormz.bmp", true);
	PisteDraw_Lataa_Kuva(kuvabufferi2,"wormz2.bmp", false);
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,20,20,620,460);
	fontti1 = PisteDraw_Font_Uusi(kuvabufferi2,1,422,14,14,29);
	PisteWait_Start();
*/

	return 0;
}

int Game_Main_Piirra(void)
{
	int x, y;
	map_x = 1 + map_x%640;

	/* T�YTET��N RUUTU HARMAALLA V�RILL� */

	// Kuvaruudun t�ytt� v�rill�:
	// PD_TAUSTABUFFER: bufferi joka t�ytet��n
	// 10: Paletin v�ri jolla t�ytet��n.
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,10);
	// Tai:
	// PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, 0, 0, 640, 480, 10);

	/* TEHD��N TAUSTAN RULLAUS */

	// KOKONAISTEN buffereiden kopiointi toisiin buffereihin:
	// kuvabufferi1: l�hdebufferi, josta kopioidaan
	// PD_TAUSTABUFFER: kohdebufferi johon kopioidaan.
	// map_x,0: X ja Y eli mihin kohtaan kohdebufferia kopioidaan

	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x,0);
	// Sama juttu kuin edellisess�
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x-640,0);

	/* PIIRRET��N PALLO HIIREN KURSORIN KOHDALLE */

	// Bufferin OSAN kopiointi toiseen bufferiin tiettyyn kohtaan:
	// kuvabufferi2: L�hdebufferi josta kopioidaan
	// PD_TAUSTABUFFER: kohdebufferi johon kopioidaan (t�ss� tapauksessa pinta joka lopussa flipataan n�yt�lle)
	// pallo_x, pallo_y: X- ja Y-kordinaatit mihin kohtaan kohdebufferia kopioidaan (vasen yl�kulma)
	// 200,141,219,160: Alue l�hdebufferista, jolta kopioidaan.
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,200,141,219,160);
	//PisteDraw_Buffer_Flip(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,true,true);


	/* PIIRRET��N LUMISADE KUVABUFFERIIN JA TAUSTABUFFERIIN*/ 
	
	// Valmistellaan Vapaa piirto bufferiin
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	// Ennen kuin voidaan sorkkia mit��n kuvabufferia niin se pit�� lukita ensin.
	// kuvabufferi1: Bufferi joka lukitaan piirt�mist� varten
	// buffer: Pointteri kuvabufferin sis�lt��n
	// tod_leveys: Kuvabufferin todellinen leveys 
	// Vaikka ruudun leveys olisi asetettu 640x480, se voi oikeasti olla leve�mpi
	PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (DWORD &)tod_leveys);
	
	// Piirret��n kuvabufferiin lumisadetta

	for (x=100;x<200;x++)
		for (y=100;y<200;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	PisteDraw_Piirto_Lopeta(kuvabufferi1); //T�ss� v�liss� mahdollinen, mutta ei pakollinen
	
	// Valmistellaan toinen bufferi piirt�mist� varten. Sit� ennen tarkistetaan,
	// onko jokin toinen bufferi lukittu. Jos on niin vapautetaan se ensin.
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);
	
	for (x=300;x<350;x++)
		for (y=300;y<350;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	// Lopetaan piirt�minen vapauttamalla lukittu bufferi.
	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

	/* PIIRRET��N "TESTI"-TEKSTI RUUDULLE */

	// Kirjoitetaan teksti ruudulle aikaisemmin varatulla fontilla //
	// fontti1: indeksiavain luotuun fonttiin jolla kirjoitetaan
	// "testi": oiskohan teksti joka ruudulle kirjoitetaan :)
	// 10,10  : mihin kirjoitetaan.
	// VARO! Fontin piirto ei reunoista piittaa vaan kirjoittaa surutta
	// my�s ruudun ulkopuoliseen muistiin jos sinne p��see k�siksi.
	// Paluuarvona saadaan kirjoitetun tekstin leveys pikselein�
	PisteDraw_Font_Kirjoita(fontti1,"piste engine testi",PD_TAUSTABUFFER,10,10);
	
	/* PIIRRET��N OHJETEKSTEJ�-TEKSTEJ� RUUDULLE */

	int tekstin_leveys = 0;
	tekstin_leveys += PisteDraw_Font_Kirjoita(fontti2,"lopetus: esc, oikea hiiren nappi tai peliohjaimen nappi 2.",PD_TAUSTABUFFER,10,40);
	PisteDraw_Font_Kirjoita(fontti2,"a ja s: feidaus.",PD_TAUSTABUFFER,10,60);

	/* PIIRRET��N RUUDULLE VAPAAN VIDEOMUISTIN M��R�*/

	char  vram[30];
	ltoa(PisteDraw_Videomuistia(), vram, 10);
	PisteDraw_Font_Kirjoita(fontti2,vram,PD_TAUSTABUFFER,300,10);

	
	/* PIIRRET��N RUUDULLE TEKSTI JOS PELIOHJAIMEN NAPPIA 2 ON PAINETTU */

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1, PI_OHJAIN_NAPPI_1))
		PisteDraw_Font_Kirjoita(fontti2,"peliohjaimen nappi 1 painettu",PD_TAUSTABUFFER,10,75);

	/* ODOTETAAN JOS LIIAN NOPEA KONE */
	
	// Jos oltiin liian nopeita, niin venataan hetki. Sama kuin esim. TickWait.
	// 10 sadasosasekuntia (muistaakseni)
	PisteWait_Wait(10);

	/* FLIPATAAN TAUSTABUFFERI (PD_TAUSTABUFFER) N�YT�LLE */
	
	// Dumpataan kaksoispuskurin PD_TAUSTABUFFER sis�lt� n�ytt�muistiin
	PisteDraw_Paivita_Naytto();

	// K�ynnistet��n taas ajastin, jota tutkitaan taas PisteWait()-metodilla.
	PisteWait_Start();



	/* Kaikki edellinen ilman turhaa s�l��...

	int x, y;
	map_x = 1 + map_x%640;
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,10);
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x,0);
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x-640,0);	
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,200,141,219,160);  
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (DWORD &)tod_leveys);

	for (x=100;x<200;x++)
		for (y=100;y<200;y++)
			buffer[x+y*tod_leveys] = rand()%30;
		
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);

	for (x=300;x<350;x++)
		for (y=300;y<350;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	PisteDraw_Piirto_Lopeta();
	PisteDraw_Font_Kirjoita(fontti1,"testi",PD_TAUSTABUFFER,10,10);
	int tekstin_leveys = 0;
	tekstin_leveys += PisteDraw_Font_Kirjoita(fontti2,"lopetus: esc tai oikea hiiren nappi.",PD_TAUSTABUFFER,10,40);
	PisteDraw_Font_Kirjoita(fontti2,"a ja s: feidaus.",PD_TAUSTABUFFER,10,60);
	char  vram[30];
	ltoa(PisteDraw_Videomuistia(), vram, 10);
	PisteDraw_Font_Kirjoita(fontti2,vram,PD_TAUSTABUFFER,300,10);	
	PisteWait_Wait(10);
	PisteDraw_Paivita_Naytto();
	PisteWait_Start();
	*/

	return 0;
}

int Game_Main(void)
{
	if (window_closed)
		return(0);

	/* HAETAAN N�PP�IMIST�N, HIIREN JA PELIOHJAINTEN T�M�NHETKISET TILAT */

	// N�pp�imist� 
	if (!PisteInput_Hae_Nappaimet())		//Haetaan n�pp�inten tilat
		DirectX_virhe = true;
	
	// Hiirulainen
	if (!PisteInput_Hae_Hiiri())			//Haetaan hiiren tila
		DirectX_virhe = true;

	// Kaikki peliohjaimet
	if (!PisteInput_Hae_Ohjaimet()){}
		//DirectX_virhe = true;

	// Lis�t��n pallon x- ja y-muuttujiin hiiren kursorin sijainnin muutos

	pallo_x = PisteInput_Hiiri_X(pallo_x);	//Tai: pallo_x += PisteInput_Hiiri_X(0);
	pallo_y = PisteInput_Hiiri_Y(pallo_y);
	
	// Lis�t��n pallon x- ja y-muuttujiin peliohjaimen 1 liikkeet

	pallo_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/50;
	pallo_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/50;

	if (pallo_x > SCREEN_WIDTH)
		pallo_x = SCREEN_WIDTH;

	if (pallo_y > SCREEN_HEIGHT)
		pallo_y = SCREEN_HEIGHT;

	if (pallo_x < 0)
		pallo_x = 0;

	if (pallo_y < 0)
		pallo_y = 0;

	Game_Main_Piirra();

	
	/* FEIDATAAN PALETTIA JOS K�YTT�J� PAINELEE A- JA S-NAPPULOITA */

	// Luetaan onko k�ytt�j� painanut A-n�pp�int�
	if (PisteInput_Keydown(DIK_A))
		PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		// Jos on, niin aloitetaan paletin feidaus mustaksi.
		// Valmiita nopeuksia: PD_FADE_HIDAS (=1), PD_FADE_NORMAALI (=2), PD_FADE_NOPEA (=5)
		// Parametriksi voi antaa oikeastaan mink� tahansa positiivisen arvon.
		// Feidaus tapahtuu arvosta 100 - 0. 
	if (PisteInput_Keydown(DIK_S))
		PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);

	if (key_delay == 0)
	{

		if (PisteInput_Keydown(DIK_1))
		{
			if (PisteSound_SFX_Soita(aani1) == PS_VIRHE)
				DirectX_virhe = true;
			key_delay = 30;
		}
		
		if (PisteInput_Keydown(DIK_2))
		{
			if (PisteSound_SFX_Soita(aani2) == PS_VIRHE)
				DirectX_virhe = true;
			key_delay = 30;
		}
	}
	else
		key_delay--;
	
	/* LOPETETAAN OHJELMA JOS K�YTT�J� PAINAA ESC:� TAI HIIREN OIKEAA NAPPIA */

	// Jos k�ytt�j� painaa ESC:� tai Hiiren oikeaa namiskuukkelia, tai peliohjaimen nappia 2,
	// poistutaan ohjelmasta.
	// PisteInputHiiriVasen() = hiiren vasen nappi
	
	if (PisteInput_Keydown(DIK_ESCAPE) 
		|| PisteInput_Hiiri_Oikea() 
		|| PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1, PI_OHJAIN_NAPPI_2))
	{
		SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		window_closed = true;
	}

	return 0;
}

int Game_Quit(void)
{

	PisteSound_Lopeta();
	PisteDraw_Lopeta();
	PisteInput_Lopeta();

	// Jos on ilmennyt virhe niin n�ytet��n tekstiboksi jossa lukee "Virhe!".
	// Kyll� se k�ytt�j� yll�ttyy t�st� informatiivisesta viestist� :)

	if (DirectX_virhe)
	{
		MessageBox(ikkunan_kahva, PisteDraw_Virheilmoitus(),"Virhe!",
				   MB_OK | MB_ICONEXCLAMATION);
		
		MessageBox(ikkunan_kahva, PisteSound_Virheilmoitus(),"Virhe!",
				   MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}













/* IKKUNA -------------------------------------------------------------------------------------*/

// Seuraa ikkuna-koodia. Allaoleva on ikkunan "tapahtumakuuntelija".


LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		
	HDC				hdc;	

	switch(msg)
	{	
		case WM_CREATE: 
	    {
		
			return(0);
		}	break;
   
		case WM_PAINT: 
		{

			hdc = BeginPaint(hwnd,&ps);	 

			EndPaint(hwnd,&ps);

			return(0);
		}	break;

		case WM_DESTROY: 
		{

			PostQuitMessage(0);
			return(0);
		}	break;

		default:break;

    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));

} 

// Kaiken alku ja juuri: WinMain. T�st� se kaikki alkaa ja t�m�n sis�ll� peli py�rii.

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

	WNDCLASSEX winclass; 
	HWND	   hwnd;	 
	MSG		   msg;		 

	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
							  CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		= LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1)); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);


	hinstance_app = hinstance;

	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,                  
		                        WINDOW_CLASS_NAME,     
							    GAME_NAME, 
							    WS_POPUP | WS_VISIBLE,
						 	    0,0,	  
							    SCREEN_WIDTH, SCREEN_HEIGHT,  
							    NULL,	   
							    NULL,	  
							    hinstance,
							    NULL)))	
		return(0);

	ikkunan_kahva = hwnd;

	Game_Init();

	ShowCursor(FALSE);

	while(!DirectX_virhe)
	{
    
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	   
			if (msg.message == WM_QUIT)
				break;
	   
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		} 
    
		Game_Main();

	} 

	Game_Quit();

	ShowCursor(TRUE);

	return(msg.wParam);
} 