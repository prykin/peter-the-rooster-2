#ifndef PK2MAP
#define PK2MAP

#include "../old_headers.h"

const char LATEST_MAP_VERSION[4] = "1.3";

const unsigned long MAP_WIDTH = 256;
const unsigned long MAP_HEIGHT = 224;
const unsigned long MAP_SIZE = MAP_WIDTH * MAP_HEIGHT;
const unsigned long BLOCK_PALETTE_WIDTH = 320;
const unsigned long BLOCK_PALETTE_HEIGHT = 480;
const unsigned char NO_BACKGROUND_IMAGE = 0;
const unsigned char NO_EXTRA = 0;

const unsigned long MAX_MAP_PROTOTYPES = 100;

const unsigned char BLOCK_RESTRICTION_DOWN = 40;
const unsigned char BLOCK_ELEVATOR_HORI = 41;
const unsigned char BLOCK_ELEVATOR_VERT = 42;
const unsigned char BLOCK_SWITCH2_UP = 43;
const unsigned char BLOCK_SWITCH2_DOWN = 45;
const unsigned char BLOCK_SWITCH3_RIGHT = 44;
const unsigned char BLOCK_SWITCH3_LEFT = 46;
const unsigned char BLOCK_LOCK = 47;
const unsigned char BLOCK_SKULL_WALL = 48;
const unsigned char BLOCK_SKULL_BACKGROUND = 49;
const unsigned char PALIKKA_ANIM1 = 60;
const unsigned char PALIKKA_ANIM2 = 65;
const unsigned char PALIKKA_ANIM3 = 70;
const unsigned char PALIKKA_ANIM4 = 75;
const unsigned char BLOCK_CURRENT_LEFT = 140;
const unsigned char BLOCK_CURRENT_RIGHT = 141;
const unsigned char BLOCK_CURRENT_UP = 142;
const unsigned char BLOCK_HIDDEN = 143;
const unsigned char BLOCK_FIRE = 144;
const unsigned char BLOCK_SWITCH1 = 145;
const unsigned char BLOCK_SWITCH2 = 146;
const unsigned char BLOCK_SWITCH3 = 147;
const unsigned char BLOCK_START = 148;
const unsigned char BLOCK_FINISH = 149;

const int SWITCH_START_VALUE = 2000;

const unsigned char WEATHER_NORMAL = 0;
const unsigned char WEATHER_RAIN = 1;
const unsigned char WEATHER_FOREST = 2;
const unsigned char WEATHER_RAIN_FOREST = 3;
const unsigned char WEATHER_SNOW = 4;

const unsigned char BACKGROUND_STATIC = 0;
const unsigned char BACKGROUND_PARALLAX_VERT = 1;
const unsigned char BACKGROUND_PARALLAX_HORI = 2;
const unsigned char BACKGROUND_PARALLAX_VERT_AND_HORI = 3;

void Map_Cos_Sin(double *cos_value, double *sin_value);

void Map_Animate(int degree, int anim, int time1, int time2, int time3, bool keys);

void Map_Set_Screen_Dimensions(int width, int height);

class PK2Map {
public:

    /* Attributes ------------------------*/

    char version[5];            // map version, e.g. "12.3"
    char block_bmp[13];    // .BMP file containing block palette
    char background_image[13];        // .BMP file for loading map background image
    char music[13];        // filename of the background music

    char name[40];            // map name
    char author[40];            // map creator

    int stage;                // which stage in the episode
    int climate;                // map climate
    int time;                // time_limit
    unsigned char extra;                // special setting, not defined
    unsigned char background;                // information on how the background image is displayed, not defined
    unsigned long switch1_time;        // how long switch 1 stays down
    unsigned long switch2_time;        // how long switch 2 stays down
    unsigned long switch3_time;        // how long switch 3 stays down
    int player_sprite;        // which prototype the player sprite is made from

    unsigned char backgrounds[MAP_SIZE];    // all map backgrounds 256*224
    unsigned char walls[MAP_SIZE];    // all map walls 256*224
    unsigned char sprites[MAP_SIZE];    // all map sprites 256*224
    char protos[MAX_MAP_PROTOTYPES][13]; // map-related sprite prototypes (file.spr)
    bool edges[MAP_SIZE];

    int block_buffer;        // index buffer containing blocks used in the map
    int background_image_buffer;    // index buffer containing map background image
    int water_block_buffer; // index buffer containing original images of water blocks

    int x, y;                        // Map icon location on map screen
    int icon;                    // Icon number displayed on map screen (from icons.bmp image)

    static char pk2_directory[256]; // Directory containing PK2.exe

    /* Methods --------------------------*/

    PK2Map();                        // Default constructor
    PK2Map(const PK2Map &map);    // Copy constructor
    ~PK2Map();                        // Destructor

    PK2Map &operator=(const PK2Map &map);    // Assignment operator

    int Load(char *path, char *name);        // Load map
    int Load_Only_Data(char *path, char *name);    // Load map without graphics
    int LoadVersion01(char *filename);    // Load map version 0.1
    int LoadVersion10(char *filename);    // Load map version 1.0
    int LoadVersion11(char *filename);    // Load map version 1.1
    int LoadVersion12(char *filename);  // Load map version 1.2
    int LoadVersion13(char *filename);  // Load map version 1.3
    int Save(char *filename);    // Save map (latest version)
    void Clear();                // Clear map
    RECT CalculateSaveArea(unsigned char *source, unsigned char *&target);

    RECT CalculateSaveArea(unsigned char *area);

    void ReadSaveArea(unsigned char *source, RECT area, int target);

    int Load_Background_Image(char *path, char *filename);

    int Load_Block_Palette(char *path, char *filename);

    int Load_Background_Music(char *filename);

    int Draw_Backgrounds(int, int, bool);

    int Draw_Walls(int, int, bool);

    void Copy(PK2Map &map);

    void Animate_Fire(void);

    void Animate_Waterfall(void);

    void Animate_Current_Up(void);

    void Animate_Water_Surface(void);

    void Animate_Water(void);
};

#endif