//#include <windows.h>
//#include <windowsx.h>
//#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
//#include <io.h>
//#include <fstream.h>
//#include <iostream.h>

#include "map.h"
#include "Engine/draw.h"

#include "old_headers.h"

double *map_cos_table;
double *map_sin_table;

int aste, vesiaste = 0, animaatio, ajastin1, ajastin2, ajastin3, avaimet;
int ruudun_leveys_palikoina = 21, ruudun_korkeus_palikoina = 16;

struct PK2KARTTA {    // Vanha version 0.1
    char versio[8];
    char nimi[40];
    unsigned char taustakuva;
    unsigned char musiikki;
    unsigned char kartta[640 * 224];
    unsigned char palikat[320 * 256];
    unsigned char extrat[640 * 480];
};

// TODO: =>
bool PK2Kartta_Onko_File(char *filename) {
    ifstream *file = new ifstream(filename, ios::binary);// | ios::nocreate);
    if (file->fail()) {
        delete (file);
        return false;
    }
    delete (file);
    return true;
}

void Map_Cos_Sin(double *cos_value, double *sin_value) {
    map_cos_table = cos_value;
    map_sin_table = sin_value;
}

// TODO: =>
void Map_Animate(int degree, int anim, int time1, int time2, int time3, bool keys) {
    aste = degree;
    animaatio = anim;
    ajastin1 = time1;
    ajastin2 = time2;
    ajastin3 = time3;
    avaimet = keys;
}

// TODO: =>
void Map_Set_Screen_Dimensions(int width, int height) {
    ruudun_leveys_palikoina = width / 32 + 1;
    ruudun_korkeus_palikoina = height / 32 + 1;
}

char PK2Map::pk2_directory[256] = "";

PK2Map::PK2Map() {
    strcpy(this->version, LATEST_MAP_VERSION);
    strcpy(this->block_bmp, "blox.bmp");
    strcpy(this->background_image, "default.bmp");
    strcpy(this->music, "default.xm");

    strcpy(this->name, "untitled");
    strcpy(this->author, "unknown");

    this->stage = 0;
    this->climate = WEATHER_NORMAL;
    this->switch1_time = SWITCH_START_VALUE;
    this->switch2_time = SWITCH_START_VALUE;
    this->switch3_time = SWITCH_START_VALUE;
    this->player_sprite = 0;
    this->time = 0;
    this->extra = NO_EXTRA;
    this->background = BACKGROUND_STATIC;

    this->x = 0;
    this->y = 0;
    this->icon = 0;

    memset(this->backgrounds, 255, sizeof(backgrounds));
    memset(this->walls, 255, sizeof(walls));
    memset(this->sprites, 255, sizeof(sprites));
    memset(this->edges, 0, sizeof(edges));

    for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
        strcpy(this->protos[i], "");
    this->background_image_buffer = PisteDraw_Buffer_Create(640, 480, true, 255);
    this->block_buffer = PisteDraw_Buffer_Create(BLOCK_PALETTE_WIDTH, BLOCK_PALETTE_HEIGHT, true, 255);
    PisteDraw_Buffer_Clear(this->background_image_buffer, 0);
    PisteDraw_Buffer_Clear(this->block_buffer, 0);
    this->water_block_buffer = PisteDraw_Buffer_Create(BLOCK_PALETTE_WIDTH, 32, true, 255);
}

PK2Map::PK2Map(const PK2Map &map) {
    strcpy(this->version, map.version);
    strcpy(this->block_bmp, map.block_bmp);
    strcpy(this->background_image, map.background_image);
    strcpy(this->music, map.music);
    strcpy(this->name, map.name);
    strcpy(this->author, map.author);

    this->stage = map.stage;
    this->climate = map.climate;
    this->switch1_time = map.switch1_time;
    this->switch2_time = map.switch2_time;
    this->switch3_time = map.switch3_time;
    this->player_sprite = map.player_sprite;
    this->time = map.time;
    this->extra = map.extra;
    this->background = map.background;
    this->x = map.x;
    this->y = map.y;
    this->icon = map.icon;
    for (int i = 0; i < MAP_SIZE; i++)
        this->backgrounds[i] = map.backgrounds[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->walls[i] = map.walls[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->sprites[i] = map.sprites[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->edges[i] = map.edges[i];
    for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
        strcpy(this->protos[i], map.protos[i]);
    this->background_image_buffer = PisteDraw_Buffer_Create(640, 480, true, 255);
    this->block_buffer = PisteDraw_Buffer_Create(BLOCK_PALETTE_WIDTH, BLOCK_PALETTE_HEIGHT, true, 255);
    this->water_block_buffer = PisteDraw_Buffer_Create(BLOCK_PALETTE_WIDTH, 32, true, 255);
    PisteDraw_Buffer_Flip_Fast(map.block_buffer, this->block_buffer, 0, 0);
    PisteDraw_Buffer_Flip_Fast(map.background_image_buffer, this->background_image_buffer, 0, 0);
    PisteDraw_Buffer_Flip_Fast(map.water_block_buffer, this->water_block_buffer, 0, 0);
}

PK2Map::~PK2Map() {
    PisteDraw_Buffer_Destroy(this->block_buffer);
    PisteDraw_Buffer_Destroy(this->background_image_buffer);
    PisteDraw_Buffer_Destroy(this->water_block_buffer);
}

// TODO: =>
/*
int PK2Map::Save(char *filename) {
	int virhe = 0;
	FILE *file;
	_setmode( _fileno( stdin ), _O_BINARY );
	if ((file = fopen(filename, "w")) == NULL) {
		return(1);
	}
	for (int i=0;i<MAP_SIZE;i++)
		if (walls[i] != 255)
			walls[i] += 50;
	for (i=0;i<MAP_SIZE;i++)
		if (backgrounds[i] != 255)
			backgrounds[i] += 50;
	for (i=0;i<MAP_SIZE;i++)
		if (sprites[i] != 255)
			sprites[i] += 50;
	strcpy(this->version, LATEST_MAP_VERSION);

	fwrite(this->version,		sizeof(char),	5,  file);
	fwrite(this->block_bmp,	sizeof(char),	13, file);
	fwrite(this->background_image,	sizeof(char),	13, file);
	fwrite(this->music,		sizeof(char),	13, file);
	fwrite(this->name,			sizeof(char),	40, file);
	fwrite(this->author,		sizeof(char),	40, file);
	fwrite(&this->duration,			sizeof(int),	1,  file);
	fwrite(&this->extra,		sizeof(unsigned char),	1,  file);
	fwrite(&this->background,		sizeof(unsigned char),	1,  file);

	if (fwrite(this->backgrounds,	sizeof(backgrounds),1,  file) != MAP_SIZE)
		virhe = 2;
	if (fwrite(this->walls,	sizeof(walls),	1,  file) != MAP_SIZE)
		virhe = 2;
	if (fwrite(this->sprites,	sizeof(sprites),1,  file) != MAP_SIZE)
		virhe = 2;

	fclose(file);
	for (i=0;i<MAP_SIZE;i++)
		if (walls[i] != 255)
			walls[i] -= 50;
	for (i=0;i<MAP_SIZE;i++)
		if (backgrounds[i] != 255)
			backgrounds[i] -= 50;
	for (i=0;i<MAP_SIZE;i++)
		if (sprites[i] != 255)
			sprites[i] -= 50;
	return(virhe);
}
*/
/*
int PK2Map::Save(char *filename) {
	char luku[8];
	ofstream *file = new ofstream(filename, ios::binary);
	strcpy(this->version, LATEST_MAP_VERSION);
	//file->write ((char *)this, sizeof (*this));
	
	file->write(this->version,		sizeof(version));
	file->write(this->block_bmp,	sizeof(block_bmp));
	file->write(this->background_image,	sizeof(background_image));
	file->write(this->music,		sizeof(music));
	file->write(this->name,			sizeof(name));
	file->write(this->author,		sizeof(author));
	
	itoa(this->stage,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->climate,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->switch1_time,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->switch2_time,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->switch3_time,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->duration,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->extra,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->background,luku,10);
	file->write(luku, sizeof(luku));
	itoa(this->player_sprite,luku,10);
	file->write(luku, sizeof(luku));
	file->write(this->backgrounds, sizeof(backgrounds));
	file->write(this->walls, sizeof(walls));
	file->write(this->sprites, sizeof(sprites));
	for (int i=0;i<MAX_MAP_PROTOTYPES;i++)
		file->write(this->prototypes[i],sizeof(prototypes[i]));
	if (file->fail()) {
		delete (file);
		return 1;
	}
	delete (file);
	
	return 0;
}
*/

// TODO: =>
RECT PK2Map::CalculateSaveArea(unsigned char *source, unsigned char *&target) {
    int x, y;
    int kartan_vasen = MAP_WIDTH,//MAP_WIDTH/2,
    kartan_oikea = 0, kartan_yla = MAP_HEIGHT,//MAP_HEIGHT/2,
    kartan_ala = 0, kartan_korkeus = 0, kartan_leveys = 0;
    RECT rajat = {0, 0, 0, 0};
    // tutkitaan kartan reunimmaiset tilet ja asetetaan edges niiden mukaan
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (source[x + y * MAP_WIDTH] != 255) {
                if (x < kartan_vasen)
                    kartan_vasen = x;
                if (y < kartan_yla)
                    kartan_yla = y;
                if (x > kartan_oikea)
                    kartan_oikea = x;
                if (y > kartan_ala)
                    kartan_ala = y;
            }
        }
    }
    kartan_leveys = kartan_oikea - kartan_vasen;
    kartan_korkeus = kartan_ala - kartan_yla;
    // onko map tyhj�?
    if (kartan_leveys < 0 || kartan_korkeus < 0) {
        kartan_vasen = kartan_yla = 0;
        kartan_ala = kartan_oikea = 1;
        kartan_leveys = kartan_oikea - kartan_vasen;
        kartan_korkeus = kartan_ala - kartan_yla;
    }
    target = new unsigned char[kartan_leveys * kartan_korkeus];
    unsigned char tile;
    for (y = 0; y < kartan_korkeus; y++) {
        for (x = 0; x < kartan_leveys; x++) {
            tile = source[(x + kartan_vasen) + (y + kartan_yla) * MAP_WIDTH];
            target[x + y * kartan_leveys] = tile;
            if (x == 0 || y == 0 || x == kartan_leveys - 1 || y == kartan_korkeus - 1)
                source[(x + kartan_vasen) + (y + kartan_yla) * MAP_WIDTH] = 104;
        }
    }
    rajat.left = kartan_vasen;
    rajat.top = kartan_yla;
    rajat.right = kartan_oikea;
    rajat.bottom = kartan_ala;

    return rajat;
}

// TODO: =>
RECT PK2Map::LaskeTallennusAlue(unsigned char *alue) {
    unsigned long x, y;
    unsigned long kartan_vasen = MAP_WIDTH, kartan_oikea = 0, kartan_yla = MAP_HEIGHT, kartan_ala = 0;
    RECT rajat = {0, 0, 0, 0};
    // tutkitaan kartan reunimmaiset tilet ja asetetaan edges niiden mukaan
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (alue[x + y * MAP_WIDTH] != 255) {
                if (x < kartan_vasen) {
                    kartan_vasen = x;
                    //alue[x+y*MAP_WIDTH] = 101;
                }
                if (y < kartan_yla) {
                    kartan_yla = y;
                    //alue[x+y*MAP_WIDTH] = 102;
                }
                if (x > kartan_oikea) {
                    kartan_oikea = x;
                    //alue[x+y*MAP_WIDTH] = 103;
                }
                if (y > kartan_ala) {
                    kartan_ala = y;
                    //alue[x+y*MAP_WIDTH] = 104;
                }
            }
        }
    }
    // onko map tyhj�?
    if (kartan_oikea < kartan_vasen || kartan_ala < kartan_yla) {
        kartan_vasen = 0;
        kartan_yla = 0;
        kartan_ala = 1;
        kartan_oikea = 1;
    }
    rajat.left = kartan_vasen;
    rajat.top = kartan_yla;
    rajat.right = kartan_oikea;
    rajat.bottom = kartan_ala;
/*
	rajat.left = 0;
	rajat.top  = 0;
	rajat.right = MAP_WIDTH;
	rajat.bottom= MAP_HEIGHT;
*/

    return rajat;
}

// TODO: =>
void PK2Map::ReadSaveArea(unsigned char *source, RECT area, int target) {
    int x, y;
    int kartan_vasen = area.left, kartan_oikea = area.right, kartan_yla = area.top, kartan_ala = area.bottom, kartan_korkeus = kartan_oikea - kartan_vasen, kartan_leveys = kartan_ala - kartan_yla;
    unsigned char tile;
    if (source != NULL && target != NULL) {
        for (y = 0; y < kartan_korkeus; y++) {
            for (x = 0; x < kartan_leveys; x++) {
                tile = source[x + y * kartan_leveys];
                if (target == 1)
                    backgrounds[(x + kartan_vasen) + (y + kartan_yla) * MAP_WIDTH] = tile;
                if (target == 2)
                    walls[(x + kartan_vasen) + (y + kartan_yla) * MAP_WIDTH] = tile;
                if (target == 3)
                    sprites[(x + kartan_vasen) + (y + kartan_yla) * MAP_WIDTH] = tile;
            }
        }
    }
}

// TODO: =>
int PK2Map::Save(char *filename) {
    char luku[8];
    unsigned long i;
    ofstream *file = new ofstream(filename, ios::binary);
    strcpy(this->version, LATEST_MAP_VERSION);
    //file->write ((char *)this, sizeof (*this));

    file->write(this->version, sizeof(version));
    file->write(this->block_bmp, sizeof(block_bmp));
    file->write(this->background_image, sizeof(background_image));
    file->write(this->music, sizeof(music));
    file->write(this->name, sizeof(name));
    file->write(this->author, sizeof(author));

    itoa(this->stage, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->climate, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->switch1_time, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->switch2_time, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->switch3_time, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->time, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->extra, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->background, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->player_sprite, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->x, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->y, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    itoa(this->icon, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    int protoja = 0;
    for (i = 0; i < MAX_MAP_PROTOTYPES; i++)
        if (strlen(this->protos[i]) > 0)
            protoja++;
    itoa(protoja, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));

    for (i = 0; i < MAX_MAP_PROTOTYPES; i++)
        if (strlen(this->protos[i]) > 0)
            file->write(protos[i], sizeof(protos[i]));
    // laske alue
    //unsigned char *alue_taustat = NULL, *alue_seinat = NULL, *alue_spritet = NULL;
    RECT alue = {0, 0, 0, 0};
    unsigned long /*koko, aloituskohta,*/ leveys, korkeus, x, y;
    unsigned long aloitus_x, aloitus_y;
    char tile[1];

    // backgrounds
    alue = CalculateSaveArea(this->backgrounds);
    leveys = alue.right - alue.left;
    korkeus = alue.bottom - alue.top;
    aloitus_x = alue.left;
    aloitus_y = alue.top;
    ltoa(aloitus_x, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(aloitus_y, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(leveys, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(korkeus, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Kirjoitetaan alue tiedostoon tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tile[0] = this->backgrounds[x + y * MAP_WIDTH];
            file->write(tile, sizeof(tile));
        }
    }
    // sein�t
    alue = CalculateSaveArea(this->walls);
    leveys = alue.right - alue.left;
    korkeus = alue.bottom - alue.top;
    aloitus_x = alue.left;
    aloitus_y = alue.top;
    ltoa(aloitus_x, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(aloitus_y, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(leveys, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(korkeus, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Kirjoitetaan alue tiedostoon tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tile[0] = this->walls[x + y * MAP_WIDTH];
            file->write(tile, sizeof(tile));
        }
    }
    // sprites
    alue = CalculateSaveArea(this->sprites);
    leveys = alue.right - alue.left;
    korkeus = alue.bottom - alue.top;
    aloitus_x = alue.left;
    aloitus_y = alue.top;
    ltoa(aloitus_x, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(aloitus_y, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(leveys, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(korkeus, luku, 10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Kirjoitetaan alue tiedostoon tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tile[0] = this->sprites[x + y * MAP_WIDTH];
            file->write(tile, sizeof(tile));
        }
    }
/*
    alue = LaskeTallennusAlue(this->walls);
    koko = (alue.right - alue.left) * (alue.bottom - alue.top);
    aloituskohta = alue.left+alue.top*MAP_WIDTH;
    ltoa(aloituskohta,luku,10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(koko,luku,10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    for (i=0;i<koko;i++){
        tile[0] = this->walls[aloituskohta+i];
        file->write(tile, sizeof(tile));
    }
    alue = LaskeTallennusAlue(this->sprites);
    koko = (alue.right - alue.left) * (alue.bottom - alue.top);
    aloituskohta = alue.left+alue.top*MAP_WIDTH;
    ltoa(aloituskohta,luku,10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    ltoa(koko,luku,10);
    file->write(luku, sizeof(luku));
    memset(luku, 0, sizeof(luku));
    for (i=0;i<koko;i++){
        tile[0] = this->sprites[aloituskohta+i];
        file->write(tile, sizeof(tile));
    }
*/
/*
    alue = LaskeTallennusAlue(this->walls);
    koko = (alue.right - alue.left) * (alue.bottom - alue.top);
    ltoa(alue.left,luku,10); file->write(luku, sizeof(luku));
    ltoa(alue.right,luku,10); file->write(luku, sizeof(luku));
    ltoa(koko,luku,10); file->write(luku, sizeof(luku));
    for (i=0;i<koko;i++){
        tile = (char *)this->walls[(alue.left+alue.top*MAP_WIDTH)+i];
        file->write(tile, sizeof(tile));
    }
    alue = LaskeTallennusAlue(this->sprites);
    koko = (alue.right - alue.left) * (alue.bottom - alue.top);
    ltoa(alue.left,luku,10); file->write(luku, sizeof(luku));
    ltoa(alue.right,luku,10); file->write(luku, sizeof(luku));
    ltoa(koko,luku,10); file->write(luku, sizeof(luku));
    for (i=0;i<koko;i++){
        tile = (char *)this->sprites[(alue.left+alue.top*MAP_WIDTH)+i];
        file->write(tile, sizeof(tile));
    }
*/
/*
    alue = LaskeTallennusAlue(this->walls, *&alue_seinat);
    if (alue_seinat != NULL) {
        ltoa(alue.left,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.right,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.bottom,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.top,luku,10); file->write(luku, sizeof(luku));
        file->write(alue_seinat, sizeof(alue_seinat));
    }
    alue = LaskeTallennusAlue(this->backgrounds, *&alue_taustat);
    if (alue_taustat != NULL) {
        ltoa(alue.left,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.right,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.bottom,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.top,luku,10); file->write(luku, sizeof(luku));
        file->write(alue_taustat, sizeof(alue_taustat));
    }
    alue = CalculateSaveArea(this->sprites, *&alue_spritet);
    if (alue_spritet != NULL) {
        ltoa(alue.left,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.right,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.bottom,luku,10); file->write(luku, sizeof(luku));
        ltoa(alue.top,luku,10); file->write(luku, sizeof(luku));
        file->write(alue_spritet, sizeof(alue_spritet));
    }
*/
/*
    file->write(this->backgrounds, sizeof(backgrounds));
    file->write(this->walls, sizeof(walls));
    file->write(this->sprites, sizeof(sprites));
*/
    if (file->fail()) {
        delete (file);
        return 1;
    }
    delete (file);
    return 0;
}

// TODO: =>
int PK2Map::Load(char *path, char *name) {
    char file[_MAX_PATH];
    strcpy(file, path);
    strcat(file, name);

    ifstream *file = new ifstream(file, ios::binary);// | ios::nocreate);
    char versio[8] = "\0";
    if (file->fail()) {
        delete (file);
        return 1;
    }
    file->read((char *) versio, sizeof(versio));
    if (file->fail()) {
        delete (file);
        return 1;
    }
    delete (file);
    int ok = 2;
    if (strcmp(versio, "1.3") == 0) {
        this->LoadVersion13(file);
        ok = 0;
    }
    if (strcmp(versio, "1.2") == 0) {
        this->LoadVersion12(file);
        ok = 0;
    }
    if (strcmp(versio, "1.1") == 0) {
        this->LoadVersion11(file);
        ok = 0;
    }
    if (strcmp(versio, "1.0") == 0) {
        this->LoadVersion10(file);
        ok = 0;
    }
    if (strcmp(versio, "0.1") == 0) {
        this->LoadVersion01(file);
        ok = 0;
    }
    Load_Block_Palette(path, this->block_bmp);
    Load_Background_Image(path, this->background_image);

    return (ok);
}

// TODO: =>
int PK2Map::Load_Only_Data(char *path, char *name) {
    char file[_MAX_PATH];
    strcpy(file, path);
    strcat(file, name);
    ifstream *tiedosto = new ifstream(file, ios::binary);// | ios::nocreate);
    char versio[8] = "\0";
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    tiedosto->read((char *) versio, sizeof(versio));
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    delete (tiedosto);
    if (strcmp(versio, "1.3") == 0)
        this->LoadVersion13(file);
    if (strcmp(versio, "1.2") == 0)
        this->LoadVersion12(file);
    if (strcmp(versio, "1.1") == 0)
        this->LoadVersion11(file);
    if (strcmp(versio, "1.0") == 0)
        this->LoadVersion10(file);
    if (strcmp(versio, "0.1") == 0)
        this->LoadVersion01(file);

    return (0);
}

// TODO: =>
int PK2Map::LoadVersion01(char *filename) {
    FILE *tiedosto;
    PK2KARTTA kartta;
    if ((tiedosto = fopen(filename, "r")) == NULL) {
        return (1);
    }
    fread(&kartta, sizeof(PK2KARTTA), 1, tiedosto);
    fclose(tiedosto);

    strcpy(this->version, LATEST_MAP_VERSION);
    strcpy(this->block_bmp, "blox.bmp");
    strcpy(this->background_image, "default.bmp");
    strcpy(this->music, "default.xm");
    strcpy(this->name, "v01");
    strcpy(this->author, "unknown");

    this->time = 0;
    this->extra = 0;
    this->background = kartta.taustakuva;

    for (int i = 0; i < MAP_SIZE; i++)
        this->walls[i] = kartta.kartta[i % MAP_WIDTH + (i / MAP_WIDTH) * 640];
    memset(this->backgrounds, 255, sizeof(backgrounds));
    memset(this->sprites, 255, sizeof(sprites));

    return (0);
}

// TODO: =>
int PK2Map::LoadVersion10(char *filename) {
    FILE *tiedosto;
    PK2Map *kartta = new PK2Map();
    if ((tiedosto = fopen(filename, "r")) == NULL) {
        return (1);
    }
    fread(kartta, sizeof(PK2Map), 1, tiedosto);
    fclose(tiedosto);

    strcpy(this->version, kartta->version);
    strcpy(this->block_bmp, kartta->block_bmp);
    strcpy(this->background_image, kartta->background_image);
    strcpy(this->music, kartta->music);
    strcpy(this->name, kartta->name);
    strcpy(this->author, kartta->author);

    this->time = kartta->time;
    this->extra = kartta->extra;
    this->background = kartta->background;

    for (int i = 0; i < MAP_SIZE; i++)
        this->backgrounds[i] = kartta->backgrounds[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->walls[i] = kartta->walls[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->sprites[i] = kartta->sprites[i];
    //Load_Block_Palette(map->block_bmp);
    //Load_Background_Image(map->background_image);
    //delete map;

    return (0);
}

// TODO: =>
int PK2Map::LoadVersion11(char *filename) {
    FILE *tiedosto;
    int virhe = 0;
    _setmode(_fileno(stdin), _O_BINARY);
    if ((tiedosto = fopen(filename, "r")) == NULL) {
        return (1);
    }
    memset(this->backgrounds, 255, sizeof(this->backgrounds));
    memset(this->walls, 255, sizeof(this->walls));
    memset(this->sprites, 255, sizeof(this->sprites));

    fread(this->version, sizeof(char), 5, tiedosto);
    fread(this->block_bmp, sizeof(char), 13, tiedosto);
    fread(this->background_image, sizeof(char), 13, tiedosto);
    fread(this->music, sizeof(char), 13, tiedosto);
    fread(this->name, sizeof(char), 40, tiedosto);
    fread(this->author, sizeof(char), 40, tiedosto);
    fread(&this->time, sizeof(int), 1, tiedosto);
    fread(&this->extra, sizeof(unsigned char), 1, tiedosto);
    fread(&this->background, sizeof(unsigned char), 1, tiedosto);
    fread(this->backgrounds, sizeof(backgrounds), 1, tiedosto);
    if (fread(this->walls, sizeof(walls), 1, tiedosto) != MAP_SIZE)
        virhe = 2;
    fread(this->sprites, sizeof(sprites), 1, tiedosto);
    fclose(tiedosto);
    for (int i = 0; i < MAP_SIZE; i++)
        if (walls[i] != 255)
            walls[i] -= 50;
    for (int i = 0; i < MAP_SIZE; i++)
        if (backgrounds[i] != 255)
            backgrounds[i] -= 50;
    for (int i = 0; i < MAP_SIZE; i++)
        if (sprites[i] != 255)
            sprites[i] -= 50;
    //Load_Block_Palette(this->block_bmp);
    //Load_Background_Image(this->background_image);

    return (virhe);
}

// TODO: =>
int PK2Map::LoadVersion12(char *filename) {
    ifstream *tiedosto = new ifstream(filename, ios::binary);// | ios::nocreate);
    char luku[8];
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    memset(this->backgrounds, 255, sizeof(this->backgrounds));
    memset(this->walls, 255, sizeof(this->walls));
    memset(this->sprites, 255, sizeof(this->sprites));
    for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
        strcpy(this->protos[i], "");
    //file->read ((char *)this, sizeof (*this));
    tiedosto->read(this->version, sizeof(version));
    tiedosto->read(this->block_bmp, sizeof(block_bmp));
    tiedosto->read(this->background_image, sizeof(background_image));
    tiedosto->read(this->music, sizeof(music));
    tiedosto->read(this->name, sizeof(name));
    tiedosto->read(this->author, sizeof(author));

    tiedosto->read(luku, sizeof(luku));
    this->stage = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->climate = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->switch1_time = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->switch2_time = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->switch3_time = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->time = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->extra = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->background = atoi(luku);
    tiedosto->read(luku, sizeof(luku));
    this->player_sprite = atoi(luku);

    tiedosto->read(this->backgrounds, sizeof(backgrounds));
    tiedosto->read(this->walls, sizeof(walls));
    tiedosto->read(this->sprites, sizeof(sprites));

    for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
        tiedosto->read(this->protos[i], sizeof(protos[i]));
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    delete (tiedosto);
    //Load_Block_Palette(this->block_bmp);
    //Load_Background_Image(this->background_image);

    return 0;
}

// TODO: =>
int PK2Map::LoadVersion13(char *filename) {
    ifstream *tiedosto = new ifstream(filename, ios::binary);// | ios::nocreate);
    char luku[8];
    unsigned long i;
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    memset(this->backgrounds, 255, sizeof(this->backgrounds));
    memset(this->walls, 255, sizeof(this->walls));
    memset(this->sprites, 255, sizeof(this->sprites));
    for (i = 0; i < MAX_MAP_PROTOTYPES; i++)
        strcpy(this->protos[i], "");
    //file->read ((char *)this, sizeof (*this));
    tiedosto->read(this->version, sizeof(version));
    tiedosto->read(this->block_bmp, sizeof(block_bmp));
    tiedosto->read(this->background_image, sizeof(background_image));
    tiedosto->read(this->music, sizeof(music));
    tiedosto->read(this->name, sizeof(name));
    tiedosto->read(this->author, sizeof(author));

    tiedosto->read(luku, sizeof(luku));
    this->stage = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->climate = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->switch1_time = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->switch2_time = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->switch3_time = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->time = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->extra = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->background = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->player_sprite = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->x = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->y = atoi(luku);
    memset(luku, 0, sizeof(luku));

    tiedosto->read(luku, sizeof(luku));
    this->icon = atoi(luku);
    memset(luku, 0, sizeof(luku));

    unsigned long lkm;
    tiedosto->read(luku, sizeof(luku));
    lkm = (int) atoi(luku);

    //for (i=0;i<MAX_MAP_PROTOTYPES;i++)
    //	itoa(lkm,prototypes[i],10);//strcpy(prototypes[i],"");
    for (i = 0; i < lkm/*MAX_MAP_PROTOTYPES*/; i++)
        tiedosto->read(protos[i], sizeof(protos[i]));

    unsigned long leveys, korkeus, aloitus_x, aloitus_y, x, y;
    char tile[1];

    // backgrounds
    tiedosto->read(luku, sizeof(luku));
    aloitus_x = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    aloitus_y = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    leveys = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    korkeus = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Luetaan alue tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tiedosto->read(tile, sizeof(tile));
            this->backgrounds[x + y * MAP_WIDTH] = tile[0];
        }
    }
    // walls
    tiedosto->read(luku, sizeof(luku));
    aloitus_x = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    aloitus_y = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    leveys = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    korkeus = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Luetaan alue tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tiedosto->read(tile, sizeof(tile));
            this->walls[x + y * MAP_WIDTH] = tile[0];
        }
    }
    //sprites
    tiedosto->read(luku, sizeof(luku));
    aloitus_x = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    aloitus_y = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    leveys = atol(luku);
    memset(luku, 0, sizeof(luku));
    tiedosto->read(luku, sizeof(luku));
    korkeus = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (y = aloitus_y; y <= aloitus_y + korkeus; y++) {    // Luetaan alue tile by tile
        for (x = aloitus_x; x <= aloitus_x + leveys; x++) {
            tiedosto->read(tile, sizeof(tile));
            this->sprites[x + y * MAP_WIDTH] = tile[0];
        }
    }
/*
    for (i=0;i<koko;i++){
        file->read(tile, sizeof(tile));
        this->backgrounds[aloituskohta+i] = (unsigned char)tile[0];
    }
*/
/*
    file->read(luku, sizeof(luku));
    aloituskohta = atol(luku);
    memset(luku, 0, sizeof(luku));
    file->read(luku, sizeof(luku));
    koko = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (i=0;i<koko;i++){
        file->read(tile, sizeof(tile));
        this->backgrounds[aloituskohta+i] = (unsigned char)tile[0];
    }

    file->read(luku, sizeof(luku));
    aloituskohta = atol(luku);
    memset(luku, 0, sizeof(luku));
    file->read(luku, sizeof(luku));
    koko = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (i=0;i<koko;i++){
        file->read(tile, sizeof(tile));
        this->walls[aloituskohta+i] = (unsigned char)tile[0];
    }

    file->read(luku, sizeof(luku));
    aloituskohta = atol(luku);
    memset(luku, 0, sizeof(luku));
    file->read(luku, sizeof(luku));
    koko = atol(luku);
    memset(luku, 0, sizeof(luku));
    for (i=0;i<koko;i++){
        file->read(tile, sizeof(tile));
        this->sprites[aloituskohta+i] = (unsigned char)tile[0];
    }
*/
/*
    unsigned char *alue_tilet = NULL;
    RECT alue = {0,0,0,0};
    unsigned long width = 0, height = 0;

    // backgrounds
    file->read(luku, sizeof(luku));	alue.left = atol(luku);
    file->read(luku, sizeof(luku));	alue.right = atol(luku);
    file->read(luku, sizeof(luku));	alue.bottom = atol(luku);
    file->read(luku, sizeof(luku));	alue.top = atol(luku);
    width = alue.right - alue.left;
    height = alue.bottom - alue.top;
    alue_tilet = new unsigned char[width*height];
    file->read(alue_tilet, sizeof(*alue_tilet));
    LueTallennusAlue(alue_tilet, alue, 1);

    //walls
    file->read(luku, sizeof(luku));	alue.left = atol(luku);
    file->read(luku, sizeof(luku));	alue.right = atol(luku);
    file->read(luku, sizeof(luku));	alue.bottom = atol(luku);
    file->read(luku, sizeof(luku));	alue.top = atol(luku);
    width = alue.right - alue.left;
    height = alue.bottom - alue.top;
    alue_tilet = new unsigned char[width*height];
    file->read(alue_tilet, sizeof(*alue_tilet));
    LueTallennusAlue(alue_tilet, alue, 2);

    //sprites
    file->read(luku, sizeof(luku));	alue.left = atol(luku);
    file->read(luku, sizeof(luku));	alue.right = atol(luku);
    file->read(luku, sizeof(luku));	alue.bottom = atol(luku);
    file->read(luku, sizeof(luku));	alue.top = atol(luku);
    width = alue.right - alue.left;
    height = alue.bottom - alue.top;
    alue_tilet = new unsigned char[width*height];
    file->read(alue_tilet, sizeof(*alue_tilet));
    ReadSaveArea(alue_tilet, alue, 3);
*/
/*
    file->read(this->backgrounds,		sizeof(backgrounds));
    file->read(this->walls,		sizeof(walls));
    file->read(this->sprites,		sizeof(sprites));
*/
    if (tiedosto->fail()) {
        delete (tiedosto);
        return 1;
    }
    delete (tiedosto);
    //Load_Block_Palette(this->block_bmp);
    //Load_Background_Image(this->background_image);

    return 0;
}

void PK2Map::Clear() {
    strcpy(this->version, LATEST_MAP_VERSION);
    strcpy(this->block_bmp, "blox.bmp");
    strcpy(this->background_image, "default.bmp");
    strcpy(this->music, "default.xm");
    strcpy(this->name, "untitled");
    strcpy(this->author, "unknown");

    this->stage = 0;
    this->climate = WEATHER_NORMAL;
    this->switch1_time = SWITCH_START_VALUE;
    this->switch2_time = SWITCH_START_VALUE;
    this->switch3_time = SWITCH_START_VALUE;
    this->player_sprite = 0;
    this->time = 0;
    this->extra = NO_EXTRA;
    this->background = NO_BACKGROUND_IMAGE;

    this->x = 0;
    this->y = 0;
    this->icon = 0;

    memset(this->backgrounds, 255, sizeof(backgrounds));
    memset(this->walls, 255, sizeof(walls));
    memset(this->sprites, 255, sizeof(sprites));

    for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
        strcpy(this->protos[i], "");
    //PisteDraw_Buffer_Clear(this->block_buffer,255);
    //PisteDraw_Buffer_Clear(this->background_image_buffer,255);
}

PK2Map &PK2Map::operator=(const PK2Map &map) {
    if (this == &map)
        return *this;
    strcpy(this->version, map.version);
    strcpy(this->block_bmp, map.block_bmp);
    strcpy(this->background_image, map.background_image);
    strcpy(this->music, map.music);
    strcpy(this->name, map.name);
    strcpy(this->author, map.author);

    this->time = map.time;
    this->extra = map.extra;
    this->background = map.background;

    for (int i = 0; i < MAP_SIZE; i++)
        this->walls[i] = map.walls[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->backgrounds[i] = map.backgrounds[i];
    for (int i = 0; i < MAP_SIZE; i++)
        this->sprites[i] = map.sprites[i];
    PisteDraw_Buffer_Flip_Fast(map.block_buffer, this->block_buffer, 0, 0);
    PisteDraw_Buffer_Flip_Fast(map.background_image_buffer, this->background_image_buffer, 0, 0);

    return *this;
}

// TODO: =>
int PK2Map::Load_Background_Image(char *path, char *filename) {
    char file[_MAX_PATH];
    strcpy(file, "");
    strcpy(file, path);
    strcat(file, filename);
    if (!PK2Kartta_Onko_File(file)) {
        strcpy(file, PK2Map::pk2_directory);
        strcat(file, "gfx\\scenery\\");
        strcat(file, filename);
        if (!PK2Kartta_Onko_File(file))
            return 1;
    }
    if (PisteDraw_Image_Load(this->background_image_buffer, file, true) == PD_ERROR)
        return 2;
    strcpy(this->background_image, filename);

    unsigned char *buffer = NULL;
    unsigned long leveys;
    int x, y;
    int color;
    PisteDraw_Draw_Begin(background_image_buffer, *&buffer, (unsigned long &) leveys);
    for (x = 0; x < 640; x++)
        for (y = 0; y < 480; y++) {
            color = buffer[x + y * leveys];
            if (color == 255)
                color--;
            buffer[x + y * leveys] = color;
        }
    PisteDraw_Draw_End(background_image_buffer);

    return 0;
}

int PK2Map::Load_Block_Palette(char *path, char *filename) {
    char file[_MAX_PATH];
    strcpy(file, "");
    strcpy(file, path);
    strcat(file, filename);
    if (!PK2Kartta_Onko_File(file)) {
        strcpy(file, PK2Map::pk2_directory);
        strcat(file, "gfx\\tiles\\");
        strcat(file, filename);
        if (!PK2Kartta_Onko_File(file))
            return 1;
    }
    if (PisteDraw_Image_Load(this->block_buffer, file, false) == PD_ERROR)
        return 2;
    PisteDraw_Buffer_Flip_Fast(this->block_buffer, water_block_buffer, 0, 0, 0, 416, 320, 448);
    strcpy(this->block_bmp, filename);
    return 0;
}

int PK2Map::Load_Background_Music(char *filename) {
    return 0;
}

void PK2Map::Copy(PK2Map &map) {
    if (this != &map) {
        strcpy(this->version, map.version);
        strcpy(this->block_bmp, map.block_bmp);
        strcpy(this->background_image, map.background_image);
        strcpy(this->music, map.music);
        strcpy(this->name, map.name);
        strcpy(this->author, map.author);

        this->stage = map.stage;
        this->climate = map.climate;
        this->switch1_time = map.switch1_time;
        this->switch2_time = map.switch2_time;
        this->switch3_time = map.switch3_time;
        this->player_sprite = map.player_sprite;
        this->time = map.time;
        this->extra = map.extra;
        this->background = map.background;

        for (int i = 0; i < MAP_SIZE; i++)
            this->walls[i] = map.walls[i];
        for (int i = 0; i < MAP_SIZE; i++)
            this->backgrounds[i] = map.backgrounds[i];
        for (int i = 0; i < MAP_SIZE; i++)
            this->sprites[i] = map.sprites[i];
        for (int i = 0; i < MAX_MAP_PROTOTYPES; i++)
            strcpy(this->protos[i], map.protos[i]);
        PisteDraw_Buffer_Clear(block_buffer, 255);
        PisteDraw_Buffer_Clear(background_image_buffer, 0);
        PisteDraw_Buffer_Flip_Fast(map.block_buffer, this->block_buffer, 0, 0);
        PisteDraw_Buffer_Flip_Fast(map.background_image_buffer, this->background_image_buffer, 0, 0);
        PisteDraw_Buffer_Flip_Fast(map.water_block_buffer, this->water_block_buffer, 0, 0);
    }
}

/* Kartanpiirtorutiineja ----------------------------------------------------------------*/

// TODO: =>
void PK2Map::Animate_Fire(void) {
    unsigned char *buffer = NULL;
    unsigned long leveys;
    int x, y;
    int color;

    PisteDraw_Draw_Begin(block_buffer, *&buffer, (unsigned long &) leveys);

    for (x = 128; x < 160; x++)
        for (y = 448; y < 479; y++) {
            color = buffer[x + (y + 1) * leveys];
            if (color != 255) {
                color %= 32;
                color = color - rand() % 4;
                if (color < 0)
                    color = 255;
                else {
                    if (color > 21)
                        color += 128;
                    else
                        color += 64;
                }
            }
            buffer[x + y * leveys] = color;
        }

    if (ajastin1 < 20) {
        for (x = 128; x < 160; x++)
            buffer[x + 479 * leveys] = rand() % 15 + 144;
    } else
        for (x = 128; x < 160; x++)
            buffer[x + 479 * leveys] = 255;

    PisteDraw_Draw_End(block_buffer);
}

// TODO: =>
void PK2Map::Animate_Waterfall(void) {
    unsigned char *buffer = NULL;
    unsigned long leveys;
    int x, y, plus;
    int color, color2;

    unsigned char temp[32 * 32];

    PisteDraw_Draw_Begin(block_buffer, *&buffer, (unsigned long &) leveys);

    for (x = 32; x < 64; x++)
        for (y = 416; y < 448; y++)
            temp[x - 32 + (y - 416) * 32] = buffer[x + y * leveys];
    color2 = (temp[0] / 32) * 32;    // mahdollistaa eriv�riset vesiputoukset

    for (x = 32; x < 64; x++) {
        plus = rand() % 2 + 2;//...+1
        for (y = 416; y < 448; y++) {
            color = temp[x - 32 + (y - 416) * 32];
            if (color != 255) {   // mahdollistaa eri leveyksiset vesiputoukset
                color %= 32;
                if (color > 10)//20
                    color--;
                if (rand() % 40 == 1)
                    color = 11 + rand() % 11;//15+rand()%8;//9+rand()%5;
                if (rand() % 160 == 1)
                    color = 30;
                buffer[x + (416 + (y + plus) % 32) * leveys] = color + color2;
            } else
                buffer[x + (416 + (y + plus) % 32) * leveys] = color;
        }
    }

    PisteDraw_Draw_End(block_buffer);
}

void PK2Map::Animate_Water_Surface(void) {
    unsigned char *buffer = NULL;
    unsigned long leveys;
    int x, y;
    unsigned char temp[32];

    PisteDraw_Draw_Begin(block_buffer, *&buffer, (unsigned long &) leveys);
    for (y = 416; y < 448; y++)
        temp[y - 416] = buffer[y * leveys];
    for (y = 416; y < 448; y++) {
        for (x = 0; x < 31; x++) {
            buffer[x + y * leveys] = buffer[x + 1 + y * leveys];
        }
    }
    for (y = 416; y < 448; y++)
        buffer[31 + y * leveys] = temp[y - 416];

    PisteDraw_Draw_End(block_buffer);
}

// TODO: =>
void PK2Map::Animate_Water(void) {
    unsigned char *buffer_lahde = NULL, *buffer_kohde = NULL;
    unsigned long leveys_lahde, leveys_kohde;
    int x, y, color1, color2, d1 = vesiaste / 2, d2;
    int sini, cosi;
    int vx, vy;
    int i;

    PisteDraw_Draw_Begin(block_buffer, *&buffer_kohde, (unsigned long &) leveys_kohde);
    PisteDraw_Draw_Begin(water_block_buffer, *&buffer_lahde, (unsigned long &) leveys_lahde);

    for (y = 0; y < 32; y++) {
        d2 = d1;
        for (x = 0; x < 32; x++) {
            sini = int((y + d2 / 2) * 11.25) % 360;
            cosi = int((x + d1 / 2) * 11.25) % 360;
            sini = (int) map_sin_table[sini];
            cosi = (int) map_cos_table[cosi];
            vy = (y + sini / 11) % 32;
            vx = (x + cosi / 11) % 32;
            if (vy < 0) {
                vy = -vy;
                vy = 31 - (vy % 32);
            }
            if (vx < 0) {
                vx = -vx;
                vx = 31 - (vx % 32);
            }
            color1 = buffer_lahde[64 + vx + vy * leveys_lahde];
            buffer_lahde[32 + x + y * leveys_lahde] = color1;
            d2 = 1 + d2 % 360;
        }
        d1 = 1 + d1 % 360;
    }
    int vy2;
    for (int p = 2; p < 5; p++) {
        i = p * 32;
        for (y = 0; y < 32; y++) {
            //d2 = d1;
            vy = y * leveys_lahde;
            vy2 = (y + 416) * leveys_kohde;
            for (x = 0; x < 32; x++) {
                vx = x + vy;
                color1 = buffer_lahde[32 + vx];
                color2 = buffer_lahde[i + vx];
                buffer_kohde[i + x + vy2] = (color1 + color2 * 2) / 3;
            }
        }
    }
    PisteDraw_Draw_End(block_buffer);
    PisteDraw_Draw_End(water_block_buffer);
}

// TODO: =>
void PK2Map::Animate_Current_Up(void) {
    unsigned char *buffer = NULL;
    unsigned long leveys;
    int x, y;
    unsigned char temp[32];

    PisteDraw_Draw_Begin(block_buffer, *&buffer, (unsigned long &) leveys);

    for (x = 64; x < 96; x++)
        temp[x - 64] = buffer[x + 448 * leveys];
    for (x = 64; x < 96; x++) {
        for (y = 448; y < 479; y++) {
            buffer[x + y * leveys] = buffer[x + (y + 1) * leveys];
        }
    }
    for (x = 64; x < 96; x++)
        buffer[x + 479 * leveys] = temp[x - 64];
    PisteDraw_Draw_End(block_buffer);
}

// TODO: =>
int PK2Map::Draw_Backgrounds(int kamera_x, int kamera_y, bool editor) {
    int palikka;
    int px = 0, py = 0, kartta_x = kamera_x / 32, kartta_y = kamera_y / 32;
    for (int x = 0; x < ruudun_leveys_palikoina; x++) {
        for (int y = 0; y < ruudun_korkeus_palikoina; y++) {
            palikka = backgrounds[x + kartta_x + (y + kartta_y) * MAP_WIDTH];
            if (palikka != 255) {
                px = ((palikka % 10) * 32);
                py = ((palikka / 10) * 32);
                if (palikka == PALIKKA_ANIM1 || palikka == PALIKKA_ANIM2 || palikka == PALIKKA_ANIM3 ||
                    palikka == PALIKKA_ANIM4)
                    px += animaatio * 32;
                PisteDraw_Buffer_Flip_Fast(block_buffer, PD_BACKBUFFER, x * 32 - (kamera_x % 32), y * 32 - (kamera_y % 32), px, py, px + 32, py + 32);
            }
        }
    }

    return 0;
}

// TODO: =>
int PK2Map::Draw_Walls(int kamera_x, int kamera_y, bool editor) {
    int palikka;
    int px = 0, py = 0, ay = 0, ax = 0, by = 0, bx = 0, kartta_x = kamera_x / 32, kartta_y = kamera_y / 32;
    int ajastin1_y = 0, ajastin2_y = 0, ajastin3_x = 0;
    if (ajastin1 > 0) {
        ajastin1_y = 64;
        if (ajastin1 < 64)
            ajastin1_y = ajastin1;
        if (ajastin1 > SWITCH_START_VALUE - 64)
            ajastin1_y = SWITCH_START_VALUE - ajastin1;
    }
    if (ajastin2 > 0) {
        ajastin2_y = 64;
        if (ajastin2 < 64)
            ajastin2_y = ajastin2;
        if (ajastin2 > SWITCH_START_VALUE - 64)
            ajastin2_y = SWITCH_START_VALUE - ajastin2;
    }
    if (ajastin3 > 0) {
        ajastin3_x = 64;
        if (ajastin3 < 64)
            ajastin3_x = ajastin3;
        if (ajastin3 > SWITCH_START_VALUE - 64)
            ajastin3_x = SWITCH_START_VALUE - ajastin3;
    }
    for (int x = 0; x < ruudun_leveys_palikoina; x++) {
        for (int y = 0; y < ruudun_korkeus_palikoina; y++) {
            palikka = walls[x + kartta_x + (y + kartta_y) * MAP_WIDTH];
            if (palikka != 255 && !(!editor && palikka == BLOCK_RESTRICTION_DOWN)) {
                px = ((palikka % 10) * 32);
                py = ((palikka / 10) * 32);
                ay = 0;
                ax = 0;
                if (!editor) {
                    if (palikka == BLOCK_ELEVATOR_VERT) {
                        ay = (int) map_sin_table[aste % 360];
                    }
                    if (palikka == BLOCK_ELEVATOR_HORI) {
                        ax = (int) map_cos_table[aste % 360];
                    }
                    if (palikka == BLOCK_SWITCH1) {
                        ay = ajastin1_y / 2;
                    }
                    if (palikka == BLOCK_SWITCH2_UP) {
                        ay = -ajastin2_y / 2;
                    }
                    if (palikka == BLOCK_SWITCH2_DOWN) {
                        ay = ajastin2_y / 2;
                    }
                    if (palikka == BLOCK_SWITCH2) {
                        ay = ajastin2_y / 2;
                    }
                    if (palikka == BLOCK_SWITCH3_RIGHT) {
                        ax = ajastin3_x / 2;
                    }
                    if (palikka == BLOCK_SWITCH3_LEFT) {
                        ax = -ajastin3_x / 2;
                    }
                    if (palikka == BLOCK_SWITCH3) {
                        ay = ajastin3_x / 2;
                    }
                }
                if (palikka == PALIKKA_ANIM1 || palikka == PALIKKA_ANIM2 || palikka == PALIKKA_ANIM3 ||
                    palikka == PALIKKA_ANIM4)
                    px += animaatio * 32;
                PisteDraw_Buffer_Flip_Fast(block_buffer, PD_BACKBUFFER, x * 32 - (kamera_x % 32) + ax, y * 32 - (kamera_y % 32) + ay, px, py, px + 32, py + 32);
            }
        }
    }
    if (vesiaste % 2 == 0) {
        Animate_Fire();
        Animate_Waterfall();
        Animate_Current_Up();
        Animate_Water_Surface();
        //PisteDraw_Palette_Rotate(224,239);
    }
    if (vesiaste % 4 == 0) {
        Animate_Water();
        PisteDraw_Palette_Rotate(224, 239);
    }
    vesiaste = 1 + vesiaste % 320;

    return 0;
}
