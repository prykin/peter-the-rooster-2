/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#define INITGUID

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include "editor.h"

struct TOOL_LIST {
    bool nakyva;
    int x, y, valinta;
};

struct MENU {
    char otsikko[30];
    int x;
    int y;
    int leveys;
    int korkeus;
    bool piilota;
    int pika;
    TOOL_LIST lista;
};

struct TIEDOSTO {
    char nimi[_MAX_PATH];
    unsigned char tyyppi;
};

struct LEIKEPOYTA {
    unsigned char taustat[MAP_SIZE];
    unsigned char seinat[MAP_SIZE];
    unsigned char spritet[MAP_SIZE];
    RECT alue;
};

struct LOKIMERKINTA {
    char teksti[200];
    unsigned char tyyppi;
};

/* M��RITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define    GAME_NAME          "PK2 LEVEL EDITOR"

#define TAUSTAKUVA_EI        0
#define TAUSTAKUVA_METSA    1

#define MUSIIKKI_EI            0

const int SCREEN_WIDTH = 1024;//1024;
const int SCREEN_HEIGHT = 768;//768;
const int SCREEN_BPP = 8;
const int MAX_COLORS_PALETTE = 256;

const unsigned char MENU_EI = 100;
const unsigned char MENU_KARTTA = 0;
const unsigned char MENU_PALIKAT = 1;
const unsigned char MENU_INFO = 2;
const unsigned char MENU_SPRITET = 3;
const unsigned char MENU_TIEDOSTOT = 4;
const unsigned char MENU_SAVE = 5;
const unsigned char MENU_HELP = 6;
const unsigned char MENU_EXIT = 7;
const unsigned char MENU_TOOLS = 8;
const unsigned char MENU_LOKI = 9;

const unsigned char MAX_MENUJA = 10;

const unsigned char EDIT_KARTTA = 0;
const unsigned char EDIT_MENU = 1;
const unsigned char EDIT_SPRITE = 2;

const int MAX_DIR_TIEDOSTOJA = 1000;

const int TIEDOSTO_DIR = 1;
const int TIEDOSTO_MAP = 2;
const int TIEDOSTO_BMP = 3;
const int TIEDOSTO_SPR = 4;
const int TIEDOSTO_MUS = 5;

const int EDIT_KERROS_TAUSTA = 0;
const int EDIT_KERROS_SEINAT = 1;
const int EDIT_KERROS_VAIN_SEINAT = 2;

const int MAX_SPRITES = 600;

const int KENTTA_FILE = 1;
const int KENTTA_NIMI = 2;
const int KENTTA_TEKIJA = 3;
const int KENTTA_MUSIIKKI = 4;

const int MAX_LOKIMERKINTOJA = 55;
const unsigned char LOKI_INFO = 1;
const unsigned char LOKI_VIRHE = 2;

struct ASETUKSET {
    char tyohakemisto[_MAX_PATH];
    char epd_hakemisto[_MAX_PATH];
    char editor_hakemisto[_MAX_PATH];
    char bg_hakemisto[_MAX_PATH];
    char tile_hakemisto[_MAX_PATH];
    char karttatiedosto[_MAX_PATH];
    MENU menut[MAX_MENUJA];
    int kartta_x;
    int kartta_y;
};

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND window_handle = NULL; // p��ikkunan kahva
HINSTANCE hinstance_app = NULL; // hinstance?
HDC global_dc = NULL; // global dc?

bool DirectX_virhe = false;// jos t�m� muuttuu todeksi niin ohjelma lopetetaan
char virheviesti[60];

bool window_closed = false;// onko ikkuna kiinni

double cos_table[360];
double sin_table[360];
int degree = 0;

unsigned char edit_tila = EDIT_KARTTA;
unsigned char aktiivinen_menu = MENU_EI;

bool liikuta_ikkunaa = false;

bool editoi_tekstia = false;

char message[60];
char working_directory[_MAX_PATH];
char pk2_hakemisto[_MAX_PATH] = " ";
char epd_hakemisto[_MAX_PATH] = " ";
char editor_hakemisto[_MAX_PATH] = "";
char bg_hakemisto[_MAX_PATH] = " ";
char tile_hakemisto[_MAX_PATH] = " ";
TIEDOSTO tiedostot[MAX_DIR_TIEDOSTOJA];

char karttatiedosto[_MAX_PATH];

unsigned long tiedostoja = 0;

bool nayta_hakemistot = true;

PK2Map *map, *undo;
bool varmistettu = false;

RECT kartta_kohde = {0, 0, 0, 0};
int kartta_kohde_status = 0;
RECT kartta_lahde = {0, 0, 0, 0};

// KUVABUFFERIT
int kuva_editori;
int kuva_kartta;

// FONTIT
int font1;
int font2;

// KONTROLLIT
int mouse_x = 10;
int mouse_y = 10;
int key_delay = 0;

// MENUT
MENU menut[MAX_MENUJA];
int menu_palikat_nayta_y = 0;
int menu_tiedostot_nayta = 1;
int menu_tiedostot_eka = 0;
int menu_spritet_eka = 0;

// KARTTA
int kartta_x = 0;
int kartta_y = 0;
int edit_palikka = 0;
int edit_kerros = EDIT_KERROS_SEINAT;

int focustile_etu = 255;
int focustile_taka = 255;
int focussprite = 255;

bool kartta_ladattu = false;

int block_animation = 0;

bool animaatio_paalla = true;

LEIKEPOYTA leikepoyta;
bool aseta_leikepoyta_alue = false;
RECT leikepoyta_alue = {0, 0, 0, 0};
TOOL_LIST leikepoyta_lista;

PK2Sprite_Prototype prototypes[MAX_PROTOTYPES];
int next_free_proto = 0;
//int vakio_spriteja = 0;
//PK2Sprite sprites[MAX_SPRITES];
int spriteja = 0;

int proto_valittu = MAX_PROTOTYPES;

// TEKSTINK�SITTELY
int editKenttaId = 0;
char editTeksti[500];
int editKursori = 0;
char editMerkki = ' ';


// LOKI

LOKIMERKINTA loki[MAX_LOKIMERKINTOJA];

bool kirjoita_loki = false;

unsigned char karttavarit[150];

// ASETUKSET

ASETUKSET settings;

/* PROTOT ---------------------------------------------------------------------------------------*/

int Level_Editor_Loki_Tallenna(char *viesti);

int Level_Editor_Piirra_Nelio(int vasen, int yla, int oikea, int ala, unsigned char color);

int Level_Editor_Kartta_Paivita(void);

void Level_Editor_Laske_TileVarit();

void Level_Editor_Aseta_Palikat(char *filename);

int Level_Editor_Kartta_Oletusasetukset();

int Level_Editor_Kartta_Lataa(char *filename);


/* EDITORI ---------------------------------------------------------------------------------------*/

bool exit_editor = false;
bool piirto_aloitettu = false;

/* ALUSTUKSET ---------------------------------------------------------------*/


void Level_Editor_Loki_Alusta() {
    for (int i = 0; i < MAX_LOKIMERKINTOJA; i++) {
        loki[i].tyyppi = 0;
        strcpy(loki[i].teksti, " ");
    }
}

void Level_Editor_Loki_Kirjaa(char *teksti, unsigned char tyyppi) {
    strcpy(message, teksti);

    for (int i = 0; i < MAX_LOKIMERKINTOJA - 1; i++) {
        strcpy(loki[i].teksti, loki[i + 1].teksti);
        loki[i].tyyppi = loki[i + 1].tyyppi;

    }
    loki[MAX_LOKIMERKINTOJA - 1].tyyppi = tyyppi;
    strcpy(loki[MAX_LOKIMERKINTOJA - 1].teksti, teksti);
}

int Level_Editor_Asetukset_Lataa(void) {
    int i;

    char tiedostonimi[_MAX_PATH];

    strcpy(tiedostonimi, editor_hakemisto);
    strcat(tiedostonimi, "//settings.dat");

    Level_Editor_Loki_Kirjaa("loading settings...", LOKI_INFO);

    ifstream *tiedosto = new ifstream(tiedostonimi, ios::in);

    if (tiedosto->fail()) {
        delete (tiedosto);
        Level_Editor_Loki_Kirjaa("no settings saved.", LOKI_INFO);
        return 1;
    }

    tiedosto->read((char *) &settings, sizeof(settings));

    if (tiedosto->fail()) {
        delete (tiedosto);
        strcpy(message, "could not read settings.dat");
        Level_Editor_Loki_Kirjaa("loading settings failed.", LOKI_VIRHE);
        return 1;
    }

    delete (tiedosto);

    strcpy(bg_hakemisto, settings.bg_hakemisto);
    strcpy(epd_hakemisto, settings.epd_hakemisto);
    strcpy(karttatiedosto, settings.karttatiedosto);
    strcpy(tile_hakemisto, settings.tile_hakemisto);
    strcpy(working_directory, settings.tyohakemisto);

    for (i = 0; i < MAX_MENUJA; i++) {
        menut[i].x = settings.menut[i].x;
        menut[i].y = settings.menut[i].y;
        menut[i].piilota = settings.menut[i].piilota;
    }

    kartta_x = settings.kartta_x;
    kartta_y = settings.kartta_y;

    if (_chdir(working_directory) == 0)
        _getcwd(working_directory, _MAX_PATH);

    Level_Editor_Kartta_Lataa(karttatiedosto);

    mouse_x = 250;
    mouse_y = 250;

    return 0;
}

void Level_Editor_Asetukset_Tallenna(void) {
    int i;

    Level_Editor_Loki_Kirjaa("saving settings", LOKI_INFO);

    strcpy(settings.bg_hakemisto, bg_hakemisto);
    strcpy(settings.editor_hakemisto, bg_hakemisto);
    strcpy(settings.epd_hakemisto, epd_hakemisto);
    strcpy(settings.karttatiedosto, karttatiedosto);
    strcpy(settings.tile_hakemisto, tile_hakemisto);
    strcpy(settings.tyohakemisto, working_directory);

    for (i = 0; i < MAX_MENUJA; i++) {
        settings.menut[i].x = menut[i].x;
        settings.menut[i].y = menut[i].y;
        settings.menut[i].piilota = menut[i].piilota;
    }

    settings.menut[MENU_EXIT].piilota = true;

    settings.kartta_x = kartta_x;
    settings.kartta_y = kartta_y;

    char tiedostonimi[_MAX_PATH];

    strcpy(tiedostonimi, editor_hakemisto);
    strcat(tiedostonimi, "//settings.dat");

    ofstream *tiedosto = new ofstream(tiedostonimi, ios::binary);
    tiedosto->write((char *) &settings, sizeof(settings));

    delete (tiedosto);
}

// LEIKEP�YT� ///////////////////////////////////////////////

void Level_Editor_Leikepoyta_Alusta(void) {
    memset(leikepoyta.seinat, 0, sizeof(leikepoyta.seinat));
    memset(leikepoyta.taustat, 0, sizeof(leikepoyta.taustat));
    memset(leikepoyta.spritet, 0, sizeof(leikepoyta.spritet));

    leikepoyta.alue.left = 0;
    leikepoyta.alue.right = 0;
    leikepoyta.alue.top = 0;
    leikepoyta.alue.bottom = 0;
}

void Level_Editor_Leikepoyta_Kopioi(RECT alue) {
    Level_Editor_Leikepoyta_Alusta();

    if (alue.right > MAP_WIDTH)
        alue.right = MAP_WIDTH;

    if (alue.left < 0)
        alue.left = 0;

    if (alue.bottom > MAP_HEIGHT)
        alue.bottom = MAP_HEIGHT;

    if (alue.top < 0)
        alue.top = 0;

    leikepoyta.alue.left = 0;
    leikepoyta.alue.top = 0;
    leikepoyta.alue.right = alue.right - alue.left;
    leikepoyta.alue.bottom = alue.bottom - alue.top;

    for (int x = alue.left; x < alue.right; x++)
        for (int y = alue.top; y < alue.bottom; y++) {
            leikepoyta.taustat[x - alue.left + (y - alue.top) * MAP_WIDTH] = map->backgrounds[x + y *
                                                                                                  MAP_WIDTH];
            leikepoyta.seinat[x - alue.left + (y - alue.top) * MAP_WIDTH] = map->walls[x + y *
                                                                                           MAP_WIDTH];
            leikepoyta.spritet[x - alue.left + (y - alue.top) * MAP_WIDTH] = map->sprites[x + y *
                                                                                              MAP_WIDTH];
        }

    Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Liita(RECT alue) {
    //alue.left = x;
    //alue.top  = y;
    //alue.right = x + leikepoyta.alue.right;
    //alue.bottom = y + leikepoyta.alue.bottom;
    //Level_Editor_Leikepoyta_Alusta();

    if (alue.right > MAP_WIDTH)
        alue.right = MAP_WIDTH;

    if (alue.left < 0)
        alue.left = 0;

    if (alue.bottom > MAP_HEIGHT)
        alue.bottom = MAP_HEIGHT;

    if (alue.top < 0)
        alue.top = 0;

    int lp_leveys = leikepoyta.alue.right - leikepoyta.alue.left, lp_korkeus =
            leikepoyta.alue.bottom - leikepoyta.alue.top;

    if (lp_leveys > 0 && lp_korkeus > 0) {
        for (int x = alue.left; x < alue.right; x++)
            for (int y = alue.top; y < alue.bottom; y++) {
                //map->sprites[x+y*MAP_WIDTH] = leikepoyta.sprites[(x-alue.left)%lp_leveys +((y-alue.top)%lp_korkeus)*MAP_WIDTH];
                if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT)
                    map->backgrounds[x + y * MAP_WIDTH] = leikepoyta.taustat[(x - alue.left) % lp_leveys +
                                                                             ((y - alue.top) % lp_korkeus) *
                                                                             MAP_WIDTH];
                if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
                    map->walls[x + y * MAP_WIDTH] = leikepoyta.seinat[(x - alue.left) % lp_leveys +
                                                                      ((y - alue.top) % lp_korkeus) *
                                                                      MAP_WIDTH];
            }
    }

    Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Liita_Koko(RECT alue) {

    alue.right = alue.left + leikepoyta.alue.right - leikepoyta.alue.left;
    alue.bottom = alue.top + leikepoyta.alue.bottom - leikepoyta.alue.top;

    if (alue.right > MAP_WIDTH)
        alue.right = MAP_WIDTH;

    if (alue.left < 0)
        alue.left = 0;

    if (alue.bottom > MAP_HEIGHT)
        alue.bottom = MAP_HEIGHT;

    if (alue.top < 0)
        alue.top = 0;

    int lp_leveys = leikepoyta.alue.right - leikepoyta.alue.left, lp_korkeus =
            leikepoyta.alue.bottom - leikepoyta.alue.top;

    if (lp_leveys > 0 && lp_korkeus > 0) {
        for (int x = alue.left; x < alue.right; x++)
            for (int y = alue.top; y < alue.bottom; y++) {
                //map->sprites[x+y*MAP_WIDTH]     = leikepoyta.sprites[(x-alue.left) +(y-alue.top)*MAP_WIDTH];
                if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT)
                    map->backgrounds[x + y * MAP_WIDTH] = leikepoyta.taustat[(x - alue.left) +
                                                                             (y - alue.top) *
                                                                             MAP_WIDTH];
                if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
                    map->walls[x + y * MAP_WIDTH] = leikepoyta.seinat[(x - alue.left) + (y - alue.top) *
                                                                                        MAP_WIDTH];
            }
    }

    Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Leikkaa(RECT alue) {
    Level_Editor_Leikepoyta_Alusta();

    if (alue.right > MAP_WIDTH)
        alue.right = MAP_WIDTH;

    if (alue.left < 0)
        alue.left = 0;

    if (alue.bottom > MAP_HEIGHT)
        alue.bottom = MAP_HEIGHT;

    if (alue.top < 0)
        alue.top = 0;

    leikepoyta.alue.left = 0;
    leikepoyta.alue.top = 0;
    leikepoyta.alue.right = alue.right - alue.left;
    leikepoyta.alue.bottom = alue.bottom - alue.top;

    for (int x = alue.left; x < alue.right; x++)
        for (int y = alue.top; y < alue.bottom; y++) {
            if (edit_kerros == EDIT_KERROS_TAUSTA || edit_kerros == EDIT_KERROS_SEINAT) {
                leikepoyta.taustat[x - alue.left + (y - alue.top) * MAP_WIDTH] = map->backgrounds[x + y *
                                                                                                      MAP_WIDTH];
                map->backgrounds[x + y * MAP_WIDTH] = 255;
            }
            if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT) {
                leikepoyta.seinat[x - alue.left + (y - alue.top) * MAP_WIDTH] = map->walls[x + y *
                                                                                               MAP_WIDTH];
                map->walls[x + y * MAP_WIDTH] = 255;
            }

            //leikepoyta.sprites[x-alue.left+(y-alue.top)*MAP_WIDTH] = map->sprites[x+y*MAP_WIDTH];
            //map->sprites[x+y*MAP_WIDTH] = 255;
        }

    Level_Editor_Kartta_Paivita();
}

void Level_Editor_Leikepoyta_Piirra() {
    //Level_Editor_Piirra_Nelio(leikepoyta.alue.left *32-kartta_x,leikepoyta.alue.top   *32-kartta_y,
    //						  leikepoyta.alue.right*32-kartta_x,leikepoyta.alue.bottom*32-kartta_y,28);

//	Level_Editor_Piirra_Nelio((leikepoyta_alue.left-kartta_x) *32,(leikepoyta_alue.top-kartta_y)  *32,
//							  (leikepoyta_alue.right-kartta_x)*32,(leikepoyta_alue.bottom-kartta_y)*32,224);

    Level_Editor_Piirra_Nelio((leikepoyta_alue.left - kartta_x) * 32 - 1, (leikepoyta_alue.top - kartta_y) * 32 - 1,
                              (leikepoyta_alue.right - kartta_x) * 32 + 1, (leikepoyta_alue.bottom - kartta_y) * 32 + 1,
                              0);

    Level_Editor_Piirra_Nelio((leikepoyta_alue.left - kartta_x) * 32, (leikepoyta_alue.top - kartta_y) * 32,
                              (leikepoyta_alue.right - kartta_x) * 32, (leikepoyta_alue.bottom - kartta_y) * 32, 31);

    Level_Editor_Piirra_Nelio((leikepoyta_alue.left - kartta_x) * 32 + 1, (leikepoyta_alue.top - kartta_y) * 32 + 1,
                              (leikepoyta_alue.right - kartta_x) * 32 - 1, (leikepoyta_alue.bottom - kartta_y) * 32 - 1,
                              0);
}


/*
void Level_Editor_Prototyyppi_Poista(int proto)
{
	prototypes[proto].New();
	strcpy(map->prototypes[proto],"");

	for (unsigned long x=0; x<MAP_SIZE; x++)
	{
		if (map->sprites[x] == proto)
			map->sprites[x] = 255;
	}
}
*/

int Level_Editor_Aseta_Episodihakemisto() {
    _getcwd(epd_hakemisto, _MAX_PATH);
    //strcpy(epd_hakemisto, "1234");
    //strcpy(message, "1234");

    return 0;
}

int Level_Editor_Lataa_PK2_Hakemisto(void) {
    Level_Editor_Loki_Kirjaa("loading pk2le_dir.ini", LOKI_INFO);

    char hakemisto[_MAX_PATH];
    ifstream *tiedosto = new ifstream("pk2le_dir.ini", ios::in);

    if (tiedosto->fail()) {
        delete (tiedosto);
        Level_Editor_Loki_Kirjaa("no pk2le_dir.ini found.", LOKI_VIRHE);
        return 1;
    }

    tiedosto->read(hakemisto, 255);

    if (tiedosto->fail()) {
        delete (tiedosto);
        strcpy(message, "could not read pk2_dir.ini");
        Level_Editor_Loki_Kirjaa("loading pk2le_dir.ini failed.", LOKI_VIRHE);
        return 1;
    }

    delete (tiedosto);

    strcpy(pk2_hakemisto, hakemisto);

    strcpy(PK2Map::pk2_directory, hakemisto);
    strcat(PK2Map::pk2_directory, "\\");

    strcpy(bg_hakemisto, PK2Map::pk2_directory);
    strcat(bg_hakemisto, "\\gfx\\scenery\\");

    strcpy(tile_hakemisto, PK2Map::pk2_directory);
    strcat(tile_hakemisto, "\\gfx\\tiles\\");

    if (_chdir(pk2_hakemisto) == 0)
        _getcwd(working_directory, _MAX_PATH);

    return 0;
}

int Level_Editor_Tallenna_PK2_Hakemisto(char *hakemisto) {
    char* tal = new char[_MAX_DIR];
    strcpy(tal, editor_hakemisto);
    strcat(tal, "//pk2le_dir.ini");
    Level_Editor_Loki_Kirjaa("saving pk2le_dir.ini.", LOKI_INFO);

    if (strcmp(pk2_hakemisto, " ") == 0) {
        ofstream *tiedosto = new ofstream(tal, ios::out);
        tiedosto->write(hakemisto, 255);

        tiedosto->close();

        if (tiedosto->fail()) {
            delete (tiedosto);
            Level_Editor_Loki_Kirjaa("failed to save pk2_dir.ini.", LOKI_VIRHE);
            return 1;
        }

        delete (tiedosto);
        strcpy(pk2_hakemisto, hakemisto);

        strcpy(PK2Map::pk2_directory, hakemisto);
        strcat(PK2Map::pk2_directory, "\\");

        strcpy(bg_hakemisto, PK2Map::pk2_directory);
        strcat(bg_hakemisto, "\\gfx\\scenery\\");

        strcpy(tile_hakemisto, PK2Map::pk2_directory);
        strcat(tile_hakemisto, "\\gfx\\tiles\\");

        if (_chdir(pk2_hakemisto) == 0)
            _getcwd(working_directory, _MAX_PATH);

        //Level_Editor_Lataa_PK2_Hakemisto();
        strcpy(message, "pk2 directory found");

        Level_Editor_Kartta_Oletusasetukset();
        menut[MENU_PALIKAT].piilota = false;
        menut[MENU_SPRITET].piilota = false;
    }

    return 0;
}

void Level_Editor_Laske_Spritet(void) {
    int proto;

    spriteja = 0;

    for (int x = 0; x < MAP_SIZE; x++) {
        proto = map->sprites[x];
        if (proto != 255)
            spriteja++;
    }

    Level_Editor_Loki_Tallenna("Spritejen maara laskettu. \n");
}

void Level_Editor_Prototyyppi_Poista(int proto) {

    if (strcmp(prototypes[proto].name, "") != 0) {
        Level_Editor_Loki_Tallenna("Poistetaan proto ");
        Level_Editor_Loki_Tallenna(prototypes[proto].name);
        Level_Editor_Loki_Tallenna("\n");
    }

    prototypes[proto].New();
    strcpy(map->protos[proto], "");

    if (next_free_proto > 0)
        next_free_proto--;

    if (map->player_sprite > proto)
        map->player_sprite--;

    if (proto < MAX_PROTOTYPES - 1) {
        for (int i = proto; i < MAX_PROTOTYPES; i++) {
            strcpy(map->protos[i], map->protos[i + 1]);
            prototypes[i].Copy(prototypes[i + 1]);
        }

        strcpy(map->protos[MAX_PROTOTYPES - 1], "");
        prototypes[MAX_PROTOTYPES - 1].New();

        for (unsigned long x = 0; x < MAP_SIZE; x++) {
            if (map->sprites[x] == proto)
                map->sprites[x] = 255;

            if (map->sprites[x] > proto && map->sprites[x] != 255)
                map->sprites[x]--;
        }
    }

    int i = MAX_PROTOTYPES - 1;
    while (strcmp(prototypes[i].name, "") == 0 && i >= 0)
        i--;

    if (i != -1)
        next_free_proto = i + 1;
}

void Level_Editor_Prototyyppi_Poista_Tyhjat() {
    Level_Editor_Loki_Kirjaa("deleting empty sprites.", LOKI_INFO);
    for (int i = 0; i < MAX_PROTOTYPES; i++) {
        if (strcmp(prototypes[i].name, "") == 0)
            Level_Editor_Prototyyppi_Poista(i);
    }
}

void Level_Editor_Prototyyppi_Tyhjenna() {
    Level_Editor_Loki_Kirjaa("deleting all sprites.", LOKI_INFO);
    //Level_Editor_Loki_Tallenna("Tyhjennetaan kaikki prototyypit.\n");

    for (int i = 0; i < MAX_PROTOTYPES; i++) {
        Level_Editor_Loki_Tallenna("Poistetaan proto ");
        Level_Editor_Loki_Tallenna(prototypes[i].name);
        Level_Editor_Loki_Tallenna("\n");
        //Level_Editor_Prototyyppi_Poista(i);
        prototypes[i].New();
        strcpy(map->protos[i], "");
    }

    next_free_proto = 0;
}

int Level_Editor_Prototyyppi_Laske_Lkm(int proto) {
    int lkm = 0;

    for (unsigned long x = 0; x < MAP_SIZE; x++) {

        if (map->sprites[x] == proto)
            lkm++;
    }

    return lkm;
}

void Level_Editor_Prototyyppi_Poista_Turhat() {
    int i = 0;

    Level_Editor_Loki_Kirjaa("deleting unused sprites:", LOKI_INFO);
    //Level_Editor_Loki_Tallenna("Tyhjennetaan kaikki prototyypit.\n");

    //for (int i=0; i<MAX_PROTOTYPES; i++)	{
    while (i < MAX_PROTOTYPES) {
        if (strcmp(prototypes[i].name, "") != 0) {
            if (Level_Editor_Prototyyppi_Laske_Lkm(i) == 0 && i != map->player_sprite) {
                Level_Editor_Loki_Tallenna("Poistetaan proto ");
                Level_Editor_Loki_Tallenna(prototypes[i].name);
                Level_Editor_Loki_Tallenna("\n");
                Level_Editor_Loki_Kirjaa(prototypes[i].name, LOKI_INFO);
                Level_Editor_Prototyyppi_Poista(i);
            } else
                i++;
        } else
            i++;
    }

    next_free_proto = 0;
}
/*
int PK_Prototyyppi_Lataa_VakioSprite(char *polku, char *file)
{
	if (next_free_proto < MAX_PROTOTYPES)
	{
		if (prototypes[next_free_proto].Load(polku, file) == 1)
			return 1;
	}
	else
		return 1;

	next_free_proto++;
	vakio_spriteja++;

	return 0;
}
*/
// Ladataan kartan mukana tulevat sprites
int Level_Editor_Prototyyppi_Lataa_Vanha(char *polku, char *tiedosto) {
    char msg[200];
    strcpy(msg, "loading sprite: ");
    strcat(msg, tiedosto);
    Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);

    //Level_Editor_Loki_Kirjaa("loading sprite.", LOKI_INFO);
    Level_Editor_Loki_Tallenna("Ladataan vanha prototyyppi ");
    Level_Editor_Loki_Tallenna(tiedosto);
    Level_Editor_Loki_Tallenna("\n");

    char tiedostopolku[255];

    strcpy(tiedostopolku, polku);
    strcat(tiedostopolku, "\\");


    if (next_free_proto < MAX_PROTOTYPES) {
        if (prototypes[next_free_proto].Load(tiedostopolku, tiedosto) == 1) {
            strcpy(message, "could not load sprite ");
            strcat(message, tiedosto);
            Level_Editor_Loki_Kirjaa(message, LOKI_VIRHE);

            prototypes[next_free_proto].New();
            strcpy(prototypes[next_free_proto].name, "loading error!");
            strcpy(prototypes[next_free_proto].file, tiedosto);
            prototypes[next_free_proto].height = 10;
            prototypes[next_free_proto].width = 10;
            prototypes[next_free_proto].frame_width = 10;
            prototypes[next_free_proto].frame_height = 10;

            next_free_proto++;

            return 1;
        }
    } else
        return 1;

    next_free_proto++;

    return 0;
}
/*
// Tarvitaan jotta my�s yhteysspritet tulisi lis�tty� kertan tietoihin
int Level_Editor_Prototyyppi_Lataa_Yhteyssprite(char *polku, char *file)
{

	char tiedostopolku[255];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	if (next_free_proto < MAX_PROTOTYPES)
	{
		if (prototypes[next_free_proto].Load(tiedostopolku, file) == 1)
		{
			strcpy(message,"could not load ");
			strcat(message,tiedostopolku);
			strcat(message,file);
			return 1;
		}

		strcpy(map->prototypes[next_free_proto],file);
	}
	else
		return 2;

	next_free_proto++;

	return 0;
}

void Level_Editor_Prototyyppi_Aseta_Muutos_Sprite(int i, char *tiedostopolku)
{
	int j = 0;
	bool loytyi = false;

	if (strcmp(prototypes[i].change_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYPES && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(prototypes[i].change_sprite,prototypes[j].file)==0)
				{
					prototypes[i].transformation = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi)
		{
			char polku[255];
			strcpy(polku,tiedostopolku);// sprites//

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, prototypes[i].change_sprite)==0)
				prototypes[i].transformation = next_free_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Bonus_Sprite(int i, char *tiedostopolku)
{
	int j = 0;
	bool loytyi = false;

	if (strcmp(prototypes[i].bonus_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYPES && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(prototypes[i].bonus_sprite,prototypes[j].file)==0)
				{
					prototypes[i].bonus = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi)
		{
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, prototypes[i].bonus_sprite)==0)
				prototypes[i].bonus = next_free_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Ammus1_Sprite(int i, char *tiedostopolku)
{
	int j = 0;
	bool loytyi = false;

	if (strcmp(prototypes[i].projectile1_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYPES && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(prototypes[i].projectile1_sprite,prototypes[j].file)==0)
				{
					prototypes[i].projectile_1 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi)
		{
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, prototypes[i].projectile1_sprite)==0)
				prototypes[i].projectile_1 = next_free_proto-1;
		}
	}
}

void Level_Editor_Prototyyppi_Aseta_Ammus2_Sprite(int i, char *tiedostopolku)
{
	int j = 0;
	bool loytyi = false;

	if (strcmp(prototypes[i].projectile2_sprite,"")!=0)
	{
		while (j<MAX_PROTOTYPES && !loytyi)
		{
			if (j!=i)
			{
				if (strcmp(prototypes[i].projectile2_sprite,prototypes[j].file)==0)
				{
					prototypes[i].projectile_2 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi)
		{
			char polku[255];
			strcpy(polku,tiedostopolku);

			if (Level_Editor_Prototyyppi_Lataa_Yhteyssprite(polku, prototypes[i].projectile2_sprite)==0)
				prototypes[i].projectile_2 = next_free_proto-1;
		}
	}
}
*/
/*
int Level_Editor_Prototyyppi_Lataa_Uusi(char *polku, char *file)
{

	char tiedostopolku[255];

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");


	if (next_free_proto < MAX_PROTOTYPES)
	{
		if (prototypes[next_free_proto].Load(tiedostopolku, file) == 1)
		{
			strcpy(message,"could not load ");
			strcat(message,tiedostopolku);
			strcat(message,file);
			return 1;
		}

		strcpy(map->prototypes[next_free_proto],file);
		strcpy(message,"loaded sprite ");
		strcat(message,map->prototypes[next_free_proto]);
	}
	else
	{
		strcpy(message,"could not load any more sprites");
		return 2;
	}

	strcpy(tiedostopolku,polku);
	strcat(tiedostopolku,"\\");

	next_free_proto++;

	return 0;
}*/

int Level_Editor_Prototyyppi_Lataa_Uusi(char *polku, char *tiedosto) {
    char msg[200];
    strcpy(msg, "loading new sprite: ");
    //strcat(msg,polku);
    //strcat(msg,"\\");
    strcat(msg, tiedosto);
    Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);

    //Level_Editor_Loki_Kirjaa("loading new sprite.", LOKI_INFO);
    Level_Editor_Loki_Tallenna("Ladataan uusi prototyyppi ");
    Level_Editor_Loki_Tallenna(tiedosto);
    Level_Editor_Loki_Tallenna("\n");

    char tiedostopolku[_MAX_PATH];

    strcpy(tiedostopolku, polku);
    strcat(tiedostopolku, "\\");


    if (next_free_proto < MAX_PROTOTYPES) {
        if (prototypes[next_free_proto].Load(tiedostopolku, tiedosto) == 1) {
            Level_Editor_Loki_Kirjaa("loading sprite failed.", LOKI_VIRHE);
            //strcpy(message,"could not load ");
            //strcat(message,tiedostopolku);
            //strcat(message,file);
            return 1;
        }

        strcpy(map->protos[next_free_proto], tiedosto);
        strcpy(message, "loaded sprite ");
        strcat(message, map->protos[next_free_proto]);
    } else {
        Level_Editor_Loki_Kirjaa("too many sprites to load.", LOKI_VIRHE);
        strcpy(message, "could not load any more sprites");
        return 2;
    }

    strcpy(tiedostopolku, polku);
    strcat(tiedostopolku, "\\");

    next_free_proto++;

    return 0;
}

int Level_Editor_Prototyyppi_Lataa_Kaikki() {
    char tiedosto[_MAX_PATH];
    int spriteja = 0;
    int viimeinen_proto = 0;

    Level_Editor_Loki_Kirjaa("loading all sprites.", LOKI_INFO);
    //strcpy(message,"map and all sprites loaded");

    for (int i = 0; i < MAX_PROTOTYPES; i++) {

        if (strcmp(map->protos[i], "") != 0) {
            viimeinen_proto = i;
            strcpy(tiedosto, pk2_hakemisto);
            strcat(tiedosto, "\\sprites\\");
            //strcat(file,map->prototypes[i]);
            spriteja++;

            if (Level_Editor_Prototyyppi_Lataa_Vanha(tiedosto, map->protos[i]) == 1) {
                Level_Editor_Loki_Tallenna("charge_time ep�onnistui.\n");

                //next_free_proto++;

                //Level_Editor_Prototyyppi_Poista(i);
                //strcpy(map->prototypes[i],"");
                //next_free_proto++;
            } else
                Level_Editor_Loki_Tallenna("charge_time onnistui.\n");
        } else
            next_free_proto++;

    }

    next_free_proto = viimeinen_proto + 1;

    strcpy(tiedosto, "sprites\\");
/*
	for (i=0;i<MAX_PROTOTYPES;i++)
	{
		Level_Editor_Prototyyppi_Aseta_Muutos_Sprite(i, file);
		Level_Editor_Prototyyppi_Aseta_Bonus_Sprite( i, file);
		Level_Editor_Prototyyppi_Aseta_Ammus1_Sprite(i, file);
		Level_Editor_Prototyyppi_Aseta_Ammus2_Sprite(i, file);
	}*/

    if (spriteja == 0)
        strcpy(message, "map loaded without sprites");

    Level_Editor_Prototyyppi_Poista_Tyhjat();

    Level_Editor_Loki_Tallenna("Tyhjat prototyypit on poistettu");

    return 0;
}

void Level_Editor_Kartta_Varmista(void) {
    //Level_Editor_Loki_Kirjaa("making backup of map (undo)", LOKI_INFO);
    undo->Copy(*map);
    varmistettu = true;
}

void Level_Editor_Kartta_Undo(void) {
    if (varmistettu) {
        Level_Editor_Loki_Kirjaa("used undo", LOKI_INFO);
        strcpy(message, "used undo");
        map->Copy(*undo);
    } else
        strcpy(message, "nothing to undo");
}

int Level_Editor_Kartta_Paivita(void) {
    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    unsigned char vari_taka, vari_etu, vari, vari_sprite;

    PisteDraw_Draw_Begin(kuva_kartta, *&buffer, (unsigned long &) tod_leveys);

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            vari_taka = map->backgrounds[x + y * MAP_WIDTH];
            vari_etu = map->walls[x + y * MAP_WIDTH];
            vari_sprite = map->sprites[x + y * MAP_WIDTH];

            vari = 0;
/*
			if (vari_taka != 255)
				color = 12+128;//96;

			if (vari_taka >= 130 && vari_taka <= 139)
				color = 12+32;

			if (vari_etu != 255)
				color = 24+128;//96;

			if (vari_etu != 255)
				color = 24+96;

			if (vari_etu >= 130 && vari_etu <= 139)
				color = 20+32;*/

            if (vari_taka < 150)
                vari = karttavarit[vari_taka];

            if (vari_etu < 150 && vari_etu != 255 && vari_etu != BLOCK_RESTRICTION_DOWN)
                vari = karttavarit[vari_etu];

            if (vari_sprite != 255)
                vari = 231;

            buffer[x + y * tod_leveys] = vari;
        }
    }

    PisteDraw_Draw_End(kuva_kartta);

    Level_Editor_Laske_Spritet();

    return 0;
}

int Level_Editor_Kartta_Oletusasetukset() {
    char tiedosto[_MAX_PATH];

    Level_Editor_Loki_Kirjaa("loading map default settings.", LOKI_INFO);

    if (strcmp(tile_hakemisto, " ") != 0) {
        //memset(file,'\0',sizeof(file));
        //strcpy(file,tile_hakemisto);
        //strcat(file,"tiles01.bmp");
        strcpy(tiedosto, "tiles01.bmp");
        Level_Editor_Aseta_Palikat(tiedosto);

        //PisteDraw_Image_Load(map->block_buffer,file,false);
    }

    if (strcmp(pk2_hakemisto, " ") != 0) {
        //memset(file,'\0',sizeof(file));
        strcpy(tiedosto, pk2_hakemisto);
        strcat(tiedosto, "\\sprites");
        Level_Editor_Prototyyppi_Lataa_Uusi(tiedosto, "rooster.spr");

    }

    strcpy(map->music, "song01.xm");
    strcpy(karttatiedosto, "untitled.map");

    return 0;
}

int Level_Editor_Kartta_Alusta(void) {
    Level_Editor_Loki_Kirjaa("clearing map.", LOKI_INFO);

    map->Clear();

    PisteDraw_Buffer_Clear(map->background_image_buffer, 37);

    Level_Editor_Prototyyppi_Tyhjenna();

    Level_Editor_Kartta_Oletusasetukset();

    Level_Editor_Kartta_Paivita();

    Level_Editor_Loki_Kirjaa("map cleared.", LOKI_INFO);

    return 0;
}

int Level_Editor_Loki_Tallenna(char *viesti) {
    if (kirjoita_loki) {
        int virhe = 0;

        char *filename = "loki.txt";

        FILE *tiedosto;

        if ((tiedosto = fopen(filename, "movement_x")) == NULL) {
            return (1);
        }

        char mjono[255];

        //memset(mjono,' ',sizeof(mjono));
        //mjono[60] = '\n';

        strcpy(mjono, viesti);

        fwrite(mjono, sizeof(char), strlen(mjono), tiedosto);

        fclose(tiedosto);
    }

    return (0);
}

int Level_Editor_Kartta_Tallenna(void) {
    Level_Editor_Loki_Kirjaa("trying to save map...", LOKI_INFO);

    int virhe;

    char filename[_MAX_PATH];

    //strcpy(filename,map->name);
    strcpy(filename, karttatiedosto);

    if (strstr(filename, ".map") == NULL)
        strcat(filename, ".map");

    if ((virhe = map->Save(filename)) != 0) {
        if (virhe == 1) {
            Level_Editor_Loki_Kirjaa("could not save map!", LOKI_VIRHE);
            //strcpy(message,"could not save map!");
            return (1);
        }
        if (virhe == 2) {
            strcpy(message, "error saving map!");
            Level_Editor_Loki_Kirjaa("error occured while saving map!", LOKI_VIRHE);
            return (1);
        }
    }

    Level_Editor_Loki_Kirjaa("map saved succesfully!", LOKI_INFO);
    strcpy(message, "map saved.");

    return (0);
}

int Level_Editor_Kartta_Lataa(char *filename) {
    int virhe;

    char msg[200];
    strcpy(msg, "loading map: ");
    strcat(msg, filename);
    Level_Editor_Loki_Kirjaa("----------------------", LOKI_INFO);
    Level_Editor_Loki_Kirjaa(msg, LOKI_INFO);
    Level_Editor_Loki_Kirjaa("----------------------", LOKI_INFO);

    //Level_Editor_Loki_Kirjaa("trying to load map.", LOKI_INFO);

    unsigned long loki_muistia = PisteDraw_GetMemoryUsed();
    char muistia[255];
    itoa(loki_muistia, muistia, 10);

    Level_Editor_Loki_Tallenna("Videomuistia: ");
    Level_Editor_Loki_Tallenna(muistia);
    Level_Editor_Loki_Tallenna("\n");

    Level_Editor_Loki_Tallenna("Aloitetaan kartan ");
    Level_Editor_Loki_Tallenna(filename);
    Level_Editor_Loki_Tallenna(" charge_time.\n");

    Level_Editor_Kartta_Varmista();

    //Level_Editor_Loki_Tallenna("Kartta varmistettu (undo).\n");

    Level_Editor_Prototyyppi_Tyhjenna();

    //Level_Editor_Loki_Tallenna("Prototyypit tyhjennetty.\n");

    strcpy(karttatiedosto, filename);

    if ((virhe = map->Load("", filename)) != 0) {
        if (virhe == 1) {
            Level_Editor_Loki_Kirjaa("could not load map!", LOKI_VIRHE);
            //Level_Editor_Loki_Tallenna("Kartan charge_time ep�onnistui.\n");
            strcpy(karttatiedosto, " ");
            return (1);
        }

        if (virhe == 2) {
            Level_Editor_Loki_Kirjaa("this editor version cant read this map!", LOKI_VIRHE);
            return (1);
        }
    }

    Level_Editor_Loki_Kirjaa("map loaded", LOKI_INFO);
    //strcpy(message,"map loaded: ");

    strcpy(karttatiedosto, filename);

    Level_Editor_Kartta_Paivita();

    //Level_Editor_Loki_Tallenna("Kartta p�ivitetty.\n");


    kartta_ladattu = true;

    //Level_Editor_Loki_Tallenna("Kartta loaded.\n");

    if (strcmp(map->version, "1.2") == 0 || strcmp(map->version, "1.3") == 0)
        Level_Editor_Prototyyppi_Lataa_Kaikki();
    else
        Level_Editor_Prototyyppi_Tyhjenna();
/*
	char polku[_MAX_DIR];
	strcpy(polku,pk2_directory);
	strcat(polku,"\\");

	if (map->Lataa_PalikkaPaletti("",map->block_bmp)!=0)
	{
		if (map->Load_Block_Palette(polku,map->block_bmp)!=0)
		{
			strcpy(message,"could not find tiles ");
			strcat(message,map->block_bmp);
		}
	}

	if (map->Lataa_Taustakuva("",map->background_image)!=0)
	{
		if (map->Load_Background_Image(polku,map->background_image)!=0)
		{
			strcpy(message,"could not find background ");
			strcat(message,map->background_image);
		}
	}
*/
    Level_Editor_Loki_Tallenna("Sprite-prototyypit on loaded.\n");

    menu_spritet_eka = 0;

    Level_Editor_Laske_Spritet();

    Level_Editor_Laske_TileVarit();

    Level_Editor_Kartta_Paivita();

    Level_Editor_Loki_Kirjaa("loading map completed.", LOKI_INFO);

    return (0);
}

void Level_Editor_Laske_TileVarit() {

    unsigned char *buffer = NULL;
    unsigned long leveys;
    int i;
    unsigned long x, y;
    unsigned long paavari;
    unsigned long keskiarvoVari;
    int lapinakyvia;
    unsigned char vari;
    unsigned long lkm;
    unsigned long paavarit[9];

    Level_Editor_Loki_Kirjaa("calculating tile colors.", LOKI_INFO);

    PisteDraw_Draw_Begin(map->block_buffer, *&buffer, (unsigned long &) leveys);

    for (int tile = 0; tile < 150; tile++) {

        paavari = 0;
        keskiarvoVari = 0;
        lapinakyvia = 0;
        lkm = 0;

        for (i = 0; i < 8; i++)
            paavarit[i] = 0;

        for (x = 0; x < 32; x++) {
            for (y = 0; y < 32; y++) {

                vari = buffer[x + ((tile % 10) * 32) + (y + (tile / 10) * 32) * leveys];


                if (vari < 224) {

                    paavari = (vari / 32);
                    paavarit[paavari] = paavarit[paavari] + 1;
                    keskiarvoVari = keskiarvoVari + (vari % 32);
                    lkm++;
                }
            }
        }

        paavari = 0;
        unsigned long paavarilkm = 0;

        for (i = 0; i < 8; i++)
            if (paavarit[i] > paavarilkm) {
                paavari = i;
                paavarilkm = paavarit[i];
            }

        if (lkm > 0)
            keskiarvoVari = keskiarvoVari / lkm;
        else
            keskiarvoVari = 0;

        if (tile < 90)
            keskiarvoVari += 3;

        if (tile == BLOCK_SWITCH1 || tile == BLOCK_SWITCH2 || tile == BLOCK_SWITCH3 || tile == BLOCK_START ||
            tile == BLOCK_FINISH)
            keskiarvoVari += 12;

        if (keskiarvoVari > 31)
            keskiarvoVari = 31;


        karttavarit[tile] = (unsigned char) (keskiarvoVari + paavari * 32);
        //karttavarit[tile] = unsigned char(paavari*32);*/
    }


    PisteDraw_Draw_End(map->block_buffer);

}


int Level_Editor_Menu_Alusta(int index, char *otsikko, int leveys, int korkeus, int x, int y, int pika) {
    menut[index].korkeus = korkeus;
    menut[index].leveys = leveys;
    menut[index].x = x;
    menut[index].y = y;
    strcpy(menut[index].otsikko, otsikko);
    menut[index].piilota = true;
    menut[index].pika = pika;
    return 0;
}


int Level_Editor_Menut_Alusta(void) {
    Level_Editor_Menu_Alusta(MENU_HELP, "help    (f1)", 256, 240, 192, 120, DIK_F1);
    Level_Editor_Menu_Alusta(MENU_KARTTA, "map     (f2)", MAP_WIDTH, MAP_HEIGHT, 492, 130,
                             DIK_F2);
    Level_Editor_Menu_Alusta(MENU_PALIKAT, "tiles   (f3)", 320, 480, 592, 145, DIK_F3);
    Level_Editor_Menu_Alusta(MENU_SPRITET, "sprites (f4)", 320, 480, 262, 45, DIK_F4);
    Level_Editor_Menu_Alusta(MENU_TIEDOSTOT, "files   (f5)", 320, 300, 192, 40, DIK_F5);
    Level_Editor_Menu_Alusta(MENU_SAVE, "save    (f6)", 320, 50, 192, 355, DIK_F6);
    Level_Editor_Menu_Alusta(MENU_INFO, "map information (f7)", 320, 240, 150, 40, DIK_F7);
    Level_Editor_Menu_Alusta(MENU_EXIT, "exit editor?", 256, 35, SCREEN_WIDTH / 2 - 128, SCREEN_HEIGHT / 2 - 15,
                             DIK_ESCAPE);
    Level_Editor_Menu_Alusta(MENU_TOOLS, "tools   (f8)", 320, 200, 422, 210, DIK_F8);
    Level_Editor_Menu_Alusta(MENU_LOKI, "log     (f9)", 320, 500, 222, 110, DIK_F9);

    menut[MENU_HELP].piilota = false;

    return 0;
}

int Level_Editor_Tiedostot_Alusta(void) {
    for (int i = 0; i < MAX_DIR_TIEDOSTOJA; i++) {
        strcpy(tiedostot[i].nimi, "\0");
        tiedostot[i].tyyppi = 0;
    }

    return 0;
}

int Level_Editor_Tiedostot_Vertaa(char *a, char *b) {
    int apituus = strlen(a);
    int bpituus = strlen(b);
    int looppi = apituus;

    if (bpituus < apituus)
        looppi = bpituus;

    strlwr(a);
    strlwr(b);

    for (int i = 0; i < looppi; i++) {
        if (a[i] < b[i])
            return 2;

        if (a[i] > b[i])
            return 1;
    }

    if (apituus > bpituus)
        return 1;

    if (apituus < bpituus)
        return 2;

    return 0;
}

int Level_Editor_Tiedostot_Aakkosta(void) {
    unsigned long i, t;
    TIEDOSTO temp;
    bool tehty;

    if (tiedostoja > 1) {
        for (i = tiedostoja - 1; i >= 0; i--) {
            tehty = true;

            for (t = 0; t < i; t++) {
                if (Level_Editor_Tiedostot_Vertaa(tiedostot[t].nimi, tiedostot[t + 1].nimi) == 1) {
                    temp = tiedostot[t];
                    tiedostot[t] = tiedostot[t + 1];
                    tiedostot[t + 1] = temp;
                    tehty = false;
                }
            }

            if (tehty)
                return 0;
        }
    }

    return 0;
}

int Level_Editor_Tiedostot_Hae_Tyyppi2(struct _finddata_t &map_file, long &hFile, int &i, char *tiedosto, int tyyppi) {
    //char file[6];
    bool loytyi = false;

    //strcpy(file,"*.");
    //strcat(file,tarkenne);

    if (i >= MAX_DIR_TIEDOSTOJA)
        return 2;

    if ((hFile = _findfirst(tiedosto, &map_file)) == -1L) {
        //strcpy(message,"no files found.");
        _findclose(hFile);
    } else {
        strcpy(tiedostot[i].nimi, map_file.name);
        tiedostot[i].tyyppi = tyyppi;
        i++;

        if (tyyppi == TIEDOSTO_MAP)
            Level_Editor_Aseta_Episodihakemisto();

        loytyi = true;
    }

    while (i < MAX_DIR_TIEDOSTOJA && _findnext(hFile, &map_file) == 0) {
        strcpy(tiedostot[i].nimi, map_file.name);
        tiedostot[i].tyyppi = tyyppi;
        i++;
        loytyi = true;
    }

    _findclose(hFile);

    if (!loytyi)
        return 1;

    return 0;
}

int Level_Editor_Tiedostot_Hae_Tyyppi(int &i, char *tiedosto, int tyyppi) {
    struct _finddata_t map_file;
    long hFile;
    bool loytyi = false;

    if (i >= MAX_DIR_TIEDOSTOJA)
        return 2;

    if ((hFile = _findfirst(tiedosto, &map_file)) == -1L) {
        //strcpy(message,"no files found.");
        _findclose(hFile);
    } else {
        strcpy(tiedostot[i].nimi, map_file.name);
        tiedostot[i].tyyppi = tyyppi;
        i++;

        if (tyyppi == TIEDOSTO_MAP)
            Level_Editor_Aseta_Episodihakemisto();

        loytyi = true;
    }

    while (i < MAX_DIR_TIEDOSTOJA && _findnext(hFile, &map_file) == 0) {
        strcpy(tiedostot[i].nimi, map_file.name);
        tiedostot[i].tyyppi = tyyppi;
        i++;
        loytyi = true;
    }

    _findclose(hFile);

    if (!loytyi)
        return 1;

    return 0;
}

int Level_Editor_Tiedostot_Hae(void) {
/*    struct _finddata_t map_file;
    long hFile;*/
    int i = 0;
/*
	if((hFile = _findfirst( "*.", &map_file )) == -1L )
       strcpy(message,"no directories found");
	else
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_DIR;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_DIR;
		i++;
	}

	_findclose( hFile );
*/
    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.", TIEDOSTO_DIR) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.map", TIEDOSTO_MAP) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.bmp", TIEDOSTO_BMP) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.pcx", TIEDOSTO_BMP) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.spr", TIEDOSTO_SPR) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.xm", TIEDOSTO_MUS) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.mod", TIEDOSTO_MUS) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.s3m", TIEDOSTO_MUS) == 2)
        return 0;

    if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "*.it", TIEDOSTO_MUS) == 2)
        return 0;

    if (strcmp(pk2_hakemisto, " ") == 0) // jos hakemistoa, jossa pk2.exe sijaitsee ei ole viel� m��ritelty...
        if (Level_Editor_Tiedostot_Hae_Tyyppi(i, "pk2.exe", 0) == 0)
            Level_Editor_Tallenna_PK2_Hakemisto(working_directory);

    /*
	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.map", &map_file )) == -1L )
       strcpy(message,"no map files found");
	else
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_MAP;
		i++;
		Level_Editor_Aseta_Episodihakemisto();
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_MAP;
		i++;
	}

	_findclose( hFile );

	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.bmp", &map_file )) != -1L )
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_BMP;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_BMP;
		i++;
	}

	_findclose( hFile );


	if (i >= MAX_DIR_TIEDOSTOJA)
		return 0;

	if((hFile = _findfirst( "*.spr", &map_file )) != -1L )
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_SPR;
		i++;
	}

	while (i<MAX_DIR_TIEDOSTOJA && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(tiedostot[i].name, map_file.name);
		tiedostot[i].type = TIEDOSTO_SPR;
		i++;
	}

	_findclose( hFile );

	if (strcmp(pk2_directory," ")==0) // jos hakemistoa, jossa pk2.exe sijaitsee ei ole viel� m��ritelty...
	{

		if((hFile = _findfirst( "*.exe", &map_file )) != -1L )
		{
			if (strcmp("PK2.exe", map_file.name)==0)
				Level_Editor_Tallenna_PK2_Hakemisto(working_directory);
		}

		while (_findnext( hFile, &map_file ) == 0)
		{
			if (strcmp("PK2.exe", map_file.name)==0)
				Level_Editor_Tallenna_PK2_Hakemisto(working_directory);
		}

		_findclose( hFile );

	}
*/
    tiedostoja = i;

    return 0;
}

void Level_Editor_Aseta_Palikat(char *filename) {
    Level_Editor_Loki_Tallenna("Ladataan palikkapaletti.\n");
    Level_Editor_Loki_Kirjaa("loading tile palette.", LOKI_INFO);

    if (map->Load_Block_Palette("", filename) == 1)
        DirectX_virhe = true;

    Level_Editor_Laske_TileVarit();

}

void Level_Editor_Aseta_Taustakuva(char *filename) {
    Level_Editor_Loki_Tallenna("Ladataan background_image.\n");
    Level_Editor_Loki_Kirjaa("loading background image.", LOKI_INFO);

    if (map->Load_Background_Image("", filename) == 1)
        DirectX_virhe = true;
}

int Level_Editor_Init(void) {
    Level_Editor_Loki_Alusta();
    Level_Editor_Loki_Kirjaa("--------------------", LOKI_INFO);
    Level_Editor_Loki_Kirjaa("starting new session", LOKI_INFO);
    Level_Editor_Loki_Kirjaa("--------------------", LOKI_INFO);
    Level_Editor_Loki_Tallenna("----------------------------------\n");
    Level_Editor_Loki_Tallenna("New istunto.\n");
    Level_Editor_Loki_Tallenna("----------------------------------\n");

    for (int ci = 0; ci < 360; ci++)
        cos_table[ci] = cos(3.1415 * 2 * (ci % 360) / 180) * 33;

    for (int si = 0; si < 360; si++)
        sin_table[si] = sin(3.1415 * 2 * (si % 360) / 180) * 33;

    Map_Cos_Sin(cos_table, sin_table);
    Map_Set_Screen_Dimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

    _getcwd(working_directory, _MAX_PATH);

    strcpy(editor_hakemisto, working_directory);

    Level_Editor_Loki_Kirjaa("looking for pk2 directory.", LOKI_INFO);
    Level_Editor_Lataa_PK2_Hakemisto();

    Level_Editor_Tiedostot_Alusta();
    Level_Editor_Tiedostot_Hae();
    Level_Editor_Tiedostot_Aakkosta();

    if (strcmp(pk2_hakemisto, " ") == 0) {
        Level_Editor_Loki_Kirjaa("pk2 directory not found.", LOKI_INFO);
        MessageBox(window_handle,
                   "Welcome to Pekka Kana 2 Level Editor :)\nUse the file window (f5) to find the directory\nwhere you have installed Pekka Kana 2.",
                   "PK2 Level Editor 1.0", MB_OK);
    } else
        MessageBox(window_handle, "Welcome to Pekka Kana 2 Level Editor:)", "PK2 Level Editor 0.91", MB_OK);


    if ((PisteInput_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app)) == PI_ERROR)
        DirectX_virhe = true;

    if ((PisteDraw_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                        MAX_COLORS_PALETTE)) == PD_ERROR)
        DirectX_virhe = true;


    if ((kuva_editori = PisteDraw_Buffer_Create(SCREEN_WIDTH, SCREEN_HEIGHT, true, 255)) == PD_ERROR)
        DirectX_virhe = true;

    if ((kuva_kartta = PisteDraw_Buffer_Create(MAP_WIDTH, MAP_HEIGHT, true, 255)) == PD_ERROR)
        DirectX_virhe = true;

    if (PisteDraw_Image_Load(kuva_editori, "pk2edit.bmp", true) == PD_ERROR) {
        Level_Editor_Loki_Kirjaa("could not load pk2edit.bmp.", LOKI_INFO);
        DirectX_virhe = true;
    }

    map = new PK2Map();
    undo = new PK2Map();

    if ((font1 = PisteDraw_Font_Create(kuva_editori, 1, 456, 8, 8, 52)) == PD_ERROR)
        DirectX_virhe = true;
    /*
	prototypes[PROTOTYPE_CHICKEN].Kana("pk2spr01.bmp");
	prototypes[PROTOTYPE_EGG].Muna("pk2spr01.bmp");
	prototypes[PROTOTYPE_CHICK].Pikkukana("pk2spr01.bmp");
	prototypes[PROTOTYPE_APPLE].Omena("pk2spr01.bmp");
*/
    Level_Editor_Kartta_Alusta();

    //Level_Editor_Kartta_Oletusasetukset();

    Level_Editor_Menut_Alusta();

    Level_Editor_Kartta_Varmista();

    PisteDraw_Fade_Palette_In(PD_FADE_FAST);

    PisteWait_Start();

    Level_Editor_Loki_Kirjaa("startup completed.", LOKI_INFO);

    Level_Editor_Asetukset_Lataa();

    return 0;
}

/* PIIRTORUTIINIT ---------------------------------------------------------------*/

int Level_Editor_Piirra_Nuolet(int x, int y, int menuId) {
    int leveys = 13, korkeus = 27;

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, x, y, 324, 1, 337, 28);

    if (PisteInput_Mouse_Left() && key_delay == 0 && menuId == aktiivinen_menu) {
        if (mouse_x > x && mouse_x < x + leveys && mouse_y > y && mouse_y < y + korkeus / 2) {
            key_delay = 8;
            return 1;
        }

        if (mouse_x > x && mouse_x < x + leveys && mouse_y > y + korkeus / 2 && mouse_y < y + korkeus) {
            key_delay = 8;
            return 2;
        }
    }

    return 0;
}

int Level_Editor_Piirra_Nuolet2(int x, int y, int menuId) {
    int leveys = 13, korkeus = 13;

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, x, y, 324, 1, 337, 15);
    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, x + 14, y, 324, 15, 337, 28);

    if (PisteInput_Mouse_Left() && key_delay == 0 && menuId == aktiivinen_menu) {
        if (mouse_x > x && mouse_x < x + 13 && mouse_y > y && mouse_y < y + 13) {
            key_delay = 8;
            return 1;
        }

        if (mouse_x > x + 14 && mouse_x < x + 27 && mouse_y > y && mouse_y < y + 13) {
            key_delay = 8;
            return 2;
        }
    }

    return 0;
}

int Level_Editor_Piirra_Nelio(int vasen, int yla, int oikea, int ala, unsigned char color) {
/*
	unsigned char *buffer = NULL;
	unsigned long tod_leveys;

	PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &)tod_leveys);

	for (int x = left_bound; x <= right_bound; x++)
	{
		for (int y = top_bound; y <= bottom_bound; y++)
		{
			if (y == top_bound || y == bottom_bound || x == left_bound || x == right_bound)
			{
				buffer[x+y*tod_leveys] = color;
			}
		}
	}

	PisteDraw_Draw_End(PD_BACKBUFFER);
*/
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, vasen, yla, oikea, yla + 1, color);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, vasen, ala - 1, oikea, ala, color);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, vasen, yla, vasen + 1, ala, color);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, oikea - 1, yla, oikea, ala, color);

    return 0;
}

void Level_Editor_Viiva_Hori(int x, int y, int pituus, unsigned char vari) {
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, x, y, x + pituus, y + 1, vari);
}

void Level_Editor_Viiva_Vert(int x, int y, int pituus, unsigned char vari) {
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, x, y, x + 1, y + pituus, vari);
}

bool Level_Editor_Nappi(char *otsikko, int vasen, int yla, int menuId) {
    bool paalla = false;

    int vali = PisteDraw_Font_Write(font1, otsikko, PD_BACKBUFFER, vasen + 2, yla + 2);

    //unsigned char *buffer = NULL;
    //unsigned long tod_leveys;
    unsigned char color = 18;

    int oikea = vasen + vali + 2, ala = yla + 12;

    if (mouse_x < oikea && mouse_x > vasen && mouse_y > yla && mouse_y < ala &&
        (menuId == aktiivinen_menu || menuId == -1)) {
        paalla = true;
        color = 29;
    }

    Level_Editor_Piirra_Nelio(vasen + 1, yla + 1, oikea + 1, ala + 1, 0);
    Level_Editor_Piirra_Nelio(vasen, yla, oikea, ala, color);

    if (paalla && PisteInput_Mouse_Left() && key_delay == 0) {
        key_delay = 20;
        return true;
    }

    return false;
}

void Level_Editor_Lista(TOOL_LIST &lista) {
    if (lista.nakyva) {
        int lista_y = 3;

        PisteDraw_Buffer_Clear(PD_BACKBUFFER, lista.x, lista.y, lista.x + 58, lista.y + 75, 12);

        if (mouse_x > lista.x && mouse_x < lista.x + 58 && mouse_y > lista.y && mouse_y < lista.y + 75)
            edit_tila = EDIT_MENU;

        Level_Editor_Piirra_Nelio(lista.x, lista.y, lista.x + 58, lista.y + 75, 1);

        if (Level_Editor_Nappi("copy  ", lista.x + 3, lista.y + lista_y, -1)) lista.valinta = 1;
        lista_y += 14;
        if (Level_Editor_Nappi("paste ", lista.x + 3, lista.y + lista_y, -1)) lista.valinta = 2;
        lista_y += 14;
        if (Level_Editor_Nappi("cut   ", lista.x + 3, lista.y + lista_y, -1)) lista.valinta = 3;
        lista_y += 14;
        if (Level_Editor_Nappi("fill  ", lista.x + 3, lista.y + lista_y, -1)) lista.valinta = 4;
        lista_y += 14;
        if (Level_Editor_Nappi("cancel", lista.x + 3, lista.y + lista_y, -1)) lista.valinta = 5;
        lista_y += 14;

        if (lista.valinta != 0)
            lista.nakyva = false;
    }
    //else
    //	lista.valinta = 0;
}

void Level_Editor_Lista_Nayta(TOOL_LIST &lista, int x, int y) {
    lista.nakyva = true;
    lista.valinta = 0;
    lista.x = x;
    lista.y = y;
}

bool Level_Editor_Linkki(char *otsikko, int vasen, int yla, int menuId) {
    bool paalla = false;

    int vali = PisteDraw_Font_Write(font1, otsikko, PD_BACKBUFFER, vasen, yla);

    int oikea = vasen + 200, //vali,
    ala = yla + 8;

    if (menuId != aktiivinen_menu)
        return false;

    if (mouse_x < oikea && mouse_x > vasen && mouse_y > yla && mouse_y < ala) {
        paalla = true;
    }

    if (paalla) {
        unsigned char *buffer = NULL;
        unsigned long tod_leveys;

        PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &) tod_leveys);

        for (int x = vasen; x <= oikea; x++)
            for (int y = yla; y <= ala; y++)
                buffer[x + y * tod_leveys] = 41;

        PisteDraw_Draw_End(PD_BACKBUFFER);

        PisteDraw_Font_Write_Transparent(font1, otsikko, PD_BACKBUFFER, vasen, yla, 90);
    }

    if (paalla && PisteInput_Mouse_Left() && key_delay == 0) {
        key_delay = 20;
        return true;
    }

    return false;
}


bool Level_Editor_Kysy2(char *teksti, int vasen, int yla, int lkm) {
    bool paalla = false;

    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    unsigned char color = 10;

    char vastaus[300];
    char merkki;
    int font_index = 0;

    int oikea = vasen + lkm * 8, ala = yla + 10;

    int pituus = strlen(teksti);

    int i;

    strcpy(vastaus, teksti);

    font_index = (mouse_x - vasen) / 8;

    if (mouse_x < oikea && mouse_x > vasen && mouse_y > yla && mouse_y < ala) {
        paalla = true;
        color = 46;
        editoi_tekstia = true;
        font_index = (mouse_x - vasen) / 8;
    }

    if (font_index < lkm && paalla) {
        merkki = PisteInput_Read_Keyboard();

        if (merkki != '\0' && key_delay == 0) {
            if (merkki == '\n') {
                for (i = font_index; i < lkm; i++)
                    vastaus[i] = ' ';
                key_delay = 20;
                //merkki = '\0';
            } else {
                vastaus[font_index] = merkki;

                font_index++;
                key_delay = 20;
                mouse_x += 8;
            }

            if (PisteInput_KeyDown(DIK_BACK)) {
                key_delay = 20;
                mouse_x -= 8;
                if (font_index > 0)
                    font_index--;
            }

            if (PisteInput_KeyDown(DIK_DELETE)) {
                for (i = font_index; i < lkm - 1; i++)
                    vastaus[i] = vastaus[i + 1];
                key_delay = 20;
            }
        }

        vastaus[lkm] = '\0';
    }

    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &) tod_leveys);

    for (int x = vasen; x <= oikea; x++)
        for (int y = yla; y <= ala; y++)
            buffer[x + y * tod_leveys] = color;

    if (paalla)
        for (int x = vasen + font_index * 8; x <= vasen + font_index * 8 + 8; x++)
            for (int y = yla; y <= ala; y++)
                buffer[x + y * tod_leveys] = 20;

    PisteDraw_Draw_End(PD_BACKBUFFER);

    PisteDraw_Font_Write_Transparent(font1, vastaus, PD_BACKBUFFER, vasen, yla + 2, 85);

    strcpy(teksti, vastaus);

    return paalla;
}

bool Level_Editor_Kysy(char *teksti, int vasen, int yla, int lkm, int id, int menuId) {
    bool editoi = false;

    unsigned char color = 10;

    char merkki;

    int oikea = vasen + lkm * 8, ala = yla + 10;

    int pituus = strlen(teksti);

    int i;

    if (menuId == aktiivinen_menu) {
        if (mouse_x < oikea && mouse_x > vasen && mouse_y > yla && mouse_y < ala && key_delay == 0 &&
                PisteInput_Mouse_Left()) {
            editKursori = (mouse_x - vasen) / 8;
            editKenttaId = id;
            memset(editTeksti, '\0', sizeof(editTeksti));
            strcpy(editTeksti, teksti);
            key_delay = 20;

            if (editKursori > pituus)
                editKursori = pituus;
        }

        if (id == editKenttaId) {
            editoi = true;
            editoi_tekstia = true;
            color = 46;
        }


        if (/*editKursori < lkm &&*/ editoi) {
            merkki = PisteInput_Read_Keyboard();

            if (merkki != '\0' && key_delay == 0) {
                if (merkki == '\n') {
                    for (i = editKursori; i < lkm; i++)
                        editTeksti[i] = '\0';

                    editKenttaId = 0;
                    key_delay = 20;
                    editoi_tekstia = false;
                } else {
                    editTeksti[editKursori] = merkki;

                    pituus = strlen(editTeksti);

                    if (editKursori < lkm - 1)
                        editKursori++;

                    if (merkki == editMerkki)
                        key_delay = 10;//5;
                    else
                        key_delay = 20;//15;

                    editMerkki = merkki;
                }
            }

            if (key_delay == 0) {
                if (PisteInput_KeyDown(DIK_BACK)) {
                    if (editKursori > 0) {
                        editKursori--;

                        for (i = editKursori; i < lkm - 1; i++)
                            editTeksti[i] = editTeksti[i + 1];

                        editTeksti[lkm - 1] = '\0';
                        editTeksti[lkm] = '\0';
                    }

                    if (editMerkki == DIK_BACK)
                        key_delay = 5;
                    else
                        key_delay = 20;

                    editMerkki = DIK_BACK;

                }

                if (PisteInput_KeyDown(DIK_DELETE)) {
                    for (i = editKursori; i < lkm - 1; i++)
                        editTeksti[i] = editTeksti[i + 1];

                    editTeksti[lkm - 1] = '\0';
                    editTeksti[lkm] = '\0';

                    if (editMerkki == 'D')
                        key_delay = 5;
                    else
                        key_delay = 20;

                    editMerkki = 'D';
                }

                if (PisteInput_KeyDown(DIK_LEFT)) {
                    if (editKursori > 0)
                        editKursori--;

                    key_delay = 5;
                }

                if (PisteInput_KeyDown(DIK_RIGHT)) {
                    if (editKursori < lkm - 1 && editKursori < pituus)
                        editKursori++;

                    key_delay = 5;
                }

            }

            editTeksti[lkm] = '\0';
        }
    }

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, vasen, yla, oikea, ala, color);

    if (editoi)
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, vasen + editKursori * 8, yla, vasen + editKursori * 8 + 8, ala, 20);

    PisteDraw_Font_Write_Transparent(font1, teksti, PD_BACKBUFFER, vasen, yla + 2, 85);

    if (editoi)
        strcpy(teksti, editTeksti);

    return editoi;
}

/*
int Level_Editor_Menu_Tyhja(int i)
{
	int x = menut[i].x,
		y = menut[i].y+16;

	jotakin...

	return 0;
}
*/

int Level_Editor_Menu_Spritet(int i) {
    int x = menut[i].x, y = menut[i].y + 16;
    int vali;

    PisteDraw_SetClipper(PD_BACKBUFFER, menut[i].x, menut[i].y, menut[i].x + menut[i].leveys,
                         menut[i].y + menut[i].korkeus + 16);

    int piirto_y = 5, piirto_x = 15, rv_korkeus = 0;
    bool rivinvaihto = false;

    int eka_proto = menu_spritet_eka;
    int vika_proto = menu_spritet_eka + 8;

    if (vika_proto > MAX_PROTOTYPES)
        vika_proto = MAX_PROTOTYPES;

    if (eka_proto < 0)
        eka_proto = 0;

    char luku[10];
    int j_luku = eka_proto;

    for (int proto = eka_proto; proto <= vika_proto; proto++) {
        if (prototypes[proto].height > 0) {
            if (prototypes[proto].height > rv_korkeus)
                rv_korkeus = prototypes[proto].height;

            if (mouse_x > x + piirto_x - 1 &&
                mouse_x < x + piirto_x + prototypes[proto].frame_width + 1/*width+1*/ &&
                mouse_y > y + piirto_y - 1 &&
                mouse_y < y + piirto_y + prototypes[proto].frame_height + 1/*height+1*/ &&
                mouse_y < menut[i].y + menut[i].korkeus + 16) {
                Level_Editor_Piirra_Nelio(x + piirto_x - 1, y + piirto_y - 1,
                                          x + piirto_x + prototypes[proto].frame_width + 1,/*  .frame_leveys+1*/
                                          y + piirto_y + prototypes[proto].frame_height + 1, /*height+1*/ 31);

                if (key_delay == 0 && PisteInput_Mouse_Left()) {
                    proto_valittu = proto;
                    key_delay = 10;
                }

                if (key_delay == 0 && PisteInput_KeyDown(DIK_DELETE)) {
                    Level_Editor_Loki_Kirjaa("deleting sprite.", LOKI_INFO);
                    Level_Editor_Prototyyppi_Poista(proto);
                    key_delay = 30;
                }
            } else {
                Level_Editor_Piirra_Nelio(x + piirto_x - 1, y + piirto_y - 1,
                                          x + piirto_x + prototypes[proto].frame_width + 1,
                                          y + piirto_y + prototypes[proto].frame_height + 1, 18);
            }

//			itoa(j_luku+1,luku,10);
//			PisteDraw_Font_Write(font1,luku,PD_BACKBUFFER,x+4,y+piirto_y);
            prototypes[proto].Draw(x + piirto_x, y + piirto_y, 0);
            PisteDraw_Font_Write(font1, prototypes[proto].name, PD_BACKBUFFER, x + 120, y + piirto_y);
            PisteDraw_Font_Write(font1, map->protos[proto], PD_BACKBUFFER, x + 120, y + piirto_y + 10);

            if (prototypes[proto].type == TYPE_GAME_CHARACTER) {
                if (proto == map->player_sprite)
                    vali = PisteDraw_Font_Write(font1, "player: yes", PD_BACKBUFFER, x + 120, y + piirto_y + 20);
                else
                    vali = PisteDraw_Font_Write(font1, "player: no", PD_BACKBUFFER, x + 120, y + piirto_y + 20);

                if (Level_Editor_Nappi("set player", x + 125 + vali, y + piirto_y + 18, i)) {
                    map->player_sprite = proto;
                }
            }

            itoa(j_luku + 1, luku, 10);
            PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + 4, y + piirto_y);

            //vali = PisteDraw_Font_Write(font1,"bonus: ",PD_BACKBUFFER,x+240,y+piirto_y);
            //PisteDraw_Font_Write(font1,prototypes[proto].bonus_sprite,PD_BACKBUFFER,x+240+vali,y+piirto_y);


            Level_Editor_Viiva_Hori(x, y + piirto_y - 3, 320, 13);

            if (prototypes[proto].frame_height > 30)
                piirto_y += prototypes[proto].frame_height + 6;  //rv_korkeus + 4;
            else
                piirto_y += 36;

            piirto_x = 15;
            rv_korkeus = 0;
/*
			else
			{
				piirto_x += prototypes[proto].width + 4;
			}
*/
            j_luku++;
        }
    }

    int nuoli = Level_Editor_Piirra_Nuolet(x + menut[i].leveys - 15, y + menut[i].korkeus - 30, i);

    if (nuoli == 1 && menu_spritet_eka > 0) {
        menu_spritet_eka--;
    }

    if (nuoli == 2 && menu_spritet_eka < MAX_PROTOTYPES - 1) {
        menu_spritet_eka++;
    }

    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    return 0;
}

int Level_Editor_Menu_Help(int i) {
    int x = menut[i].x, y = menut[i].y + 16, my = y + 19;

    PisteDraw_Font_Write(font1, "f1 = help", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f2 = map", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f3 = tile palette", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f4 = sprites", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f5 = load map/files", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f6 = save map", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f7 = map information", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "f8 = clear map", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "s  = quick save ", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "u  = undo ", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "movement_x  = animate moving tiles", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "shift = background / foreground", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "mouse left  = draw", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "mouse right = remove", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "mouse left  = paint (map menu)", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "mouse right = go to (map menu)", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "alt + mouse = select area", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "ctrl + c = copy", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "ctrl + movement_y = paste simple", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "ctrl + v = paste pattern", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "ctrl + x = cut", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "arrows = scroll map", PD_BACKBUFFER, x + 3, my);
    my += 9;
    PisteDraw_Font_Write(font1, "esc = exit", PD_BACKBUFFER, x + 3, my);
    my += 9;

    return 0;
}

int Level_Editor_Menu_Kartta(int i) {
    int x = menut[i].x, y = menut[i].y + 15;

    PisteDraw_Buffer_Flip_Fast(kuva_kartta, PD_BACKBUFFER, x, y, 0, 0, MAP_WIDTH,
                               MAP_HEIGHT);

    if (aktiivinen_menu == i) {
        if (menut[i].lista.nakyva)
            Level_Editor_Lista(menut[i].lista);

        RECT nelio = {0, 0, 0, 0};
        nelio.left = mouse_x - (SCREEN_WIDTH / 32) / 2;
        nelio.top = mouse_y - (SCREEN_HEIGHT / 32) / 2;

        if (nelio.left < x)
            nelio.left = x;

        if (nelio.left + SCREEN_WIDTH / 32 > x + menut[i].leveys)
            nelio.left = x + menut[i].leveys - SCREEN_WIDTH / 32;

        if (nelio.top < y)
            nelio.top = y;

        if (nelio.top + SCREEN_HEIGHT / 32 > y + menut[i].korkeus)
            nelio.top = y + menut[i].korkeus - SCREEN_HEIGHT / 32;

        nelio.right = nelio.left + SCREEN_WIDTH / 32;
        nelio.bottom = nelio.top + SCREEN_HEIGHT / 32;

        Level_Editor_Piirra_Nelio(nelio.left - 1, nelio.top - 1, nelio.right + 1, nelio.bottom + 1, 0);
        Level_Editor_Piirra_Nelio(nelio.left, nelio.top, nelio.right, nelio.bottom, 31);
        Level_Editor_Piirra_Nelio(nelio.left + 1, nelio.top + 1, nelio.right - 1, nelio.bottom - 1, 0);

        if (PisteInput_Mouse_Left() && mouse_y > y && key_delay == 0) {
            kartta_x = nelio.left - x;
            kartta_y = nelio.top - y;
        }
        /*
		if (PisteInput_Hiiri_Oikea() && mouse_y > y && kartta_kohde_status == 0 && key_delay == 0)
		{
			kartta_kohde_status = 1;
			kartta_kohde.left	= mouse_x;
			kartta_kohde.top	= mouse_y;
			kartta_kohde.right	= mouse_x;
			kartta_kohde.bottom = mouse_y;
			key_delay = 25;
		}

		if (kartta_kohde_status == 1)
		{
			if (kartta_kohde.left < x)
				kartta_kohde.left = x;

			if (kartta_kohde.top < y)
				kartta_kohde.top = y;

			if (mouse_x >= kartta_kohde.left && mouse_x < x+menut[i].width)
				kartta_kohde.right  = mouse_x;
			if (mouse_y >= kartta_kohde.top && mouse_y < y+menut[i].height)
				kartta_kohde.bottom = mouse_y;

			Level_Editor_Piirra_Nelio(kartta_kohde.left, kartta_kohde.top, kartta_kohde.right, kartta_kohde.bottom, 48);

			if ((PisteInput_Mouse_Right() && mouse_y > y && key_delay == 0) || edit_tila == EDIT_KARTTA)
			{
				kartta_kohde_status = 2;
				Level_Editor_Lista_Nayta(menut[i].lista,x+90,y+35);
				key_delay = 25;
			}
		}

		if (kartta_kohde_status == 2)
		{
			if (kartta_kohde.left < x)
				kartta_kohde.left = x;

			if (kartta_kohde.top < y)
				kartta_kohde.top = y;

			if (kartta_kohde.right > x+menut[i].width)
				kartta_kohde.right = x+menut[i].width;

			if (kartta_kohde.bottom > y+menut[i].height)
				kartta_kohde.bottom = y+menut[i].height;

			int k_vasen = kartta_kohde.left - x,
				k_yla	= kartta_kohde.top - y,
				k_oikea = kartta_kohde.right - x,
				k_ala	= kartta_kohde.bottom - y,
				kx = 0, ky = 0;

			Level_Editor_Piirra_Nelio(kartta_kohde.left, kartta_kohde.top, kartta_kohde.right, kartta_kohde.bottom, 48);

			if (menut[i].lista.valinta == 4)
			{
				Level_Editor_Kartta_Varmista();

				for (kx = k_vasen; kx < k_oikea; kx++)
					for (ky = k_yla; ky < k_ala; ky++)
					{
						if (edit_tila == EDIT_SPRITE)
						{
							map->sprites[kx+ky*MAP_WIDTH] = proto_valittu;
						}
						else
						{
							if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
								map->walls[kx+ky*MAP_WIDTH] = edit_palikka;
							if (edit_kerros == EDIT_KERROS_TAUSTA)
								map->backgrounds[kx+ky*MAP_WIDTH] = edit_palikka;
						}
					}

				Level_Editor_Kartta_Paivita();

				strcpy(message,"u = undo paste");

				kartta_kohde_status = 0;
				key_delay = 25;
			}

			if (menut[i].lista.valinta == 3)
			{
				Level_Editor_Kartta_Varmista();

				for (kx = k_vasen; kx < k_oikea; kx++)
					for (ky = k_yla; ky < k_ala; ky++)
					{
						if (edit_tila == EDIT_SPRITE)
						{
							map->sprites[kx+ky*MAP_WIDTH] = 255;
						}
						else
						{
							if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
								map->walls[kx+ky*MAP_WIDTH] = 255;
							if (edit_kerros == EDIT_KERROS_TAUSTA)
								map->backgrounds[kx+ky*MAP_WIDTH] = 255;
						}
					}

				Level_Editor_Kartta_Paivita();

				strcpy(message,"u = undo delete");

				kartta_kohde_status = 0;
				key_delay = 25;
			}

			if (!menut[i].lista.nakyva)
				kartta_kohde_status = 0;


			//PisteDraw_Font_Kirjoita_Lapinakyva(font1,"mouse right = cancel, space = fill", PD_BACKBUFFER, x+2, menut[i].height+menut[i].y-17, 70);
			//PisteDraw_Font_Write_Transparent(font1,"delete = delete", PD_BACKBUFFER, x+2, menut[i].height+menut[i].y-9, 70);

		}*/
    }

    return 0;
}

int Level_Editor_Menu_Palikat(int i) {
    int x = menut[i].x, y = menut[i].y + 15, y_plus = menu_palikat_nayta_y * 32;

    char luku[10];

    PisteDraw_Buffer_Flip_Fast(map->block_buffer, PD_BACKBUFFER, x, y, 0, 0 + y_plus, 320, 480 + y_plus);

    if (aktiivinen_menu == i) {
        RECT nelio = {0, 0, 0, 0};

        nelio.left = (((mouse_x - x) / 32) * 32) + x;
        nelio.top = (((mouse_y - y) / 32) * 32) + y;

        if (nelio.left < x)
            nelio.left = x;

        if (nelio.left + 32 > x + menut[i].leveys)
            nelio.left = x + menut[i].leveys - 32;

        if (nelio.top < y)
            nelio.top = y;

        if (nelio.top + 32 > y + menut[i].korkeus)
            nelio.top = y + menut[i].korkeus - 32;

        nelio.right = nelio.left + 32;
        nelio.bottom = nelio.top + 32;

        Level_Editor_Piirra_Nelio(nelio.left + 1, nelio.top + 1, nelio.right + 1, nelio.bottom + 1, 0);
        Level_Editor_Piirra_Nelio(nelio.left, nelio.top, nelio.right, nelio.bottom, 120);

        int px = (mouse_x - x) / 32;
        int py = ((mouse_y - y) / 32) * 10;

        itoa(px + py + menu_palikat_nayta_y * 10 + 1, luku, 10);
        PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, nelio.left + 2, nelio.top + 2, 65);

        if (PisteInput_Mouse_Left() && mouse_y > y) {
            edit_palikka = px + py + menu_palikat_nayta_y * 10;
            proto_valittu = MAX_PROTOTYPES;
            key_delay = 15;
        }

        /*if (PisteInput_Mouse_Right() && mouse_y > y && key_delay == 0)
		{
			menu_palikat_nayta_y += 2;
			if (menu_palikat_nayta_y > 2)
				menu_palikat_nayta_y = 0;
			key_delay = 15;
		}*/
    }

    return 0;
}

int Level_Editor_Menu_Tiedostot(int i) {
    int x = menut[i].x, y = menut[i].y + 16, linkki_y = 0;

    bool paivita = false;
    char otsikko[_MAX_DIR];

    if (menu_tiedostot_eka < 0)
        menu_tiedostot_eka = 0;

    if (menu_tiedostot_eka > MAX_DIR_TIEDOSTOJA - 2)
        menu_tiedostot_eka = MAX_DIR_TIEDOSTOJA - 2;

    int eka_tiedosto = menu_tiedostot_eka;
    int vika_tiedosto = menu_tiedostot_eka + 35;

    if (vika_tiedosto > MAX_DIR_TIEDOSTOJA)
        vika_tiedosto = MAX_DIR_TIEDOSTOJA;

    if (eka_tiedosto < 0)
        eka_tiedosto = 0;

    int ti = menu_tiedostot_eka;
    int tilask = 0;

    //for (int ti=eka_tiedosto;ti<vika_tiedosto;ti++)
    while (linkki_y < 33 * 9 && ti < MAX_DIR_TIEDOSTOJA) {
        if (tiedostot[ti].tyyppi != 0) {
            if (tiedostot[ti].tyyppi == TIEDOSTO_DIR && nayta_hakemistot) {
                strcpy(otsikko, tiedostot[ti].nimi);
                PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, x + 5, y + linkki_y + 3, 338, 1, 346, 8);
                //strcat(otsikko,"...");
                if (Level_Editor_Linkki(otsikko, x + 5 + 10, y + linkki_y + 3, i)) {
                    if (_chdir(tiedostot[ti].nimi) == 0) {
                        _getcwd(working_directory, _MAX_PATH);
                        paivita = true;
                        strcpy(message, "changed directory");
                        menu_tiedostot_eka = 0;
                    } else
                        strcpy(message, "could not open directory");
                }
                linkki_y += 9;
            }

            switch (menu_tiedostot_nayta) {
                case 1:
                    if (tiedostot[ti].tyyppi == TIEDOSTO_MAP) {
                        if (Level_Editor_Linkki(tiedostot[ti].nimi, x + 5, y + linkki_y + 3, i))
                            Level_Editor_Kartta_Lataa(tiedostot[ti].nimi);
                        linkki_y += 9;
                    }
                    break;
                case 2:
                    if (tiedostot[ti].tyyppi == TIEDOSTO_BMP) {
                        if (Level_Editor_Linkki(tiedostot[ti].nimi, x + 5, y + linkki_y + 3, i)) {
                            Level_Editor_Aseta_Palikat(tiedostot[ti].nimi);
                            strcpy(message, "tile palette loaded from picture");
                        }
                        linkki_y += 9;
                    }
                    break;
                case 3:
                    if (tiedostot[ti].tyyppi == TIEDOSTO_SPR) {
                        if (Level_Editor_Linkki(tiedostot[ti].nimi, x + 5, y + linkki_y + 3, i))
                            if (Level_Editor_Prototyyppi_Lataa_Uusi(working_directory, tiedostot[ti].nimi) != 0)//..New
                            {
                                //strcpy(message,"could not load sprite ");
                                //strcat(message,tiedostot[ti].name);
                            }
                        linkki_y += 9;
                    }
                    break;
                case 4:
                    if (tiedostot[ti].tyyppi == TIEDOSTO_BMP) {
                        if (Level_Editor_Linkki(tiedostot[ti].nimi, x + 5, y + linkki_y + 3, i)) {
                            Level_Editor_Aseta_Taustakuva(tiedostot[ti].nimi);
                            strcpy(message, "background loaded from picture");
                        }
                        linkki_y += 9;
                    }
                    break;
                case 5:
                    if (tiedostot[ti].tyyppi == TIEDOSTO_MUS) {
                        if (Level_Editor_Linkki(tiedostot[ti].nimi, x + 5, y + linkki_y + 3, i)) {
                            //Level_Editor_Aseta_Taustakuva(tiedostot[ti].name);
                            strcpy(map->music, tiedostot[ti].nimi);
                            strcpy(message, "loaded music.");
                        }
                        linkki_y += 9;
                    }
                    break;
                default:
                    break;//linkki_y += 9; break;
            }

            /*
			if (Level_Editor_Linkki(tiedostot[ti].name,x+5,y+linkki_y+3))
			{
				if (tiedostot[ti].type == TIEDOSTO_DIR)
				{
					if (_chdir(tiedostot[ti].name) == 0)
					{
						_getcwd(working_directory, _MAX_PATH );
						paivita = true;
						strcpy(message,"changed directory");

					}
					else
						strcpy(message,"could not open directory");
				}

				if (tiedostot[ti].type == TIEDOSTO_MAP)
				{
					Level_Editor_Kartta_Lataa(tiedostot[ti].name);
				}

				if (tiedostot[ti].type == TIEDOSTO_BMP)
				{
					Level_Editor_Aseta_Palikat(tiedostot[ti].name);
					strcpy(message,"block palette loaded from picture");
				}

				if (tiedostot[ti].type == TIEDOSTO_SPR)
				{
					if (Level_Editor_Prototyyppi_Lataa_Uusi(tiedostot[ti].name) != 0)
					{
						strcpy(message,"could not load sprite ");
						strcat(message,tiedostot[ti].name);
					}
				}
			}*/
        }
        ti++;
    }

    Level_Editor_Viiva_Vert(x + menut[i].leveys - 102, y, 300, 41);

    if (Level_Editor_Nappi("save   ", x + menut[i].leveys - 96, y + 2, i)) {
        menut[MENU_SAVE].piilota = false;
        menu_tiedostot_eka = 0;

        if (strcmp(epd_hakemisto, " ") != 0) {
            if (_chdir(epd_hakemisto) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to last map folder");
                paivita = true;
            }
        }
    }

    Level_Editor_Viiva_Hori(x + menut[i].leveys - 102, y + 20, 100, 41);

    if (Level_Editor_Nappi("maps   ", x + menut[i].leveys - 96, y + 24, i)) {
        menu_tiedostot_nayta = 1;
        menu_tiedostot_eka = 0;

        if (strcmp(epd_hakemisto, " ") != 0) {
            if (_chdir(epd_hakemisto) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to last map folder");
                paivita = true;
            }
        }
    }

    if (Level_Editor_Nappi("tiles  ", x + menut[i].leveys - 96, y + 38, i)) {
        menu_tiedostot_nayta = 2;
        menu_tiedostot_eka = 0;

        if (strcmp(tile_hakemisto, " ") == 0)
            strcpy(tile_hakemisto, epd_hakemisto);

        if (strcmp(tile_hakemisto, " ") != 0) {
            if (_chdir(tile_hakemisto) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to last map folder");
                paivita = true;
            }
        }
    }

    if (Level_Editor_Nappi("sprites", x + menut[i].leveys - 96, y + 52, i)) {
        menu_tiedostot_nayta = 3;
        menu_tiedostot_eka = 0;

        if (strcmp(pk2_hakemisto, " ") != 0) {
            char temp[_MAX_DIR];
            strcpy(temp, pk2_hakemisto);
            strcat(temp, "\\sprites");
            if (_chdir(temp) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to sprites folder");
                paivita = true;
            }
        }
    }

    if (Level_Editor_Nappi("scenery", x + menut[i].leveys - 96, y + 66, i)) {
        menu_tiedostot_nayta = 4;
        menu_tiedostot_eka = 0;

        if (strcmp(bg_hakemisto, " ") == 0)
            strcpy(bg_hakemisto, epd_hakemisto);

        if (strcmp(bg_hakemisto, " ") != 0) {
            if (_chdir(bg_hakemisto) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to last map folder");
                paivita = true;
            }
        }
    }

    if (Level_Editor_Nappi("music  ", x + menut[i].leveys - 96, y + 80, i)) {
        menu_tiedostot_nayta = 5;
        menu_tiedostot_eka = 0;

        if (strcmp(pk2_hakemisto, " ") != 0) {
            char temp[_MAX_DIR];
            strcpy(temp, pk2_hakemisto);
            strcat(temp, "\\music");
            if (_chdir(temp) == 0) {
                _getcwd(working_directory, _MAX_PATH);
                strcpy(message, "moved to music folder");
                paivita = true;
            }
        }
    }

    if (nayta_hakemistot) {
        if (Level_Editor_Nappi("hide dirs", x + menut[i].leveys - 96, y + 110, i)) nayta_hakemistot = !nayta_hakemistot;
    } else {
        if (Level_Editor_Nappi("show dirs", x + menut[i].leveys - 96, y + 110, i)) nayta_hakemistot = !nayta_hakemistot;
    }

    if (paivita) {
        Level_Editor_Tiedostot_Alusta();
        Level_Editor_Tiedostot_Hae();
        Level_Editor_Tiedostot_Aakkosta();
    }

    int nuoli = Level_Editor_Piirra_Nuolet(x + menut[i].leveys - 15, y + menut[i].korkeus - 30, i);

    if (nuoli == 1 && menu_tiedostot_eka > 0) {
        menu_tiedostot_eka -= 15;
    }

    if (nuoli == 2 && menu_tiedostot_eka < MAX_DIR_TIEDOSTOJA - 1) {
        menu_tiedostot_eka += 15;
    }

    return 0;
}

int Level_Editor_Menu_Tallenna(int i) {
    int x = menut[i].x, y = menut[i].y + 16;

    PisteDraw_Font_Write(font1, "type the name of the level:", PD_BACKBUFFER, x + 3, y + 3);

    Level_Editor_Kysy(karttatiedosto, x + 3, y + 15, 12, KENTTA_FILE, i);

    if (Level_Editor_Nappi("save", x + 3, y + 30, i)) {
        Level_Editor_Kartta_Tallenna();
        Level_Editor_Tiedostot_Alusta();
        Level_Editor_Tiedostot_Hae();
        Level_Editor_Tiedostot_Aakkosta();
    }

    return 0;
}

int Level_Editor_Menu_Lopetus(int i) {
    int x = menut[i].x, y = menut[i].y + 16;

    PisteDraw_Font_Write(font1, "do you really want to quit?", PD_BACKBUFFER, x + 3, y + 3);

    if (Level_Editor_Nappi("quit", x + 3, y + 20, i)) {
        PisteDraw_Fade_Palette_Out(PD_FADE_FAST);
        exit_editor = true;
        key_delay = 15;
    }

    if (Level_Editor_Nappi("cancel", x + 45, y + 20, i)) {
        menut[i].piilota = true;
        key_delay = 15;
    }

    if (Level_Editor_Nappi("save and quit", x + 100, y + 20, i)) {
        if (Level_Editor_Kartta_Tallenna() == 0) {
            PisteDraw_Fade_Palette_Out(PD_FADE_FAST);
            exit_editor = true;
        }

        key_delay = 15;
    }

    return 0;
}

int Level_Editor_Menu_Tiedot(int i) {
    int x = menut[i].x, y = menut[i].y + 16, my = y + 19, vali = 0;

    char luku[20];

    vali = PisteDraw_Font_Write(font1, "version", PD_BACKBUFFER, x + 3, my);
    PisteDraw_Font_Write(font1, map->version, PD_BACKBUFFER, x + 3 + vali + 16, my);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "name:", PD_BACKBUFFER, x + 3, my);
    Level_Editor_Kysy(map->name, x + 83, my, 16, KENTTA_NIMI, i);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "file name:", PD_BACKBUFFER, x + 3, my);
    Level_Editor_Kysy(karttatiedosto, x + 83, my, 12, KENTTA_FILE, i);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "creator:", PD_BACKBUFFER, x + 3, my);
    Level_Editor_Kysy(map->author, x + 83, my, 29, KENTTA_TEKIJA, i);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "tile picture:", PD_BACKBUFFER, x + 3, my);
    PisteDraw_Font_Write(font1, map->block_bmp, PD_BACKBUFFER, x + 3 + vali + 16, my);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "background picture:", PD_BACKBUFFER, x + 3, my);
    PisteDraw_Font_Write(font1, map->background_image, PD_BACKBUFFER, x + 3 + vali + 16, my);
    my += 12;

    vali = PisteDraw_Font_Write(font1, "background music:", PD_BACKBUFFER, x + 3, my);
    //PisteDraw_Font_Write(font1,map->music, PD_BACKBUFFER, x+3+vali+16, my);my+=12;
    Level_Editor_Kysy(map->music, x + 3 + vali + 16, my, 12, KENTTA_MUSIIKKI, i);
    my += 12;

    int nuoli;
    nuoli = Level_Editor_Piirra_Nuolet2(x + 108, my, i);
    if (nuoli == 2 && map->stage > 0)
        map->stage--;
    if (nuoli == 1)
        map->stage++;

    if (map->stage < 0)
        map->stage = 0;

    itoa(map->stage, luku, 10);
    vali = PisteDraw_Font_Write(font1, "level:", PD_BACKBUFFER, x + 3, my);
    PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + 3 + vali + 16, my);
    my += 12;
    my += 10;

    nuoli = Level_Editor_Piirra_Nuolet2(x + 108, my, i);
    if (nuoli == 2 && map->time > 0)
        map->time -= 10;
    if (nuoli == 1)
        map->time += 10;

    if (map->time < 0)
        map->time = 0;

    int min = map->time / 60, sek = map->time % 60;

    vali = PisteDraw_Font_Write(font1, "time:", PD_BACKBUFFER, x + 3, my);
    if (min < 10)
        vali += PisteDraw_Font_Write(font1, "0", PD_BACKBUFFER, x + 3 + vali + 16, my);
    itoa(min, luku, 10);
    vali += PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + 3 + vali + 16, my);

    if (sek < 10)
        vali += PisteDraw_Font_Write(font1, ":0", PD_BACKBUFFER, x + 3 + vali + 16, my);
    else
        vali += PisteDraw_Font_Write(font1, ":", PD_BACKBUFFER, x + 3 + vali + 16, my);

    itoa(sek, luku, 10);
    vali += PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + 3 + vali + 16, my);

    my += 16;


    char tausta[100];

    switch (map->background) {
        case BACKGROUND_STATIC                :
            strcpy(tausta, "no scrolling  ");
            break;
        case BACKGROUND_PARALLAX_HORI            :
            strcpy(tausta, "left and right");
            break;
        case BACKGROUND_PARALLAX_VERT            :
            strcpy(tausta, "up and down   ");
            break;
        case BACKGROUND_PARALLAX_VERT_AND_HORI    :
            strcpy(tausta, "free scrolling");
            break;
        default                                :
            break;
    }

    vali = PisteDraw_Font_Write(font1, "background scrolling:", PD_BACKBUFFER, x + 3, my);
    if (Level_Editor_Nappi(tausta, x + 3 + vali + 16, my, i)) {
        map->background++;
        map->background %= 4;
    }
    my += 16;

    switch (map->climate) {
        case WEATHER_NORMAL                    :
            strcpy(tausta, "normal       ");
            break;
        case WEATHER_RAIN                        :
            strcpy(tausta, "rain         ");
            break;
        case WEATHER_FOREST                        :
            strcpy(tausta, "leaves       ");
            break;
        case WEATHER_RAIN_FOREST                    :
            strcpy(tausta, "rain + leaves");
            break;
        case WEATHER_SNOW                    :
            strcpy(tausta, "snow         ");
            break;
        default                                :
            break;
    }

    vali = PisteDraw_Font_Write(font1, "special:", PD_BACKBUFFER, x + 3, my);
    if (Level_Editor_Nappi(tausta, x + 3 + vali + 16, my, i)) {
        map->climate++;
        map->climate %= 5;
    }

    my += 19;

    /* Kartan icon */
    vali = PisteDraw_Font_Write(font1, "map icon:", PD_BACKBUFFER, x + 3, my);
    itoa(map->icon + 1, luku, 10);
    PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + 3 + vali + 16, my);

    nuoli = Level_Editor_Piirra_Nuolet2(x + 118, my, i);
    if (nuoli == 1)
        map->icon++;
    if (nuoli == 2)
        map->icon--;

    if (map->icon < 0)
        map->icon = 0;

    my += 16;

    /* Kartan x-kordinaatti */
    vali = PisteDraw_Font_Write(font1, "map x:", PD_BACKBUFFER, x + 3, my);
    itoa(map->x, luku, 10);
    PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + vali + 9, my);
    nuoli = Level_Editor_Piirra_Nuolet2(x + 100, my, i);
    if (nuoli == 1) map->x += 15;
    if (nuoli == 2) map->x -= 15;
    if (map->x < 0) map->x = 0;
    if (map->x > 620) map->x = 620;

    vali = PisteDraw_Font_Write(font1, "map y:", PD_BACKBUFFER, x + 3 + 150, my);
    itoa(map->y, luku, 10);
    PisteDraw_Font_Write(font1, luku, PD_BACKBUFFER, x + vali + 9 + 150, my);
    nuoli = Level_Editor_Piirra_Nuolet2(x + 100 + 150, my, i);
    if (nuoli == 1) map->y += 15;
    if (nuoli == 2) map->y -= 15;
    if (map->y < 0) map->y = 0;
    if (map->y > 620) map->y = 620;

    my += 9;

    /*
	vali = PisteDraw_Font_Kirjoita(font1,"block palette:", PD_BACKBUFFER, x+3, my);
	PisteDraw_Font_Write(font1,map->name, PD_BACKBUFFER, x+3+vali+16, my);my+=9;
	*/
    return 0;
}

int Level_Editor_Menu_Tools(int i) {
    int x = menut[i].x, y = menut[i].y + 16;

    //PisteDraw_Font_Write(font1,"", PD_BACKBUFFER, x+3, y+3);

    //Level_Editor_Kysy(karttatiedosto/*map->name*/,x+3,y+15,12, KENTTA_FILE);

    if (Level_Editor_Nappi("clear sprites", x + 10, y + 10, i)) {
        if (i == aktiivinen_menu) {
            Level_Editor_Kartta_Varmista();
            Level_Editor_Prototyyppi_Tyhjenna();
        }
    }

    y += 25;

    if (Level_Editor_Nappi("clear unused sprites", x + 10, y + 10, i)) {
        if (i == aktiivinen_menu) {
            Level_Editor_Kartta_Varmista();
            Level_Editor_Prototyyppi_Poista_Turhat();
            menu_spritet_eka = 0;
            //Level_Editor_Prototyyppi_Tyhjenna();
        }
    }

    y += 25;

    if (Level_Editor_Nappi("clear all", x + 10, y + 10, i)) {
        if (i == aktiivinen_menu) {
            Level_Editor_Kartta_Varmista();
            Level_Editor_Kartta_Alusta();
        }
    }

    y += 25;

    return 0;
}

int Level_Editor_Menu_Loki(int i) {
    int x = menut[i].x, y = menut[i].y + 16;

    int yl = 0;

    for (int l = 0; l < MAX_LOKIMERKINTOJA; l++) {

        if (loki[l].tyyppi == LOKI_VIRHE) {
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, x + 3, y + 3 + l * 9 + 3, x + 7, y + 7 + l * 9 + 3, 20 + 64);
        }

        if (loki[l].tyyppi == LOKI_INFO) {
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, x + 3, y + 3 + l * 9 + 3, x + 7, y + 7 + l * 9 + 3, 20 + 96);
        }

        PisteDraw_Font_Write(font1, loki[l].teksti, PD_BACKBUFFER, x + 3 + 8, y + 3 + l * 9);

    }


    return 0;
}

int Level_Editor_Menu_Piirra(int index, bool tayta, unsigned char vari) {
    int leveys = menut[index].leveys;
    int korkeus = menut[index].korkeus;

    leveys = leveys - 14;

    //PisteDraw_Buffer_Clear(PD_BACKBUFFER, menut[index].x-1, menut[index].y-1,
    //					   menut[index].x+menut[index].width+1, menut[index].y+16+height, 0);

    Level_Editor_Piirra_Nelio(menut[index].x - 1, menut[index].y - 1, menut[index].x + menut[index].leveys + 1,
                              menut[index].y + 16 + korkeus, 0);

    if (tayta) {
        double kork = menut[index].korkeus / 6.0;
        double ky = 0;
        for (int c = 0; c < 6; c++) {
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, menut[index].x, menut[index].y + 15 + int(ky),
                                   menut[index].x + menut[index].leveys, menut[index].y + 15 + int(ky + kork),
                                   6 - c + vari);
            ky += kork;
        }
    }

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, menut[index].x, menut[index].y, 1, 1, 5, 16);

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, menut[index].x + 4, menut[index].y, 6, 1, 6 + leveys,
                               16);

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, menut[index].x + 2 + leveys, menut[index].y, 311, 1, 323,
                               16);

    if (index == aktiivinen_menu)
        PisteDraw_Font_Write(font1, menut[index].otsikko, PD_BACKBUFFER, menut[index].x + 5, menut[index].y + 3);
    else
        PisteDraw_Font_Write_Transparent(font1, menut[index].otsikko, PD_BACKBUFFER, menut[index].x + 5,
                                         menut[index].y + 3, 50);

/*
	PisteDraw_Buffer_Clear(PD_BACKBUFFER, menut[index].x, menut[index].y+15,
						   menut[index].x+menut[index].width, menut[index].y+16+height, 0);
*/

    switch (index) {
        case MENU_HELP        :
            Level_Editor_Menu_Help(index);
            break;
        case MENU_KARTTA    :
            Level_Editor_Menu_Kartta(index);
            break;
        case MENU_PALIKAT    :
            Level_Editor_Menu_Palikat(index);
            break;
        case MENU_SPRITET    :
            Level_Editor_Menu_Spritet(index);
            break;
        case MENU_TIEDOSTOT    :
            Level_Editor_Menu_Tiedostot(index);
            break;
        case MENU_SAVE        :
            Level_Editor_Menu_Tallenna(index);
            break;
        case MENU_INFO        :
            Level_Editor_Menu_Tiedot(index);
            break;
        case MENU_TOOLS        :
            Level_Editor_Menu_Tools(index);
            break;
        case MENU_LOKI        :
            Level_Editor_Menu_Loki(index);
            break;
        case MENU_EXIT        :
            Level_Editor_Menu_Lopetus(index);
            break;
            //case MENU_TYOKALUT	: Level_Editor_Menu_Tyokalut(index);break;
        default                :
            break;
    }

    return 0;
}

int Level_Editor_Menut_Piirra(void) {
    for (int i = 0; i < MAX_MENUJA; i++) {
        if (!menut[i].piilota && i != aktiivinen_menu)
            Level_Editor_Menu_Piirra(i, true, COLOR_BLUE);
    }

    if (aktiivinen_menu != MENU_EI && aktiivinen_menu < MAX_MENUJA)
        Level_Editor_Menu_Piirra(aktiivinen_menu, true, COLOR_BLUE);

    return 0;
}

int Level_Editor_Piirra_Spritet(void) {
    int proto;

    for (int x = 0; x < SCREEN_WIDTH / 32; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / 32; y++) {
            proto = map->sprites[x + kartta_x + (y + kartta_y) * MAP_WIDTH];

            if (proto != 255 && prototypes[proto].type != TYPE_NOTHING) {
                prototypes[proto].Draw(x * 32 + 16 - prototypes[proto].width / 2,
                                       y * 32 - prototypes[proto].height + 32, 0);
            }
        }
    }
    return 0;
}

int Level_Editor_Piirra_Kartta(void) {
    /*
	int palikka;
	int px = 0,
		py = 0;*/

    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,640,480);

    if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_TAUSTA)
        map->Draw_Backgrounds(kartta_x * 32, kartta_y * 32, animaatio_paalla);

    Level_Editor_Piirra_Spritet();

    if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT)
        map->Draw_Walls(kartta_x * 32, kartta_y * 32, animaatio_paalla);

    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    return 0;
}

/* T�ss� editoidaan kartan background ja seina taulukoita */

int Level_Editor_Edit_Kartta(void) {
    int x = kartta_x + mouse_x / 32;
    int y = kartta_y + mouse_y / 32;

    if (y > 223)
        y = 223;

    if (edit_tila == EDIT_SPRITE || edit_tila == EDIT_KARTTA) {
        if (!PisteInput_Mouse_Left() && !PisteInput_Mouse_Right())
            piirto_aloitettu = false;

        if ((PisteInput_Mouse_Left() || PisteInput_Mouse_Right()) && piirto_aloitettu == false) {
            piirto_aloitettu = true;
            Level_Editor_Kartta_Varmista();
        }
    } else
        piirto_aloitettu = false;


    if (edit_tila == EDIT_KARTTA) {
        focustile_etu = map->walls[x + y * MAP_WIDTH];
        focustile_taka = map->backgrounds[x + y * MAP_WIDTH];

        if (PisteInput_Mouse_Left() && key_delay == 0) {

            if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT) {
                map->walls[x + y * MAP_WIDTH] = edit_palikka;
            }

            if (edit_kerros == EDIT_KERROS_TAUSTA) {
                map->backgrounds[x + y * MAP_WIDTH] = edit_palikka;
            }

            Level_Editor_Kartta_Paivita();
            strcpy(message, "map was updated.");
            key_delay = 5;
        }

        if (PisteInput_Mouse_Right() && key_delay == 0) {


            if (edit_kerros == EDIT_KERROS_SEINAT || edit_kerros == EDIT_KERROS_VAIN_SEINAT) {
                map->walls[x + y * MAP_WIDTH] = 255;
            }

            if (edit_kerros == EDIT_KERROS_TAUSTA) {
                map->backgrounds[x + y * MAP_WIDTH] = 255;
            }

            //Level_Editor_Lista_Nayta(leikepoyta_lista, mouse_x, mouse_y);

            Level_Editor_Kartta_Paivita();
            strcpy(message, "map was updated.");
            key_delay = 5;
        }

    }

    if (edit_tila == EDIT_SPRITE) {
        focussprite = map->sprites[x + y * MAP_WIDTH];

        if (PisteInput_Mouse_Left() && key_delay == 0) {
            map->sprites[x + y * MAP_WIDTH] = proto_valittu;
            Level_Editor_Kartta_Paivita();
            strcpy(message, "map was updated.");
            key_delay = 5;
        }

        if (PisteInput_Mouse_Right() && key_delay == 0) {
            map->sprites[x + y * MAP_WIDTH] = 255;
            Level_Editor_Kartta_Paivita();
            strcpy(message, "map was updated.");
            key_delay = 5;
        }
    }

    return 0;
}

int Level_Editor_Piirra_Infoa(void) {
    int vali = 0;
    char luku[40];

    if (edit_tila == EDIT_KARTTA) {
        vali += PisteDraw_Font_Write_Transparent(font1, "x: ", PD_BACKBUFFER, 2 + vali, 2, 65);

        itoa(kartta_x + mouse_x / 32, luku, 10);

        vali += 8 + PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 2 + vali, 2, 65);

        vali += PisteDraw_Font_Write_Transparent(font1, "y: ", PD_BACKBUFFER, 2 + vali, 2, 65);

        itoa(kartta_y + mouse_y / 32, luku, 10);

        vali += 10 + PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 2 + vali, 2, 65);


        if (focustile_etu != 255)
            itoa(focustile_etu + 1, luku, 10);
        else
            strcpy(luku, "n/movement_x");
        PisteDraw_Font_Write_Transparent(font1, "foreground tile: ", PD_BACKBUFFER, 2, 14, 65);
        PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 150, 14, 65);

        if (focustile_taka != 255)
            itoa(focustile_taka + 1, luku, 10);
        else
            strcpy(luku, "n/movement_x");
        PisteDraw_Font_Write_Transparent(font1, "background tile: ", PD_BACKBUFFER, 2, 24, 65);
        PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 150, 24, 65);

    }

    if (edit_tila == EDIT_SPRITE) {
        PisteDraw_Font_Write_Transparent(font1, "sprite: ", PD_BACKBUFFER, 2, 14, 65);
        if (focussprite != 255)
            PisteDraw_Font_Write_Transparent(font1, prototypes[focussprite].name, PD_BACKBUFFER, 90, 14, 65);
        else
            PisteDraw_Font_Write_Transparent(font1, "n/movement_x", PD_BACKBUFFER, 90, 14, 65);
    }

    vali += PisteDraw_Font_Write_Transparent(font1, map->name, PD_BACKBUFFER, 2 + vali, 2, 65);

    PisteDraw_Font_Write_Transparent(font1, message, PD_BACKBUFFER, 320, 2, 65);

    if (edit_kerros == EDIT_KERROS_SEINAT)
        PisteDraw_Font_Write_Transparent(font1, "layer: both", PD_BACKBUFFER, 320, 12, 65);
    if (edit_kerros == EDIT_KERROS_TAUSTA)
        PisteDraw_Font_Write_Transparent(font1, "layer: only background", PD_BACKBUFFER, 320, 12, 65);
    if (edit_kerros == EDIT_KERROS_VAIN_SEINAT)
        PisteDraw_Font_Write_Transparent(font1, "layer: only foreground", PD_BACKBUFFER, 320, 12, 65);

    PisteDraw_Font_Write_Transparent(font1,
                                     "f1=help, f2=map, f3=tiles, f4=sprites, f5=files, f6=save map, f7=map info, f8=tools, f9=log, esc=exit",
                                     PD_BACKBUFFER, 2, SCREEN_HEIGHT - 10, 75);

    //PisteDraw_Font_Write_Transparent(font1,working_directory,PD_BACKBUFFER,2,SCREEN_HEIGHT-25,65);
    PisteDraw_Font_Write_Transparent(font1, PK2Map::pk2_directory, PD_BACKBUFFER, 2, SCREEN_HEIGHT - 25, 65);

    vali = PisteDraw_Font_Write_Transparent(font1, "sprites: ", PD_BACKBUFFER, 640, 2, 65);
    itoa(spriteja, luku, 10);
    vali += PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 640 + vali, 2, 65);
    vali += PisteDraw_Font_Write_Transparent(font1, "/", PD_BACKBUFFER, 640 + vali, 2, 65);
    itoa(MAX_SPRITES, luku, 10);
    vali += PisteDraw_Font_Write_Transparent(font1, luku, PD_BACKBUFFER, 640 + vali, 2, 65);
    /*
	if (Level_Editor_Nappi("help (f1)",20,SCREEN_HEIGHT-20))
		menut[MENU_HELP].is_hidden = !menut[MENU_HELP].is_hidden;

	if (Level_Editor_Nappi("map (f2)",110,SCREEN_HEIGHT-20))
		menut[MENU_KARTTA].is_hidden = !menut[MENU_KARTTA].is_hidden;
*/
    return 0;
}

int Level_Editor_Piirra_Kursori(void) {
    int x, y;
    x = (mouse_x / 32) * 32;
    y = (mouse_y / 32) * 32;

    if (x > SCREEN_WIDTH - 32)
        x = SCREEN_WIDTH - 32;
    if (y > SCREEN_HEIGHT - 32)
        y = SCREEN_HEIGHT - 32;

    int px = ((edit_palikka % 10) * 32);
    int py = ((edit_palikka / 10) * 32);

    if (edit_tila == EDIT_KARTTA) {
        Level_Editor_Piirra_Nelio(x + 1, y + 1, x + 33, y + 33, 0);
        PisteDraw_Buffer_Flip_Fast(map->block_buffer, PD_BACKBUFFER, x, y, px, py, px + 32, py + 32);
        Level_Editor_Piirra_Nelio(x, y, x + 32, y + 32, 57);
    }

    if (edit_tila == EDIT_SPRITE) {
        prototypes[proto_valittu].Draw(x + 16 - prototypes[proto_valittu].width / 2,
                                       y - prototypes[proto_valittu].height + 32, 0);
        //prototypes[proto_valittu].Draw(x,y-prototypes[proto_valittu].height+32,0);
    }

    PisteDraw_Buffer_Flip_Fast(kuva_editori, PD_BACKBUFFER, mouse_x, mouse_y, 1, 33, 19, 51);

    return 0;
}

int Level_Editor_Piirra(void) {
    int kytkin_anim;

    if (!animaatio_paalla) {
        if (block_animation < 64)
            kytkin_anim = block_animation;
        else
            kytkin_anim = SWITCH_START_VALUE - 64;

        Map_Animate(degree, block_animation / 20, SWITCH_START_VALUE - kytkin_anim,
                    SWITCH_START_VALUE - kytkin_anim, SWITCH_START_VALUE - kytkin_anim, true);
    } else
        Map_Animate(degree, block_animation / 20, 0, 0, 0, false);

    block_animation = 1 + block_animation % 99;//100

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 37);

    //if (kartta_ladattu)
    //PisteDraw_Buffer_Flip_Fast(map->background_image_buffer,PD_BACKBUFFER,0,0);

    switch (map->background) {
        case BACKGROUND_STATIC                :
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0);
            break;
        case BACKGROUND_PARALLAX_HORI            :
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 0);
            break;
        case BACKGROUND_PARALLAX_VERT            :
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 480);
            break;
        case BACKGROUND_PARALLAX_VERT_AND_HORI    :
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 0);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 480);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 480);
            break;
        default                                :
            break;
    }

    Level_Editor_Piirra_Kartta();

    Level_Editor_Menut_Piirra();

    Level_Editor_Piirra_Infoa();

    Level_Editor_Leikepoyta_Piirra();

    if (leikepoyta_lista.nakyva)
        Level_Editor_Lista(leikepoyta_lista);

    Level_Editor_Piirra_Kursori();

    if (aktiivinen_menu == MENU_EI)
        Level_Editor_Edit_Kartta();

    PisteWait_Wait(10);

    PisteDraw_UpdateScreen();

    PisteWait_Start();

    //Level_Editor_Loki_Tallenna("Piirtorutiinit on suoritettu.");

    return 0;
}

/* MUUT RUTIINIT -------------------------------------------------------*/


int Level_Editor_Menut(void) {

//	aktiivinen_menu = MENU_EI;
    bool aktiivisia = false;

    //if (PisteInput_Read_Keyboard())
    //	aktiivinen_menu

    for (int i = 0; i < MAX_MENUJA; i++) {
        if (PisteInput_KeyDown(menut[i].pika) && key_delay == 0) {
            if (menut[i].piilota)
                menut[i].piilota = false;
            else
                menut[i].piilota = true;

            key_delay = 15;
        }

        if (menut[i].x < 0)
            menut[i].x = 0;

        if (menut[i].x + menut[i].leveys > SCREEN_WIDTH)
            menut[i].x -= (menut[i].x + menut[i].leveys) - SCREEN_WIDTH;

        if (menut[i].y < 0)
            menut[i].y = 0;

        if (menut[i].y + menut[i].korkeus + 16 > SCREEN_HEIGHT)
            menut[i].y -= (menut[i].y + 16 + menut[i].korkeus) - SCREEN_HEIGHT;

        if (aktiivinen_menu == i) {
            /*
			if (PisteInput_Mouse_Left())
				liikuta_ikkunaa = true;
			else
				liikuta_ikkunaa = false;*/

            if ((mouse_x > menut[i].x) && (mouse_x < menut[i].x + menut[i].leveys) && (mouse_y > menut[i].y) &&
                (mouse_y < menut[i].y + 16) && PisteInput_Mouse_Left()) {
                //menut[i].x += PisteInput_Mouse_X(0);
                //menut[i].y += PisteInput_Mouse_Y(0);
                liikuta_ikkunaa = true;
                //menut[i].x
            }

            if (!PisteInput_Mouse_Left())
                liikuta_ikkunaa = false;

            if (liikuta_ikkunaa) {
                menut[i].x += PisteInput_Mouse_X(0);
                menut[i].y += PisteInput_Mouse_Y(0);
                aktiivisia = true;
                //liikuta_ikkunaa = true;
                //menut[i].x
            }

            if ((mouse_x > menut[i].x + menut[i].leveys - 10) && (mouse_x < menut[i].x + menut[i].leveys) &&
                (mouse_y > menut[i].y) && (mouse_y < menut[i].y + 16) && PisteInput_Mouse_Left()) {
                menut[i].piilota = true;
                key_delay = 15;
            }

        }

        if ((mouse_x > menut[i].x) && (mouse_x < menut[i].x + menut[i].leveys) && (mouse_y > menut[i].y) &&
            (mouse_y < menut[i].y + menut[i].korkeus + 16) && !menut[i].piilota &&
            (aktiivinen_menu == MENU_EI || i == aktiivinen_menu)) {
            aktiivinen_menu = i;
            aktiivisia = true;
        }

    }

    if (!aktiivisia) {
        aktiivinen_menu = MENU_EI;
        liikuta_ikkunaa = false;
    }

    if (aktiivinen_menu != MENU_EI) {
        edit_tila = EDIT_MENU;
    } else {
        if (proto_valittu == MAX_PROTOTYPES)
            edit_tila = EDIT_KARTTA;
        else
            edit_tila = EDIT_SPRITE;

    }

    return 0;
}

int Level_Editor_Kursori(void) {
    mouse_x = PisteInput_Mouse_X(mouse_x);
    mouse_y = PisteInput_Mouse_Y(mouse_y);

    if (mouse_x > SCREEN_WIDTH)
        mouse_x = SCREEN_WIDTH;

    if (mouse_x < 0)
        mouse_x = 0;

    if (mouse_y > SCREEN_HEIGHT)
        mouse_y = SCREEN_HEIGHT;

    if (mouse_y < 0)
        mouse_y = 0;

    return 0;
}

int Level_Editor_Reunat(void) {

    if (mouse_x < 2)
        kartta_x -= 1;

    if (mouse_x > SCREEN_WIDTH - 2)
        kartta_x += 1;

    if (mouse_y < 2)
        kartta_y -= 1;

    if (mouse_y > SCREEN_HEIGHT - 2)
        kartta_y += 1;

    if (!editoi_tekstia) {
        if (PisteInput_KeyDown(DIK_LEFT) && key_delay == 0) {
            kartta_x--;
            key_delay = 5;
        }

        if (PisteInput_KeyDown(DIK_RIGHT) && key_delay == 0) {
            kartta_x++;
            key_delay = 5;
        }
    }

    if (PisteInput_KeyDown(DIK_UP) && key_delay == 0) {
        kartta_y--;
        key_delay = 5;
    }

    if (PisteInput_KeyDown(DIK_DOWN) && key_delay == 0) {
        kartta_y++;
        key_delay = 5;
    }

    if (kartta_x < 0)
        kartta_x = 0;

    if (kartta_x > MAP_WIDTH - SCREEN_WIDTH / 32)
        kartta_x = MAP_WIDTH - SCREEN_WIDTH / 32;

    if (kartta_y < 0)
        kartta_y = 0;

    if (kartta_y > MAP_HEIGHT - SCREEN_HEIGHT / 32)
        kartta_y = MAP_HEIGHT - SCREEN_HEIGHT / 32;

    return 0;
}

int Level_Editor_Main(void) {
    if (window_closed)
        return (0);

    /* HAETAAN N�PP�IMIST�N, HIIREN JA PELIOHJAINTEN T�M�NHETKISET TILAT */

    // N�pp�imist�
    if (!PisteInput_Update_Keyboard())        //Haetaan n�pp�inten tilat
        DirectX_virhe = true;

    // Hiirulainen
    if (!PisteInput_Update_Mouse())            //Haetaan hiiren tila
        DirectX_virhe = true;

    //editoi_tekstia = false;

    Level_Editor_Kursori();

    Level_Editor_Reunat();

    editoi_tekstia = false;

    Level_Editor_Piirra();

    Level_Editor_Menut();

    if (key_delay > 0)
        key_delay--;

    degree = 1 + degree % 359;

    if (PisteInput_KeyDown(DIK_RALT)) {
        if (!aseta_leikepoyta_alue) {
            leikepoyta_alue.left = (mouse_x / 32) + kartta_x;
            leikepoyta_alue.top = (mouse_y / 32) + kartta_y;
            aseta_leikepoyta_alue = true;
        }

        leikepoyta_alue.right = (mouse_x / 32) + kartta_x;
        leikepoyta_alue.bottom = (mouse_y / 32) + kartta_y;

        long vaihto;

        if (leikepoyta_alue.right < leikepoyta_alue.left) {
            vaihto = leikepoyta_alue.right;
            leikepoyta_alue.right = leikepoyta_alue.left;
            leikepoyta_alue.left = vaihto;
        }

        if (leikepoyta_alue.bottom < leikepoyta_alue.top) {
            vaihto = leikepoyta_alue.top;
            leikepoyta_alue.top = leikepoyta_alue.bottom;
            leikepoyta_alue.bottom = vaihto;
        }

    } else {
        if (aseta_leikepoyta_alue) {
            aseta_leikepoyta_alue = false;
            //Level_Editor_Lista_Nayta(leikepoyta_lista,mouse_x,mouse_y);
        }
    }


    if (key_delay == 0 && !editoi_tekstia) {
        if (PisteInput_KeyDown(DIK_LCONTROL)) {
            //Level_Editor_Kartta_Varmista();

            if (PisteInput_KeyDown(DIK_C)) {

                Level_Editor_Leikepoyta_Kopioi(leikepoyta_alue);
                key_delay = 20;
            }
            if (PisteInput_KeyDown(DIK_V)) {

                Level_Editor_Leikepoyta_Liita(leikepoyta_alue);
                key_delay = 20;
            }
            if (PisteInput_KeyDown(DIK_X)) {

                Level_Editor_Leikepoyta_Leikkaa(leikepoyta_alue);
                key_delay = 20;
            }
            if (PisteInput_KeyDown(DIK_B)) {
                RECT temp = {mouse_x / 32 + kartta_x, mouse_y / 32 + kartta_y, mouse_x / 32 + kartta_x,
                             mouse_y / 32 + kartta_y};
                Level_Editor_Leikepoyta_Liita_Koko(temp);
                key_delay = 20;
            }
        }
/*
		if (PisteInput_Keydown(DIK_F9))
		{
			Level_Editor_Kartta_Varmista();
			Level_Editor_Prototyyppi_Tyhjenna();
			key_delay = 20;
		}
/*
		if (PisteInput_KeyDown(DIK_F8))
		{
			Level_Editor_Kartta_Varmista();
			Level_Editor_Kartta_Alusta();
			key_delay = 20;
		}
*/

        if (PisteInput_KeyDown(DIK_S)) {
            Level_Editor_Kartta_Tallenna();
            key_delay = 20;
        }

        if (PisteInput_KeyDown(DIK_A)) {
            animaatio_paalla = !animaatio_paalla;
            key_delay = 20;
        }

        if (PisteInput_KeyDown(DIK_U)) {
            Level_Editor_Kartta_Undo();
            Level_Editor_Kartta_Paivita();
            key_delay = 20;
        }

        if (PisteInput_KeyDown(DIK_RSHIFT)) {
            switch (edit_kerros) {
                case EDIT_KERROS_SEINAT:
                    edit_kerros = EDIT_KERROS_VAIN_SEINAT;
                    break;
                case EDIT_KERROS_VAIN_SEINAT:
                    edit_kerros = EDIT_KERROS_TAUSTA;
                    break;
                case EDIT_KERROS_TAUSTA:
                    edit_kerros = EDIT_KERROS_SEINAT;
                    break;
                default:
                    break;
            }

            key_delay = 20;
        }

    }

    if (exit_editor) {
        exit_editor = true;
    }

    if (exit_editor && PisteDraw_Fade_Palette_Done()) {
        SendMessage(window_handle, WM_CLOSE, 0, 0);
        window_closed = true;
    }

    return 0;
}


int Level_Editor_Quit(void) {

    //delete map;
    //delete undo;

    PisteDraw_Quit();
    PisteInput_Quit();

    if (DirectX_virhe) {
        strcpy(virheviesti, PisteDraw_GetError());
        MessageBox(window_handle, virheviesti, "PK2 LevelEditor", MB_OK | MB_ICONEXCLAMATION);
    }

    Level_Editor_Asetukset_Tallenna();

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
char backupfile[_MAX_PATH];


switch(msg)
{
case WM_ACTIVATE:
{
if (((BOOL)
HIWORD(wparam)
!= 0 ||
LOWORD(wparam)
== WA_INACTIVE) && !exit_editor) {

if (map != NULL) {
strcpy(karttatiedosto,
"backup.map");

/*
					strcpy(backupfile,"");
					strcpy(backupfile,editor_hakemisto);
					strcat(backupfile,"\\backup2.mop");
					map->Save(backupfile);
					*/
Level_Editor_Kartta_Tallenna();

PostQuitMessage(0);
window_closed = true;
}}

if (((BOOL)
HIWORD(wparam)
!= 0 ||
LOWORD(wparam)
== WA_INACTIVE) && exit_editor) {

if (map != NULL) {
strcpy(backupfile,
"");
strcpy(backupfile, editor_hakemisto
);
strcat(backupfile,
"\\backup.mop");
map->Save(backupfile);
PostQuitMessage(0);
window_closed = true;
}}}

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

int IDI_ICON1;

int IDC_CURSOR1;

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

Level_Editor_Init();

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

Level_Editor_Main();

}

Level_Editor_Quit();

ShowCursor(TRUE);

return(msg.wParam);
} 

