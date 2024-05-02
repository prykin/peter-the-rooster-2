/* 
PisteEngine - PisteDraw 1.5	
16.06.2002	Janne Kivilahti / Piste Gamez

Versio 1.2
----------

	Maksimim��r� buffereita korotettu 200 -> 2000.

	PisteDraw_Buffer_Clear(int i, int left_bound, int top_bound, int right_bound, int bottom_bound, unsigned char color);
	Klipperi lis�tty.

	PisteDraw_Font_Write_Transparent(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
	Lis�tty. Toimii vain 256-v�risess�, 32:n osiin pilkotussa paletissa.

	PisteDraw_Draw_End(int i);
	Lis�tty kuormitettu version, jossa on int muotoinen parametri.
	Mahdollistaa useiden pintojen lukitsemisen yht�aikaa. Vanha version toimii my�s.

	PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
	Lis�tty kuormitettu version.
	Mahdollistaa bittikartan piirron, joka on peilattu sivusuunnassa ja/tai yl�salaisin.

Versio 1.3
----------

	char *PisteDraw_GetError() lis�tty. Helpottaa virheen etsint��.

	PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
	Eri kokoisten kuvien charge_time on nyt mahdollista.

	int PisteDraw_Palette_Rotate(int eka_vari, int vika_vari)
	mahdollistaa v�rien py�ritt�misen

Versio 1.4
----------

	PCX-kuvien charge_time PisteDraw_Image_Load() -aliohjelmalla. Aliohjelma tutkii onko tiedoston
	p��te bmp vai pcx ja lataa kuvan sen perusteella. Virhett� pukkaa, jos file ei ole
	kumpaakaan type�.


	unsigned long PisteDraw_GetMemoryUsedMax()

    PisteDraw_SetMarginLeft(int left_bound);
	PisteDraw_SetMarginTop(int top_bound);

Versio 1.5
----------

	Pintojen palautus.

*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "draw.h"
#include "font.h"
#include "log.h"
#include <io.h>
#include <stdio.h>
#include <fstream.h>
#include <mmsystem.h>
#include <malloc.h>
#include <memory.h>

/* DEFINES -----------------------------------------------------------------------------------*/

#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define BITMAP_ID            0x4D42

typedef struct BITMAP_FILE_TAG {
    BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
    BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
    PALETTEENTRY paletti[256];      // we will store the palette here
    unsigned char *buffer;           // this is movement_x pointer to the data

} BITMAP_FILE, *BITMAP_FILE_PTR;

struct BITMAP_BUFFER {
    LPDIRECTDRAWSURFACE4 lpdds;
    RECT klipperi;
    bool video_muisti;
    int leveys;
    int korkeus;
    bool lukittu;
};

struct RECTA {
    unsigned long left;
    unsigned long top;
    unsigned long right;
    unsigned long bottom;
};


/* VARIABLES ---------------------------------------------------------------------------------*/

const int MAX_BUFFEREITA = 2000;
const int MAX_FONTTEJA = 10;

HWND PD_main_window_handle = NULL; // globally track main window
HINSTANCE PD_hinstance_app = NULL; // globally track hinstance
HDC PD_global_dc = NULL; // tracks movement_x global dc


LPDIRECTDRAW4 PD_lpdd = NULL;   // dd object
LPDIRECTDRAWSURFACE4 PD_lpddsprimary = NULL;   // dd primary surface
PALETTEENTRY PD_paletti[256];          // color palette
PALETTEENTRY PD_paletti_nyt[256];      // paletin talletusta varten
LPDIRECTDRAWPALETTE PD_lpddpal = NULL;   // movement_x pointer to the created dd palette
DDSURFACEDESC2 PD_ddsd;                  // movement_x direct draw surface description struct
DDBLTFX PD_ddbltfx;               // used to fill
DDSCAPS2 PD_ddscaps;                  // movement_x direct draw surface capabilities struct
BITMAP_BUFFER PD_buffers[MAX_BUFFEREITA]; // taulukko kuvabuffereista

int PD_ruudun_leveys;
int PD_ruudun_korkeus;
int PD_ruudun_bpp;
int PD_max_varit;
int PD_buffereita_varattu = 2;

int vasen_marginaali = 0, yla_marginaali = 0;

int PD_paletti_fade_pros = 100;
int PD_paletti_fade_laskuri = 0;

PisteFont *PD_fontit[MAX_FONTTEJA];
int PD_fontbuffer;
int PD_font_korkein = 0;

char virhe[60];

bool PD_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

LPDIRECTDRAWSURFACE4 PisteDraw_Create_Surface(int width, int height, int mem_flags, unsigned char color);

int PisteDraw_Load_Bitmap(BITMAP_FILE_PTR bitmap, char *filename);

int PisteDraw_Flip_Bitmap(unsigned char *image, int bytes_per_line, int height);

int PisteDraw_Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

int PisteDraw_Lataa_PCX(char *filename, int buffer_index, bool lataa_paletti);

int PisteDraw_Lataa_PCX_Paletti(char *filename);

/* METHODS -----------------------------------------------------------------------------------*/

int PD_Loki_Kirjoita(char *viesti) {
    return (0);

    int virhe = 0;

    char *filename = "fonttiloki.txt";

    FILE *tiedosto;

    if ((tiedosto = fopen(filename, "movement_x")) == NULL) {
        return (1);
    }

    char mjono[255];

    //memset(mjono,' ',sizeof(mjono));
    //mjono[60] = '\n';

    strcpy(mjono, viesti);

    fwrite(mjono, sizeof(CHAR), strlen(mjono), tiedosto);

    fclose(tiedosto);

    return (0);
}


int
PisteDraw_Init(HWND &main_window_handle, HINSTANCE &hinstance_app, int width, int height, int bpp, int max_colors) {

    if (PD_unload) {

        strcpy(virhe, "Uh, oh, I think we have movement_x bug...");

        PD_main_window_handle = (HWND &) main_window_handle;
        PD_hinstance_app = (HINSTANCE &) hinstance_app;
        PD_ruudun_leveys = width;
        PD_ruudun_korkeus = height;
        PD_ruudun_bpp = bpp;
        PD_max_varit = max_colors;

        LPDIRECTDRAW temp = NULL;                 // v�liaikainen rajapinta jolla haetaan uusin version
        int i;

        if (FAILED(DirectDrawCreate(NULL, &temp, NULL)))    // luo rajapintaosoitin versioon 1.0
        {
            strcpy(virhe, "Cannot initialize DirectDraw!");
            PisteLog_Write("[Error] Piste Draw: Cannot initialize DirectDraw! \n");
            return PD_ERROR;
        }

        if (FAILED(temp->QueryInterface(IID_IDirectDraw4, (LPVOID * ) & PD_lpdd))) // osoitin v 4.0
        {
            strcpy(virhe, "Cannot initialize DirectDraw4!");
            PisteLog_Write("[Error] Piste Draw: Cannot initialize DirectDraw4! \n");
            return PD_ERROR;
        }

        temp->Release();    // tuhotaan v�liaikainen rajapinta
        temp = NULL;

        if (FAILED(PD_lpdd->SetCooperativeLevel(PD_main_window_handle, // Yhteisty� Windowsin kanssa..
                                                DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE |
                                                DDSCL_ALLOWREBOOT))) {
            strcpy(virhe, "Failed to cooperate with Windows!");
            PisteLog_Write("[Error] Piste Draw: Failed to cooperate with Windows! \n");
            return PD_ERROR;
        }

        if (FAILED(PD_lpdd->SetDisplayMode(PD_ruudun_leveys, PD_ruudun_korkeus, PD_ruudun_bpp, 0, 0))) {
            strcpy(virhe, "Unable to change video mode!");
            PisteLog_Write("[Error] Piste Draw: Unable to change video mode! \n");
            return PD_ERROR;
        }

        DD_INIT_STRUCT(PD_ddsd);

        PD_ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        PD_ddsd.dwBackBufferCount = 2;                //Kolmoispuskurointi = primary + 2 taustapuskuria
        PD_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

        if (FAILED(PD_lpdd->CreateSurface(&PD_ddsd, &PD_lpddsprimary, NULL))) {
            strcpy(virhe, "Cannot create primary surface!");
            PisteLog_Write("[Error] Piste Draw: Cannot create primary surface! \n");
            return PD_ERROR;
        }

        PD_buffers[PD_BACKBUFFER].leveys = width;
        PD_buffers[PD_BACKBUFFER].korkeus = height;
        PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, width, height);
        PD_ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

        if (FAILED(PD_lpddsprimary->GetAttachedSurface(&PD_ddsd.ddsCaps, &PD_buffers[PD_BACKBUFFER].lpdds))) {
            strcpy(virhe, "Cannot attach back buffer to primary surface!");
            PisteLog_Write("[Error] Piste Draw: Cannot attach back buffer to primary surface! \n");
            return PD_ERROR;
        }

        PD_buffers[PD_BACKBUFFER2].leveys = width;
        PD_buffers[PD_BACKBUFFER2].korkeus = height;
        PisteDraw_SetClipper(PD_BACKBUFFER2, 0, 0, width, height);

        for (i = 1; i < 255; i++)                            //Luodaan 8-bittinen paletti
        {
            PD_paletti[i].peRed = 0;
            PD_paletti[i].peGreen = 0;
            PD_paletti[i].peBlue = 0;
            PD_paletti[i].peFlags = PC_NOCOLLAPSE;
        }

        if (FAILED(PD_lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE, PD_paletti, &PD_lpddpal,
                                          NULL))) {
            PisteLog_Write("[Error] Piste Draw: Cannot create 8-bit palette! \n");
            strcpy(virhe, "Cannot create 8-bit palette!");
            return PD_ERROR;
        }

        if (FAILED(PD_lpddsprimary->SetPalette(PD_lpddpal))) {
            PisteLog_Write("[Error] Piste Draw: Cannot set palette! \n");
            strcpy(virhe, "Cannot set palette!");
            return PD_ERROR;
        }

        /*LIITET��N KLIPPERI KUVAN REUNOJEN YLIKIRJOITUSTA EST�M��N*/

        PD_fontbuffer = PisteDraw_Buffer_Create(PD_ruudun_leveys, 10, true, 255);

        for (i = 2; i < MAX_BUFFEREITA; i++)        // alustetaan kuvabufferi taulukko.
            PD_buffers[i].lpdds = NULL;                // 0 ja 1 on varattu taustapuskureille

        vasen_marginaali = 0;
        yla_marginaali = 0;

        PD_unload = false;
    }

    return 0;
}

int PisteDraw_SetVideoMode(int width, int height, int bpp, int max_colors) {
    if (FAILED(PD_lpdd->SetDisplayMode(PD_ruudun_leveys, PD_ruudun_korkeus, PD_ruudun_bpp, 0, 0))) {
        strcpy(virhe, "Unable to change video mode!");
        PisteLog_Write("[Error] Piste Draw: Unable to change video mode! \n");
        return PD_ERROR;
    }
    return 0;
}

int PisteDraw_Quit() {

    if (!PD_unload) {

        int i;

        for (i = 0; i < MAX_FONTTEJA; i++) {
            if (PD_fontit[i] != NULL)
                delete PD_fontit[i];
            PD_fontit[i] = NULL;
        }

        for (i = 2; i < MAX_BUFFEREITA; i++)        // 0 ja 1 on varattu taustapuskureille
            PisteDraw_Buffer_Destroy(i);

        if (PD_lpddpal) {
            PD_lpddpal->Release();
        }

        PisteDraw_Buffer_Destroy(PD_BACKBUFFER);

        PisteDraw_Buffer_Destroy(PD_BACKBUFFER2);

        if (PD_lpddsprimary) {
            PD_lpddsprimary->Release();
        }

        if (PD_lpdd) {
            PD_lpdd->Release();
        }

        PD_unload = true;
    }

    return 0;
}

unsigned long PisteDraw_GetMemoryUsed() {
    DDCAPS hel_caps, hal_caps;

    DD_INIT_STRUCT(hel_caps);
    DD_INIT_STRUCT(hal_caps);

    if (FAILED(PD_lpdd->GetCaps(&hal_caps, &hel_caps))) {
        strcpy(virhe, "Cannot aquire system information!");
        return PD_ERROR;
    }

    return hal_caps.dwVidMemFree;
}

unsigned long PisteDraw_GetMemoryUsedMax() {
    DDCAPS hel_caps, hal_caps;

    DD_INIT_STRUCT(hel_caps);
    DD_INIT_STRUCT(hal_caps);

    if (FAILED(PD_lpdd->GetCaps(&hal_caps, &hel_caps))) {
        strcpy(virhe, "Cannot aquire system information!");
        PisteLog_Write("[Error] Piste Draw: Cannot aquire system information! \n");
        return PD_ERROR;
    }

    return hal_caps.dwVidMemTotal;
}

int PisteDraw_Buffer_Create(int width, int height, bool video_memory, unsigned char color) {
    int i = 0, varaus;
    bool find = false;

    DDCAPS hel_caps, hal_caps;

    DD_INIT_STRUCT(hel_caps);
    DD_INIT_STRUCT(hal_caps);

    if (FAILED(PD_lpdd->GetCaps(&hal_caps, &hel_caps))) {
        strcpy(virhe, "Cannot aquire system information!");
        PisteLog_Write("[Error] Piste Draw: Cannot aquire system information to create movement_x new buffer! \n");
        return PD_ERROR;
    }

    if ((unsigned long) (width * height) > hal_caps.dwVidMemFree) {        //tarkastetaan riitt��k� videomuistia
        video_memory = false;
        PisteLog_Write("[Warning] Piste Draw: Out of video memory - creating movement_x new buffer in system memory! \n");
    }

    if (video_memory) {
        varaus = DDSCAPS_VIDEOMEMORY;
    } else {
        varaus = DDSCAPS_SYSTEMMEMORY;
    }

    while ((i < MAX_BUFFEREITA) && !find) {
        if (PD_buffers[i].lpdds == NULL)    // Onko puskurin pinta jo varattu?
        {
            if ((PD_buffers[i].lpdds = PisteDraw_Create_Surface(width, height, varaus, color)) != NULL) {
                PD_buffers[i].leveys = width;
                PD_buffers[i].korkeus = height;
                PD_buffers[i].klipperi.left = 0;
                PD_buffers[i].klipperi.top = 0;
                PD_buffers[i].klipperi.right = width;
                PD_buffers[i].klipperi.bottom = height;
                PD_buffers[i].video_muisti = video_memory;
                PD_buffers[i].lukittu = false;
                find = true;
                PD_buffereita_varattu++;
            } else {
                i = PD_ERROR;
                strcpy(virhe, "Cannot create movement_x new DirectDraw surface!");
                PisteLog_Write("[Error] Piste Draw: Cannot create movement_x new DirectDraw surface! \n");
            }
        } else {
            i++;
        }
    }

    if (!find) {
        strcpy(virhe, "PisteEngine has run out of buffers!");
        PisteLog_Write("[Error] Piste Draw: PisteDraw has run out of buffers! \n");
        i = PD_ERROR;
    }

    return i;
}

bool PisteDraw_Buffer_Destroy(int i) {
    if (i < 0 || i > MAX_BUFFEREITA)
        return false;

    if (PD_buffers[i].lpdds != NULL) {
        PD_buffers[i].lpdds->Release();
        PD_buffers[i].lpdds = NULL;
        PD_buffereita_varattu--;
    } else
        return false;

    return true;
}

int PisteDraw_Image_Load(int index, char *filename, bool load_palette) {
    BITMAP_FILE bitmap;

    bool ok = false;

    char paate[4];
    for (int i = 0; i < 4; i++)
        paate[i] = toupper(filename[strlen(filename) - 3 + i]);
    paate[4] = '\0';

    if (strcmp(paate, "BMP") == 0) {
        ok = true;
        if (PD_buffers[index].lpdds) {
            if (PisteDraw_Load_Bitmap(&bitmap, filename) != 0) {
                strcpy(virhe, "Could not load picture ");
                strcat(virhe, filename);
                strcat(virhe, "!");
                PisteLog_Write("[Error] Piste Draw: ");
                PisteLog_Write(virhe);
                PisteLog_Write("\n");
                return PD_ERROR;
            }

            DD_INIT_STRUCT(PD_ddsd);

            if (FAILED(PD_buffers[index].lpdds->Lock(NULL, &PD_ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL))) {
                strcpy(virhe, "Could not lock surface after loading movement_x picture!");
                PisteLog_Write("[Error] Piste Draw: Could not lock surface after loading movement_x picture! \n");
                return PD_ERROR;
            }

            unsigned char *back_buffer = (unsigned char *) PD_ddsd.lpSurface;

            int x;
            int y;
            int leveys, korkeus;

            if (bitmap.bitmapinfoheader.biWidth > PD_buffers[index].leveys)
                leveys = PD_buffers[index].leveys;
            else
                leveys = bitmap.bitmapinfoheader.biWidth;

            if (bitmap.bitmapinfoheader.biHeight > PD_buffers[index].korkeus)
                korkeus = PD_buffers[index].korkeus;
            else
                korkeus = bitmap.bitmapinfoheader.biHeight;

            for (y = 0; y < korkeus; y++) {
                for (x = 0; x < leveys; x++) {
                    back_buffer[x + y * PD_ddsd.lPitch] = bitmap.buffer[x + y * bitmap.bitmapinfoheader.biWidth];
                }
            }

            if (FAILED(PD_buffers[index].lpdds->Unlock(NULL))) {
                strcpy(virhe, "Could not unlock surface after loading movement_x picture!");
                PisteLog_Write("[Error] Piste Draw: Could not unlock surface after loading movement_x picture! \n");
                return PD_ERROR;
            }


            PisteDraw_Unload_Bitmap_File(&bitmap);

            if (load_palette) {

                if (FAILED(PD_lpddpal->SetEntries(0, 0, PD_max_varit, bitmap.paletti))) {
                    strcpy(virhe, "Could not set palette after loading movement_x picture!");
                    PisteLog_Write("[Error] Piste Draw: Could not set palette after loading movement_x picture! \n");
                    return PD_ERROR;
                }

                for (int pi = 0; pi < 256; pi++) {
                    PD_paletti[pi].peBlue = bitmap.paletti[pi].peBlue;
                    PD_paletti[pi].peRed = bitmap.paletti[pi].peRed;
                    PD_paletti[pi].peGreen = bitmap.paletti[pi].peGreen;
                    PD_paletti[pi].peFlags = bitmap.paletti[pi].peFlags;
                }

            }

        } else
            return PD_ERROR;
    }

    if (strcmp(paate, "PCX") == 0) {
        ok = true;
        if (PisteDraw_Lataa_PCX(filename, index, load_palette) == PD_ERROR) {
            strcpy(virhe, "Could not load picture ");
            strcat(virhe, filename);
            strcat(virhe, "!");
            PisteLog_Write("[Error] Piste Draw: ");
            PisteLog_Write(virhe);
            PisteLog_Write("\n");
            return PD_ERROR;
        }
    }

    if (!ok) {
        strcpy(virhe, "Could not load bitmap file of type ");
        strcat(virhe, paate);
        return PD_ERROR;
    }

    return 0;
}

int PisteDraw_Buffer_Flip(int i) {
    DD_INIT_STRUCT(PD_ddsd);
    DD_INIT_STRUCT(PD_ddbltfx);

    RECT dest_rect;
    dest_rect.left = 0;
    dest_rect.right = PD_ruudun_leveys;
    dest_rect.top = 0;
    dest_rect.bottom = PD_ruudun_korkeus;

    if (FAILED(PD_buffers[PD_BACKBUFFER].lpdds->Blt(&dest_rect, PD_buffers[i].lpdds, &dest_rect,
                                                    (DDBLT_WAIT | DDBLT_KEYSRC), NULL))) {
        strcpy(virhe, "Unable to flip surface!");
        PisteLog_Write("[Error] Piste Draw: Unable to flip surface!\n");
        return PD_ERROR;
    }
    return 0;
}

int PisteDraw_Buffer_Flip(int source_index, int destination_index, int x, int y, bool flip_x, bool flip_y) {
    DD_INIT_STRUCT(PD_ddsd);                //Alustetaan DirectX pintakuvaus
    DD_INIT_STRUCT(PD_ddbltfx);                //Alustetaan Blittaus tehosteet
    int leveys;                                //Apumuuttuja
    int korkeus;                            //Apumuuttuja
    RECT dest_rect;                            //Alue kohdebufferista josta kopioidaan l�hdebufferiin
    RECT source_rect;                        //Alue l�hdebufferista josta kopioidaan kohdebufferiin
    RECT *klipperi_kohde;                    //Pointteri kohdebufferin leikkuriin
    RECT *klipperi_lahde;                    //Pointteri l�hdebufferin leikkuriin

    x += vasen_marginaali;
    y += yla_marginaali;

    if (flip_x)
        PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;

    if (flip_y)
        PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORUPDOWN;

    if (flip_x && flip_y)
        PD_ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT | DDBLTFX_MIRRORUPDOWN;

    klipperi_kohde = &PD_buffers[destination_index].klipperi;
    klipperi_lahde = &PD_buffers[source_index].klipperi;

    int vasen = klipperi_lahde->left;
    int oikea = klipperi_lahde->right;
    int yla = klipperi_lahde->top;
    int ala = klipperi_lahde->bottom;

    leveys = oikea - vasen;    // lasketaan apuja
    korkeus = ala - yla;

    if (x + leveys > klipperi_kohde->right)            //Varmistetaan ett� ei piirret� kohdebufferin
    {                                                //rajojen ulkopuolelle
        oikea = vasen + klipperi_kohde->right - x;
    }

    if (x < klipperi_kohde->left) {
        vasen = vasen - x + klipperi_kohde->left;
    }

    if (y + korkeus > klipperi_kohde->bottom) {
        ala = yla + klipperi_kohde->bottom - y;
    }

    if (y < klipperi_kohde->top) {
        yla = yla - y + klipperi_kohde->top;
    }

    if (oikea > vasen && ala > yla) {

        dest_rect.left = vasen + x;
        dest_rect.right = oikea + x;
        dest_rect.top = yla + y;
        dest_rect.bottom = ala + y;

        source_rect.left = vasen;
        source_rect.right = oikea;
        source_rect.top = yla;
        source_rect.bottom = ala;

        if (flip_x) {
            if (x < klipperi_kohde->left) {
                source_rect.left = klipperi_lahde->left;
                source_rect.right = klipperi_lahde->right - (vasen - klipperi_lahde->left);
            } else {
                source_rect.left = klipperi_lahde->left + (klipperi_lahde->right - oikea);
                source_rect.right = klipperi_lahde->right;
            }
        }

        if (flip_y) {
            if (y < klipperi_kohde->top) {
                source_rect.top = klipperi_lahde->top;
                source_rect.bottom = klipperi_lahde->bottom - (yla - klipperi_lahde->top);
            } else {
                source_rect.top = klipperi_lahde->top + (klipperi_lahde->bottom - ala);
                source_rect.bottom = klipperi_lahde->bottom;
            }
        }

        if (FAILED(PD_buffers[destination_index].lpdds->Blt(&dest_rect, PD_buffers[source_index].lpdds, &source_rect,
                                                            (DDBLT_WAIT | DDBLT_KEYSRC | DDBLT_DDFX), &PD_ddbltfx))) {
            PisteLog_Write("[Error] Piste Draw: Unable to flip surface!\n");
            return PD_ERROR;
        }
    }

    return 0;
}

int
PisteDraw_Buffer_Flip_Fast(int source_index, int destination_index, int x, int y, int left, int top, int right, int bottom) {
    DD_INIT_STRUCT(PD_ddsd);                //Alustetaan DirectX pintakuvaus
    DD_INIT_STRUCT(PD_ddbltfx);                //Alustetaan Blittaus tehosteet
    int leveys;                                //Apumuuttuja
    int korkeus;                            //Apumuuttuja
    RECT source_rect;                        //Alue l�hdebufferista josta kopioidaan kohdebufferiin
    RECT *klipperi_kohde;                    //Pointteri kohdebufferin leikkuriin
    RECT *klipperi_lahde;

    x += vasen_marginaali;
    y += yla_marginaali;

    klipperi_kohde = &PD_buffers[destination_index].klipperi;
    klipperi_lahde = &PD_buffers[source_index].klipperi;

    if (left < PD_buffers[source_index].klipperi.left)    //Tarkistetaan ett� dataa ei oteta l�hdebufferin
        left = PD_buffers[source_index].klipperi.left;    //rajojen ulkopuolelta

    if (right > PD_buffers[source_index].klipperi.right)
        right = PD_buffers[source_index].klipperi.right;

    if (top < PD_buffers[source_index].klipperi.top)
        top = PD_buffers[source_index].klipperi.top;

    if (bottom > PD_buffers[source_index].klipperi.bottom)
        bottom = PD_buffers[source_index].klipperi.bottom;

    leveys = right - left;    // lasketaan apuja
    korkeus = bottom - top;

    if (x + leveys > klipperi_kohde->right)            //Varmistetaan ett� ei piirret� kohdebufferin
    {                                                //rajojen ulkopuolelle
        right = left + klipperi_kohde->right - x;
    }

    if (x < klipperi_kohde->left) {
        left = left - x + klipperi_kohde->left;
        x = klipperi_kohde->left;
    }

    if (y + korkeus > klipperi_kohde->bottom) {
        bottom = top + klipperi_kohde->bottom - y;
    }

    if (y < klipperi_kohde->top) {
        top = top - y + klipperi_kohde->top;
        y = klipperi_kohde->top;
    }

    if (right > left && bottom > top) {
        source_rect.left = left;
        source_rect.right = right;
        source_rect.top = top;
        source_rect.bottom = bottom;

        if (FAILED(PD_buffers[destination_index].lpdds->BltFast(x, y, PD_buffers[source_index].lpdds, &source_rect,
                                                                (DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))) {
            PisteLog_Write("[Error] Piste Draw: Unable to flip surface!\n");
            return PD_ERROR; // blitataan kaikki paitsi l�pin�kyvyysv�ri
        }
    }

    return 0;
}

int PisteDraw_Buffer_Flip_Fast(int source_index, int destination_index, int x, int y) {
    DD_INIT_STRUCT(PD_ddsd);                //Alustetaan DirectX pintakuvaus
    DD_INIT_STRUCT(PD_ddbltfx);                //Alustetaan Blittaus tehosteet
    int leveys;                                //Apumuuttuja
    int korkeus;                            //Apumuuttuja
    RECT source_rect;                        //Alue l�hdebufferista josta kopioidaan kohdebufferiin
    RECT *klipperi_kohde;                    //Pointteri kohdebufferin leikkuriin
    RECT *klipperi_lahde;                    //Pointteri l�hdebufferin leikkuriin

    x += vasen_marginaali;
    y += yla_marginaali;

    klipperi_kohde = &PD_buffers[destination_index].klipperi;
    klipperi_lahde = &PD_buffers[source_index].klipperi;

    int vasen = klipperi_lahde->left;
    int oikea = klipperi_lahde->right;
    int yla = klipperi_lahde->top;
    int ala = klipperi_lahde->bottom;

    leveys = oikea - vasen;    // lasketaan apuja
    korkeus = ala - yla;

    if (x + leveys > klipperi_kohde->right)            //Varmistetaan ett� ei piirret� kohdebufferin
    {                                                //rajojen ulkopuolelle
        oikea = vasen + klipperi_kohde->right - x;
    }

    if (x < klipperi_kohde->left) {
        vasen = vasen - x + klipperi_kohde->left;
        x = klipperi_kohde->left;
    }

    if (y + korkeus > klipperi_kohde->bottom) {
        ala = yla + klipperi_kohde->bottom - y;
    }

    if (y < klipperi_kohde->top) {
        yla = yla - y + klipperi_kohde->top;
        y = klipperi_kohde->top;
    }

    if (oikea > vasen && ala > yla) {
        source_rect.left = vasen;
        source_rect.right = oikea;
        source_rect.top = yla;
        source_rect.bottom = ala;

        if (FAILED(PD_buffers[destination_index].lpdds->BltFast(x, y, PD_buffers[source_index].lpdds, &source_rect,
                                                                (DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))) {
            PisteLog_Write("[Error] Piste Draw: BlitFast failed!\n");
            return PD_ERROR; // blitataan kaikki paitsi l�pin�kyvyysv�ri
        }
    }

    return 0;
}

int PisteDraw_Draw_Begin(int i, unsigned char *&back_buffer, unsigned long &lPitch) {
    if (PD_buffers[i].lukittu)
        if (PisteDraw_Draw_End(i) == PD_ERROR) {
            PisteLog_Write("[Error] Piste Draw: Can't start free draw: Surface unlock failed!\n");
            return PD_ERROR;
        }

    DD_INIT_STRUCT(PD_ddsd);
    //PD_piirto_buffer_index = i;

    if (FAILED(PD_buffers[i].lpdds->Lock(NULL, &PD_ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL))) {
        PisteLog_Write("[Error] Piste Draw: Can't start free draw: Surface lock failed!\n");
        return PD_ERROR;
    }

    back_buffer = (unsigned char *) PD_ddsd.lpSurface;
    lPitch = (unsigned long &) PD_ddsd.lPitch;

    PD_buffers[i].lukittu = true;

    return 0;
}

int PisteDraw_Draw_End(void) {
    int i = 0;

    while (i < MAX_BUFFEREITA /*PD_buffereita_varattu*/) {
        if (PD_buffers[i].lpdds != NULL)
            if (PD_buffers[i].lukittu) {
                if (FAILED(PD_buffers[i].lpdds->Unlock(NULL))) {
                    PisteLog_Write("[Error] Piste Draw: Free draw - surface unlock failed!\n");
                    return PD_ERROR;
                }

                PD_buffers[i].lukittu = false;
            }
    }
    return 0;
}

int PisteDraw_Draw_End(int i) {
    if (PD_buffers[i].lukittu) {
        if (FAILED(PD_buffers[i].lpdds->Unlock(NULL))) {
            PisteLog_Write("[Error] Piste Draw: Free draw - surface unlock failed!\n");
            return PD_ERROR;
        }

        PD_buffers[i].lukittu = false;
    }
    return 0;
}

int PisteDraw_Buffer_Clear(int i, unsigned char color) {
    DD_INIT_STRUCT(PD_ddbltfx);
    PD_ddbltfx.dwFillColor = color;
    IDirectDrawSurface4 *buffer;

    if (i >= MAX_BUFFEREITA)
        return PD_ERROR;

    if (PD_buffers[i].lpdds == NULL)
        return PD_ERROR;

    buffer = PD_buffers[i].lpdds;

    RECT dest_rect;
    dest_rect.left = 0;
    dest_rect.right = PD_buffers[i].leveys;
    dest_rect.top = 0;
    dest_rect.bottom = PD_buffers[i].korkeus;

    if (FAILED(buffer->Blt(&dest_rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &PD_ddbltfx))) {
        PisteLog_Write("[Error] Piste Draw: Buffer fill failed!\n");
        return PD_ERROR;
    }

    return 0;
}

int PisteDraw_Buffer_Clear(int i,  int left, int top, int right, int bottom, unsigned char color) {

    DD_INIT_STRUCT(PD_ddbltfx);
    PD_ddbltfx.dwFillColor = color;
    IDirectDrawSurface4 *buffer;

    if (i >= MAX_BUFFEREITA)
        return PD_ERROR;

    if (PD_buffers[i].lpdds == NULL)
        return PD_ERROR;

    buffer = PD_buffers[i].lpdds;

    left += vasen_marginaali;
    right += vasen_marginaali;
    top += yla_marginaali;
    bottom += yla_marginaali;

    RECT dest_rect;

    dest_rect.left = (long) left;
    dest_rect.right = (long) right;
    dest_rect.top = (long) top;
    dest_rect.bottom = (long) bottom;

    if (dest_rect.left < PD_buffers[i].klipperi.left)
        dest_rect.left = PD_buffers[i].klipperi.left;

    if (dest_rect.right > PD_buffers[i].klipperi.right)
        dest_rect.right = PD_buffers[i].klipperi.right;

    if (dest_rect.top < PD_buffers[i].klipperi.top)
        dest_rect.top = PD_buffers[i].klipperi.top;

    if (dest_rect.bottom > PD_buffers[i].klipperi.bottom)
        dest_rect.bottom = PD_buffers[i].klipperi.bottom;

    if (FAILED(buffer->Blt(&dest_rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &PD_ddbltfx))) {
        PisteLog_Write("[Error] Piste Draw: Buffer fill area failed!\n");
        return PD_ERROR;
    }

    return 0;
}

void PisteDraw_SetClipper(int i, int left, int top, int right, int bottom) {
    BITMAP_BUFFER *buffer;

    buffer = &PD_buffers[i];

    buffer->klipperi.left = left;
    buffer->klipperi.right = right;
    buffer->klipperi.top = top;
    buffer->klipperi.bottom = bottom;
}

void PisteDraw_SetMarginLeft(int left) {
    vasen_marginaali = left;
}

void PisteDraw_SetMarginTop(int top) {
    yla_marginaali = top;
}

// Palettiin liittyv�t metodit

int PisteDraw_Fade_Paletti_Do(int pros) {
    unsigned char i;

    if (FAILED(PD_lpddpal->GetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette get entries failed!\n");
        return PD_ERROR;
    }

    for (i = 0; i < 255; i++) {
        PD_paletti_nyt[i].peRed = (PD_paletti[i].peRed * pros / 100);
        PD_paletti_nyt[i].peGreen = (PD_paletti[i].peGreen * pros / 100);
        PD_paletti_nyt[i].peBlue = (PD_paletti[i].peBlue * pros / 100);
    }

    if (FAILED(PD_lpddpal->SetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette set entries failed!\n");
        return PD_ERROR;
    }

    return 0;
}

int PisteDraw_Palette_Rotate(unsigned char first_color, unsigned char last_color) {
    unsigned char i;

    if (FAILED(PD_lpddpal->GetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette get entries failed!\n");
        return PD_ERROR;
    }

    PALETTEENTRY temp_vari;

    temp_vari.peRed = PD_paletti_nyt[last_color].peRed;
    temp_vari.peGreen = PD_paletti_nyt[last_color].peGreen;
    temp_vari.peBlue = PD_paletti_nyt[last_color].peBlue;

    for (i = last_color; i > first_color; i--) {
        PD_paletti_nyt[i].peRed = PD_paletti_nyt[i - 1].peRed;
        PD_paletti_nyt[i].peGreen = PD_paletti_nyt[i - 1].peGreen;
        PD_paletti_nyt[i].peBlue = PD_paletti_nyt[i - 1].peBlue;
    }

    PD_paletti_nyt[first_color].peRed = temp_vari.peRed;
    PD_paletti_nyt[first_color].peGreen = temp_vari.peGreen;
    PD_paletti_nyt[first_color].peBlue = temp_vari.peBlue;

    if (FAILED(PD_lpddpal->SetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette set entries failed!\n");
        return PD_ERROR;
    }

    return 0;
}

/*

int PisteDraw_Palette_Rotate(unsigned char eka_vari, unsigned char vika_vari)
{
	unsigned char i;
	
	if (FAILED(PD_lpddpal->GetEntries(0,0,255,PD_paletti_nyt)))
		return PD_ERROR;

	PALETTEENTRY temp_vari;
		
	temp_vari.peRed   = PD_paletti_nyt[eka_vari].peRed;
	temp_vari.peGreen = PD_paletti_nyt[eka_vari].peGreen;
	temp_vari.peBlue  = PD_paletti_nyt[eka_vari].peBlue;

	for (i=eka_vari;i<vika_vari;i++)
	{
		PD_paletti_nyt[i].peRed   = PD_paletti_nyt[i+1].peRed;
		PD_paletti_nyt[i].peGreen = PD_paletti_nyt[i+1].peGreen;
		PD_paletti_nyt[i].peBlue  = PD_paletti_nyt[i+1].peBlue;
	}

	PD_paletti_nyt[vika_vari].peRed   = temp_vari.peRed;
	PD_paletti_nyt[vika_vari].peGreen = temp_vari.peGreen;
	PD_paletti_nyt[vika_vari].peBlue  = temp_vari.peBlue;

	if (FAILED(PD_lpddpal->SetEntries(0,0,255,PD_paletti_nyt)))
		return PD_ERROR;
	
	return 0;
}
*/
int PisteDraw_Fade_Palette(void) {
    if ((PD_paletti_fade_pros < 100 && PD_paletti_fade_laskuri > 0) ||
        (PD_paletti_fade_pros > 0 && PD_paletti_fade_laskuri < 0)) {
        PD_paletti_fade_pros += PD_paletti_fade_laskuri;
        if (PisteDraw_Fade_Paletti_Do(PD_paletti_fade_pros) == PD_ERROR) {
            PisteLog_Write("[Error] Piste Draw: Palette fade failed!\n");
            return PD_ERROR;
        }
    }

    return 0;
}

void PisteDraw_Fade_Paletti_Set(int laskuri) {
    PD_paletti_fade_laskuri = laskuri;
}

void PisteDraw_Fade_Palette_In(int count) {
    PD_paletti_fade_pros = 0;
    PD_paletti_fade_laskuri = count;
}

void PisteDraw_Fade_Palette_Out(int count) {
    PD_paletti_fade_pros = 100;
    PD_paletti_fade_laskuri = -count;
}

bool PisteDraw_Fade_Palette_Done(void) {
    if (PD_paletti_fade_pros > 0 && PD_paletti_fade_laskuri < 0)
        return false;

    if (PD_paletti_fade_pros < 100 && PD_paletti_fade_laskuri > 0)
        return false;

    return true;
}

int PisteDraw_Reset_Palette(void) {
    if (FAILED(PD_lpddpal->SetEntries(0, 0, 255, PD_paletti))) {
        PisteLog_Write("[Error] Piste Draw: Palette reset failed!\n");
        return PD_ERROR;
    }

    return 0;
}

int PisteDraw_Palette_Get(PALETTEENTRY *&palette) {
    if (FAILED(PD_lpddpal->GetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette get failed!\n");
        return PD_ERROR;
    }

    palette = (PALETTEENTRY *) PD_paletti_nyt;

    return 0;
}

int PisteDraw_Palette_Set(void) {
    if (FAILED(PD_lpddpal->SetEntries(0, 0, 255, PD_paletti_nyt))) {
        PisteLog_Write("[Error] Piste Draw: Palette set failed!\n");
        return PD_ERROR;
    }

    return 0;
}

void PisteDraw_UpdateScreen() {
    PisteDraw_Fade_Palette();
    while (FAILED(PD_lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
}

int PisteDraw_GetFrameCount() {
    if (FAILED(PD_lpdd->RestoreAllSurfaces()))
        return PD_ERROR;

    return 0;
}

int PisteDraw_Font_Create(int buffer_index, int buffer_x, int buffer_y, int width, int height, int count) {
    int index = 0;
    bool found = false;

    while (index < MAX_FONTTEJA && !found) {
        if (PD_fontit[index] == NULL) {
            PD_fontit[index] = new PisteFont(height, width, count);

            unsigned char *buffer = NULL;
            unsigned long ruudun_leveys;

            PisteDraw_Draw_Begin(buffer_index, *&buffer, (unsigned long &) ruudun_leveys);
            PD_fontit[index]->GetBitmap(buffer_x, buffer_y, ruudun_leveys, buffer);
            PisteDraw_Draw_End(buffer_index);
            found = true;

            if (height > PD_font_korkein) {
                PD_font_korkein = height;
                PisteDraw_Buffer_Destroy(PD_fontbuffer);
                PD_fontbuffer = PisteDraw_Buffer_Create(PD_ruudun_leveys, height, true, 255);
            }

        } else
            index++;
    }

    if (!found) {
        strcpy(virhe, "PisteEngine has run out of free fonts!");
        PisteLog_Write("[Error] Piste Draw: PisteDraw has run out of free fonts!\n");
        index = PD_ERROR;
    }

    return index;
}

int PisteDraw_Font_Create(char *path, char *file) {
    int index = 0;
    bool found = false;

    PD_Loki_Kirjoita("PisteDraw_Font_Create\n");
    PD_Loki_Kirjoita(path);
    PD_Loki_Kirjoita(file);

    PisteLog_Write("- PisteDraw: Loading movement_x font from file: ");
    PisteLog_Write(path);
    PisteLog_Write(file);
    PisteLog_Write("\n");

    while (index < MAX_FONTTEJA && !found) {
        if (PD_fontit[index] == NULL) {
            PD_Loki_Kirjoita("Tyhja font\n");

            PD_fontit[index] = new PisteFont();

            if (PD_fontit[index]->LoadFromFile(path, file) == -1) {
                strcpy(virhe, "PisteEngine can't load movement_x font from file!");
                PisteLog_Write("[Error] Piste Draw: Loading font failed!\n");

                PD_Loki_Kirjoita("Lataus ep�onnistui\n");

                return PD_ERROR;
            }

            PD_Loki_Kirjoita("Lataus onnistui\n");

            found = true;

            if (PD_fontit[index]->GetHeight() > PD_font_korkein) {
                PD_font_korkein = PD_fontit[index]->GetHeight();
                PisteDraw_Buffer_Destroy(PD_fontbuffer);
                PD_fontbuffer = PisteDraw_Buffer_Create(PD_ruudun_leveys, PD_font_korkein, true, 255);
            }

        } else
            index++;
    }

    if (!found) {
        strcpy(virhe, "PisteEngine has run out of free fonts!");
        PisteLog_Write("[Error] Piste Draw: PisteDraw has run out of free fonts!\n");
        index = PD_ERROR;
    }

    PD_Loki_Kirjoita("Kaikki ok.\n");

    return index;
}

int PisteDraw_Font_Write(int font_index, char *text, int buffer_index, int x, int y) {
    int tekstin_leveys;

    tekstin_leveys = PD_fontit[font_index]->DrawString(text, x, y, buffer_index);

    return tekstin_leveys;
}

int PisteDraw_Font_Write_Transparent(int font_index, char *text, int buffer_index, int x, int y, int transparency) {
    unsigned char *buffer = NULL;
    unsigned long ruudun_leveys;
    int tekstin_leveys;

    x += vasen_marginaali;
    y += yla_marginaali;

    if (PisteDraw_Draw_Begin(buffer_index, *&buffer, (unsigned long &) ruudun_leveys) == PD_ERROR)
        return PD_ERROR;

    tekstin_leveys = PD_fontit[font_index]->DrawStringTransparent(x, y, ruudun_leveys,
                                                                  PD_buffers[buffer_index].klipperi, text,
                                                                  buffer, false, transparency);

    if (PisteDraw_Draw_End(buffer_index) == PD_ERROR)
        return PD_ERROR;

    return tekstin_leveys;
}

char *PisteDraw_GetError() {
    return virhe;
}

/*
int PisteDraw_Font_Write_Transparent(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
{
	unsigned char *buffer = NULL;
	unsigned long screen_width;
	int tekstin_leveys;

	if (PisteDraw_Buffer_Clear(PD_fontbuffer,255) == PD_ERROR)
		return PD_ERROR;

	PisteDraw_Buffer_Flip_Fast(buffer_index, PD_fontbuffer,0,0,x,y,x+640,y+PD_font_korkein);

	if (PisteDraw_Draw_Begin(PD_fontbuffer, *&buffer, (unsigned long &) screen_width) == PD_ERROR)
		return PD_ERROR;

	tekstin_leveys = PD_fontit[font_index]->DrawStringTransparent(x, y, screen_width, merkkijono, buffer, false, pros);

	if (PisteDraw_Draw_End(PD_fontbuffer) == PD_ERROR)
		return PD_ERROR;
	
	if (PisteDraw_Buffer_Flip_Fast(PD_fontbuffer, buffer_index, x, y, 0, 0, tekstin_leveys,
								PD_fontit[font_index]->GetHeight()) == PD_ERROR)
		return PD_ERROR;

	return tekstin_leveys;
}*/
///////// BMP-KUVATIEDOSTON LATAUSRUTIINIT //////////////////////////

int PisteDraw_Load_Bitmap(BITMAP_FILE_PTR bitmap, char *filename) {
    int file_handle, index;

    unsigned char *temp_buffer = NULL;
    OFSTRUCT file_data;

    if ((file_handle = OpenFile(filename, &file_data, OF_READ)) == -1)
        return 1;

    _lread(file_handle, &bitmap->bitmapfileheader, sizeof(BITMAPFILEHEADER));

    if (bitmap->bitmapfileheader.bfType != BITMAP_ID) {
        _lclose(file_handle);
        return 2;
    }

    _lread(file_handle, &bitmap->bitmapinfoheader, sizeof(BITMAPINFOHEADER));

    if (bitmap->bitmapinfoheader.biBitCount == 8) {
        _lread(file_handle, &bitmap->paletti, PD_max_varit * sizeof(PALETTEENTRY));

        for (index = 0; index < PD_max_varit; index++) {
            // reverse the red and green fields
            int temp_color = bitmap->paletti[index].peRed;
            bitmap->paletti[index].peRed = bitmap->paletti[index].peBlue;
            bitmap->paletti[index].peBlue = temp_color;

            bitmap->paletti[index].peFlags = PC_NOCOLLAPSE;
        }

    }

    _lseek(file_handle, -(int) (bitmap->bitmapinfoheader.biSizeImage), SEEK_END);

    if (bitmap->bitmapinfoheader.biBitCount == 8 || bitmap->bitmapinfoheader.biBitCount == 16 ||
        bitmap->bitmapinfoheader.biBitCount == 24) {

        if (!(bitmap->buffer = (unsigned char *) malloc(bitmap->bitmapinfoheader.biSizeImage))) {
            _lclose(file_handle);

            return 3;
        }

        _lread(file_handle, bitmap->buffer, bitmap->bitmapinfoheader.biSizeImage);

    } else
        return 4;

    _lclose(file_handle);

    PisteDraw_Flip_Bitmap(bitmap->buffer, bitmap->bitmapinfoheader.biWidth * (bitmap->bitmapinfoheader.biBitCount / 8),
                          bitmap->bitmapinfoheader.biHeight);
    return 0;
}


int PisteDraw_Unload_Bitmap_File(BITMAP_FILE_PTR bitmap) {
    if (bitmap->buffer) {
        free(bitmap->buffer);

        bitmap->buffer = NULL;
    }
    return 0;
}


int PisteDraw_Flip_Bitmap(unsigned char *image, int bytes_per_line, int height) {
    unsigned char *buffer;
    int index;

    if (!(buffer = (unsigned char *) malloc(bytes_per_line * height)))
        return 1;

    memcpy(buffer, image, bytes_per_line * height);

    for (index = 0; index < height; index++)
        memcpy(&image[((height - 1) - index) * bytes_per_line], &buffer[index * bytes_per_line], bytes_per_line);

    free(buffer);

    return 0;
}

// DirectDraw pinnan luonti

LPDIRECTDRAWSURFACE4 PisteDraw_Create_Surface(int width, int height, int mem_flags, unsigned char color) {
    LPDIRECTDRAWSURFACE4 lpdds;

    DD_INIT_STRUCT(PD_ddsd);

    PD_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

    PD_ddsd.dwWidth = width;
    PD_ddsd.dwHeight = height;

    PD_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

    if (FAILED(PD_lpdd->CreateSurface(&PD_ddsd, &lpdds, NULL)))
        return (NULL);

    DDCOLORKEY color_key;
    color_key.dwColorSpaceLowValue = color;
    color_key.dwColorSpaceHighValue = color;

    if (FAILED(lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key)))
        return (NULL);

    return (lpdds);
}

// PCX-Kuvan charge_time ///////////////////////////////////////////////////////////7

int PisteDraw_Lataa_PCX(char *filename, int buffer_index, bool lataa_paletti) {
    int xsize, ysize, tavu1, tavu2, position = 0;
    FILE *handle = fopen(filename, "rb");

    if (handle == NULL) {
        return PD_ERROR;
    }

    unsigned char *buffer = NULL;
    unsigned long ruudun_leveys;

    if (PisteDraw_Draw_Begin(buffer_index, *&buffer, (unsigned long &) ruudun_leveys) == PD_ERROR)
        return PD_ERROR;

    fseek(handle, 8, SEEK_SET);

    xsize = fgetc(handle) + (fgetc(handle) << 8) + 1;

    ysize = fgetc(handle) + (fgetc(handle) << 8) + 1;

    fseek(handle, 128, SEEK_SET);

    while (position < xsize * ysize) {
        tavu1 = fgetc(handle);

        if (tavu1 > 192) {
            tavu2 = fgetc(handle);

            for (; tavu1 > 192; tavu1--)
                buffer[position++] = tavu2;

        } else
            buffer[position++] = tavu1;
    }
    fclose(handle);

    if (PisteDraw_Draw_End(buffer_index) == PD_ERROR)
        return PD_ERROR;

    if (lataa_paletti)
        if (PisteDraw_Lataa_PCX_Paletti(filename) == PD_ERROR)
            return PD_ERROR;

    return 0;
}

int PisteDraw_Lataa_PCX_Paletti(char *filename) {
    FILE *handle = fopen(filename, "rb");
    char paletti[3 * 256];
    int c;

    if (handle == NULL) {
        return PD_ERROR;
    }

    fseek(handle, -768, SEEK_END);

    for (c = 0; c < 256 * 3; c++)
        paletti[c] = fgetc(handle) / 4;

    fclose(handle);

    return 0;
}
