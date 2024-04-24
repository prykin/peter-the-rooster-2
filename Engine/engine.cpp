/* GAME.CPP		PisteEngine Esimerkki-ohjelma v0.1 by Janne Kivilahti 27.12.2001
				Ohjelman toiminta:
				Ruudulla on background_image, joka vierii vasemmalta right.
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
				on duration paljon selityksi�. Koeta saada niist� jotakin irti :)

				Rajoituksia:
				
			-	Vapaassa piirrossa ei voi olla kuin yksi pinta lukittuna kerrallaan.
			-	Kun jokin pinta on lukittuna vapaassa piirrossa, 
				et voi k�ytt�� PisteDraw_Font_Write()-metodia.
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

/* M��RITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define    GAME_NAME          "ANY GAME"

const int SCREEN_WIDTH = 800;    // �l� koske! Muut arvot eiv�t (viel�) toimi!
const int SCREEN_HEIGHT = 600;    // �l� koske!
const int SCREEN_BPP = 8;    // �l� koske!
const int MAX_COLORS_PALETTE = 256;    // �l� koske!

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND window_handle = NULL; // p��ikkunan kahva
HINSTANCE hinstance_app = NULL; // hinstance?
HDC global_dc = NULL; // global dc?

bool DirectX_virhe = false;// jos t�m� muuttuu todeksi niin ohjelma lopetetaan
char DirectX_virhe_viesti[2] = " ";  // ei viel� (kunnolla) k�yt�ss�

bool window_closed = false;// onko ikkuna kiinni

int kuvabufferi1 = NULL;                // index PisteDraw:n kuvabufferitaulukkoon
int kuvabufferi2 = NULL;                // index PisteDraw:n kuvabufferitaulukkoon

int font1,                            // index PisteDraw:n fonttitaulukkoon
font2;                            // index PisteDraw:n fonttitaulukkoon

int aani1;                                // index PisteSoundin ��nitaulukkoon
int aani2;                                // index PisteSoundin ��nitaulukkoon

int pallo_x = 320;
int pallo_y = 240;
int map_x = 0;

int key_delay = 0;

/* PELI ---------------------------------------------------------------------------------------*/

int Game_Init(void) {

    // Alustetaan PisteDraw - Ruudulle piirtely
    if ((PisteSound_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, 1, 22050, 8)) == PS_ERROR) {
        DirectX_virhe = true;
        return 1;
    }

    // Alustetaan PisteInput - Hiiren ja n�pp�imist�n hallinta
    if ((PisteInput_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app)) == PI_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Alustetaan PisteDraw - Ruudulle piirtely
    if ((PisteDraw_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                        MAX_COLORS_PALETTE)) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Luodaan ensimm�inen kuvabufferi:
    // SCREEN_WIDTH ja SCREEN_HEIGHT: koko ruudun height ja width (melko itsest��nselv��?)
    // true: Bufferi sijaitsee videomuistissa (VRAM). Jos systeemimuistissa niin false.
    // 255: L�pin�kyvyysv�ri.
    // Metodi palauttaa integerin, joka on indeksiavain bufferitaulukkoon. Eli ota key talteen.
    if ((kuvabufferi1 = PisteDraw_Buffer_Create(SCREEN_WIDTH, SCREEN_HEIGHT, true, 255)) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Tehd��n toinen samanlainen
    if ((kuvabufferi2 = PisteDraw_Buffer_Create(SCREEN_WIDTH, SCREEN_HEIGHT, true, 255)) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Ladataan kuva ensimm�iseen bufferiin. T�ss� tapauksessa background_image.
    // kuvabufferi1: index PisteDraw:n taulukkoon, jossa on kuvabuffereita.
    // "wormz.bmp": Kuva joka ladataan.
    // true: Ladataan ja m��r�t��n paletti. Jos ei ladata palettia, niin false

    if (PisteDraw_Image_Load(kuvabufferi1, "wormz.bmp", true) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Ladataan toinen kuva tokaan bufferiin. Ei ladata palettia. Kuvassa on spritej� yms...
    if (PisteDraw_Image_Load(kuvabufferi2, "wormz2.bmp", false) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }


    // Luodaan uusi font. V�h�n kinkkisempi juttu: Joutuu laskeskelemaan pikseleit� kuvassa.
    // kuvabufferi2: Bufferi, josta fontin grafiikat otetaan.
    // 1,422: X- ja Y-kordinaatit josta kohtaa bufferista fontin grafiikat alkavat. Vasen- ja yl�kulma.
    // 14,14: Yhden kirjaimen width ja height.
    // 29: Kuinka monta eri kirjainta t�ss� fontissa on.
    if ((font1 = PisteDraw_Font_Create(kuvabufferi2, 1, 422, 14, 14, 29)) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }

    // Ja sama juttu...
    if ((font2 = PisteDraw_Font_Create(kuvabufferi2, 1, 467, 6, 12, 46)) == PD_ERROR) {
        DirectX_virhe = true;
        return 1;
    }

    // Asetetaan taustabufferille edges joiden yli ei voi piirt��. PD_BACKBUFFER viittaa
    // siihen kuvabufferiin, joka flipataan n�yt�lle kerran framessa. Jos halutaan asettaa
    // edges jollekin muulle kuvabufferille, k�ytet��n indeksiavainta: esim. kuvabufferi1.
    // HUOM.! Jokaiselle PisteDraw_Buffer_Create() metodilla luodulle kuvabufferille asetetaan
    // automaattisesti edges: ruudun width ja height. Mutta t�ll� siis voidaan erikseen
    // asettaa ne.
    // PD_BACKBUFFER: kuvabufferi, jolle edges asetetaan.
    // 20,20,620,460: Vasen edge, yl�edge, right_bound edge, alareuna.
    PisteDraw_SetClipper(PD_BACKBUFFER, 20, 20, 620, 460);


    if ((aani1 = PisteSound_SFX_New("sfx1.wav")) == PS_ERROR) {
        DirectX_virhe = true;
        return 1;
    }

    if ((aani2 = PisteSound_SFX_New("sfx2.wav")) == PS_ERROR) {
        DirectX_virhe = true;
        return 1;
    }

    // Toimii kuin TickCount (tai itseasiassa on se). Aloitetaan ajanotto ekan kerran.
    PisteWait_Start();



/*	Tai koko homma lyhyesti ilman virhetarkistuksia ja h�pin�it�...

	PisteInput_Init((HWND &)window_handle, (HINSTANCE &)hinstance_app);
	PisteDraw_Init((HWND &)window_handle, (HINSTANCE &) hinstance_app,
	 			     SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE);
	kuvabufferi1 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	kuvabufferi2 = PisteDraw_Buffer_Create(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	PisteDraw_Lataa_Kuva(kuvabufferi1,"wormz.bmp", true);
	PisteDraw_Image_Load(kuvabufferi2,"wormz2.bmp", false);
	PisteDraw_SetClipper(PD_BACKBUFFER,20,20,620,460);
	font1 = PisteDraw_Font_Create(kuvabufferi2,1,422,14,14,29);
	PisteWait_Start();
*/

    return 0;
}

int Game_Main_Piirra(void) {
    int x, y;
    map_x = 1 + map_x % 640;

    /* T�YTET��N RUUTU HARMAALLA V�RILL� */

    // Kuvaruudun t�ytt� v�rill�:
    // PD_BACKBUFFER: bufferi joka t�ytet��n
    // 10: Paletin v�ri jolla t�ytet��n.
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 10);
    // Tai:
    // PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0, 0, 640, 480, 10);

    /* TEHD��N TAUSTAN RULLAUS */

    // KOKONAISTEN buffereiden kopiointi toisiin buffereihin:
    // kuvabufferi1: l�hdebufferi, josta kopioidaan
    // PD_BACKBUFFER: kohdebufferi johon kopioidaan.
    // map_x,0: X ja Y eli mihin kohtaan kohdebufferia kopioidaan

    PisteDraw_Buffer_Flip_Fast(kuvabufferi1, PD_BACKBUFFER, map_x, 0);
    // Sama juttu kuin edellisess�
    PisteDraw_Buffer_Flip_Fast(kuvabufferi1, PD_BACKBUFFER, map_x - 640, 0);

    /* PIIRRET��N PALLO HIIREN KURSORIN KOHDALLE */

    // Bufferin OSAN kopiointi toiseen bufferiin tiettyyn kohtaan:
    // kuvabufferi2: L�hdebufferi josta kopioidaan
    // PD_BACKBUFFER: kohdebufferi johon kopioidaan (t�ss� tapauksessa pinta joka lopussa flipataan n�yt�lle)
    // pallo_x, pallo_y: X- ja Y-kordinaatit mihin kohtaan kohdebufferia kopioidaan (left_bound yl�kulma)
    // 200,141,219,160: Alue l�hdebufferista, jolta kopioidaan.
    PisteDraw_Buffer_Flip_Fast(kuvabufferi2, PD_BACKBUFFER, pallo_x, pallo_y, 200, 141, 219, 160);
    //PisteDraw_Buffer_Flip(kuvabufferi2,PD_BACKBUFFER,pallo_x,pallo_y,true,true);


    /* PIIRRET��N LUMISADE KUVABUFFERIIN JA TAUSTABUFFERIIN*/

    // Valmistellaan Vapaa piirto bufferiin
    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    // Ennen kuin voidaan sorkkia mit��n kuvabufferia niin se pit�� lukita ensin.
    // kuvabufferi1: Bufferi joka lukitaan piirt�mist� varten
    // buffer: Pointteri kuvabufferin sis�lt��n
    // tod_leveys: Kuvabufferin todellinen width
    // Vaikka ruudun width olisi asetettu 640x480, se voi oikeasti olla leve�mpi
    PisteDraw_Draw_Begin(kuvabufferi1, *&buffer, (unsigned long &) tod_leveys);

    // Piirret��n kuvabufferiin lumisadetta

    for (x = 100; x < 200; x++)
        for (y = 100; y < 200; y++)
            buffer[x + y * tod_leveys] = rand() % 30;

    PisteDraw_Draw_End(kuvabufferi1); //T�ss� v�liss� mahdollinen, mutta ei pakollinen

    // Valmistellaan toinen bufferi piirt�mist� varten. Sit� ennen tarkistetaan,
    // onko jokin toinen bufferi lukittu. Jos on niin vapautetaan se ensin.
    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &) tod_leveys);

    for (x = 300; x < 350; x++)
        for (y = 300; y < 350; y++)
            buffer[x + y * tod_leveys] = rand() % 30;

    // Lopetaan piirt�minen vapauttamalla lukittu bufferi.
    PisteDraw_Draw_End(PD_BACKBUFFER);

    /* PIIRRET��N "TESTI"-TEKSTI RUUDULLE */

    // Kirjoitetaan text ruudulle aikaisemmin varatulla fontilla //
    // font1: indeksiavain luotuun fonttiin jolla kirjoitetaan
    // "testi": oiskohan text joka ruudulle kirjoitetaan :)
    // 10,10  : mihin kirjoitetaan.
    // VARO! Fontin piirto ei reunoista piittaa vaan kirjoittaa surutta
    // my�s ruudun ulkopuoliseen muistiin jos sinne p��see k�siksi.
    // Paluuarvona saadaan kirjoitetun tekstin width pikselein�
    PisteDraw_Font_Write(font1, "piste engine testi", PD_BACKBUFFER, 10, 10);

    /* PIIRRET��N OHJETEKSTEJ�-TEKSTEJ� RUUDULLE */

    int tekstin_leveys = 0;
    tekstin_leveys += PisteDraw_Font_Write(font2, "lopetus: esc, right_bound hiiren nappi tai peliohjaimen nappi 2.",
                                           PD_BACKBUFFER, 10, 40);
    PisteDraw_Font_Write(font2, "movement_x ja s: feidaus.", PD_BACKBUFFER, 10, 60);

    /* PIIRRET��N RUUDULLE VAPAAN VIDEOMUISTIN M��R�*/

    char vram[30];
    ltoa(PisteDraw_GetMemoryUsed(), vram, 10);
    PisteDraw_Font_Write(font2, vram, PD_BACKBUFFER, 300, 10);


    /* PIIRRET��N RUUDULLE TEKSTI JOS PELIOHJAIMEN NAPPIA 2 ON PAINETTU */

    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1))
        PisteDraw_Font_Write(font2, "peliohjaimen nappi 1 painettu", PD_BACKBUFFER, 10, 75);

    /* ODOTETAAN JOS LIIAN NOPEA KONE */

    // Jos oltiin liian nopeita, niin venataan hetki. Sama kuin esim. TickWait.
    // 10 sadasosasekuntia (muistaakseni)
    PisteWait_Wait(10);

    /* FLIPATAAN TAUSTABUFFERI (PD_BACKBUFFER) N�YT�LLE */

    // Dumpataan kaksoispuskurin PD_BACKBUFFER sis�lt� n�ytt�muistiin
    PisteDraw_UpdateScreen();

    // K�ynnistet��n taas timer, jota tutkitaan taas PisteWait()-metodilla.
    PisteWait_Start();



    /* Kaikki edellinen ilman turhaa s�l��...

    int x, y;
    map_x = 1 + map_x%640;
    PisteDraw_Buffer_Clear(PD_BACKBUFFER,10);
    PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_BACKBUFFER,map_x,0);
    PisteDraw_Buffer_Flip_Fast(kuvabufferi1,PD_BACKBUFFER,map_x-640,0);
    PisteDraw_Buffer_Flip_Fast(kuvabufferi2,PD_BACKBUFFER,pallo_x,pallo_y,200,141,219,160);
    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (unsigned long &)tod_leveys);

    for (x=100;x<200;x++)
        for (y=100;y<200;y++)
            buffer[x+y*tod_leveys] = rand()%30;

    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &)tod_leveys);

    for (x=300;x<350;x++)
        for (y=300;y<350;y++)
            buffer[x+y*tod_leveys] = rand()%30;

    PisteDraw_Draw_End();
    PisteDraw_Font_Kirjoita(font1,"testi",PD_BACKBUFFER,10,10);
    int tekstin_leveys = 0;
    tekstin_leveys += PisteDraw_Font_Kirjoita(font2,"lopetus: esc tai right_bound hiiren nappi.",PD_BACKBUFFER,10,40);
    PisteDraw_Font_Kirjoita(font2,"movement_x ja s: feidaus.",PD_BACKBUFFER,10,60);
    char  vram[30];
    ltoa(PisteDraw_GetMemoryUsed(), vram, 10);
    PisteDraw_Font_Write(font2,vram,PD_BACKBUFFER,300,10);
    PisteWait_Wait(10);
    PisteDraw_UpdateScreen();
    PisteWait_Start();
    */

    return 0;
}

int Game_Main(void) {
    if (window_closed)
        return (0);

    /* HAETAAN N�PP�IMIST�N, HIIREN JA PELIOHJAINTEN T�M�NHETKISET TILAT */

    // N�pp�imist�
    if (!PisteInput_Update_Keyboard())        //Haetaan n�pp�inten tilat
        DirectX_virhe = true;

    // Hiirulainen
    if (!PisteInput_Update_Mouse())            //Haetaan hiiren tila
        DirectX_virhe = true;

    // Kaikki peliohjaimet
    if (!PisteInput_Update_Controllers()) {}
    //DirectX_virhe = true;

    // Lis�t��n pallon x- ja y-muuttujiin hiiren kursorin sijainnin transformation

    pallo_x = PisteInput_Mouse_X(pallo_x);    //Tai: pallo_x += PisteInput_Mouse_X(0);
    pallo_y = PisteInput_Mouse_Y(pallo_y);

    // Lis�t��n pallon x- ja y-muuttujiin peliohjaimen 1 liikkeet

    pallo_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 50;
    pallo_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 50;

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
    if (PisteInput_KeyDown(DIK_A))
        PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
    // Jos on, niin aloitetaan paletin feidaus mustaksi.
    // Valmiita nopeuksia: PD_FADE_SLOW (=1), PD_FADE_NORMAL (=2), PD_FADE_FAST (=5)
    // Parametriksi voi antaa oikeastaan mink� tahansa positiivisen arvon.
    // Feidaus tapahtuu arvosta 100 - 0.
    if (PisteInput_KeyDown(DIK_S))
        PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);

    if (key_delay == 0) {

        if (PisteInput_KeyDown(DIK_1)) {
            if (PisteSound_SFX_Play(aani1) == PS_ERROR)
                DirectX_virhe = true;
            key_delay = 30;
        }

        if (PisteInput_KeyDown(DIK_2)) {
            if (PisteSound_SFX_Play(aani2) == PS_ERROR)
                DirectX_virhe = true;
            key_delay = 30;
        }
    } else
        key_delay--;

    /* LOPETETAAN OHJELMA JOS K�YTT�J� PAINAA ESC:� TAI HIIREN OIKEAA NAPPIA */

    // Jos k�ytt�j� painaa ESC:� tai Hiiren oikeaa namiskuukkelia, tai peliohjaimen nappia 2,
    // poistutaan ohjelmasta.
    // PisteInputHiiriVasen() = hiiren left_bound nappi

    if (PisteInput_KeyDown(DIK_ESCAPE) || PisteInput_Mouse_Right() ||
            PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_2)) {
        SendMessage(window_handle, WM_CLOSE, 0, 0);
        window_closed = true;
    }

    return 0;
}

int Game_Quit(void) {

    PisteSound_Stop();
    PisteDraw_Quit();
    PisteInput_Quit();

    // Jos on ilmennyt virhe niin n�ytet��n tekstiboksi jossa lukee "Virhe!".
    // Kyll� se k�ytt�j� yll�ttyy t�st� informatiivisesta viestist� :)

    if (DirectX_virhe) {
        MessageBox(window_handle, PisteDraw_GetError(), "Virhe!", MB_OK | MB_ICONEXCLAMATION);

        MessageBox(window_handle, PisteSound_GetError(), "Virhe!", MB_OK | MB_ICONEXCLAMATION);
    }
    return 0;
}













/* IKKUNA -------------------------------------------------------------------------------------*/

// Seuraa ikkuna-koodia. Allaoleva on ikkunan "tapahtumakuuntelija".


LRESULT CALLBACK
WindowProc(HWND
hwnd,
UINT msg, WPARAM
wparam,
LPARAM lparam
)
{
PAINTSTRUCT ps;
HDC hdc;

switch(msg)
{
case WM_CREATE:
{

return(0);
}    break;

case WM_PAINT:
{

hdc = BeginPaint(hwnd, &ps);

EndPaint(hwnd, &ps
);

return(0);
}    break;

case WM_DESTROY:
{

PostQuitMessage(0);
return(0);
}    break;

default:break;

}

return (
DefWindowProc(hwnd, msg, wparam, lparam
));

}

// Kaiken alku ja juuri: WinMain. T�st� se kaikki alkaa ja t�m�n sis�ll� peli py�rii.

int WINAPI
WinMain(    HINSTANCE
hinstance,
HINSTANCE hprevinstance, LPSTR
lpcmdline,
int ncmdshow
)
{

WNDCLASSEX winclass;
HWND hwnd;
MSG msg;

winclass.
cbSize = sizeof(WNDCLASSEX);
winclass.
style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
winclass.
lpfnWndProc = WindowProc;
winclass.
cbClsExtra = 0;
winclass.
cbWndExtra = 0;
winclass.
hInstance = hinstance;
winclass.
hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
winclass.
hCursor = LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
winclass.
hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
winclass.
lpszMenuName = NULL;
winclass.
lpszClassName = WINDOW_CLASS_NAME;
winclass.
hIconSm = LoadIcon(NULL, IDI_APPLICATION);


hinstance_app = hinstance;

if (!
RegisterClassEx(&winclass)
)
return(0);

if (!(
hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, GAME_NAME, WS_POPUP | WS_VISIBLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                      NULL, NULL, hinstance, NULL)
))
return(0);

window_handle = hwnd;

Game_Init();

ShowCursor(FALSE);

while(!DirectX_virhe)
{

if (
PeekMessage(&msg,
NULL,0,0,PM_REMOVE))
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