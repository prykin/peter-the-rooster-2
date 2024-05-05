/*
Pekka Kana 2 v1.2

Change history:

v1.2

- Splitting the episode screen into multiple parts: 10 episodes per screen
- Music loading from the episode directory as well.
- Map music loading from the episode directory
- Sprite loading from the episode directory
- Snow effect
- Episodes sorted alphabetically
- Swapping items with the Tab key
- Skipping the score calculation in the score screen by pressing ENTER
- Max sprites increased to 800
- Font loading from movement_x separate file, which is defined in the language file
- Fixed movement_x bug with reading data outside the map
- Optimized sprite routines.
- Additional log writing
- Scrollbar in language selections
- Browsing menus with keyboard and gamepad
- Added AI for sprites
- Static area where sprites are active (-320 -> +320: -240 -> +240)
- Fixed movement_x score calculation bug
- Invisibility bonus
- Respawning sprites
- Arrow sprites
- Replacing the initial information with infosign.txt file
*/

/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

//#define WIN32_LEAN_AND_MEAN
//#define INITGUID
////#define NDEBUG

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <fstream.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include "Engine/input.h"
#include "Engine/draw.h"
#include "Engine/wait.h"
#include "Engine/sound.h"
#include "Engine/log.h"
#include "Language/language.h"
#include "map.h"
#include "sprites.h"
#include "midasdll.h" // TODO: midas library

//#include "old_headers.h"

// #include "Include/ddraw.h"
// #include "Include/dsound.h"
// #include "Include/dinput.h"
// #include "Include/midasdll.h"

/* TYPE DEFINITIONS ---------------------------------------------------------------------------*/

// ******* C H E A T S *****************
bool cheats = false;
// ******* C H E A T S *****************

const int MAX_SPRITES = 800;//600;
//const int MAX_PROTOTYPES = 100;
const int MAX_ITEMS = 4;
const int MAX_SAVES = 10;

int IDI_ICON1;
int IDC_CURSOR1;

const unsigned char BLOCK_BACKGROUND = 0;
const unsigned char BLOCK_WALL = 1;
const unsigned char BLOCK_SLOPE_RIGHT_UP = 2;
const unsigned char BLOCK_SLOPE_LEFT_UP = 3;
const unsigned char BLOCK_SLOPE_RIGHT_DOWN = 4;
const unsigned char BLOCK_SLOPE_LEFT_DOWN = 5;
const unsigned char BLOCK_SLOPE_UP = 6;
const unsigned char BLOCK_SLOPE_DOWN = 7;

const unsigned char MAX_BLOCK_MASKS = 150;

const unsigned char PARTICLE_NOTHING = 0;
const unsigned char PARTICLE_STAR = 1;
const unsigned char PARTICLE_FEATHER = 2;
const unsigned char PARTICLE_DUST_CLOUD = 3;
const unsigned char PARTICLE_LIGHT = 4;
const unsigned char PARTICLE_SPARK = 5;
const unsigned char PARTICLE_DOT = 6;
const unsigned char PARTICLE_SMOKE_CLOUD = 7;
const unsigned char PARTICLE_STAR2 = 8;

const unsigned char BACKGROUND_PARTICLE_WATER_DROP = 1;
const unsigned char BACKGROUND_PARTICLE_LEAF1 = 2;
const unsigned char BACKGROUND_PARTICLE_LEAF2 = 3;
const unsigned char BACKGROUND_PARTICLE_LEAF3 = 4;
const unsigned char BACKGROUND_PARTICLE_LEAF4 = 5;
const unsigned char BACKGROUND_PARTICLE_SNOWFLAKE1 = 6;
const unsigned char BACKGROUND_PARTICLE_SNOWFLAKE2 = 7;
const unsigned char BACKGROUND_PARTICLE_SNOWFLAKE3 = 8;
const unsigned char BACKGROUND_PARTICLE_SNOWFLAKE4 = 9;

const int MAX_PARTICLES = 300;//200;
const int MAX_BACKGROUND_PARTICLES = 200;
const int MAX_FADE_TEXTS = 40;

const int EPISODE_MAX_STAGES = 50;
const int MAX_EPISODES = 300;

const int MAX_NOTIFICATION_DISPLAY_TIME = 700;

const int STATE_NOT_INITIALIZED = 0;
const int STATE_BASIC_INITIALIZATION = 1;
const int STATE_INTRO = 2;
const int STATE_MENUS = 3;
const int STATE_MAP = 4;
const int STATE_GAME = 5;
const int STATE_SCORE_CALCULATION = 6;
const int STATE_END = 7;

const unsigned char MENU_MAIN = 0;
const unsigned char MENU_EPISODES = 1;
const unsigned char MENU_CONTROLS = 2;
const unsigned char MENU_GRAPHICS = 3;
const unsigned char MENU_SOUNDS = 4;
const unsigned char MENU_NAME = 5;
const unsigned char MENU_LOAD = 6;
const unsigned char MENU_SAVE = 7;

struct EpisodeStage {
    char file[_MAX_PATH];
    char name[40];
    int x, y;
    int order;
    bool completed;
    int icon;
};

struct Block {
    unsigned char code;
    bool background;
    unsigned char left, right, up, down;
    int left_bound, right_bound, top_bound, bottom_bound;
    bool water;
    bool edge;
};

struct BlockMask {
    short int up[32];
    short int down[32];
    short int left[32];
    short int right[32];
};

struct Particle {
    int type;
    double x;
    double y;
    double a;
    double b;
    int animation;
    int duration;
    double weight;
    int color;
};

struct FadeText {
    char text[20];
    int font;
    int x, y, timer;
};

struct SaveData {
    int stage;
    char episode[_MAX_PATH];
    char name[20];
    bool in_use;
    bool stage_completed[EPISODE_MAX_STAGES];
    unsigned long points;
};

struct GameSettings {
    double version; // Version
    bool loaded; // Initialized if not loaded
    char language[_MAX_PATH]; // Language
    // Graphics
    int screen_height;
    int screen_width;
    bool transparent_objects;
    bool transparent_menu_texts;
    bool weather_effects;
    bool show_items;
    bool background_sprites;
    // Controls
    unsigned char control_left;
    unsigned char control_right;
    unsigned char control_jump;
    unsigned char control_down;
    unsigned char control_run;
    unsigned char control_attack1;
    unsigned char control_attack2;
    unsigned char control_use_item;
    // Audio
    bool music;
    bool sounds;
};

struct EpisodeScores {
    // Best scores for each stage in the episode
    unsigned long scores[EPISODE_MAX_STAGES];
    // Top scoring players in each stage in the episode
    char top_players[EPISODE_MAX_STAGES][20];
    // Best times for each stage in the episode
    unsigned long times[EPISODE_MAX_STAGES];
    // Fastest players in each stage in the episode
    char fastest_players[EPISODE_MAX_STAGES][20];

    unsigned long episode_top_scores;
    char episode_top_player[20];
};

/* DEFINITIONS --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define    GAME_NAME          "PEKKA KANA 2 (v1.2)"

// 320x240 | 512x384 | 640x480 | 800x600 | 1024x768 | 1152x864 | 1280x1024
int SCREEN_WIDTH = 800;    //640
int SCREEN_HEIGHT = 600;    //480
const int SCREEN_BPP = 8;
const int MAX_COLORS_PALETTE = 256;

int MARGIN_HORI = 0;
int MARGIN_VERT = 0;

const int SOUNDS_STEREO = 1;
const int SOUNDS_SAMPLERATE = 22050;
const int SOUNDS_BITRATE = 8;

int MAP_DRAWING_WIDTH = 800;
int MAP_DRAWING_HEIGHT = 480;
bool LIMIT_MAP_DRAWING_AREA = true;

/* GLOBAL VARIABLES ---------------------------------------------------------------------------*/
HWND window_handle = NULL;         // Main window handle
HINSTANCE hinstance_app = NULL;     // hinstance?
HDC global_dc = NULL;             // global dc?

bool window_active = true;            // Is the window active or minimized?

bool PK2_error = false;             // If this becomes true, the program will terminate
char PK2_error_message[2] = " ";    // Not yet (properly) in use

bool window_closed = false;         // Is the window closed?
bool end_game = false;

bool unload = false;
bool tables_calculated = false;

// SETTINGS
GameSettings settings;

// START SETUP VARIABLES
HWND hSetupWindow, hRB_640x480, hRB_800x600, hRB_1024x768, hBT_Play, hBT_Music, hBT_Exit, hCB_Music, hCB_SFX, hCMB_Languages;

const int ID_RB = 100, ID_BT_Play = 500, ID_BT_Music = 600, ID_BT_Exit = 700, ID_CB_Music = 800, ID_CB_SFX = 900, ID_CMB_Languages = 1000;

bool initial_setup = true;

// INFO VARIABLES
bool draw_info = false;
int info_sprites = 0;
int info_sprites_drawn = 0;
int info_sprites_active = 0;

char message[60];

// MAP
PK2Map *map;
char next_map[_MAX_PATH];

// SPRITES AND THEIR PROTOTYPES
PK2Sprite_Prototype prototypes[MAX_PROTOTYPES];

PK2Sprite sprites[MAX_SPRITES];

int background_sprites[MAX_SPRITES]; //background_sprites

int next_free_proto = 0;

PK2Sprite_Prototype *items[MAX_ITEMS];
int num_items = 0;

// BLOCKS AND MASKS
Block blocks[300];

Block calculated_blocks[150];//150

BlockMask block_masks[MAX_BLOCK_MASKS];

// PARTICLES
Particle particles[MAX_PARTICLES];
int particle_index = 0;

Particle background_particles[MAX_BACKGROUND_PARTICLES];
int background_particle_index = 0;

// FADE TEXTS
FadeText fade_texts[MAX_FADE_TEXTS];
int fade_text_index = 0;

// IMAGE BUFFERS
int game_image;
int game_image_sysmem;
int background_image;

// FONTS
int font1;
int font2;
int font3;
int font4;
int font5;

// CONTROLS
int mouse_x = 10;
int mouse_y = 10;
double menu_cursor_x = SCREEN_WIDTH / 2;
double menu_cursor_y = SCREEN_HEIGHT / 2;
double menu_cursor_xk = 0;
double menu_cursor_yk = 0;

unsigned char control_left = PI_KB_LEFT;
unsigned char control_right = PI_KB_RIGHT;
unsigned char control_jump = PI_KB_UP;
unsigned char control_down = PI_KB_DOWN;
unsigned char control_run = PI_KB_RALT;
unsigned char control_attack1 = PI_KB_RCONTROL;
unsigned char control_attack2 = PI_KB_RSHIFT;
unsigned char control_use_item = PI_KB_SPACE;

int key_delay = 0;

// CAMERA COORDINATES
int camera_x = 0;
int camera_y = 0;
double dcamera_x = 0;
double dcamera_y = 0;
double dcamera_a = 0;
double dcamera_b = 0;

// STAGE AND EPISODE
int stage = 1;
int num_stages = 1;
int num_episodes = 0;
int current_stage_index = 1;
char episodes[MAX_EPISODES][_MAX_PATH];
char episode[_MAX_PATH];
int episode_page = 0;
EpisodeStage stages[EPISODE_MAX_STAGES];
bool stage_completed = false;
bool retry = false;
bool game_over = false;
unsigned long end_timer = 0;
unsigned long stage_points = 0;
unsigned long fake_points = 0;

// PLAYER
int player_index = 0;
unsigned long points = 0;
unsigned long points_add = 0;
char player_name[20] = " ";

bool name_edit = false;

// BLOCK RELATED TIMERS
int switch1 = 0, switch2 = 0, switch3 = 0;
int block_animation = 0;

// SOUND EFFECTS
int switch_sound, jump_sound, splash_sound, unlock_sound, menu_sound, shoot_sound, crow_sound, explosion_sound, score_counter_sound;
int sprite_sounds[50]; // sound buffers used by sprites
int sound_volume = 90;

// MUSIC
int music_volume = 64;
int music_max_volume = 64;
int music_current_volume = 64;

// BACKGROUND IMAGE
//int background_extr;

// SAVES
SaveData saves[MAX_SAVES];
int load_game = -1;

// OTHER
double cos_table[360];
double sin_table[360];
unsigned char color_table31[256];
unsigned char color_table_gray[256];
unsigned char proc_table[32][100];
unsigned char color_table[256][256][100];

int degree = 0, degree_temp = 0;

int num_keys = 0;

unsigned long game_time = 0;
int seconds = 0;
bool time_limit = false;

int earthquake = 0;
int story_switch = 0;

int item_panel_x = 10;

int notification_timer = 0;
char notification[80] = " ";

// SCORE CALCULATION
EpisodeScores episode_scores;

int score_calculation_phase = 0;
int score_calculation_delay = 0;
unsigned long bonus_points = 0, time_points = 0, energy_points = 0, item_points = 0, rescue_points = 0;

bool stage_new_record = false;
bool stage_new_record_time = false;
bool episode_new_record = false;
bool episode_new_record_shown = false;

// GAME VARIABLES
char working_directory[_MAX_PATH];
bool pause = false;
int game_state = STATE_NOT_INITIALIZED;
int next_game_state = STATE_BASIC_INITIALIZATION;
bool game_in_progress = false;
bool switch_from_map_to_game = false;
bool switch_from_score_to_map = false;

int invisibility = 0;

// INTRO
unsigned long intro_counter = 0;
bool switch_from_intro_to_menu = false;

// END SCREEN
unsigned long end_counter = 0;
bool switch_from_end_to_menu = false;

// GRAPHICS
bool double_speed = false;
bool skip_frame = false;

// MENUS
int current_menu = MENU_MAIN;
int read_controls_menu = 0;
int menu_name_index = 0;
char menu_name_prev_char = '\0';
int selected_menu_id = 0;
int choice_menu_id = 1;

RECT menu_rectangle;

// MIDAS
MIDASmodule module;
MIDASmodulePlayHandle playHandle;
bool MIDASerror = false;
bool module_loaded = false;
bool music_playing = false;
char current_song[255] = "";
unsigned char music_max_volume_char = 64;

// FRAMERATE
int fps = 0;
int fps_counter = 0;
unsigned long fps_time = 0;
bool show_fps = false;

// LANGUAGE AND GAME TEXTS
PisteLanguage *texts;

int txt_setup_options, txt_setup_videomodes, txt_setup_music_and_sounds, txt_setup_music, txt_setup_sounds, txt_setup_language, txt_setup_play, txt_setup_exit,
    txt_intro_presents, txt_intro_a_game_by, txt_intro_original, txt_intro_tested_by, txt_intro_thanks_to, txt_intro_translation, txt_intro_translator,
    txt_mainmenu_new_game, txt_mainmenu_continue, txt_mainmenu_load_game, txt_mainmenu_save_game, txt_mainmenu_controls, txt_mainmenu_graphics, txt_mainmenu_sounds, txt_mainmenu_exit,
    txt_mainmenu_return,
    txt_loadgame_title, txt_loadgame_info, txt_loadgame_episode, txt_loadgame_level,
    txt_savegame_title, txt_savegame_info, txt_savegame_episode, txt_savegame_level,
    txt_controls_title, txt_controls_moveleft, txt_controls_moveright, txt_controls_jump, txt_controls_duck, txt_controls_walkslow, txt_controls_eggattack, txt_controls_doodleattack, txt_controls_useitem, txt_controls_edit, txt_controls_kbdef, txt_controls_gp4def, txt_controls_gp6def,
    txt_gfx_title, txt_gfx_tfx_on, txt_gfx_tfx_off, txt_gfx_tmenus_on, txt_gfx_tmenus_off, txt_gfx_items_on, txt_gfx_items_off, txt_gfx_weather_on, txt_gfx_weather_off, txt_gfx_bgsprites_on, txt_gfx_bgsprites_off, txt_gfx_speed_normal, txt_gfx_speed_double,
    txt_sound_title, txt_sound_sfx_volume, txt_sound_music_volume, txt_sound_more, txt_sound_less,
    txt_playermenu_type_name, txt_playermenu_continue, txt_playermenu_clear, txt_player_default_name,
    txt_episodes_choose_episode, txt_episodes_no_maps, txt_episodes_get_more,
    txt_map_total_score, txt_map_next_level, txt_map_episode_best_player, txt_map_episode_hiscore, txt_map_level_best_player, txt_map_level_hiscore, txt_map_level_fastest_player, txt_map_level_best_time,
    txt_score_screen_title, txt_score_screen_level_score, txt_score_screen_bonus_score, txt_score_screen_time_score, txt_score_screen_energy_score, txt_score_screen_item_score, txt_score_screen_total_score, txt_score_screen_new_level_hiscore, txt_score_screen_new_level_best_time, txt_score_screen_new_episode_hiscore, txt_score_screen_continue,
    txt_game_score, txt_game_time, txt_game_energy, txt_game_items, txt_game_attack1, txt_game_attack2, txt_game_keys, txt_game_clear, txt_game_timebonus, txt_game_ko, txt_game_timeout, txt_game_tryagain, txt_game_locksopen, txt_game_newdoodle, txt_game_newegg, txt_game_newitem, txt_game_loading, txt_game_paused,
    txt_end_congratulations, txt_end_chickens_saved, txt_end_the_end,
    txt_info01, txt_info02, txt_info03, txt_info04, txt_info05, txt_info06, txt_info07, txt_info08, txt_info09, txt_info10, txt_info11, txt_info12, txt_info13, txt_info14, txt_info15, txt_info16, txt_info17, txt_info18, txt_info19;

/* PROTOTYPES --------------------------------------------------------------------------------*/
int Set_Map_Sprites();

void Initialize_Items();

void Add_Episode_Directory(char *file);

/* GAME ---------------------------------------------------------------------------------------*/
bool Is_File(char *filename) {
    ifstream *file = new ifstream(filename, ios::binary); //| ios::nocreate);
    if (file->fail()) {
        delete (file);
        return false;
    }
    delete (file);
    return true;
}

// SETTINGS
void Initialize_Settings() {
    settings.loaded = false;
    strcpy(settings.language, "english.txt");

    settings.screen_width = 640;
    settings.screen_height = 480;
    settings.transparent_objects = true;
    settings.transparent_menu_texts = false;
    settings.weather_effects = true;
    settings.show_items = true;
    settings.background_sprites = true;

    settings.sounds = true;
    settings.music = true;

    settings.control_left = PI_KB_LEFT;
    settings.control_right = PI_KB_RIGHT;
    settings.control_jump = PI_KB_UP;
    settings.control_down = PI_KB_DOWN;
    settings.control_run = PI_KB_RALT;
    settings.control_attack1 = PI_KB_RCONTROL;
    settings.control_attack2 = PI_KB_RSHIFT;
    settings.control_use_item = PI_KB_SPACE;
}

int Load_Settings(char *filename) {
    PisteLog_Write("- Trying to load settings from movement_x file.\n");
    ifstream *file = new ifstream(filename, ios::binary);// | ios::nocreate);
    char version[4];
    if (file->fail()) {
        PisteLog_Write("  - No setting file found. Creating default settings.\n");
        delete (file);
        Initialize_Settings();
        return 1;
    }
    file->read((char *) version, 4);
    if (strcmp(version, "1.0") == 0) {
        PisteLog_Write("- Found movement_x version 1.0 settings file.\n");
        file->read((char *) &settings, sizeof(settings));
    }
    delete (file);
    settings.loaded = true;

    control_left = settings.control_left;
    control_right = settings.control_right;
    control_jump = settings.control_jump;
    control_down = settings.control_down;
    control_run = settings.control_run;
    control_attack1 = settings.control_attack1;
    control_attack2 = settings.control_attack2;
    control_use_item = settings.control_use_item;

    return 0;
}

int Save_Settings(char *filename) {
    settings.screen_height = SCREEN_HEIGHT;
    settings.screen_width = SCREEN_WIDTH;

    settings.control_left = control_left;
    settings.control_right = control_right;
    settings.control_jump = control_jump;
    settings.control_down = control_down;
    settings.control_run = control_run;
    settings.control_attack1 = control_attack1;
    settings.control_attack2 = control_attack2;
    settings.control_use_item = control_use_item;

    ofstream *file = new ofstream(filename, ios::binary);
    file->write("1.0", 4);
    file->write((char *) &settings, sizeof(settings));

    delete (file);
    return 0;
}

// POINTS
void Initialize_Episode_Scores() {
    for (int i = 0; i < EPISODE_MAX_STAGES; i++) {
        episode_scores.scores[i] = 0;
        episode_scores.times[i] = 0;
        strcpy(episode_scores.top_players[i], " ");
        strcpy(episode_scores.fastest_players[i], " ");
    }

    episode_scores.episode_top_scores = 0;
    strcpy(episode_scores.episode_top_player, " ");
}

int Compare_Episode_Scores(int episode, unsigned long scores, unsigned long time, bool final_scores) {
    int count = 0;
    if (!final_scores) {
        if (scores > episode_scores.scores[episode]) {
            strcpy(episode_scores.top_players[episode], player_name);
            episode_scores.scores[episode] = scores;
            stage_new_record = true;
            count++;
        }
        if ((time < episode_scores.times[episode] || episode_scores.times[stage] == 0) && map->time > 0) {
            strcpy(episode_scores.fastest_players[episode], player_name);
            episode_scores.times[episode] = time;
            stage_new_record_time = true;
            count++;
        }
    } else {
        if (scores > episode_scores.episode_top_scores) {
            episode_scores.episode_top_scores = scores;
            strcpy(episode_scores.episode_top_player, player_name);
            episode_new_record = true;
            count++;
        }
    }
    return count;
}

int Load_Episode_Scores(char *filename) {
    Add_Episode_Directory(filename);
    ifstream *file = new ifstream(filename, ios::binary);// | ios::nocreate);
    char version[4];
    if (file->fail()) {
        delete (file);
        Initialize_Episode_Scores();
        return 1;
    }
    file->read((char *) version, 4);
    if (strcmp(version, "1.0") == 0) {
        file->read((char *) &episode_scores, sizeof(episode_scores));
    }
    delete (file);
    return 0;
}

int Save_Episode_Scores(char *filename) {
    Add_Episode_Directory(filename);
    ofstream *file = new ofstream(filename, ios::binary);
    file->write("1.0", 4);
    file->write((char *) &episode_scores, sizeof(episode_scores));
    delete (file);
    return 0;
}

// LANGUAGE AND TEXTS
void Load_Additional_Information() {
    PisteLanguage *temp = new PisteLanguage();
    char info_file[_MAX_PATH] = "infosign.txt";
    int index1 = 0;
    int index2 = 0;
    char title[255];
    Add_Episode_Directory(info_file);
    if (Is_File(info_file)) {
        if (temp->Read_File(info_file)) {
            for (int i = 0; i < 19; i++) {
                switch (i + 1) {
                    case 1:
                        strcpy(title, "info01");
                        break;
                    case 2:
                        strcpy(title, "info02");
                        break;
                    case 3:
                        strcpy(title, "info03");
                        break;
                    case 4:
                        strcpy(title, "info04");
                        break;
                    case 5:
                        strcpy(title, "info05");
                        break;
                    case 6:
                        strcpy(title, "info06");
                        break;
                    case 7:
                        strcpy(title, "info07");
                        break;
                    case 8:
                        strcpy(title, "info08");
                        break;
                    case 9:
                        strcpy(title, "info09");
                        break;
                    case 10:
                        strcpy(title, "info10");
                        break;
                    case 11:
                        strcpy(title, "info11");
                        break;
                    case 12:
                        strcpy(title, "info12");
                        break;
                    case 13:
                        strcpy(title, "info13");
                        break;
                    case 14:
                        strcpy(title, "info14");
                        break;
                    case 15:
                        strcpy(title, "info15");
                        break;
                    case 16:
                        strcpy(title, "info16");
                        break;
                    case 17:
                        strcpy(title, "info17");
                        break;
                    case 18:
                        strcpy(title, "info18");
                        break;
                    case 19:
                        strcpy(title, "info19");
                        break;
                    default:
                        break;
                }
                if ((index1 = texts->Get_Index(title)) != -1 && (index2 = temp->Get_Index(title)) != -1) {
                    texts->Replace_Text(index1, temp->Get_Text(index2));
                }
            }
        }
    }
    delete (temp);
}

bool Load_Language(char *file) {
    PisteLog_Write("- Trying to load movement_x language file: ");
    PisteLog_Write(file);
    PisteLog_Write(" \n");
    if (!texts->Read_File(file)) {
        PisteLog_Write("  - Loading file failed.\n");
        PK2_error = true;
        return false;
    }
    // Starting window
    txt_setup_options = texts->Get_Index("setup options");
    txt_setup_videomodes = texts->Get_Index("setup video modes");
    txt_setup_music_and_sounds = texts->Get_Index("setup music & sounds");
    txt_setup_music = texts->Get_Index("setup music");
    txt_setup_sounds = texts->Get_Index("setup sounds");
    txt_setup_language = texts->Get_Index("setup language");
    txt_setup_play = texts->Get_Index("setup play");
    txt_setup_exit = texts->Get_Index("setup exit");

    // Intro
    txt_intro_presents = texts->Get_Index("intro presents");
    txt_intro_a_game_by = texts->Get_Index("intro movement_x game by");
    txt_intro_original = texts->Get_Index("intro original character design");
    txt_intro_tested_by = texts->Get_Index("intro tested by");
    txt_intro_thanks_to = texts->Get_Index("intro thanks to");
    txt_intro_translation = texts->Get_Index("intro translation");
    txt_intro_translator = texts->Get_Index("intro translator");

    // Main menu
    txt_mainmenu_new_game = texts->Get_Index("main menu new game");
    txt_mainmenu_continue = texts->Get_Index("main menu continue");
    txt_mainmenu_load_game = texts->Get_Index("main menu load game");
    txt_mainmenu_save_game = texts->Get_Index("main menu save game");
    txt_mainmenu_controls = texts->Get_Index("main menu controls");
    txt_mainmenu_graphics = texts->Get_Index("main menu graphics");
    txt_mainmenu_sounds = texts->Get_Index("main menu sounds");
    txt_mainmenu_exit = texts->Get_Index("main menu exit game");

    txt_mainmenu_return = texts->Get_Index("back to main menu");

    // Loading
    txt_loadgame_title = texts->Get_Index("load menu title");
    txt_loadgame_info = texts->Get_Index("load menu info");
    txt_loadgame_episode = texts->Get_Index("load menu episode");
    txt_loadgame_level = texts->Get_Index("load menu level");

    // Saving
    txt_savegame_title = texts->Get_Index("save menu title");
    txt_savegame_info = texts->Get_Index("save menu info");
    txt_savegame_episode = texts->Get_Index("save menu episode");
    txt_savegame_level = texts->Get_Index("save menu level");

    // Controls
    txt_controls_title = texts->Get_Index("controls menu title");
    txt_controls_moveleft = texts->Get_Index("controls menu move left");
    txt_controls_moveright = texts->Get_Index("controls menu move right");
    txt_controls_jump = texts->Get_Index("controls menu jump");
    txt_controls_duck = texts->Get_Index("controls menu duck");
    txt_controls_walkslow = texts->Get_Index("controls menu walk slow");
    txt_controls_eggattack = texts->Get_Index("controls menu egg attack");
    txt_controls_doodleattack = texts->Get_Index("controls menu doodle attack");
    txt_controls_useitem = texts->Get_Index("controls menu use item");
    txt_controls_edit = texts->Get_Index("controls menu edit");
    txt_controls_kbdef = texts->Get_Index("controls menu keyboard def");
    txt_controls_gp4def = texts->Get_Index("controls menu gamepad4");
    txt_controls_gp6def = texts->Get_Index("controls menu gamepad6");

    txt_gfx_title = texts->Get_Index("graphics menu title");
    txt_gfx_tfx_on = texts->Get_Index("graphics menu transparency fx on");
    txt_gfx_tfx_off = texts->Get_Index("graphics menu transparency fx off");
    txt_gfx_tmenus_on = texts->Get_Index("graphics menu menus are transparent");
    txt_gfx_tmenus_off = texts->Get_Index("graphics menu menus are not transparent");
    txt_gfx_items_on = texts->Get_Index("graphics menu item bar is visible");
    txt_gfx_items_off = texts->Get_Index("graphics menu item bar is not visible");
    txt_gfx_weather_on = texts->Get_Index("graphics menu weather fx on");
    txt_gfx_weather_off = texts->Get_Index("graphics menu weather fx off");
    txt_gfx_bgsprites_on = texts->Get_Index("graphics menu bg sprites on");
    txt_gfx_bgsprites_off = texts->Get_Index("graphics menu bg sprites off");
    txt_gfx_speed_normal = texts->Get_Index("graphics menu game speed normal");
    txt_gfx_speed_double = texts->Get_Index("graphics menu game speed double");

    txt_sound_title = texts->Get_Index("sounds menu title");
    txt_sound_sfx_volume = texts->Get_Index("sounds menu sfx volume");
    txt_sound_music_volume = texts->Get_Index("sounds menu music volume");
    txt_sound_more = texts->Get_Index("sounds menu more");
    txt_sound_less = texts->Get_Index("sounds menu less");

    txt_playermenu_type_name = texts->Get_Index("player screen type your name");
    txt_playermenu_continue = texts->Get_Index("player screen continue");
    txt_playermenu_clear = texts->Get_Index("player screen clear");
    txt_player_default_name = texts->Get_Index("player default name");

    txt_episodes_choose_episode = texts->Get_Index("episode menu choose episode");
    txt_episodes_no_maps = texts->Get_Index("episode menu no maps");
    txt_episodes_get_more = texts->Get_Index("episode menu get more episodes at");

    txt_map_total_score = texts->Get_Index("map screen total score");
    txt_map_next_level = texts->Get_Index("map screen next level");
    txt_map_episode_best_player = texts->Get_Index("episode best player");
    txt_map_episode_hiscore = texts->Get_Index("episode hiscore");
    txt_map_level_best_player = texts->Get_Index("level best player");
    txt_map_level_hiscore = texts->Get_Index("level hiscore");
    txt_map_level_fastest_player = texts->Get_Index("level fastest player");
    txt_map_level_best_time = texts->Get_Index("level best time");

    txt_score_screen_title = texts->Get_Index("score screen title");
    txt_score_screen_level_score = texts->Get_Index("score screen level score");
    txt_score_screen_bonus_score = texts->Get_Index("score screen bonus score");
    txt_score_screen_time_score = texts->Get_Index("score screen time score");
    txt_score_screen_energy_score = texts->Get_Index("score screen health score");
    txt_score_screen_item_score = texts->Get_Index("score screen item score");
    txt_score_screen_total_score = texts->Get_Index("score screen total score");
    txt_score_screen_new_level_hiscore = texts->Get_Index("score screen new level hiscore");
    txt_score_screen_new_level_best_time = texts->Get_Index("score screen new level best time");
    txt_score_screen_new_episode_hiscore = texts->Get_Index("score screen new episode hiscore");
    txt_score_screen_continue = texts->Get_Index("score screen continue");

    txt_game_score = texts->Get_Index("score");
    txt_game_time = texts->Get_Index("game time");
    txt_game_energy = texts->Get_Index("health");
    txt_game_items = texts->Get_Index("items");
    txt_game_attack1 = texts->Get_Index("attack 1");
    txt_game_attack2 = texts->Get_Index("attack 2");
    txt_game_keys = texts->Get_Index("keys");
    txt_game_clear = texts->Get_Index("level clear");
    txt_game_timebonus = texts->Get_Index("time bonus");
    txt_game_ko = texts->Get_Index("knocked out");
    txt_game_timeout = texts->Get_Index("time out");
    txt_game_tryagain = texts->Get_Index("try again");
    txt_game_locksopen = texts->Get_Index("locks open");
    txt_game_newdoodle = texts->Get_Index("new doodle attack");
    txt_game_newegg = texts->Get_Index("new egg attack");
    txt_game_newitem = texts->Get_Index("new item");
    txt_game_loading = texts->Get_Index("loading");
    txt_game_paused = texts->Get_Index("game paused");

    txt_end_congratulations = texts->Get_Index("end congratulations");
    txt_end_chickens_saved = texts->Get_Index("end chickens saved");
    txt_end_the_end = texts->Get_Index("end the end");

    txt_info01 = texts->Get_Index("info01");
    txt_info02 = texts->Get_Index("info02");
    txt_info03 = texts->Get_Index("info03");
    txt_info04 = texts->Get_Index("info04");
    txt_info05 = texts->Get_Index("info05");
    txt_info06 = texts->Get_Index("info06");
    txt_info07 = texts->Get_Index("info07");
    txt_info08 = texts->Get_Index("info08");
    txt_info09 = texts->Get_Index("info09");
    txt_info10 = texts->Get_Index("info10");
    txt_info11 = texts->Get_Index("info11");
    txt_info12 = texts->Get_Index("info12");
    txt_info13 = texts->Get_Index("info13");
    txt_info14 = texts->Get_Index("info14");
    txt_info15 = texts->Get_Index("info15");
    txt_info16 = texts->Get_Index("info16");
    txt_info17 = texts->Get_Index("info17");
    txt_info18 = texts->Get_Index("info18");
    txt_info19 = texts->Get_Index("info19");

    PisteLog_Write("  - Loading file completed.\n");
    return true;
}

// MIDAS
int Midas_Init() {
    if (settings.music) {
        PisteLog_Write("  - MIDASstartup \n");
        MIDASstartup();
        PisteLog_Write("  - MIDASloadConfig(\"data\\msetup.ini\") \n");
        if (!MIDASloadConfig("data\\msetup.ini")) {
            PisteLog_Write("  - data\\msetup.ini not found, showing MIDASconfig window \n");
            if (!MIDASconfig()) {
                PisteLog_Write("  - MIDASconfig failed! \n");
                MIDASerror = true;
                return (1);
            } else {
                PisteLog_Write("  - MIDASsaveConfig \n");
                MIDASsaveConfig("data\\msetup.ini");
            }
        }
        MIDASsetOption(MIDAS_OPTION_DSOUND_HWND, (unsigned long) window_handle);
        MIDASsetOption(MIDAS_OPTION_DSOUND_MODE, MIDAS_DSOUND_FORCE_STREAM);
        MIDASsetOption(MIDAS_OPTION_DSOUND_OBJECT, (unsigned long) PisteSound_Get_DirectSound());
        MIDASinit();
        MIDASstartBackgroundPlay(0);
    }

    return 0;
}



void Midas_End() {
    if (settings.music) {
        if (/*!MIDASerror*/true) {
            if (module_loaded) {
                if (music_playing)
                    MIDASstopModule(playHandle);
                MIDASfreeModule(module);
            }
        }
        MIDASstopBackgroundPlay();
        MIDASclose();
    }
}

int Midas_Stop() {
    if (settings.music) {
        if (music_playing) {
            if (playHandle != NULL)
                MIDASstopModule(playHandle);
            music_playing = false;
        }
    }

    return 0;
}

int Midas_Play() {
    if (settings.music) {
        if (!music_playing) {
            playHandle = MIDASplayModule(module, true);
            music_playing = true;
        }
    }

    return (0);
}

int Midas_Load_Music(char *filename) {
    PisteLog_Write("  - MIDAS: Loading music file: ");
    PisteLog_Write(filename);
    PisteLog_Write("\n");
    if (settings.music) {
        if (!MIDASerror && strcmp(filename, current_song) != 0) {
            if (!Is_File(filename)) {
                PisteLog_Write("  - File not found. \n");
                return 2;
            }
            strcpy(current_song, filename);
            if (module_loaded) {
                Midas_Stop();
                MIDASfreeModule(module);
                module_loaded = false;
            }
            if (!(module = MIDASloadModule(filename))) {
                PisteLog_Write("  - Loading file failed! \n");
                return 1;
            }
            playHandle = MIDASplayModule(module, true);
            module_loaded = true;
            music_playing = true;
            if (!(MIDASsetMusicVolume(playHandle, music_max_volume))) {
                PisteLog_Write("  - Setting music volume failed! \n");
                return 1;
            }
        }
    }
    return (0);
}

void Add_Working_Directory(char *file) {
    char new_file[255];
    strcpy(new_file, working_directory);
    strcat(new_file, "\\");
    strcat(new_file, file);
    strcpy(file, new_file);
}

void Add_Episode_Directory(char *file) {
    char new_file[255];
    strcpy(new_file, working_directory);
    strcat(new_file, "\\episodes\\");
    strcat(new_file, episode);
    strcat(new_file, "\\");
    strcat(new_file, file);
    strcpy(file, new_file);
}

void New_Game() {
    points = 0;
    stage = 1;
}

void New_Level() {
    game_time = map->time;
    if (game_time > 0)
        time_limit = true;
    else
        time_limit = false;
    end_timer = 0;
    seconds = 1000;
    stage_points = 0;//points;
    game_over = false;
    stage_completed = false;
    switch1 = 0;
    switch1 = 0;
    switch3 = 0;
    story_switch = 0;
    earthquake = 0;
    pause = false;
    notification_timer = 0;
    invisibility = 0;
}

void Initialize_Levels() {
    for (int i = 0; i < EPISODE_MAX_STAGES; i++) {
        strcpy(stages[i].name, "");
        strcpy(stages[i].file, "");
        stages[i].x = 0;
        stages[i].y = 0;
        stages[i].order = -1;
        stages[i].completed = false;
        stages[i].icon = 0;
    }
}

void Find_Levels() {
    char directory[_MAX_PATH];
    PK2Map *temp = new PK2Map();
    strcpy(directory, "");
    Add_Episode_Directory(directory);
    strcat(directory, "*.map");
    struct _finddata_t map_file; //TODO: _finddata_t?
    long hFile;
    int i = 0;
    if ((hFile = _findfirst(directory, &map_file)) == -1L)
        strcpy(message, "no maps found");
    else {
        strcpy(stages[i].file, map_file.name);
        i++;
    }
    while (i < EPISODE_MAX_STAGES && _findnext(hFile, &map_file) == 0) {
        strcpy(stages[i].file, map_file.name);
        i++;
    }
    _findclose(hFile);
    strcpy(directory, "");
    Add_Episode_Directory(directory);
    num_stages = i;
    for (i = 0; i <= num_stages; i++) {
        if (temp->Load_Only_Data(directory, stages[i].file) == 0) {
            strcpy(stages[i].name, temp->name);
            stages[i].x = temp->x;// 142 + i*35;
            stages[i].y = temp->y;// 270;
            stages[i].order = temp->stage;
            stages[i].icon = temp->icon;
        }
    }
    EpisodeStage stage;
    bool stop = false;
    while (!stop) {
        stop = true;
        for (i = 0; i < num_stages; i++) {
            if (stages[i].order > stages[i + 1].order) {
                stage = stages[i];
                stages[i] = stages[i + 1];
                stages[i + 1] = stage;
                stop = false;
            }
        }
    }
    delete temp;
}

int Compare_Episodes(char *a, char *b) {
    int a_len = strlen(a);
    int b_len = strlen(b);
    int loop = a_len;
    if (b_len < a_len)
        loop = b_len;
    strlwr(a);
    strlwr(b);
    for (int i = 0; i < loop; i++) {
        if (a[i] < b[i])
            return 2;
        if (a[i] > b[i])
            return 1;
    }
    if (a_len > b_len)
        return 1;
    if (a_len < b_len)
        return 2;

    return 0;
}

int Sort_Episodes(void) {
    unsigned long i, t;
    char temp[_MAX_PATH] = "";
    bool done;
    if (num_episodes > 1) {
        for (i = num_episodes - 1; i >= 0; i--) {
            done = true;
            //for (t=0;t<i;t++) {
            for (t = 2; t < i + 2; t++) {
                if (Compare_Episodes(episodes[t], episodes[t + 1]) == 1) {
                    strcpy(temp, episodes[t]);
                    strcpy(episodes[t], episodes[t + 1]);
                    strcpy(episodes[t + 1], temp);
                    done = false;
                }
            }
            if (done)
                return 0;
        }
    }

    return 0;
}

int Find_Episodes() {
    for (int i = 0; i < MAX_EPISODES; i++)
        strcpy(episodes[i], "");
    char directory[_MAX_PATH];
    strcpy(directory, "episodes\\");
    strcat(directory, "*.");
    struct _finddata_t map_file;
    long hFile;
    int i = 0;
    num_episodes = 0;
    if ((hFile = _findfirst(directory, &map_file)) == -1L)
        return 1;
    else {
        strcpy(episodes[i], map_file.name);
        i++;
    }
    while (i < MAX_EPISODES && _findnext(hFile, &map_file) == 0) {
        strcpy(episodes[i], map_file.name);
        i++;
    }
    _findclose(hFile);
    num_episodes = i - 2;
    Sort_Episodes();

    return 0;
}

int Clear_Saves(void) {
    for (int i = 0; i < MAX_SAVES; i++) {
        saves[i].in_use = false;
        strcpy(saves[i].episode, " ");
        strcpy(saves[i].name, "empty");
        saves[i].stage = 0;
        saves[i].points = 0;
        for (int j = 0; j < EPISODE_MAX_STAGES; j++)
            saves[i].stage_completed[j] = false;
    }

    return 0;
}

// TODO: continue
int Find_All_Saves(char *filename) {
    char version[2];
    char lkmc[8];
    int count, i;
    ifstream *file = new ifstream(filename, ios::binary);
    if (file->fail()) {
        delete (file);
        Clear_Saves();
        return 1;
    }
    Clear_Saves();
    file->read(version, sizeof(version));
    if (strcmp(version, "1") == 0) {
        file->read(lkmc, sizeof(lkmc));
        count = atoi(lkmc);
        for (i = 0; i < count; i++)
            file->read((char *) &saves[i], sizeof(saves[i]));
    }
    delete (file);

    return 0;
}

int Save_All_Saves(char *filename) {
    char version[2] = "1";
    char count[8];
    itoa(MAX_SAVES, count, 10);
    ofstream *file = new ofstream(filename, ios::binary);
    file->write(version, sizeof(version));
    file->write(count, sizeof(count));
    for (int i = 0; i < MAX_SAVES; i++) {
        file->write((char *) &saves[i], sizeof(saves[i]));
    }
    delete (file);

    return 0;
}

int Load_Game(int i) {
    if (strcmp(saves[i].episode, " ") != 0) {
        strcpy(episode, saves[i].episode);
        strcpy(player_name, saves[i].name);
        stage = saves[i].stage;
        points = saves[i].points;
        Initialize_Levels();
        //for (int j = 0;j < EPISODE_MAX_STAGES;j++)
        //	stages[j].completed = saves[i].stage_completed[j];
        next_game_state = STATE_MAP;
        load_game = i;
        game_in_progress = false;
    }

    return 0;
}

int Save_Game(int i) {
    saves[i].in_use = true;
    strcpy(saves[i].episode, episode);
    strcpy(saves[i].name, player_name);
    saves[i].stage = stage;
    saves[i].points = points;
    for (int j = 0; j < EPISODE_MAX_STAGES; j++)
        saves[i].stage_completed[j] = stages[j].completed;
    Save_All_Saves("data\\saves.dat");

    return 0;
}

void Play_Sound(int sound, int volume, int x, int y, int freq, bool random_freq) {
    if (sound > -1 && sound_volume > 0 && volume > 0) {
        if (x < camera_x + SCREEN_WIDTH && x > camera_x && y < camera_y + SCREEN_HEIGHT && y > camera_y) {
            volume = volume / (100 / sound_volume);
            if (volume > 100)
                volume = 100;
            if (volume < 0)
                volume = 0;
            int pan = camera_x + (SCREEN_WIDTH / 2) - x;
            if (random_freq)
                freq = freq + rand() % 4000 - rand() % 2000;
            //if (PisteSound_SFX_Play(sound,sound_volume) == PS_ERROR)
            //	PK2_error = true;
            if (PisteSound_SFX_Play(sound, sound_volume, pan * -2, freq) == PS_ERROR)
                PK2_error = true;
        }
    }
}

void Play_Sound_Menu(int sound, int volume) {
    if (sound > -1 && sound_volume > 0 && volume > 0) {
        volume = volume / (100 / sound_volume);
        if (volume > 100)
            volume = 100;
        if (volume < 0)
            volume = 0;
        int freq = 22050 + rand() % 5000 - rand() % 5000;
        if (PisteSound_SFX_Play(sound, sound_volume, 0, freq) == PS_ERROR)
            PK2_error = true;
    }
}

void Calculate_Tables(void) {
    int i;
    for (i = 0; i < 256; i++) {
        if (i < 32)
            color_table31[i] = i;
        else
            color_table31[i] = 31;
    }
    for (i = 0; i < 256; i++)
        color_table_gray[i] = i % 32;
    for (int ci = 0; ci < 360; ci++)
        cos_table[ci] = cos(3.1415 * 2 * (ci % 360) / 180) * 33;
    for (int si = 0; si < 360; si++)
        sin_table[si] = sin(3.1415 * 2 * (si % 360) / 180) * 33;
    for (i = 0; i < 32; i++)
        for (int percent = 0; percent < 100; percent++)
            proc_table[i][percent] = (i * percent) / 100;
    int c, v1, v2;
    for (v1 = 0; v1 < 256; v1++)
        for (v2 = 0; v2 < 256; v2++)
            for (int percent = 0; percent < 100; percent++) {
                c = v1 % 32;
                c = (c * percent) / 100;
                c += v2 % 32;
                if (c > 31)
                    c = 31;
                color_table[v1][v2][percent] = c;
            }
}

void Update_Calculated_Blocks(void) {
    calculated_blocks[BLOCK_ELEVATOR_HORI].left_bound = (int)cos_table[degree % 360];
    calculated_blocks[BLOCK_ELEVATOR_HORI].right_bound = (int)cos_table[degree % 360];
    calculated_blocks[BLOCK_ELEVATOR_VERT].bottom_bound = (int)sin_table[degree % 360];
    calculated_blocks[BLOCK_ELEVATOR_VERT].top_bound = (int)sin_table[degree % 360];

    int switch1_y = 0, switch2_y = 0, switch3_x = 0;

    if (switch1 > 0) {
        switch1_y = 64;
        if (switch1 < 64)
            switch1_y = switch1;
        if (switch1 > SWITCH_START_VALUE - 64)
            switch1_y = SWITCH_START_VALUE - switch1;
    }
    if (switch1 > 0) {
        switch2_y = 64;
        if (switch1 < 64)
            switch2_y = switch1;
        if (switch1 > SWITCH_START_VALUE - 64)
            switch2_y = SWITCH_START_VALUE - switch1;
    }
    if (switch3 > 0) {
        switch3_x = 64;
        if (switch3 < 64)
            switch3_x = switch3;
        if (switch3 > SWITCH_START_VALUE - 64)
            switch3_x = SWITCH_START_VALUE - switch3;
    }

    switch1_y /= 2;
    switch2_y /= 2;
    switch3_x /= 2;

    calculated_blocks[BLOCK_SWITCH1].bottom_bound = switch1_y;
    calculated_blocks[BLOCK_SWITCH1].top_bound = switch1_y;

    calculated_blocks[BLOCK_SWITCH2_UP].bottom_bound = -switch2_y;
    calculated_blocks[BLOCK_SWITCH2_UP].top_bound = -switch2_y;

    calculated_blocks[BLOCK_SWITCH2_DOWN].bottom_bound = switch2_y;
    calculated_blocks[BLOCK_SWITCH2_DOWN].top_bound = switch2_y;

    calculated_blocks[BLOCK_SWITCH2].bottom_bound = switch2_y;
    calculated_blocks[BLOCK_SWITCH2].top_bound = switch2_y;

    calculated_blocks[BLOCK_SWITCH3_RIGHT].right_bound = switch3_x;
    calculated_blocks[BLOCK_SWITCH3_RIGHT].left_bound = switch3_x;
    calculated_blocks[BLOCK_SWITCH3_RIGHT].code = BLOCK_ELEVATOR_HORI;

    calculated_blocks[BLOCK_SWITCH3_LEFT].right_bound = -switch3_x;
    calculated_blocks[BLOCK_SWITCH3_LEFT].left_bound = -switch3_x;
    calculated_blocks[BLOCK_SWITCH3_LEFT].code = BLOCK_ELEVATOR_HORI;

    calculated_blocks[BLOCK_SWITCH3].bottom_bound = switch3_x;
    calculated_blocks[BLOCK_SWITCH3].top_bound = switch3_x;
}

int Calculate_Blocks(void) {
    Block block;
    for (int i = 0; i < 150; i++) {
        block = calculated_blocks[i];
        block.left_bound = 0;
        block.right_bound = 0;//32
        block.top_bound = 0;
        block.bottom_bound = 0;//32
        block.code = i;

        if ((i < 80 || i > 139) && i != 255) {
            block.background = false;
            block.right = BLOCK_WALL;
            block.left = BLOCK_WALL;
            block.up = BLOCK_WALL;
            block.down = BLOCK_WALL;

            // Special floors
            if (i > 139) {
                block.right = BLOCK_BACKGROUND;
                block.left = BLOCK_BACKGROUND;
                block.up = BLOCK_BACKGROUND;
                block.down = BLOCK_BACKGROUND;
            }
            // Walk-through floor
            if (i == BLOCK_RESTRICTION_DOWN) {
                block.right = BLOCK_BACKGROUND;
                block.up = BLOCK_BACKGROUND;
                block.down = BLOCK_WALL;
                block.left = BLOCK_BACKGROUND;
                block.bottom_bound -= 27;
            }
            // Hills
            if (i > 49 && i < 60) {
                block.right = BLOCK_BACKGROUND;
                block.up = BLOCK_WALL;
                block.down = BLOCK_WALL;
                block.left = BLOCK_BACKGROUND;
                block.bottom_bound += 1;
            }
            // Switches
            if (i >= BLOCK_SWITCH1 && i <= BLOCK_SWITCH3) {
                block.right = BLOCK_WALL;
                block.up = BLOCK_WALL;
                block.down = BLOCK_WALL;
                block.left = BLOCK_WALL;
            }
        } else {
            block.background = true;
            block.right = BLOCK_BACKGROUND;
            block.left = BLOCK_BACKGROUND;
            block.up = BLOCK_BACKGROUND;
            block.down = BLOCK_BACKGROUND;
        }
        if (i > 131 && i < 140)
            block.water = true;
        else
            block.water = false;
        calculated_blocks[i] = block;
    }
    Update_Calculated_Blocks();

    return 0;
}

int Create_Block_Masks(void) {
    unsigned char *buffer = NULL;
    unsigned long width;
    int x, y;
    unsigned char color;
    PisteDraw_Draw_Begin(map->block_buffer, *&buffer, (unsigned long &)width);

    for (int mask = 0; mask < MAX_BLOCK_MASKS; mask++) {
        for (x = 0; x < 32; x++) {
            y = 0;
            while (y < 31 && (color = buffer[x + (mask % 10) * 32 + (y + (mask / 10) * 32) * width]) == 255) {
                y++;
            }
            block_masks[mask].down[x] = y;
        }
        for (x = 0; x < 32; x++) {
            y = 31;
            while (y >= 0 && (color = buffer[x + (mask % 10) * 32 + (y + (mask / 10) * 32) * width]) == 255) {
                y--;
            }
            block_masks[mask].up[x] = 31 - y;
        }
/*
		for (y=0; y<32; y++) {
			x=0;
			while (x<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*width])==255) {
				x++;
			}

			block_masks[mask].right[y] = x;
			//buffer[x+(mask%10)*32 + (y+(mask/10)*32)*width] = 0;
		}
*/
    }
    PisteDraw_Draw_End(map->block_buffer);

    return 0;
}

int Remove_Color_254_From_Blocks(void) {
    unsigned char *buffer = NULL;
    unsigned long width;
    int x, y;
    if (PisteDraw_Draw_Begin(map->block_buffer, *&buffer, (unsigned long &)width) == 1)
        return 1;
    for (y = 0; y < 480; y++)
        for (x = 0; x < 320; x++)
            if (buffer[x + y * width] == 254)
                buffer[x + y * width] = 255;
    if (PisteDraw_Draw_End(map->block_buffer) == 1)
        return 1;

    return 0;
}

void Set_Bounds(void) {
    if (LIMIT_MAP_DRAWING_AREA)
        PisteDraw_SetClipper(PD_BACKBUFFER, MARGIN_HORI, MARGIN_VERT, MAP_DRAWING_WIDTH + MARGIN_HORI, MAP_DRAWING_HEIGHT + MARGIN_VERT);
    else
        PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// TODO: continue
int Blur_Image(int buffer, unsigned long width, int height) {
    unsigned char *temp_buffer = NULL;
    unsigned long temp_width;
    unsigned char color,/* color2, color3,*/ color32;
    unsigned long x, mx, my;
    int y;
    double factor;
    if (PisteDraw_Draw_Begin(buffer, *&temp_buffer, (unsigned long &)temp_width) == 1)
        return 1;
    if (width > temp_width)
        width = temp_width;
    height -= 2;
    width -= 2;
    factor = 3;//2.25;//2
    //for (y=0;y<height;y++)
    for (y = 35; y < height - 30; y++) {
        my = (y) * temp_width;
        //for(x=0;x<width;x++)
        for (x = 30; x < width - 30; x++) {
            mx = x + my;
            color = temp_buffer[mx];
            color32 = COLOR_TURQUOISE;//(color>>5)<<5;
            color %= 32;
            if (x == 30 || x == width - 31 || y == 35 || y == height - 31)
                color = int((double) color / (factor / 1.5));//1.25
            else
                color = int((double) color / factor);//1.25
            color += color32;
            temp_buffer[mx] = color;
        }
        if (factor > 1.005)
            factor = factor - 0.005;
    }
    if (PisteDraw_Draw_End(buffer) == 1)
        return 1;

    return 0;
}

int Copy_Game_State_To_Background(void) {
    unsigned char *buffer1 = NULL, *buffer2 = NULL;
    unsigned long width1, width2;
    int x, y;
    if (PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer1, (unsigned long &)width1) == 1)
        return 1;
    if (PisteDraw_Draw_Begin(background_image, *&buffer2, (unsigned long &)width2) == 1)
        return 1;
    for (y = 0; y < 478; y++)
        for (x = 0; x < 638; x++)
            buffer2[x + y * width2] = buffer1[(x + MARGIN_HORI) + (y + MARGIN_VERT) * width1];
    if (PisteDraw_Draw_End(background_image) == 1) {
        PisteDraw_Draw_End(PD_BACKBUFFER);
        return 1;
    }
    if (PisteDraw_Draw_End(PD_BACKBUFFER) == 1)
        return 1;

    return 0;
}

void Calculate_Map_Edges() {
    unsigned char tile1, tile2, tile3;
    bool edge = false;
    memset(map->edges, false, sizeof(map->edges));
    for (int x = 1; x < MAP_WIDTH - 1; x++)
        for (int y = 0; y < MAP_HEIGHT - 1; y++) {
            edge = false;
            tile1 = map->walls[x + y * MAP_WIDTH];
            if (tile1 > BLOCK_FINISH)
                map->walls[x + y * MAP_WIDTH] = 255;
            tile2 = map->walls[x + (y + 1) * MAP_WIDTH];
            if (tile1 > 79 || tile1 == BLOCK_RESTRICTION_DOWN) tile1 = 1; else tile1 = 0;
            if (tile2 > 79) tile2 = 1; else tile2 = 0;
            if (tile1 == 1 && tile2 == 1) {
                tile1 = map->walls[x + 1 + (y + 1) * MAP_WIDTH];
                tile2 = map->walls[x - 1 + (y + 1) * MAP_WIDTH];
                if (tile1 < 80 && !(tile1 < 60 && tile1 > 49)) tile1 = 1; else tile1 = 0;
                if (tile2 < 80 && !(tile2 < 60 && tile2 > 49)) tile2 = 1; else tile2 = 0;
                if (tile1 == 1) {
                    tile3 = map->walls[x + 1 + y * MAP_WIDTH];
                    if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_RESTRICTION_DOWN)
                        edge = true;
                }
                if (tile2 == 1) {
                    tile3 = map->walls[x - 1 + y * MAP_WIDTH];
                    if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_RESTRICTION_DOWN)
                        edge = true;
                }
                if (edge) {
                    map->edges[x + y * MAP_WIDTH] = true;
                    //map->backgrounds[x+y*MAP_WIDTH] = 49;
                }
            }
        }
}

int Draw_Wave_Text(char *text, int font, int x, int y) {
    int length = strlen(text);
    int space = 0;
    char letter[3] = " \0";
    int ys, xs;
    if (length > 0) {
        for (int i = 0; i < length; i++) {
            ys = (int)(sin_table[((i + degree) * 8) % 360]) / 7;
            xs = (int)(cos_table[((i + degree) * 8) % 360]) / 9;
            letter[0] = text[i];
            PisteDraw_Font_Write(font4, letter, PD_BACKBUFFER, x + space - xs + 3, y + ys + 3);
            space += PisteDraw_Font_Write(font, letter, PD_BACKBUFFER, x + space - xs, y + ys);
        }
    }

    return space;
}

int Draw_Wave_Text_Slow(char *text, int font, int x, int y) {
    int length = strlen(text);
    int space = 0;
    char letter[3] = " \0";
    int ys, xs;
    if (length > 0) {
        for (int i = 0; i < length; i++) {
            ys = (int)(sin_table[((i + degree) * 4) % 360]) / 9;
            xs = (int)(cos_table[((i + degree) * 4) % 360]) / 11;
            letter[0] = text[i];
            if (settings.transparent_menu_texts)
                space += PisteDraw_Font_Write_Transparent(font, letter, PD_BACKBUFFER, x + space - xs, y + ys, 75);
            else {
                PisteDraw_Font_Write(font4, letter, PD_BACKBUFFER, x + space - xs + 1, y + ys + 1);
                space += PisteDraw_Font_Write(font, letter, PD_BACKBUFFER, x + space - xs, y + ys);
            }
        }
    }

    return space;
}

int Draw_Transparent_Object(int source_buffer, unsigned long source_x, unsigned long source_y, unsigned long source_width, unsigned long source_height, unsigned long target_x, unsigned long target_y, int percent, unsigned char color) {
    target_x += MARGIN_HORI;
    target_y += MARGIN_VERT;

    int left = target_x, right = target_x + source_width, top = target_y, bottom = target_y + source_height;

    if (percent > 99)
        percent = 99;
    if (left < MARGIN_HORI)
        left = MARGIN_HORI;
    if (top < MARGIN_VERT)
        top = MARGIN_VERT;
    if (LIMIT_MAP_DRAWING_AREA) {
        if (right > MAP_DRAWING_WIDTH + MARGIN_HORI)
            right = MAP_DRAWING_WIDTH + MARGIN_HORI;
        if (bottom > MAP_DRAWING_HEIGHT + MARGIN_VERT)
            bottom = MAP_DRAWING_HEIGHT + MARGIN_VERT;
    } else {
        if (right > SCREEN_WIDTH)
            right = SCREEN_WIDTH;
        if (bottom > SCREEN_HEIGHT)
            bottom = SCREEN_HEIGHT;
    }

    left -= target_x;
    right -= target_x;
    top -= target_y;
    bottom -= target_y;

    if (left > right || top > bottom || percent < 1 || percent > 99)
        return 1;
    unsigned char *buffer_source = NULL, *buffer_destination = NULL, color1, *pcolor2;
    unsigned long source_buffer_width, target_buffer_width, source_cursor, target_cursor;
    //target_cursor2;
    int px, py;

    PisteDraw_Draw_Begin(source_buffer, *&buffer_source, (unsigned long &) source_buffer_width);
    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_destination, (unsigned long &) target_buffer_width);

    source_cursor = source_y * source_buffer_width + source_x;
    target_cursor = target_y * target_buffer_width + target_x;

    for (py = top; py < bottom; py++) {
        for (px = left; px < right; px++) {
            color1 = buffer_source[px + source_cursor];
            if (color1 != 255) {
                pcolor2 = &buffer_destination[px + target_cursor];
                *pcolor2 = color_table[color1][*pcolor2][percent] + color;
            }
        }
        source_cursor += source_buffer_width;
        target_cursor += target_buffer_width;
    }
    PisteDraw_Draw_End(PD_BACKBUFFER);
    PisteDraw_Draw_End(source_buffer);

    return 0;
}

// TODO: continue
int Draw_Transparent_Object2(int source_buffer, unsigned long source_x, unsigned long source_y, unsigned long source_width, unsigned long source_height, unsigned long target_x, unsigned long target_y, int percent, unsigned char color) {
    if (percent < 0)
        percent = 0;
    if (percent > 99)
        percent = 99;

    int left = target_x, right = target_x + source_width, top = target_y, bottom = target_y + source_height;

    if (left < 16)
        left = 16;
    if (right > SCREEN_WIDTH - 16)
        right = SCREEN_WIDTH - 16;
    if (top < 16)
        top = 16;
    if (bottom > SCREEN_HEIGHT - 16)
        bottom = SCREEN_HEIGHT - 16;

    left -= target_x;
    right -= target_x;
    top -= target_y;
    bottom -= target_y;

    if (left < right && top < bottom) {
        unsigned char *buffer_source = NULL, *buffer_target = NULL;
        unsigned long source_buffer_width, target_buffer_width;
        int color1, color2, source_cursor, target_cursor, px, py;
        PisteDraw_Draw_Begin(source_buffer, *&buffer_source, (unsigned long &) source_buffer_width);
        PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_target, (unsigned long &) target_buffer_width);
        for (py = top; py < bottom; py++) {
            source_cursor = (py + source_y) * source_buffer_width + source_x;

            //source_cursor =  py+source_y;
            //source_cursor *= source_buffer_width;
            //source_cursor += source_x;

            target_cursor = (py + target_y) * target_buffer_width + target_x;
            //target_cursor =  py+target_y;
            //target_cursor *= target_buffer_width;
            //target_cursor += target_x;

            for (px = left; px < right; px++) {
                color1 = buffer_source[px + source_cursor];
                if (color1 != 255) {
                    color2 = buffer_target[px + target_cursor];
                    color1 %= 32;
                    color2 %= 32;
                    //color1 = color_table_gray[color1];
                    //color2 = color_table_gray[color2];
                    //color1 = proc_table[color1][percent];
                    color1 = (color1 * percent) / 100;
                    color1 += color2;
                    if (color1 > 31)
                        color1 = 31;
                    //color1 = color_table31[color1]; // sets color to value 31 if greater than 31
                    //color1 += color;
                    buffer_target[px + target_cursor] = color1 + color;
                }
            }
        }
        PisteDraw_Draw_End(PD_BACKBUFFER);
        PisteDraw_Draw_End(source_buffer);
    } else
        return 1;

    return 0;
}

// TODO: continue
int Draw_Transparent_Object3(int source_buffer, unsigned long source_x, unsigned long source_y, unsigned long source_width, unsigned long source_height, unsigned long target_x, unsigned long target_y, int percent, unsigned char color) {
    if (percent < 0)
        percent = 0;
    if (percent > 99)
        percent = 99;

    int left = target_x, right = target_x + source_width, top = target_y, bottom = target_y + source_height;

    if (left < 16)
        left = 16;
    if (right > SCREEN_WIDTH - 16)
        right = SCREEN_WIDTH - 16;

    if (top < 16)
        top = 16;
    if (bottom > SCREEN_HEIGHT - 16)
        bottom = SCREEN_HEIGHT - 16;

    left -= target_x;
    right -= target_x;
    top -= target_y;
    bottom -= target_y;

    if (left < right && top < bottom && percent > 0) {
        unsigned char *buffer_source = NULL, *buffer_target = NULL;
        unsigned long source_buffer_width, target_buffer_width;
        unsigned char color1, color2;
        unsigned long source_cursor, target_cursor;
        int px, py;
        PisteDraw_Draw_Begin(source_buffer, *&buffer_source, (unsigned long &) source_buffer_width);
        PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_target, (unsigned long &) target_buffer_width);
        for (py = top; py < bottom; py++) {
            source_cursor = (py + source_y) * source_buffer_width + source_x;
            target_cursor = (py + target_y) * target_buffer_width + target_x;
            for (px = left; px < right; px++) {
                color1 = buffer_source[px + source_cursor];
                if (color1 != 255) {
                    color2 = buffer_target[px + target_cursor];
                    //color1 =  color_table_gray[color1];//color1 % 32
                    color1 = color1 % 32;
                    color1 = proc_table[color1][percent];//(color1*percent)/100;
                    //color1 += color_table_gray[color2];//color2 % 32
                    color1 += color2 % 32;
                    color1 = color_table31[color1] + color; // sets color to value 31 if greater than 31
                    buffer_target[px + target_cursor] = color1;
                }
            }
        }
        PisteDraw_Draw_End(PD_BACKBUFFER);
        PisteDraw_Draw_End(source_buffer);
    } else
        return 1;

    return 0;
}

// TODO: continue
int Draw_Transparent_Object4(unsigned long source_x, unsigned long source_y, unsigned long source_width, unsigned long source_height, unsigned long target_x, unsigned long target_y, int percent, unsigned char color, unsigned char *source_buffer, unsigned char *target_buffer, unsigned long source_buffer_width, unsigned long target_buffer_width) {
    target_x += MARGIN_HORI;
    target_y += MARGIN_VERT;

    int left = target_x, right = target_x + source_width, top = target_y, bottom = target_y + source_height;

    if (percent > 99)
        percent = 99;
    if (left < MARGIN_HORI)
        left = MARGIN_HORI;
    if (top < MARGIN_VERT)
        top = MARGIN_VERT;
    if (LIMIT_MAP_DRAWING_AREA) {
        if (right > MAP_DRAWING_WIDTH + MARGIN_HORI)
            right = MAP_DRAWING_WIDTH + MARGIN_HORI;
        if (bottom > MAP_DRAWING_HEIGHT + MARGIN_VERT)
            bottom = MAP_DRAWING_HEIGHT + MARGIN_VERT;
    } else {
        if (right > SCREEN_WIDTH)
            right = SCREEN_WIDTH;
        if (bottom > SCREEN_HEIGHT)
            bottom = SCREEN_HEIGHT;
    }

    left -= target_x;
    right -= target_x;
    top -= target_y;
    bottom -= target_y;

    if (left > right || top > bottom || percent < 1 || percent > 99)
        return 1;

    unsigned char color1, *pcolor2;
    unsigned long source_cursor, target_cursor;
    int px, py;
    source_cursor = source_y * source_buffer_width + source_x;
    target_cursor = target_y * target_buffer_width + target_x;
    for (py = top; py < bottom; py++) {
        for (px = left; px < right; px++) {
            color1 = source_buffer[px + source_cursor];

            if (color1 != 255) {
                pcolor2 = &target_buffer[px + target_cursor];
                *pcolor2 = color_table[color1][*pcolor2][percent] + color;
            }
        }
        source_cursor += source_buffer_width;
        target_cursor += target_buffer_width;
    }

    return 0;
}

// TODO: continue
int Draw_Transparent_Point(unsigned long target_x, unsigned long target_y, int percent, unsigned char color) {
    target_x += MARGIN_HORI;
    target_y += MARGIN_VERT;

    if (percent > 99)
        percent = 99;
    if (LIMIT_MAP_DRAWING_AREA) {
        if (target_x > (unsigned long) MAP_DRAWING_WIDTH || target_x < (unsigned long) MARGIN_HORI || target_y > (unsigned long) MAP_DRAWING_HEIGHT || target_y < (unsigned long) MARGIN_VERT)
            return 1;
    } else {
        if (target_x > (unsigned long) SCREEN_WIDTH || target_x < (unsigned long) MARGIN_HORI || target_y > (unsigned long) SCREEN_HEIGHT || target_y < (unsigned long) MARGIN_VERT)
            return 1;
    }

    unsigned char *buffer_target = NULL, color1, *color2;
    unsigned long target_buffer_width;

    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_target, (unsigned long &) target_buffer_width);
    color1 = buffer_target[target_y * target_buffer_width + target_x];
    if (color1 != 255) {
        color2 = &buffer_target[target_y * target_buffer_width + target_x];
        *color2 = color_table[color1][*color2][percent] + color;
    }
    PisteDraw_Draw_End(PD_BACKBUFFER);
    return 0;
}

// TODO: continue
int Draw_Shadow(int source_buffer, unsigned long source_x, unsigned long source_y, unsigned long source_width, unsigned long source_height, unsigned long target_x, unsigned long target_y) {
    int left = target_x, right = target_x + source_width, top = target_y, bottom = target_y + source_height;
    if (left < 16)
        left = 16;
    if (right > SCREEN_WIDTH - 16)
        right = SCREEN_WIDTH - 16;
    if (top < 16)
        top = 16;
    if (bottom > SCREEN_HEIGHT - 16)
        bottom = SCREEN_HEIGHT - 16;

    left -= target_x;
    right -= target_x;
    top -= target_y;
    bottom -= target_y;

    if (left < right && top < bottom) {
        unsigned char *buffer_source = NULL, *buffer_target = NULL;
        unsigned long source_buffer_width, target_buffer_width;
        int color, source_cursor, target_cursor;
        PisteDraw_Draw_Begin(source_buffer, *&buffer_source, (unsigned long &) source_buffer_width);
        PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_target, (unsigned long &) target_buffer_width);
        for (int py = top; py < bottom; py++) {
            source_cursor = (py + source_y) * source_buffer_width + source_x;
            target_cursor = (py + target_y) * target_buffer_width + target_x;
            for (int px = left; px < right; px++) {
                color = buffer_source[px + source_cursor];
                if (color != 255) {
                    color = buffer_target[px + target_cursor];
                    color %= 32;
                    color /= 2;
                    buffer_target[px + target_cursor] = color;
                }
            }
        }
        PisteDraw_Draw_End(PD_BACKBUFFER);
        PisteDraw_Draw_End(source_buffer);
    } else
        return 1;

    return 0;
}

void New_Notification(char *text) {
    if (strcmp(text, notification) != 0 || notification_timer == 0) {
        strcpy(notification, text);
        notification_timer = MAX_NOTIFICATION_DISPLAY_TIME;
    }
}

void Initialize_Fade_Texts() {
    for (int i = 0; i < MAX_FADE_TEXTS; i++) {
        fade_texts[i].timer = 0;
    }
}

void New_Fade_Text(int font, char *text, unsigned long x, unsigned long y, unsigned long timer) {
    fade_texts[fade_text_index].font = font;
    strcpy(fade_texts[fade_text_index].text, text);
    fade_texts[fade_text_index].x = x;
    fade_texts[fade_text_index].y = y;
    fade_texts[fade_text_index].timer = timer;
    fade_text_index++;
    if (fade_text_index >= MAX_FADE_TEXTS)
        fade_text_index = 0;
}

int Draw_Fade_Texts() {
    int percent;
    for (int i = 0; i < MAX_FADE_TEXTS; i++) {
        if (fade_texts[i].timer > 0) {
            if (fade_texts[i].timer > 50)
                percent = 100;
            else
                percent = fade_texts[i].timer * 2;
            if (settings.transparent_objects && percent < 100) {
                if (PisteDraw_Font_Write_Transparent(fade_texts[i].font, fade_texts[i].text, PD_BACKBUFFER, fade_texts[i].x - camera_x, fade_texts[i].y - camera_y, percent) == PD_ERROR)
                    return 1;
            } else
                PisteDraw_Font_Write(fade_texts[i].font, fade_texts[i].text, PD_BACKBUFFER, fade_texts[i].x - camera_x, fade_texts[i].y - camera_y);
        }
    }
    return 0;
}

void Fade_Texts() {
    for (int i = 0; i < MAX_FADE_TEXTS; i++) {
        if (fade_texts[i].timer > 0) {
            fade_texts[i].timer--;
            if (fade_texts[i].timer % 2 == 0)
                fade_texts[i].y--;
            if (fade_texts[i].x < camera_x || fade_texts[i].x > camera_x + SCREEN_WIDTH || fade_texts[i].y < camera_y || fade_texts[i].y > camera_y + SCREEN_HEIGHT)
                fade_texts[i].timer = 0;
        }
    }
}

int Draw_Point(unsigned long x, unsigned long y, int percent, unsigned char color) {
    //if (percent > 99 || !settings.transparent_objects)
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, x - camera_x, y - camera_y, x - camera_x + 1, y - camera_y + 1, color + 25);
    //else
    //	Draw_Transparent_Point(x-camera_x, y-camera_y, percent * 8, color);

    return 0;
}

int Draw_Star(unsigned long x, unsigned long y, int percent, unsigned char color) {
    if (percent > 99 || !settings.transparent_objects)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 1, 1, 11, 11);
    else
        Draw_Transparent_Object(game_image_sysmem, 2, 2, 10, 10, x - camera_x, y - camera_y, percent, color);

    return 0;
}

int Draw_ImpactEffect(unsigned long x, unsigned long y) {
    int frame_x = ((degree % 12) / 3) * 58;
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x - 28 + 8, y - camera_y - 27 + 8, 1 + frame_x, 83, 1 + 57 + frame_x, 83 + 55);//140<-83
    return 0;
}

int Draw_Light(unsigned long x, unsigned long y, int percentage, unsigned char color) {
    if (settings.transparent_objects)
        Draw_Transparent_Object(game_image_sysmem, 1, 14, 13, 13, x - camera_x, y - camera_y, percentage, color);
    else {
        int vx = (color / 32) * 14;
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 1 + vx, 14 + 14, 14 + vx, 27 + 14);
    }

    return 0;
}

int Draw_Spark(unsigned long x, unsigned long y, int percentage, unsigned char color) {
    if (settings.transparent_objects)
        Draw_Transparent_Object(game_image_sysmem, 99, 14, 7, 7, x - camera_x, y - camera_y, percentage, color);
    else {
        int vx = (color / 32) * 8;
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 99 + vx, 14 + 14, 106 + vx, 21 + 14);
    }

    return 0;
}

void Draw_Feather(int x, int y, int &anim) {
    int x_plus = (anim / 7) * 21;
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 14 + x_plus, 1, 34 + x_plus, 12);
    anim++;
    if (anim > 63)
        anim = 0;
}

void Draw_SmokeCloud(int x, int y, int &anim) {
    int frame = (anim / 7);
    int x_plus = (frame % 17) * 36;
    int y_plus = 0;
    if (anim < 7 * 30) {
        if (frame > 16)
            y_plus = 32;
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 1 + x_plus, 338 + y_plus, 34 + x_plus, 366 + y_plus);
        anim++;
    }
    //if (animation > 63)
    //	animation = 0;
}

void Draw_MultiCloud(unsigned long x, unsigned long y, int percentage, unsigned char color) {
    if (percentage > 99 || !settings.transparent_objects)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x - camera_x, y - camera_y, 226, 2, 224, 49);
    else
        Draw_Transparent_Object(game_image_sysmem, 226, 2, 18, 19, x - camera_x, y - camera_y, percentage, color);
}

int Clear_Particles() {
    int i = 0;
    particle_index = 0;
    while (i < MAX_PARTICLES) {
        particles[i].a = 0;
        particles[i].duration = 0;
        particles[i].animation = 0;
        particles[i].b = 0;
        particles[i].type = PARTICLE_NOTHING;
        particles[i].x = 0;
        particles[i].y = 0;
        particles[i].weight = 0;
        particles[i].color = 0;
        i++;
    }

    return 0;
}

void New_Particle(int type, double x, double y, double a, double b, int duration, double weight, int color) {
    if (x < camera_x + SCREEN_WIDTH + 10 && x > camera_x - 10 && y < camera_y + SCREEN_HEIGHT + 10 && y > camera_y - 10) {
        if (particle_index >= MAX_PARTICLES)
            particle_index = 0;
        particles[particle_index].a = a;
        particles[particle_index].duration = duration;
        particles[particle_index].animation = rand() % 63;
        particles[particle_index].b = b;
        particles[particle_index].type = type;
        particles[particle_index].x = x;
        particles[particle_index].y = y;
        particles[particle_index].weight = weight;
        particles[particle_index].color = color;
        particle_index++;
    }
}

// TODO: continue
int Draw_Particles() {
    int percent;
    int draw_temp_width, draw_height;

    if (LIMIT_MAP_DRAWING_AREA) {
        draw_temp_width = MAP_DRAWING_WIDTH;
        draw_height = MAP_DRAWING_HEIGHT;
    } else {
        draw_temp_width = SCREEN_WIDTH;
        draw_height = SCREEN_HEIGHT;
    }
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].duration > 0) {
            if (particles[i].x < camera_x + draw_temp_width && particles[i].x > camera_x && particles[i].y < camera_y + draw_height && particles[i].y > camera_y) {
                if (particles[i].duration < 100)
                    percent = particles[i].duration;
                else
                    percent = 100;
                switch (particles[i].type) {
                    case PARTICLE_STAR:
                        Draw_Star((int) particles[i].x, (int) particles[i].y, percent, particles[i].color);
                        break;
                    case PARTICLE_FEATHER:
                        Draw_Feather((int) particles[i].x, (int) particles[i].y, particles[i].animation);
                        break;
                    case PARTICLE_DUST_CLOUD:
                        Draw_MultiCloud((int) particles[i].x, (int) particles[i].y, percent, particles[i].color);
                        break;
                    case PARTICLE_SMOKE_CLOUD:
                        Draw_SmokeCloud((int) particles[i].x, (int) particles[i].y, particles[i].animation);
                        break;
                    case PARTICLE_LIGHT:
                        Draw_Light((int) particles[i].x, (int) particles[i].y, percent, particles[i].color);
                        break;
                    case PARTICLE_SPARK:
                        Draw_Spark((int) particles[i].x, (int) particles[i].y, percent, particles[i].color);
                        break;
                    case PARTICLE_DOT:
                        Draw_Point((int) particles[i].x, (int) particles[i].y, percent, particles[i].color);
                        break;
                    default:
                        break;
                }
            }
        } else
            particles[i].duration = 0;
    }
    return 0;
}

int Process_Particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].duration > 0) {
            particles[i].x += particles[i].a;
            particles[i].y += particles[i].b;
            if (particles[i].weight > 0)
                particles[i].b += particles[i].weight;
            particles[i].duration--;
        }
    }
    return 0;
}

int Clear_BackgroundParticles() {
    int i = 0;
    background_particle_index = 0;
    //map->climate = WEATHER_RAIN;
    while (i < MAX_BACKGROUND_PARTICLES) {
        background_particles[i].a = rand() % 10 - rand() % 10;
        background_particles[i].duration = 1;
        background_particles[i].animation = rand() % 10;
        background_particles[i].b = rand() % 9 + 1;
        background_particles[i].type = PARTICLE_NOTHING;
        background_particles[i].x = rand() % MAP_DRAWING_WIDTH;
        background_particles[i].y = rand() % MAP_DRAWING_HEIGHT;
        background_particles[i].weight = rand() % 10;
        background_particles[i].color = 0;
        i++;
    }
    if (map->climate == WEATHER_RAIN || map->climate == WEATHER_RAIN_FOREST)
        for (i = 0; i < MAX_BACKGROUND_PARTICLES; i++) {
            background_particles[i].type = BACKGROUND_PARTICLE_WATER_DROP;
        }
    if (map->climate == WEATHER_FOREST || map->climate == WEATHER_RAIN_FOREST)
        for (i = 0; i < MAX_BACKGROUND_PARTICLES / 8; i++) {
            background_particles[i].type = BACKGROUND_PARTICLE_LEAF1 + rand() % 4;
        }
    if (map->climate == WEATHER_SNOW) {
        for (i = 0; i < MAX_BACKGROUND_PARTICLES / 2; i++) {
            background_particles[i].type = BACKGROUND_PARTICLE_SNOWFLAKE4;
        }
        for (i = 0; i < MAX_BACKGROUND_PARTICLES / 3; i++) {
            background_particles[i].type = BACKGROUND_PARTICLE_SNOWFLAKE1 + rand() % 2 + 1;//3
        }
    }
    return 0;
}

void Process_BackgroundParticle_WaterDrop(Particle &p) {
//	p.x += p.movement_x / 3;
    p.y += p.b / 3.0 + 2.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, kx, ky, kx + 1, ky + 4, 40 + (int)p.b);
}

void Process_BackgroundParticle_Leaf1(Particle &p) {
    p.x += p.a / 9.0;
    p.y += p.b / 9.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, kx, ky, kx + 2, ky + 2, 96 + 6 + (int)p.b + (int)(p.x + p.y) % 10);
}

void Process_BackgroundParticle_Leaf2(Particle &p) {
    p.x += p.a / 14.0;
    p.y += p.b / 9.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y), frame = (int(p.y / 10) % 4) * 23;
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 1 + frame, 141, 21 + frame, 152);
}

void Process_BackgroundParticle_Leaf3(Particle &p) {
    p.x += p.a / 12.0;
    p.y += p.b / 9.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y), frame = (int(p.y / 5) % 4) * 20;
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 93 + frame, 141, 109 + frame, 150);
}

void Process_BackgroundParticle_Leaf4(Particle &p) {
    p.x += p.a / 11.0;
    p.y += p.b / 9.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y), frame = (int(p.y / 5) % 2) * 14;
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 173 + frame, 141, 183 + frame, 150);
}

void Process_BackgroundParticle_Snowflake1(Particle &p) {
    //p.x += p.movement_x / 11.0;
    p.x += sin_table[int(p.y) % 360] / 50.0;
    p.y += p.b / 7.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 1, 155, 8, 162);
}

void Process_BackgroundParticle_Snowflake2(Particle &p) {
    //p.x += p.movement_x / 11.0;
    p.x += sin_table[int(p.y) % 360] / 100.0;
    p.y += p.b / 8.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 11, 155, 16, 160);
}

void Process_BackgroundParticle_Snowflake3(Particle &p) {
    //p.x += p.movement_x / 11.0;
    p.y += p.b / 8.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, kx, ky, 19, 155, 22, 158);
}

void Process_BackgroundParticle_Snowflake4(Particle &p) {
    //p.x += p.movement_x / 9.0;
    p.y += p.b / 9.0;
    int kx = (int)(p.x - camera_x), ky = (int)(p.y - camera_y);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, kx, ky, kx + 2, ky + 2, 20 + (int) p.b);
}

int Process_BackgroundParticles() {
    Set_Bounds();
    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,MAP_DRAWING_WIDTH,MAP_DRAWING_HEIGHT);
    for (int i = 0; i < MAX_BACKGROUND_PARTICLES; i++) {
        if (background_particles[i].type != PARTICLE_NOTHING) {
            switch (background_particles[i].type) {
                case BACKGROUND_PARTICLE_WATER_DROP:
                    Process_BackgroundParticle_WaterDrop(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_LEAF1:
                    Process_BackgroundParticle_Leaf1(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_LEAF2:
                    Process_BackgroundParticle_Leaf2(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_LEAF3:
                    Process_BackgroundParticle_Leaf3(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_LEAF4:
                    Process_BackgroundParticle_Leaf4(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_SNOWFLAKE1:
                    Process_BackgroundParticle_Snowflake1(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_SNOWFLAKE2:
                    Process_BackgroundParticle_Snowflake2(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_SNOWFLAKE3:
                    Process_BackgroundParticle_Snowflake3(background_particles[i]);
                    break;
                case BACKGROUND_PARTICLE_SNOWFLAKE4:
                    Process_BackgroundParticle_Snowflake4(background_particles[i]);
                    break;
                default:
                    background_particles[i].type = PARTICLE_NOTHING;
            }
            if (background_particles[i].x > camera_x + MAP_DRAWING_WIDTH)
                background_particles[i].x = camera_x + int(background_particles[i].x - camera_x + MAP_DRAWING_WIDTH) % MAP_DRAWING_WIDTH;
            if (background_particles[i].x < camera_x)
                background_particles[i].x = camera_x + MAP_DRAWING_WIDTH - int(camera_x - background_particles[i].x) % MAP_DRAWING_WIDTH;
            if (background_particles[i].y > camera_y + MAP_DRAWING_HEIGHT)
                background_particles[i].y = camera_y + int(background_particles[i].y - camera_y + MAP_DRAWING_HEIGHT) % MAP_DRAWING_HEIGHT;
            if (background_particles[i].y < camera_y)
                background_particles[i].y = camera_y + MAP_DRAWING_HEIGHT - int(camera_y - background_particles[i].y) % MAP_DRAWING_HEIGHT;
        }
    }

    return 0;
}

void Create_FeatherEffect(unsigned long x, unsigned long y) {
    for (int i = 0; i < 9; i++)//6
        New_Particle(PARTICLE_FEATHER, x + rand() % 17 - rand() % 17, y + rand() % 20 - rand() % 10, (rand() % 16 - rand() % 16) / 10.0, (45 + rand() % 45) / 100.0, 300 + rand() % 40, 0, 0);
}

void Create_SplashEffect(unsigned long x, unsigned long y, unsigned char color) {
/*
	for (int i=0;i<12;i++)
		New_Particle(	PARTICLE_LIGHT,x+rand()%17-13,y+rand()%17-13, (rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3, rand()%50+60,0.025,color);
*/
    for (int i = 0; i < 7; i++)
        New_Particle(PARTICLE_SPARK, x + rand() % 17 - 13, y + rand() % 17 - 13, (rand() % 5 - rand() % 5) / 4.0, (rand() % 4 - rand() % 7) / 3.0, rand() % 50 + 40, 0.025, color);//0.015
    for (int i = 0; i < 20; i++)
        New_Particle(PARTICLE_DOT, x + rand() % 17 - 13, y + rand() % 17 - 13, (rand() % 5 - rand() % 5) / 4.0, (rand() % 2 - rand() % 7) / 3.0, rand() % 50 + 40, 0.025, color);//0.015
    Play_Sound(splash_sound, 100, x, y, SOUNDS_SAMPLERATE, true);
}

void Create_ExplosionEffect(unsigned long x, unsigned long y, unsigned char color) {
    for (int i = 0; i < 3; i++)
        New_Particle(PARTICLE_SMOKE_CLOUD, x + rand() % 17 - 32, y + rand() % 17, 0, double((rand() % 4) + 3) / -10.0, 450, 0.0, color);
    for (int i = 0; i < 9; i++)//12
        New_Particle(PARTICLE_LIGHT, x + rand() % 17 - 13, y + rand() % 17 - 13, (rand() % 7 - rand() % 7) / 5.0, (rand() % 7 - rand() % 7) / 3.0, rand() % 40 + 60, 0.025, color);
    for (int i = 0; i < 8; i++)//8//10
        New_Particle(PARTICLE_SPARK, x + rand() % 17 - 13, y + rand() % 17 - 13,
                     (rand() % 3 - rand() % 3),//(rand()%7-rand()%7)/5,
                     (rand() % 7 - rand() % 7) / 3, rand() % 20 + 60, 0.015, color);//50+60
    for (int i = 0; i < 20; i++)//12
        New_Particle(PARTICLE_DOT, x + rand() % 17 - 13, y + rand() % 17 - 13, (rand() % 7 - rand() % 7) / 5.0, (rand() % 7 - rand() % 7) / 3.0, rand() % 40 + 60, 0.025, color);
}

void Create_SmokeEffect(unsigned long x, unsigned long y, unsigned char color) {
    for (int i = 0; i < 3; i++)
        New_Particle(PARTICLE_SMOKE_CLOUD, x + rand() % 17 - 32, y + rand() % 17, 0, double((rand() % 3) + 3) / -10.0/*-0.3*/, 450, 0.0, color);
    for (int i = 0; i < 6; i++)
        New_Particle(PARTICLE_DUST_CLOUD, x + rand() % 30 - rand() % 30 - 10, y + rand() % 30 - rand() % 30 + 10, 0, -0.3, rand() % 50 + 60, 0, color);
}

void Create_SmokeCloudsEffect(unsigned long x, unsigned long y) {
    for (int i = 0; i < 5; i++)
        New_Particle(PARTICLE_SMOKE_CLOUD, x + rand() % 17 - 32, y + rand() % 17, 0, double((rand() % 3) + 3) / -10.0/*-0.3*/, 450, 0.0, 0);
}

void Create_StarEffect(unsigned long x, unsigned long y, unsigned char color) {
    for (int i = 0; i < 5; i++)
        New_Particle(PARTICLE_STAR, x - 5, y - 5, (rand() % 3 - rand() % 3) / 1.5, rand() % 3 * -1, 100, (rand() % 5 + 5) / 100.0/* 0.05*/, color);//300
    for (int i = 0; i < 3; i++)//12
        New_Particle(PARTICLE_DOT, x - 5, y - 5, (rand() % 3 - rand() % 3) / 1.5, rand() % 3 * -1, 100, (rand() % 5 + 5) / 100.0, color);
}

void Create_Effects(unsigned char effect, unsigned long x, unsigned long y) {
    switch (effect) {
        case DESTRUCTION_FEATHERS:
            Create_FeatherEffect(x, y);
            break;
        case DESTRUCTION_STARS_GRAY:
            Create_StarEffect(x, y, 0);
            break;
        case DESTRUCTION_STARS_BLUE:
            Create_StarEffect(x, y, 32);
            break;
        case DESTRUCTION_STARS_RED:
            Create_StarEffect(x, y, 64);
            break;
        case DESTRUCTION_STARS_GREEN:
            Create_StarEffect(x, y, 96);
            break;
        case DESTRUCTION_STARS_ORANGE:
            Create_StarEffect(x, y, 128);
            break;
        case DESTRUCTION_STARS_PURPLE:
            Create_StarEffect(x, y, 160);
            break;
        case DESTRUCTION_STARS_TURQUOISE:
            Create_StarEffect(x, y, 192);
            break;
        case DESTRUCTION_EXPLOSION_GRAY:
            Create_ExplosionEffect(x, y, 0);
            break;
        case DESTRUCTION_EXPLOSION_BLUE:
            Create_ExplosionEffect(x, y, 32);
            break;
        case DESTRUCTION_EXPLOSION_RED:
            Create_ExplosionEffect(x, y, 64);
            break;
        case DESTRUCTION_EXPLOSION_GREEN:
            Create_ExplosionEffect(x, y, 96);
            break;
        case DESTRUCTION_EXPLOSION_ORANGE:
            Create_ExplosionEffect(x, y, 128);
            break;
        case DESTRUCTION_EXPLOSION_PURPLE:
            Create_ExplosionEffect(x, y, 160);
            break;
        case DESTRUCTION_EXPLOSION_TURQUOISE:
            Create_ExplosionEffect(x, y, 192);
            break;
        case DESTRUCTION_SMOKE_GRAY:
            Create_SmokeEffect(x, y, 0);
            break;
        case DESTRUCTION_SMOKE_BLUE:
            Create_SmokeEffect(x, y, 32);
            break;
        case DESTRUCTION_SMOKE_RED:
            Create_SmokeEffect(x, y, 64);
            break;
        case DESTRUCTION_SMOKE_GREEN:
            Create_SmokeEffect(x, y, 96);
            break;
        case DESTRUCTION_SMOKE_ORANGE:
            Create_SmokeEffect(x, y, 128);
            break;
        case DESTRUCTION_SMOKE_PURPLE:
            Create_SmokeEffect(x, y, 160);
            break;
        case DESTRUCTION_SMOKE_TURQUOISE:
            Create_SmokeEffect(x, y, 192);
            break;
        case DESTRUCTION_SMOKE_CLOUDS:
            Create_SmokeCloudsEffect(x, y);
            break;
        default:
            break;
    }
}

// TODO: continue
int Map_Change_SkullBlocks() {
    unsigned char wall, background;
    unsigned long x, y;
    for (x = 0; x < MAP_WIDTH; x++)
        for (y = 0; y < MAP_HEIGHT; y++) {
            wall = map->walls[x + y * MAP_WIDTH];
            background = map->backgrounds[x + y * MAP_WIDTH];
            if (wall == BLOCK_SKULL_WALL) {
                map->walls[x + y * MAP_WIDTH] = 255;
                if (background != BLOCK_SKULL_WALL)
                    Create_SmokeCloudsEffect(x * 32 + 6, y * 32 + 6);
            }
            if (background == BLOCK_SKULL_BACKGROUND && wall == 255) {
                map->walls[x + y * MAP_WIDTH] = BLOCK_SKULL_WALL;
            }
        }
    earthquake = 90;//60
    //New_Notification(texts->Get_Text(txt_game_locksopen));
    Calculate_Map_Edges();

    return 0;
}

int Unlock_Map() {
    unsigned char block;
    unsigned long x, y;
    num_keys = 0;
    for (x = 0; x < MAP_WIDTH; x++)
        for (y = 0; y < MAP_HEIGHT; y++) {
            block = map->walls[x + y * MAP_WIDTH];
            if (block == BLOCK_LOCK) {
                map->walls[x + y * MAP_WIDTH] = 255;
                Create_SmokeCloudsEffect(x * 32 + 6, y * 32 + 6);
            }
        }
    earthquake = 90;//60
    New_Notification(texts->Get_Text(txt_game_locksopen));
    Calculate_Map_Edges();
    return 0;
}

int Count_Keys() {
    unsigned char sprite;
    unsigned long x;
    num_keys = 0;
    for (x = 0; x < MAP_SIZE; x++) {
        sprite = map->sprites[x];
        if (sprite != 255)
            if (prototypes[sprite].key && prototypes[sprite].destruction != DESTRUCTION_INDESTRUCTIBLE)
                num_keys++;
    }

    return 0;
}

// TODO: continue
int Find_Start() {
    double start_x = 320, start_y = 196;
    int number_of_starts = 0, start = 0, x, y;

    for (x = 0; x < MAP_SIZE; x++)
        if (map->walls[x] == BLOCK_START)
            number_of_starts++;

    if (number_of_starts > 0) {
        start = rand() % number_of_starts + 1;
        number_of_starts = 1;

        for (x = 0; x < MAP_WIDTH; x++)
            for (y = 0; y < MAP_HEIGHT; y++)
                if (map->walls[x + y * MAP_WIDTH] == BLOCK_START) {
                    if (number_of_starts == start) {
                        start_x = x * 32;
                        start_y = y * 32;
                    }

                    number_of_starts++;
                }
    }

    sprites[player_index].x = start_x + sprites[player_index].type->width / 2;
    sprites[player_index].y = start_y - sprites[player_index].type->height / 2;

    camera_x = (int) sprites[player_index].x;
    camera_y = (int) sprites[player_index].y;
    dcamera_x = camera_x;
    dcamera_y = camera_y;

    return 0;
}

void Clear_Prototype() {
    for (int i = 0; i < MAX_PROTOTYPES; i++) {
        for (int j = 0; j < MAX_SOUNDS; j++)
            if (prototypes[i].sounds[j] > -1)
                PisteSound_SFX_Destroy(prototypes[i].sounds[j]);
        prototypes[i].New();
        strcpy(map->protos[i], "");
    }
    next_free_proto = 0;
}

// TODO: continue
int Load_Prototype_Sound(char *path, char *file) {
    if (strcmp(file, "") != 0) {
        char audio_file[255];
        strcpy(audio_file, path);
        strcat(audio_file, file);
        int i = PisteSound_SFX_New(audio_file);
        if (i == PS_ERROR) {
            PK2_error = true;
            return -1;
        } else
            return i;
    }

    return -1;
}

int Load_Old_Prototype(char *path, char *file) {
    char sound_path[255];
    char test_path[255];
    strcpy(sound_path, path);

    PisteLog_Write("- Loading sprite: ");
    PisteLog_Write(path);
    PisteLog_Write(file);
    PisteLog_Write("\n");

    if (next_free_proto < MAX_PROTOTYPES) {
        if (prototypes[next_free_proto].Load(path, file) == 1) {
            PisteLog_Write("  - Loading failed\n");
            return 1;
        }
        prototypes[next_free_proto].index = next_free_proto;
    } else {
        PisteLog_Write("  - Error: Maximum limit of sprite types exceeded! \n");
        return 2;
    }

    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (strcmp(prototypes[next_free_proto].sound_files[i], "") != 0) {
            /*PisteLog_Write("  - Loading sound: ");
			PisteLog_Write(sound_path);
			PisteLog_Write(prototypes[next_free_proto].sound_files[i]);
			PisteLog_Write("\n");*/

            strcpy(test_path, sound_path);
            strcat(test_path, "\\");
            strcat(test_path, prototypes[next_free_proto].sound_files[i]);

            if (Is_File(test_path))
                prototypes[next_free_proto].sounds[i] = Load_Prototype_Sound(sound_path, prototypes[next_free_proto].sound_files[i]);
            else {
                //if (prototypes[next_free_proto].sounds[i] == -1) {
                //strcpy(sound_path, "sprites\\");
                _getcwd(sound_path, _MAX_PATH);
                strcat(sound_path, "\\sprites\\");

                strcpy(test_path, sound_path);
                strcat(test_path, "\\");
                strcat(test_path, prototypes[next_free_proto].sound_files[i]);

                if (Is_File(test_path))
                    prototypes[next_free_proto].sounds[i] = Load_Prototype_Sound(sound_path, prototypes[next_free_proto].sound_files[i]);
            }
        }
        //if (prototypes[next_free_proto].sounds[i] == -1)
        //	return 3;
    }
    PisteLog_Write("  - Loading completed. \n");
    next_free_proto++;

    return 0;
}

void Set_Prototype_Change_Sprite(int i) {
    int j = 0;
    bool found = false;
    if (strcmp(prototypes[i].transformation_sprite, "") != 0) {
        while (j < MAX_PROTOTYPES && !found) {
            if (j != i) {
                if (strcmp(prototypes[i].transformation_sprite, prototypes[j].file) == 0) {
                    prototypes[i].transformation = j;
                    found = true;
                }
            }
            j++;
        }
        if (!found) {
            char path[_MAX_PATH];
            strcpy(path, "sprites\\");
            //Add_Episode_Directory(path);
            if (Load_Old_Prototype(path, prototypes[i].transformation_sprite) == 0)
                prototypes[i].transformation = next_free_proto - 1; // each charge_time increases next_free_proto:movement_x
        }
    }
}

void Set_Prototype_Bonus_Sprite(int i) {
    int j = 0;
    bool found = false;
    if (strcmp(prototypes[i].bonus_sprite, "") != 0) {
        while (j < MAX_PROTOTYPES && !found) {
            if (j != i) {
                if (strcmp(prototypes[i].bonus_sprite, prototypes[j].file) == 0) {
                    prototypes[i].bonus = j;
                    found = true;
                }
            }
            j++;
        }
        if (!found) {
            char path[_MAX_PATH];
            strcpy(path, "sprites\\");
            //Add_Episode_Directory(path);
            if (Load_Old_Prototype(path, prototypes[i].bonus_sprite) == 0)
                prototypes[i].bonus = next_free_proto - 1;
        }
    }
}

void Set_Prototype_Projectile1_Sprite(int i) {
    int j = 0;
    bool found = false;
    if (strcmp(prototypes[i].projectile1_sprite, "") != 0) {
        while (j < MAX_PROTOTYPES && !found) {
            if (j != i) {
                if (strcmp(prototypes[i].projectile1_sprite, prototypes[j].file) == 0) {
                    prototypes[i].projectile1 = j;
                    found = true;
                }
            }
            j++;
        }
        if (!found) {
            char path[_MAX_PATH];
            strcpy(path, "sprites\\");
            //Add_Episode_Directory(path);
            if (Load_Old_Prototype(path, prototypes[i].projectile1_sprite) == 0)
                prototypes[i].projectile1 = next_free_proto - 1;
        }
    }
}

void Set_Prototype_Projectile2_Sprite(int i) {
    int j = 0;
    bool found = false;
    if (strcmp(prototypes[i].projectile2_sprite, "") != 0) {
        while (j < MAX_PROTOTYPES && !found) {
            if (j != i) {
                if (strcmp(prototypes[i].projectile2_sprite, prototypes[j].file) == 0) {
                    prototypes[i].projectile2 = j;
                    found = true;
                }
            }
            j++;
        }
        if (!found) {
            char path[_MAX_PATH];
            strcpy(path, "sprites\\");
            //Add_Episode_Directory(path);
            if (Load_Old_Prototype(path, prototypes[i].projectile2_sprite) == 0)
                prototypes[i].projectile2 = next_free_proto - 1;
        }
    }
}

int Load_All_Prototypes() {
    char path[_MAX_PATH];
    int final_proto;

    for (int i = 0; i < MAX_PROTOTYPES; i++) {
        if (strcmp(map->protos[i], "") != 0) {
            final_proto = i;
            strcpy(path, "");
            Add_Episode_Directory(path);
            //Add_Episode_Directory(path);

            if (Load_Old_Prototype(path, map->protos[i]) == 1) {
                strcpy(path, "");
                strcpy(path, "sprites\\");
                //Add_Episode_Directory(path);
                if (Load_Old_Prototype(path, map->protos[i]) == 1)
                    next_free_proto++;
                //return -1;
            }
        } else
            next_free_proto++;
    }

    next_free_proto = final_proto + 1;

    for (int i = 0; i < MAX_PROTOTYPES; i++) {
        Set_Prototype_Change_Sprite(i);
        Set_Prototype_Bonus_Sprite(i);
        Set_Prototype_Projectile1_Sprite(i);
        Set_Prototype_Projectile2_Sprite(i);
    }

    return 0;
}

void Set_InitialDirections(void) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (player_index >= 0 && player_index < MAX_SPRITES && !sprites[i].is_hidden) {
            sprites[i].movement_x = 0;
            if (sprites[i].type->Has_AI(AI_RANDOM_START_DIRECTION_HORIZONTAL)) {
                while (sprites[i].movement_x == 0) {
                    sprites[i].movement_x = ((rand() % 2 - rand() % 2) * sprites[i].type->max_speed) / 3.5;//2;
                }
            }
            if (sprites[i].type->Has_AI(AI_RANDOM_START_DIRECTION_VERT)) {
                while (sprites[i].movement_y == 0) {
                    sprites[i].movement_y = ((rand() % 2 - rand() % 2) * sprites[i].type->max_speed) / 3.5;//2;
                }
            }
            if (sprites[i].type->Has_AI(AI_START_DIRECTION_TOWARDS_PLAYER)) {
                if (sprites[i].x < sprites[player_index].x)
                    sprites[i].movement_x = sprites[i].type->max_speed / 3.5;
                if (sprites[i].x > sprites[player_index].x)
                    sprites[i].movement_x = (sprites[i].type->max_speed * -1) / 3.5;
            }
            if (sprites[i].type->Has_AI(AI_START_DIRECTION_TOWARDS_PLAYER_VERT)) {
                if (sprites[i].y < sprites[player_index].y)
                    sprites[i].movement_y = sprites[i].type->max_speed / -3.5;
                if (sprites[i].y > sprites[player_index].y)
                    sprites[i].movement_y = sprites[i].type->max_speed / 3.5;
            }
/*
			for (int ai=0;ai<SPRITE_MAX_AI;ai++) {
				if (sprites[i].type->AI[ai] == AI_RANDOM_START_DIRECTION_HORIZONTAL) {
					while (sprites[i].movement_x == 0)
						sprites[i].movement_x = ((rand()%2 - rand()%2) * sprites[i].type->max_speed) / 2;
				}
				if (sprites[i].type->AI[ai] == AI_START_DIRECTION_TOWARDS_PLAYER) {
					if (sprites[i].x < sprites[player_index].x)
						sprites[i].movement_x = sprites[i].type->max_speed / 3.5;
					if (sprites[i].x > sprites[player_index].x)
						sprites[i].movement_x = (sprites[i].type->max_speed * -1) / 3.5;
				}
			}
*/
        }
    }
}

int Load_Map(char *name) {
    Initialize_Items();
    Clear_Prototype();
    char path[_MAX_PATH];
    strcpy(path, "");
    Add_Episode_Directory(path);

    PisteLog_Write("- Trying to load movement_x map: ");
    PisteLog_Write(path);
    PisteLog_Write(name);
    PisteLog_Write("\n");

    if (map->Load(path, name) == 1) {
        PisteLog_Write("  - Loading failed. \n");
        return 1;
    }
    New_Level();

    if (strcmp(map->version, "1.2") == 0 || strcmp(map->version, "1.3") == 0) {
        if (Load_All_Prototypes() == 1)
            return 1;
    }
    Create_Block_Masks();

    if (Remove_Color_254_From_Blocks() == 1)
        return 1;

    Set_Map_Sprites();
    Find_Start();
    Set_InitialDirections();
    Count_Keys();
    Clear_Particles();
    Clear_BackgroundParticles();
    Calculate_Map_Edges();

    if (strcmp(map->music, "") != 0) {
        char song[_MAX_PATH] = "";
        Add_Episode_Directory(song);
        strcat(song, map->music);

        if (Midas_Load_Music(song) != 0) {
            strcpy(song, "music\\");
            strcat(song, map->music);

            if (Midas_Load_Music(song) != 0)
                if (Midas_Load_Music("music\\song01.xm") != 0)
                    PK2_error = true;
        }
    }
    PisteLog_Write("  - Loading map completed. \n");

    return 0;
}

void Disable_Block_Background(Block &block) {
    block.background = false;
    block.right = BLOCK_WALL;
    block.left = BLOCK_WALL;
    block.up = BLOCK_WALL;
    block.down = BLOCK_WALL;

    // Special floors
    if (block.code > 139) {
        block.right = BLOCK_BACKGROUND;
        block.left = BLOCK_BACKGROUND;
        block.up = BLOCK_BACKGROUND;
        block.down = BLOCK_BACKGROUND;
    }

    // Elevators
    if (block.code == BLOCK_ELEVATOR_HORI) {
        block.left_bound += (int) cos_table[degree % 360];
        block.right_bound += (int) cos_table[degree % 360];
    }
    if (block.code == BLOCK_ELEVATOR_VERT) {
        block.bottom_bound += (int) sin_table[degree % 360];
        block.top_bound += (int) sin_table[degree % 360];
    }

    // Passable floor
    if (block.code == BLOCK_RESTRICTION_DOWN) {
        block.right = BLOCK_BACKGROUND;
        block.up = BLOCK_BACKGROUND;
        block.down = BLOCK_WALL;
        block.left = BLOCK_BACKGROUND;
        block.bottom_bound -= 27;
    }

    // Hills
    if (block.code > 49 && block.code < 60) {
        block.right = BLOCK_BACKGROUND;
        block.up = BLOCK_WALL;
        block.down = BLOCK_WALL;
        block.left = BLOCK_BACKGROUND;
        block.bottom_bound += 1;
    }

    // Switches
    if (block.code >= BLOCK_SWITCH1 && block.code <= BLOCK_SWITCH3) {
        block.right = BLOCK_WALL;
        block.up = BLOCK_WALL;
        block.down = BLOCK_WALL;
        block.left = BLOCK_WALL;
    }

    // Floors affected by switches
    int switch1_y = 0, switch2_y = 0, switch3_x = 0;

    if (switch1 > 0) {
        switch1_y = 64;
        if (switch1 < 64)
            switch1_y = switch1;
        if (switch1 > SWITCH_START_VALUE - 64)
            switch1_y = SWITCH_START_VALUE - switch1;
    }
    if (switch2 > 0) {
        switch2_y = 64;
        if (switch2 < 64)
            switch2_y = switch2;
        if (switch2 > SWITCH_START_VALUE - 64)
            switch2_y = SWITCH_START_VALUE - switch2;
    }
    if (switch3 > 0) {
        switch3_x = 64;
        if (switch3 < 64)
            switch3_x = switch3;
        if (switch3 > SWITCH_START_VALUE - 64)
            switch3_x = SWITCH_START_VALUE - switch3;
    }
    if (block.code == BLOCK_SWITCH2_UP) {
        block.bottom_bound -= switch2_y / 2;
        block.top_bound -= switch2_y / 2;
    }
    if (block.code == BLOCK_SWITCH2_DOWN) {
        block.bottom_bound += switch2_y / 2;
        block.top_bound += switch2_y / 2;
    }
    if (block.code == BLOCK_SWITCH2) {
        block.bottom_bound += switch2_y / 2;
        block.top_bound += switch2_y / 2;
    }
    if (block.code == BLOCK_SWITCH3_RIGHT) {
        block.right_bound += switch3_x / 2;
        block.left_bound += switch3_x / 2;
        block.code = BLOCK_ELEVATOR_HORI; // The same idea applies for pushing in the side direction
    }
    if (block.code == BLOCK_SWITCH3_LEFT) {
        block.right_bound -= switch3_x / 2;
        block.left_bound -= switch3_x / 2;
        block.code = BLOCK_ELEVATOR_HORI; // The same idea applies for pushing in the side direction
    }
    if (block.code == BLOCK_SWITCH3) {
        block.bottom_bound += switch3_x / 2;
        block.top_bound += switch3_x / 2;
    }
    if (block.code == BLOCK_SWITCH1) {
        block.bottom_bound += switch1_y / 2;
        block.top_bound += switch1_y / 2;
    }
}

Block Examine_Block(int x, int y) {
    Block block;
    if (x < 0 || x > MAP_WIDTH || y < 0 || y > MAP_WIDTH) {
        block.code = 255;
        block.background = true;
        block.left_bound = x * 32;
        block.right_bound = x * 32 + 32;
        block.top_bound = y * 32;
        block.bottom_bound = y * 32 + 32;
        block.water = false;
        block.edge = true;
        return block;
    }
    unsigned char i = map->walls[x + y * MAP_WIDTH];
    if (i < 150) {
        block = calculated_blocks[i];
        block.left_bound = x * 32 + calculated_blocks[i].left;
        block.right_bound = x * 32 + 32 + calculated_blocks[i].right;
        block.top_bound = y * 32 + calculated_blocks[i].up;
        block.bottom_bound = y * 32 + 32 + calculated_blocks[i].down;
    } else {
        block.code = 255;
        block.background = true;
        block.left_bound = x * 32;
        block.right_bound = x * 32 + 32;
        block.top_bound = y * 32;
        block.bottom_bound = y * 32 + 32;
        block.water = false;
    }
    i = map->backgrounds[x + y * MAP_WIDTH];
    if (i > 131 && i < 140)
        block.water = true;
    block.edge = map->edges[x + y * MAP_WIDTH];
    return block;
}

int Add_BackgroundSprite(int index) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (background_sprites[i] == -1) {
            background_sprites[i] = index;
            return 0;
        }
    }
    return 1;
}

void Sort_BackgroundSprites(void) {
    bool stop = false;
    int l = 1;
    int temp;
    while (l < MAX_SPRITES && stop == false) {
        stop = true;
        for (int i = 0; i < MAX_SPRITES - 1; i++) {
            if (background_sprites[i] == -1 || background_sprites[i + 1] == -1)
                i = MAX_SPRITES;
            else {
                if (sprites[background_sprites[i]].type->parallax_coefficient > sprites[background_sprites[i + 1]].type->parallax_coefficient) {
                    temp = background_sprites[i];
                    background_sprites[i] = background_sprites[i + 1];
                    background_sprites[i + 1] = temp;
                    stop = false;
                }
            }
        }
        l++;
    }
}

int Add_Sprite(PK2Sprite_Prototype &proto, int player, double x, double y, int parent) {
    bool added = false;
    int i = 0;
    while (!added && i < MAX_SPRITES) {
        if (sprites[i].is_hidden) {
            sprites[i] = PK2Sprite(&proto, player, false, x, y);

            sprites[i].x = x + 16 + 1;
            sprites[i].y += sprites[i].type->height / 2;
            sprites[i].start_x = sprites[i].x;
            sprites[i].start_y = sprites[i].y;

            if (proto.type == TYPE_BACKGROUND)
                Add_BackgroundSprite(i);
            if (parent != MAX_SPRITES) {
                sprites[i].parent_sprite = parent;
            } else {
                sprites[i].parent_sprite = i;
            }
            added = true;
        } else
            i++;
    }

    return 0;
}

int Add_BonusSprite(PK2Sprite_Prototype &proto, int player, double x, double y, int parent) {
    bool added = false;
    int i = 0;

    while (!added && i < MAX_SPRITES) {
        if (sprites[i].is_hidden) {
            sprites[i] = PK2Sprite(&proto, player, false, x, y);
            sprites[i].x += sprites[i].type->width;
            sprites[i].y += sprites[i].type->height / 2;
            sprites[i].start_x = sprites[i].x;
            sprites[i].start_y = sprites[i].y;
            sprites[i].jump_timer = 1;//-1 - rand()%3;
            //sprites[i].movement_y = rand()%2 - rand()%2;
            sprites[i].movement_x = rand() % 2 - rand() % 4;
            sprites[i].hit_counter = 35;//25

            if (proto.type == TYPE_BACKGROUND)
                Add_BackgroundSprite(i);
            if (parent != MAX_SPRITES) {
                sprites[i].parent_sprite = parent;
            } else {
                sprites[i].parent_sprite = i;
            }
            added = true;
        } else
            i++;
    }

    return 0;
}

int Add_ProjectileSprite(PK2Sprite_Prototype &proto, int player, double x, double y, int parent) {
    bool added = false;
    int i = 0;

    while (!added && i < MAX_SPRITES) {
        if (sprites[i].is_hidden) {
            sprites[i] = PK2Sprite(&proto, player, false, x/*-proto.width/2*/, y);

            //sprites[i].x += sprites[i].type->width;
            //sprites[i].y += sprites[i].type->height/2;

            if (proto.Has_AI(AI_THROWING_WEAPON)) {
                if ((int) sprites[parent].movement_x == 0) {
                    // If the "shooter" is a player or the bullet speed is zero
                    if (sprites[parent].player == 1 || sprites[i].type->max_speed == 0) {
                        if (!sprites[parent].flip_horizontal)
                            sprites[i].movement_x = sprites[i].type->max_speed;
                        else
                            sprites[i].movement_x = -sprites[i].type->max_speed;
                    } else // or if it's an enemy
                    {
                        if (!sprites[parent].flip_horizontal)
                            sprites[i].movement_x = 1 + rand() % (int) sprites[i].type->max_speed;
                        else
                            sprites[i].movement_x = -1 - rand() % -(int) sprites[i].type->max_speed;
                    }
                } else {
                    if (!sprites[parent].flip_horizontal)
                        sprites[i].movement_x = sprites[i].type->max_speed + sprites[parent].movement_x;
                    else
                        sprites[i].movement_x = -sprites[i].type->max_speed + sprites[parent].movement_x;
                    //sprites[i].movement_x = sprites[parent].movement_x * 1.5;
                }
                sprites[i].jump_timer = 1;
            } else if (proto.Has_AI(AI_EGG)) {
                sprites[i].y = sprites[parent].y + 10;
                sprites[i].movement_x = sprites[parent].movement_x / 1.5;
            } else {
                if (!sprites[parent].flip_horizontal)
                    sprites[i].movement_x = sprites[i].type->max_speed;
                else
                    sprites[i].movement_x = -sprites[i].type->max_speed;
            }

            if (parent != MAX_SPRITES) {
                sprites[i].parent_sprite = parent;
                sprites[i].is_enemy = sprites[parent].is_enemy;
            } else {
                sprites[i].parent_sprite = i;
            }

            if (proto.type == TYPE_BACKGROUND)
                Add_BackgroundSprite(i);

            added = true;
        } else
            i++;
    }

    return 0;
}

int Clear_Sprites() {
    int i = 0;

    while (i < MAX_SPRITES) {
        sprites[i] = PK2Sprite();
        background_sprites[i] = -1;
        i++;
    }

    return 0;
}

int Set_Map_Sprites() {
    Clear_Sprites();

    Add_Sprite(prototypes[map->player_sprite], 1, 0, 0, MAX_SPRITES);

    int sprite;

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            sprite = map->sprites[x + y * MAP_WIDTH];

            if (sprite != 255 && prototypes[sprite].height > 0) {
                Add_Sprite(prototypes[sprite], 0, x * 32, y * 32 - prototypes[sprite].height + 32, MAX_SPRITES);
            }
        }
    }

    Sort_BackgroundSprites();

    return 0;
}

int Draw_Bonuses(void) {
    int bonus;
    int px = 0, py = 0;

    for (int x = 0; x < SCREEN_WIDTH / 32; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / 32; y++) {
            bonus = map->sprites[x + camera_x / 32 + (y + camera_y / 32) * MAP_WIDTH];

            if (bonus != 255 && prototypes[bonus].type == TYPE_BONUS && prototypes[bonus].weight <= 0) {
                px = x * 32 + prototypes[bonus].width / 2 - camera_x % 32;
                py = y * 32 - prototypes[bonus].height + 32 - camera_y % 32;
                py += (int) cos_table[(degree + (x) * 20) % 360] / 3;
                prototypes[bonus].Draw(px, py, 0);
            }
        }
    }
    return 0;
}

void Initialize_Items() {
    for (int i = 0; i < MAX_ITEMS; i++)
        items[i] = NULL;
}

int Draw_Items() {
    int x, y;
    Set_Bounds();
    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    y = MAP_DRAWING_HEIGHT - 35;//36
    x = item_panel_x + 35;//40

    unsigned char v1, v2;
    num_items = 0;
    for (int i = 0; i < MAX_ITEMS; i++)
        if (items[i] != NULL) {
            num_items++;
            if (i == 0) {
                v1 = 31;
                v2 = 16 + 128;
            } else {
                v1 = 0;
                v2 = 16;
            }
            if (false) {
                PisteDraw_Buffer_Clear(PD_BACKBUFFER, x - items[i]->width / 2 + 4, y - items[i]->height / 2 + 4, x + items[i]->width / 2 + 4, y + items[i]->height / 2 + 4, 0);
                PisteDraw_Buffer_Clear(PD_BACKBUFFER, x - items[i]->width / 2 - 1, y - items[i]->height / 2 - 1, x + items[i]->width / 2 + 1, y + items[i]->height / 2 + 1, v1);
                PisteDraw_Buffer_Clear(PD_BACKBUFFER, x - items[i]->width / 2 + 1, y - items[i]->height / 2 + 1, x + items[i]->width / 2 - 1, y + items[i]->height / 2 - 1, v2);
            }
            items[i]->Draw(x - items[i]->width / 2, y - items[i]->height / 2, 0);
            x += 38;
        }
    Set_Bounds();
    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    return 0;
}

bool Add_Item(PK2Sprite_Prototype *proto) {
    int i = 0;
    bool added = false;
    char message[80];
    strcpy(message, texts->Get_Text(txt_game_newitem));  //"movement_x new item: ";
    while (i < MAX_ITEMS && !added) {
        while (i < MAX_ITEMS && !added) {
            if (items[i] == NULL) {
                added = true;
                items[i] = proto;
                //strcat(message, proto->name);
                New_Notification(message);
            }
            i++;
        }
        return added;
    }
}

int Use_Item() {
    if (items[0] != NULL)
        Add_Sprite(*items[0], 0, sprites[player_index].x - items[0]->width, sprites[player_index].y, MAX_SPRITES/*player_index*/);
    for (int i = 0; i < MAX_ITEMS - 1; i++)
        items[i] = items[i + 1];
    items[MAX_ITEMS - 1] = NULL;

    return 0;
}

int Switch_Item() {
    if (items[0] == NULL)
        return 0;
    PK2Sprite_Prototype *temp = items[0];
    for (int i = 0; i < MAX_ITEMS - 1; i++)
        items[i] = items[i + 1];
    int count = 0;
    while (count < MAX_ITEMS - 1 && items[count] != NULL)
        count++;
    items[count] = temp;

    return 0;
}

void Check_Wall(PK2Sprite &sprite, Block &block, double &sprite_x, double &sprite_y, double &sprite_a, double &sprite_b, double &sprite_left, double &sprite_right, double &sprite_top, double &sprite_bottom, int &sprite_width, int &sprite_height, int &map_left, int &map_top, bool &right, bool &left, bool &up, bool &down) {
    // check if block left_bound or right_bound is a wall.
    if (sprite_top < block.bottom_bound && sprite_bottom - 1 > block.top_bound) {
        if (sprite_right + sprite_a - 1 > block.left_bound && sprite_left + sprite_a < block.right_bound) {
            // Check if sprite is going inside the right-side block.
            if (sprite_right + sprite_a < block.right_bound) {
                // Is the block a wall?
                if (block.right == BLOCK_WALL) {
                    right = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.left_bound - sprite_width / 2;
                }
            }
            if (sprite_left + sprite_a > block.left_bound) {
                if (block.left == BLOCK_WALL) {
                    left = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.right_bound + sprite_width / 2;
                }
            }
        }
    }
    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;

    // ...check if the block is a floor or ceiling.
    if (sprite_left < block.right_bound && sprite_right - 1 > block.left_bound) {
        if (sprite_bottom + sprite_b - 1 >= block.top_bound && sprite_top + sprite_b <= block.bottom_bound) {
            if (sprite_bottom + sprite_b - 1 <= block.bottom_bound) {
                if (block.down == BLOCK_WALL) {
                    down = false;
                    if (block.code == BLOCK_ELEVATOR_VERT)
                        sprite_y = block.top_bound - sprite_height / 2;
                    if (sprite_bottom - 1 >= block.top_bound && sprite_b >= 0) {
                        if (block.code != BLOCK_ELEVATOR_HORI)
                            sprite_y = block.top_bound - sprite_height / 2;
                    }
                }
            }
            if (sprite_top + sprite_b > block.top_bound) {
                if (block.up == BLOCK_WALL) {
                    up = false;
                    if (sprite_top < block.bottom_bound) {
                        if (block.code != BLOCK_ELEVATOR_HORI)
                            sprite.is_crouching = true;
                    }
                }
            }
        }
    }
}

/*######################### MOVE #################################*/

double sprite_x, sprite_y, sprite_a, sprite_b, sprite_left, sprite_right, sprite_top, sprite_bottom;
int sprite_width, sprite_height;
int map_left, map_top,
//		blocks_x_amount,
//		blocks_y_amount,
x = 0, y = 0;

bool right, left, up, down, in_water;
unsigned char max_speed;

void Check_Wall(PK2Sprite &sprite, Block &block) {
    int mask_index;

    if (sprite_x <= block.right_bound && sprite_x >= block.left_bound && sprite_y <= block.bottom_bound && sprite_y >= block.top_bound) {
        /**********************************************************************/
        /* Check if the wall's background is a water background               */
        /**********************************************************************/
        if (block.water) {
            sprite.in_water = true;
        }

        /**********************************************************************/
        /* Check if the wall is a fire, switch on top, and sprite is ready    */
        /**********************************************************************/
        if (block.code == BLOCK_FIRE && switch1 == 0 && sprite.hit_counter == 0) {
            sprite.damage_received = 2;
            sprite.damage_type_received = DAMAGE_FIRE;
        }

        /**********************************************************************/
        /* Check if the wall is a hiding place                                */
        /**********************************************************************/
        if (block.code == BLOCK_HIDDEN)
            sprite.hiding = true;

        /**********************************************************************/
        /* Check if the wall is an exit                                       */
        /**********************************************************************/
        if (block.code == BLOCK_FINISH && sprite.player != 0) {
            if (!stage_completed) {
                if (Midas_Load_Music("music\\hiscore.xm") != 0)
                    PK2_error = true;
                stage_completed = true;
                stages[current_stage_index].completed = true;
/*
				points += stage_points;
				points += duration * 5;
				fake_points = points;
*/
                if (stages[current_stage_index].order >= stage)
                    stage++;
            }
        }
/*
		if (block.code == BLOCK_CURRENT_UP) {
			//sprite_b -= 0.25;
		}
*/
    }

    if (sprite_left <= block.right_bound - 4 && sprite_right >= block.left_bound + 4 && sprite_top <= block.bottom_bound && sprite_bottom >= block.top_bound + 16) {
        /**********************************************************************/
        /* Check if the wall is a fire, switch on top, and sprite is ready    */
        /**********************************************************************/
        if (block.code == BLOCK_FIRE && switch1 == 0 && sprite.hit_counter == 0) {
            sprite.damage_received = 2;
            sprite.damage_type_received = DAMAGE_FIRE;
        }
    }

    /* Check if it's a wall that blocks downward movements */
    if ((block.code < 80 || block.code > 139) && block.code != BLOCK_RESTRICTION_DOWN && block.code < 150) {
        mask_index = (int) (sprite_x + sprite_a) - block.left_bound;
        if (mask_index < 0)
            mask_index = 0;
        if (mask_index > 31)
            mask_index = 31;
        block.top_bound += block_masks[block.code].down[mask_index];
        if (block.top_bound >= block.bottom_bound - 2)
            block.down = BLOCK_BACKGROUND;
        block.bottom_bound -= block_masks[block.code].up[mask_index];
    }

    if (sprite_left <= block.right_bound + 2 && sprite_right >= block.left_bound - 2 && sprite_top <= block.bottom_bound && sprite_bottom >= block.top_bound) {
        /**********************************************************************/
        /* Check if the sprite is a key and the wall is a lock                */
        /**********************************************************************/
        if (block.code == BLOCK_LOCK && sprite.type->key) {
            map->walls[block.left_bound / 32 + (block.top_bound / 32) * MAP_WIDTH] = 255;
            Calculate_Map_Edges();

            sprite.is_hidden = true;

            if (sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
                num_keys--;
                if (num_keys < 1)
                    Unlock_Map();
            }

            Create_ExplosionEffect(block.left_bound + 16, block.top_bound + 10, 0);
            Play_Sound(unlock_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
        }

        /**************************************************************************/
        /* Check if the sprite has ended up in a flow (conveyor belt, wind, etc.) */
        /**************************************************************************/
        if (block.code == BLOCK_CURRENT_LEFT && left)
            sprite_a -= 0.02;

        if (block.code == BLOCK_CURRENT_RIGHT && right)
            sprite_a += 0.02;    //0.05

        /*********************************************************************/
        /* Check if the sprite has come to the edge from which it can fall   */
        /*********************************************************************/
        if (block.edge && sprite.jump_timer <= 0 && sprite_y < block.bottom_bound && sprite_y > block.top_bound) {
            /* && sprite_bottom <= block.bottom_bound+2)*/ // has the sprite come to the edge
            if (sprite_left > block.left_bound)
                sprite.edge_left = true;

            if (sprite_right < block.right_bound)
                sprite.edge_right = true;
        }
    }

    // Check if the block is a left_bound or right_bound wall.
    if (sprite_top < block.bottom_bound && sprite_bottom - 1 > block.top_bound) {
        if (sprite_right + sprite_a - 1 > block.left_bound && sprite_left + sprite_a < block.right_bound) {
            // Investigate if the sprite is going inside the right-side block.
            if (sprite_right + sprite_a < block.right_bound) {
                // Is the block a wall?
                if (block.right == BLOCK_WALL) {
                    right = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.left_bound - sprite_width / 2;
                }
            }
            if (sprite_left + sprite_a > block.left_bound) {
                if (block.left == BLOCK_WALL) {
                    left = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.right_bound + sprite_width / 2;
                }
            }
        }
    }

    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;

    // ...check if the block is a floor or ceiling.

    if (sprite_left < block.right_bound && sprite_right - 1 > block.left_bound) {
        if (sprite_bottom + sprite_b - 1 >= block.top_bound && sprite_top + sprite_b <= block.bottom_bound) {
            if (sprite_bottom + sprite_b - 1 <= block.bottom_bound) {
                if (block.down == BLOCK_WALL) {
                    down = false;
                    if (block.code == BLOCK_ELEVATOR_VERT)
                        sprite_y = block.top_bound - sprite_height / 2;
                    if (sprite_bottom - 1 >= block.top_bound && sprite_b >= 0) {
                        //sprite_y -= sprite_bottom - block.top_bound;
                        if (block.code != BLOCK_ELEVATOR_HORI) {
                            sprite_y = block.top_bound - sprite_height / 2;
                        }
                    }
                    if (sprite.total_weight >= 1) {
                        if (block.code == BLOCK_SWITCH1 && switch1 == 0) {
                            switch1 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        if (block.code == BLOCK_SWITCH2 && switch2 == 0) {
                            switch2 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        if (block.code == BLOCK_SWITCH3 && switch3 == 0) {
                            switch3 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        //if (sprite_b > 3 && sprite.weight >= 2)
                        //	story_switch = 5;
                    }
                }
            }
            if (sprite_top + sprite_b > block.top_bound) {
                if (block.up == BLOCK_WALL) {
                    up = false;
                    if (sprite_top < block.bottom_bound) {
                        if (block.code == BLOCK_ELEVATOR_VERT && sprite.is_crouching) {
                            sprite.damage_received = 2;
                            sprite.damage_type_received = DAMAGE_HIT;
                        }
                        if (block.code != BLOCK_ELEVATOR_HORI) {
                            //if (sprite.is_crouching)
                            //	sprite_y = block.bottom_bound + sprite_height /2;
                            sprite.is_crouching = true;
                        }
                    }
                }
            }
        }
    }
}

void Check_Wall_Debug(PK2Sprite &sprite, Block &block, double &sprite_x, double &sprite_y, double &sprite_a, double &sprite_b, double &sprite_left, double &sprite_right, double &sprite_top, double &sprite_bottom, int &sprite_width, int &sprite_height, int &map_left, int &map_top, bool &right, bool &left, bool &up, bool &down) {
    int mask_index = 0;

    if (sprite_x <= block.right_bound && sprite_x >= block.left_bound && sprite_y <= block.bottom_bound && sprite_y >= block.top_bound) {
        /**********************************************************************/
        /* Check if the wall's background is a water background               */
        /**********************************************************************/
        if (block.water) {
            sprite.in_water = true;
        }

        /*****************************************************************************************/
        /* Investigate if the wall is approaching, the switch is on top, and the sprite is ready */
        /*****************************************************************************************/
/*
        if (block.code == BLOCK_FIRE && switch1 == 0 && sprite.hit_counter == 0)
		{
			sprite.damage_received = 2;
			sprite.damage_type_received = DAMAGE_FIRE;
		}
*/
		/**********************************************************************/
        /* Check if the wall is a hiding place                                */
        /**********************************************************************/
        if (block.code == BLOCK_HIDDEN)
            sprite.hiding = true;

        /**********************************************************************/
        /* Check if the wall is an exit                                       */
        /**********************************************************************/
        if (block.code == BLOCK_FINISH && sprite.player != 0) {
            if (!stage_completed) {
                if (Midas_Load_Music("music\\hiscore.xm") != 0)
                    PK2_error = true;
                stage_completed = true;
                stages[current_stage_index].completed = true;
                stage++;
            }
        }
/*
		if (block.code == BLOCK_CURRENT_UP)
		{
			//sprite_b -= 0.25;
		}
*/
    }

    if (sprite_left <= block.right_bound - 4 && sprite_right >= block.left_bound + 4 && sprite_top <= block.bottom_bound && sprite_bottom >= block.top_bound + 16) {
        /*****************************************************************************************/
        /* Investigate if the wall is approaching, the switch is on top, and the sprite is ready */
        /*****************************************************************************************/
        if (block.code == BLOCK_FIRE && switch1 == 0 && sprite.hit_counter == 0) {
            sprite.damage_received = 2;
            sprite.damage_type_received = DAMAGE_FIRE;
        }
    }

    if ((block.code < 80 || block.code > 139) && block.code != BLOCK_RESTRICTION_DOWN && block.code < 150) {
        mask_index = (int) (sprite_x + sprite_a) - block.left_bound;
        if (mask_index < 0)
            mask_index = 0;
        if (mask_index > 31)
            mask_index = 31;
        block.top_bound += block_masks[block.code].down[mask_index];
        if (block.top_bound >= block.bottom_bound - 2)
            block.down = BLOCK_BACKGROUND;
        block.bottom_bound -= block_masks[block.code].up[mask_index];
    }

    if (sprite_left <= block.right_bound + 2 && sprite_right >= block.left_bound - 2 && sprite_top <= block.bottom_bound && sprite_bottom >= block.top_bound) {
        /**********************************************************************/
        /* Investigate if the sprite is a key and the wall is a lock          */
        /**********************************************************************/
        if (block.code == BLOCK_LOCK && sprite.type->key) {
            map->walls[block.left_bound / 32 + (block.top_bound / 32) * MAP_WIDTH] = 255;
            Calculate_Map_Edges();
            sprite.is_hidden = true;
            if (sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
                num_keys--;
                if (num_keys < 1)
                    Unlock_Map();
            }
            Create_ExplosionEffect(block.left_bound + 16, block.top_bound + 10, 0);
            Play_Sound(unlock_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
        }

        /**************************************************************************/
        /* Check if the sprite has ended up in a flow (conveyor belt, wind, etc.) */
        /**************************************************************************/
        if (block.code == BLOCK_CURRENT_LEFT && left)
            sprite_a -= 0.02;
        if (block.code == BLOCK_CURRENT_RIGHT && right)
            sprite_a += 0.02;    //0.05

        /*********************************************************************/
        /* Check if the sprite has come to the edge from which it can fall   */
        /*********************************************************************/
        if (block.edge && sprite.jump_timer <= 0 && sprite_y < block.bottom_bound && sprite_y > block.top_bound) {
            /* && sprite_bottom <= block.bottom_bound+2)*/ // has the sprite come to the edge
            if (sprite_left > block.left_bound)
                sprite.edge_left = true;
            if (sprite_right < block.right_bound)
                sprite.edge_right = true;
        }
    }

    // check if block left_bound or right_bound is a wall.
    if (sprite_top < block.bottom_bound && sprite_bottom - 1 > block.top_bound) {
        if (sprite_right + sprite_a - 1 > block.left_bound && sprite_left + sprite_a < block.right_bound) {
            // Check if sprite is going inside the right-side block.
            if (sprite_right + sprite_a < block.right_bound) {
                // Is the block a wall?
                if (block.right == BLOCK_WALL) {
                    right = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.left_bound - sprite_width / 2;
                }
            }
            if (sprite_left + sprite_a > block.left_bound) {
                if (block.left == BLOCK_WALL) {
                    left = false;
                    if (block.code == BLOCK_ELEVATOR_HORI)
                        sprite_x = block.right_bound + sprite_width / 2;
                }
            }
        }
    }

    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;

    // ...check if the block is a floor or ceiling.

    if (sprite_left < block.right_bound && sprite_right - 1 > block.left_bound) {
        if (sprite_bottom + sprite_b - 1 >= block.top_bound && sprite_top + sprite_b <= block.bottom_bound) {
            if (sprite_bottom + sprite_b - 1 <= block.bottom_bound) {
                if (block.down == BLOCK_WALL) {
                    down = false;
                    if (block.code == BLOCK_ELEVATOR_VERT)
                        sprite_y = block.top_bound - sprite_height / 2;
                    if (sprite_bottom - 1 >= block.top_bound && sprite_b >= 0) {
                        //sprite_y -= sprite_bottom - block.top_bound;
                        if (block.code != BLOCK_ELEVATOR_HORI) {
                            sprite_y = block.top_bound - sprite_height / 2;
                        }
                    }
                    if (sprite.total_weight >= 1) {
                        if (block.code == BLOCK_SWITCH1 && switch1 == 0) {
                            switch1 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        if (block.code == BLOCK_SWITCH2 && switch2 == 0) {
                            switch2 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        if (block.code == BLOCK_SWITCH3 && switch3 == 0) {
                            switch3 = SWITCH_START_VALUE;
                            story_switch = 64;
                            Play_Sound(switch_sound, 100, (int) sprite_x, (int) sprite_y, SOUNDS_SAMPLERATE, false);
                        }
                        //if (sprite_b > 3 && sprite.weight >= 2)
                        //	story_switch = 5;
                    }
                }
            }
            if (sprite_top + sprite_b > block.top_bound) {
                if (block.up == BLOCK_WALL) {
                    up = false;
                    if (sprite_top < block.bottom_bound) {
                        if (block.code == BLOCK_ELEVATOR_VERT && sprite.is_crouching) {
                            sprite.damage_received = 2;
                            sprite.damage_type_received = DAMAGE_HIT;
                        }
                        if (block.code != BLOCK_ELEVATOR_HORI) {
                            //if (sprite.is_crouching)
                            //	sprite_y = block.bottom_bound + sprite_height /2;
                            sprite.is_crouching = true;
                        }
                    }
                }
            }
        }
    }
}

//TODO: =>
int Move_Sprite(int i) {
    if (i >= MAX_SPRITES || i < 0)
        return -1;
    if (!sprites[i].type)
        return -1;
/*
	double	sprite_x,
			sprite_y,
			sprite_a,
			sprite_b,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width,
			sprite_height;

	int		map_left,
			map_top,

			x = 0,
			y = 0;

	bool	right,
			left,
			up,
			down,

			in_water;

	unsigned char   max_speed;
*/
    sprite_x = 0;
    sprite_y = 0;
    sprite_a = 0;
    sprite_b = 0;

    sprite_left = 0;
    sprite_right = 0;
    sprite_top = 0;
    sprite_bottom = 0;

    sprite_width = 0;
    sprite_height = 0;

    map_left = 0;
    map_top = 0;

    x = 0;
    y = 0;

    right = false;
    left = false;
    up = false;
    down = false;

    in_water = false;

    max_speed = 0;

    // Variables
/*
	PK2Sprite &sprite = sprites[i];

	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.movement_x,
			sprite_b = sprite.movement_y,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width  = sprite.type->width,
			sprite_height = sprite.type->height;

	int		map_left,
			map_top,

			blocks_x_count,
			blocks_y_count,

			x = 0,
			y = 0;

	bool	right	= true,
			left	= true,
			up		= true,
			down		= true;

	bool	in_water = sprite.in_water;

	unsigned char   max_speed = sprite.type->max_speed;
*/

    if (sprites[i].type == NULL)
        return 1;

    PK2Sprite &sprite = sprites[i];

    sprite_x = sprite.x;
    sprite_y = sprite.y;
    sprite_a = sprite.movement_x;
    sprite_b = sprite.movement_y;

    sprite_width = sprite.type->width;
    sprite_height = sprite.type->height;

    x = 0;
    y = 0;
    right = true, left = true, up = true, down = true;

    in_water = sprite.in_water;
    sprite.is_crouching = false;//sprite.hiding; //sprite is not crouching by default
    sprite.edge_left = false;
    sprite.edge_right = false;

    // Move the actual variables to auxiliary variables.
    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;
    sprite_top = sprite_y - sprite_height / 2;
    sprite_bottom = sprite_y + sprite_height / 2;

    max_speed = (unsigned char) sprite.type->max_speed;
    /* Add speed to stunned sprites */
    if (sprite.health < 1/* && sprite.type->max_speed == 0*/)
        max_speed = 3;
    // Calculate the remaining sprite attack
    if (sprite.attack_timer_1 > 0)
        sprite.attack_timer_1--;
    if (sprite.attack_timer_2 > 0)
        sprite.attack_timer_2--;
    if (sprite.charge_time > 0)    // duration between two shots (laying)
        sprite.charge_time--;
    if (sprite.transformation_timer > 0)    // duration for changes
        sprite.transformation_timer--;

    /*****************************************************************************************/
    /* Player sprite and its control                                                         */
    /*****************************************************************************************/

    bool additional_speed = true;
    bool slow_down = false;

    if (sprite.player != 0 && sprite.health > 0) {
        /* WALKING */
        if (PisteInput_Read_Controller(control_run))
            additional_speed = false;

        /* ATTACK 1 */
        if (PisteInput_Read_Controller(control_attack1) && sprite.charge_time == 0 && sprite.projectile_1 != -1) {
            sprite.attack_timer_1 = sprite.type->attack1_duration;
        } else { /* ATTACK 2 */
            if (PisteInput_Read_Controller(control_attack2) && sprite.charge_time == 0 && sprite.projectile_2 != -1)
                sprite.attack_timer_2 = sprite.type->attack2_duration;
        }

        /* CROUCHING */
        sprite.is_crouching = false;

        if (PisteInput_Read_Controller(control_down) && !sprite.can_move_down) {
            sprite.is_crouching = true;
            sprite_top += sprite_height / 1.5;
        }

        double a_additional = 0;

        /* MOVING RIGHT */
        if (PisteInput_Read_Controller(control_right)) {
            a_additional = 0.04;//0.08;

            if (additional_speed) {
                if (rand() % 20 == 1 && sprite.animation_index == ANIMATION_WALK) //rand()%30
                    New_Particle(PARTICLE_DUST_CLOUD, sprite_x - 8, sprite_bottom - 8, 0.25, -0.25, 40, 0, 0);

                a_additional += 0.09;//0.05
            }

            if (sprite.can_move_down)
                a_additional /= 1.5;//2.0

            sprite.flip_horizontal = false;
        }

        /* MOVING LEFT */
        if (PisteInput_Read_Controller(control_left)) {
            a_additional = -0.04;

            if (additional_speed) {
                if (rand() % 20 == 1 && sprite.animation_index == ANIMATION_WALK) {
                    New_Particle(PARTICLE_DUST_CLOUD, sprite_x - 8, sprite_bottom - 8, -0.25, -0.25, 40, 0, 0);
                }

                a_additional -= 0.09;
            }

            if (sprite.can_move_down)    // when the sprite touches the ground, friction affects
                a_additional /= 1.5;//2.0

            sprite.flip_horizontal = true;
        }

        if (sprite.is_crouching)    // the speed slows down when crouching
            a_additional /= 10;

        sprite_a += a_additional;

        /* JUMPING */
        if (sprite.type->weight > 0) {
            if (PisteInput_Read_Controller(control_jump)) {
                if (!sprite.is_crouching) {
                    if (sprite.jump_timer == 0)
                        Play_Sound(jump_sound, 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency,
                                   sprite.type->random_freq);
                    if (sprite.jump_timer <= 0)
                        sprite.jump_timer = 1; //10;
                }
            } else {
/*
				if (sprite.jump_timer > 0 && sprite.jump_timer < 55)
					sprite.jump_timer = 55;
*/
                if (sprite.jump_timer > 0 && sprite.jump_timer < 45) //40
                    sprite.jump_timer = 55;
            }

            /* falling quietly downward */
            if (PisteInput_Read_Controller(control_jump) && sprite.jump_timer >= 150/*90+20*/ && sprite.type->can_float_down)
                slow_down = true;
/*
			if (slow_down && sprite_b > 0) {
				sprite_b /= 4;//3
			}
*/
        }
            /* MOVING UP AND DOWN */
        else { // if the player-sprite's weight is 0 - for example, as a bird
            if (PisteInput_Read_Controller(control_jump)) {
                sprite_b -= 0.15;
            }

            if (PisteInput_Read_Controller(control_down)) {
                sprite_b += 0.15;
            }

            sprite.jump_timer = 0;
        }

        /* AI that also applies to the player */
        for (int ai = 0; ai < SPRITE_MAX_AI; ai++)
            switch (sprite.type->AI[ai]) {
                case AI_CHANGE_IF_ENERGY_LESS_THAN_2:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_If_Energy_Less_Than_2(prototypes[sprite.type->transformation]);
                    break;
                case AI_CHANGE_IF_ENERGY_MORE_THAN_1:
                    if (sprite.type->transformation > -1) {
                        if (sprite.AI_Change_If_Energy_More_Than_1(prototypes[sprite.type->transformation]) == 1)
                            Create_Effects(DESTRUCTION_SMOKE_GRAY, (unsigned long) sprite.x, (unsigned long) sprite.y);
                    }
                    break;
                case AI_CHANGE_TIMER:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_Timer(prototypes[sprite.type->transformation]);
                    break;
                case AI_DAMAGE_FROM_WATER:
                    sprite.AI_Takes_Damage_From_Water();
                    break;

                case AI_CHANGE_IF_HIT:
                    if (sprite.type->transformation > -1) {
                        sprite.AI_Change_If_Hit(prototypes[sprite.type->transformation]);
                    }
                    break;
                default:
                    break;
            }

        /* It won't work if the player is something other than a game character */
        if (sprite.type->type != TYPE_GAME_CHARACTER)
            sprite.health = 0;
    }

    //TODO: =>
    /*****************************************************************************************/
    /* Jump                                                                                  */
    /*****************************************************************************************/

    bool jump_maximum = sprite.jump_timer >= 90;

    // If we have jumped / in the air:
    if (sprite.jump_timer > 0) {
        if (sprite.jump_timer < 50 - sprite.type->max_jump)
            sprite.jump_timer = 50 - sprite.type->max_jump;

        if (sprite.jump_timer < 10)
            sprite.jump_timer = 10;

        if (!jump_maximum) {
            //sprite_b = (sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.0;//-4
            sprite_b = -sin_table[sprite.jump_timer] / 8;//(sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.5;
            if (sprite_b > sprite.type->max_jump) {
                sprite_b = sprite.type->max_jump / 10.0;
                sprite.jump_timer = 90 - sprite.jump_timer;
            }
        }

        if (sprite.jump_timer < 180)
            sprite.jump_timer += 2;
    }

    if (sprite.jump_timer < 0)
        sprite.jump_timer++;

    if (sprite_b > 0 && !jump_maximum)
        sprite.jump_timer = 90;//sprite.type->max_jump*2;

    /*****************************************************************************************/
    /* Recovering from impact                                                                */
    /*****************************************************************************************/

    if (sprite.hit_counter > 0) // how long the sprite recovers from impact
        sprite.hit_counter--;

    /*****************************************************************************************/
    /* Effect of gravity                                                                  */
    /*****************************************************************************************/

    if (sprite.weight != 0 && (sprite.jump_timer <= 0 || sprite.jump_timer >= 45))
        sprite_b += sprite.weight / 1.25;// + sprite_b/1.5;

    if (slow_down && sprite_b > 0) {
        sprite_b /= 1.3;//1.5;//3
    }

    /*****************************************************************************************/
    /* By default, the sprite is not in water nor hiding                                     */
    /*****************************************************************************************/

    sprite.in_water = false;
    sprite.hiding = false;

    /*****************************************************************************************/
    /* Speed limits                                                                          */
    /*****************************************************************************************/

    if (sprite_b > 4.0)//4
        sprite_b = 4.0;//4

    if (sprite_b < -4.0)
        sprite_b = -4.0;

    if (sprite_a > max_speed)
        sprite_a = max_speed;

    if (sprite_a < -max_speed)
        sprite_a = -max_speed;

    /*****************************************************************************************/
    /* Collisions with blocks                                                                 */
    /*****************************************************************************************/

    int blocks_x_count, blocks_y_count, blocks_count;
    unsigned long p;

    if (sprite.type->collides_with_tiles) {
        blocks_x_count = (int) ((sprite_width) / 32) + 4;
        blocks_y_count = (int) ((sprite_height) / 32) + 4;

        map_left = (int) (sprite_left) / 32;
        map_top = (int) (sprite_top) / 32;

        // READ BLOCKS FROM THE MAP AND EXAMINE THEM

        //memset(blocks,255,sizeof(blocks));
/*
		for (y=0;y<blocks_y_count;y++)
			for (x=0;x<blocks_x_count;x++)
			{
				p = x+y*blocks_x_count;

				if (p<300 && p>=0)
					blocks[p] = Examine_Block(map_left+x-1,map_top+y-1);
			}
*/
        for (y = 0; y < blocks_y_count; y++)
            for (x = 0; x < blocks_x_count; x++) {
                blocks[x + (y * blocks_x_count)] = Examine_Block(map_left + x - 1, map_top + y - 1);
            }
/**/
        /*****************************************************************************************/
        /* Go through the blocks surrounding the sprite                                          */
        /*****************************************************************************************/

        blocks_count = blocks_y_count * blocks_x_count;

        for (y = 0; y < blocks_y_count; y++)
            for (x = 0; x < blocks_x_count; x++) {
                p = x + y * blocks_x_count;
                if (p < 300 && p >= 0)
                    Check_Wall(sprite, blocks[p]);
/*
				Check_Wall_Debug(sprite, blocks[p],
					sprite_x,
					sprite_y,
					sprite_a,
					sprite_b,
					sprite_left,
					sprite_right,
					sprite_top,
					sprite_bottom,
					sprite_width,
					sprite_height,
					map_left,
					map_top,
					right,
					left,
					up,
					down);
*/
            }

    }
    /*****************************************************************************************/
    /* If the sprite is underwater                                                           */
    /*****************************************************************************************/

    if (sprite.in_water) {
        if (!sprite.type->can_swim || sprite.health < 1) {
/*
			if (sprite_b > 0)
				sprite_b /= 2.0;
			sprite_b -= (1.5-sprite.weight)/10;
*/
            sprite_b /= 2.0;
            sprite_a /= 1.05;
            if (sprite.jump_timer > 0 && sprite.jump_timer < 90)
                sprite.jump_timer--;
        }
        if (rand() % 80 == 1)
            New_Particle(PARTICLE_SPARK, sprite_x - 4, sprite_y, 0, -0.5 - rand() % 2, rand() % 30 + 30, 0, 32);
    }
    if (in_water != sprite.in_water) {// the sprite gets into the water or comes out of the water
        Create_SplashEffect((int) sprite_x, (int) sprite_y, 32);
    }

    /*****************************************************************************************/
    /* Sprite's basic weight                                                                 */
    /*****************************************************************************************/

    sprite.weight = sprite.initial_weight;
    sprite.total_weight = sprite.weight;

    if (sprite.health < 1 && sprite.weight == 0)
        sprite.weight = 1;

    /*****************************************************************************************/
    /* Sprite's collision with other sprites                                                    */
    /*****************************************************************************************/

    int destruction;
    double sprite2_upper; // related to crouching
    Block sprite_block;

    PK2Sprite *sprite2;

    for (int sprite_index = 0; sprite_index < MAX_SPRITES; sprite_index++) {
        sprite2 = &sprites[sprite_index];
        if (sprite_index != i && /*!sprite2->is_hidden*/sprite2->is_active) {
            if (sprite2->is_crouching)
                sprite2_upper = sprite2->type->height / 3;//1.5;
            else
                sprite2_upper = 0;
            if (sprite2->type->obstacle && sprite.type->collides_with_tiles) {
                if (sprite_x - sprite_width / 2 + sprite_a <= sprite2->x + sprite2->type->width / 2 &&
                    sprite_x + sprite_width / 2 + sprite_a >= sprite2->x - sprite2->type->width / 2 &&
                    sprite_y - sprite_height / 2 + sprite_b <= sprite2->y + sprite2->type->height / 2 &&
                    sprite_y + sprite_height / 2 + sprite_b >= sprite2->y - sprite2->type->height / 2) {
                    sprite_block.code = 0;
                    sprite_block.bottom_bound = (int) sprite2->y + sprite2->type->height / 2;
                    sprite_block.right_bound = (int) sprite2->x + sprite2->type->width / 2;
                    sprite_block.left_bound = (int) sprite2->x - sprite2->type->width / 2;
                    sprite_block.top_bound = (int) sprite2->y - sprite2->type->height / 2;
                    sprite_block.water = false;
                    //sprite_block.code = BLOCK_ELEVATOR_VERT;
                    Disable_Block_Background(sprite_block);
                    if (!sprite.type->obstacle) {
                        if (!sprite2->type->blocks_down)
                            sprite_block.down = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_up)
                            sprite_block.up = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_right)
                            sprite_block.right = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_left)
                            sprite_block.left = BLOCK_BACKGROUND;
                    }
                    if (sprite2->movement_x > 0)
                        sprite_block.code = BLOCK_ELEVATOR_HORI;
                    if (sprite2->movement_y > 0)
                        sprite_block.code = BLOCK_ELEVATOR_VERT;
                    Check_Wall(sprite, sprite_block, sprite_x, sprite_y, sprite_a, sprite_b, sprite_left, sprite_right,
                               sprite_top, sprite_bottom, sprite_width, sprite_height, map_left, map_top, right, left,
                               up, down);
                }
            }

            if (sprite_x <= sprite2->x + sprite2->type->width / 2 &&
                sprite_x >= sprite2->x - sprite2->type->width / 2 &&
                sprite_y/*top_bound*/ <= sprite2->y + sprite2->type->height / 2 &&
                sprite_y/*bottom_bound*/ >= sprite2->y - sprite2->type->height / 2 + sprite2_upper) {
                // same-type sprites change direction upon collision
                if (sprite.type->index == sprite2->type->index && sprite2->health > 0/* && sprite.player == 0*/) {
                    if (sprite.x < sprite2->x)
                        right = false;
                    if (sprite.x > sprite2->x)
                        left = false;
                    if (sprite.y < sprite2->y)
                        down = false;
                    if (sprite.y > sprite2->y)
                        up = false;
                }

                if (sprite.type->Has_AI(AI_ARROWS_AFFECT)) {
                    if (sprite2->type->Has_AI(AI_ARROW_RIGHT)) {
                        sprite_a = sprite.type->max_speed / 3.5;
                        sprite_b = 0;
                    } else if (sprite2->type->Has_AI(AI_ARROW_LEFT)) {
                        sprite_a = sprite.type->max_speed / -3.5;
                        sprite_b = 0;
                    }
                    if (sprite2->type->Has_AI(AI_ARROW_UP)) {
                        sprite_b = sprite.type->max_speed / -3.5;
                        sprite_a = 0;
                    } else if (sprite2->type->Has_AI(AI_ARROW_DOWN)) {
                        sprite_b = sprite.type->max_speed / 3.5;
                        sprite_a = 0;
                    }
                }

                /* sprites can exchange information about the player's location */
/*
                if (sprite.last_seen_player_x != -1 && sprite2->last_seen_player_x == -1) {
					sprite2->last_seen_player_x = sprite.last_seen_player_x + rand()%30 - rand()%30;
					sprite.last_seen_player_x = -1;
				}
*/
                if (sprite.is_enemy != sprite2->is_enemy && sprite.parent_sprite != sprite_index) {
                    if (sprite2->type->type != TYPE_BACKGROUND && sprite.type->type != TYPE_BACKGROUND &&
                        sprite2->type->type != TYPE_TELEPORTER && sprite2->hit_counter == 0 && sprite.hit_counter == 0 &&
                        sprite2->health > 0 && sprite.health > 0 && sprite2->damage_received < 1) {
                        // Does another sprite drop on top?
                        if (sprite2->movement_y > 2 && sprite2->weight >= 0.5 && sprite2->y < sprite_y && !sprite.type->obstacle && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
                            //sprite.damage_received = (int)sprite2->weight;//1;
                            sprite.damage_received = (int) (sprite2->weight + sprite2->movement_y / 4);
                            sprite.damage_type_received = DAMAGE_FALL;
                            sprite2->jump_timer = 1;
                        }
                        // Is the other sprite harmful
                        if (sprite.type->damage > 0 && sprite2->type->type != TYPE_BONUS) {
                            sprite2->damage_received = sprite.type->damage;
                            sprite2->damage_type_received = sprite.type->damage_type;
                            sprite.attack_timer_1 = sprite.type->attack1_duration;
                            // Projectiles break upon collision
                            if (sprite2->type->type == TYPE_PROJECTILE) {
                                sprite.damage_received = 1;//sprite2->type->damage;
                                sprite.damage_type_received = sprite2->type->damage_type;
                            }
                            if (sprite.type->type == TYPE_PROJECTILE) {
                                sprite.damage_received = 1;//sprite2->type->damage;
                                sprite.damage_type_received = sprite2->type->damage_type;
                            }
                        }
                    }
                }

                // add the weight of the other sprite touching the sprite to the sprite's weight
                if (sprite.weight > 0)
                    sprite.total_weight += sprites[sprite_index].type->weight;
            }
        }
    }

    /*****************************************************************************************/
    /* If the sprite has suffered damage                                                     */
    /*****************************************************************************************/

    // if invisible, only takes damage from fire.
    if (invisibility > 0 && sprite.damage_received != 0 && sprite.damage_type_received != DAMAGE_FIRE && i == player_index) {
        sprite.damage_received = 0;
        sprite.damage_type_received = DAMAGE_NONE;
    }

    if (sprite.damage_received != 0 && sprite.health > 0 && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
        if (sprite.type->protection != sprite.damage_type_received || sprite.type->protection == DAMAGE_NONE) {
            sprite.health -= sprite.damage_received;
            sprite.hit_counter = DAMAGE_TIME;
            if (sprite.damage_type_received == DAMAGE_ELECTRIC)
                sprite.hit_counter *= 6;
            Play_Sound(sprite.type->sounds[SOUND_DAMAGE], 100, (int) sprite.x, (int) sprite.y, sprite.type->sound_frequency, sprite.type->random_freq);
            if (sprite.type->destruction % 100 == DESTRUCTION_FEATHERS) {
                Create_Effects(DESTRUCTION_FEATHERS, (unsigned long) sprite.x, (unsigned long) sprite.y);
            }
            if (sprite.type->type != TYPE_PROJECTILE) {
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, -1, -1, 60, 0.01, 128);
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, 0, -1, 60, 0.01, 128);
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, 1, -1, 60, 0.01, 128);
            }
            if (sprite.Has_AI(AI_CHANGE_SKULLS_IF_HIT))
                Map_Change_SkullBlocks();
            if (sprite.Has_AI(AI_ATTACK_1_IF_HIT)) {
                sprite.attack_timer_1 = sprite.type->attack1_duration;
                sprite.charge_time = 0;
            }
            if (sprite.Has_AI(AI_ATTACK_2_IF_HIT)) {
                sprite.attack_timer_2 = sprite.type->attack2_duration;
                sprite.charge_time = 0;
            }
        }
        sprite.damage_received = 0;
        sprite.damage_type_received = DAMAGE_NONE;

        /*****************************************************************************************/
        /* If the sprite is destroyed                                                            */
        /*****************************************************************************************/

        if (sprite.health < 1) {
            destruction = sprite.type->destruction;
            if (destruction != DESTRUCTION_INDESTRUCTIBLE) {
                if (sprite.type->bonus > -1 && sprite.type->bonus_count > 0)
                    if (sprite.type->always_drop_bonus || rand() % 4 == 1)
                        for (int bi = 0; bi < sprite.type->bonus_count; bi++)
                            Add_BonusSprite(prototypes[sprite.type->bonus], 0, sprite_x - 11 + (10 - rand() % 20), sprite_bottom - 16 - (10 + rand() % 20), i);
                if (sprite.Has_AI(AI_CHANGE_SKULLS_IF_KNOCKED_OUT) && !sprite.Has_AI(AI_CHANGE_SKULLS_IF_HIT))
                    Map_Change_SkullBlocks();
                if (destruction >= DESTRUCTION_ANIMATION)
                    destruction -= DESTRUCTION_ANIMATION;
                else
                    sprite.is_hidden = true;

                Create_Effects(destruction, (unsigned long) sprite.x, (unsigned long) sprite.y);
                Play_Sound(sprite.type->sounds[SOUND_DESTRUCTION], 100, (int) sprite.x, (int) sprite.y, sprite.type->sound_frequency, sprite.type->random_freq);

                if (sprite.Has_AI(AI_NEW_IF_DESTROYED)) {
                    Add_Sprite(*sprite.type, 0, sprite.start_x - sprite.type->width, sprite.start_y - sprite.type->height, i);
                }
                if (sprite.type->type == TYPE_GAME_CHARACTER && sprite.type->points != 0) {
                    char number[10];
                    itoa(sprite.type->points, number, 10);
                    New_Fade_Text(font2, number, (int) sprites[i].x - 8, (int) sprites[i].y - 8, 80);
                    points_add += sprite.type->points;
                }
            } else
                sprite.health = 1;
        }
    }

    if (sprite.hit_counter == 0)
        sprite.damage_type_received = DAMAGE_NONE;

    /*****************************************************************************************/
    /* Checks                                                                                */
    /*****************************************************************************************/

    if (!right) {
        if (sprite_a > 0)
            sprite_a = 0;
    }
    if (!left) {
        if (sprite_a < 0)
            sprite_a = 0;
    }
    if (!up) {
        if (sprite_b < 0)
            sprite_b = 0;

        if (!jump_maximum)
            sprite.jump_timer = 95;//sprite.type->max_jump * 2;
    }
    if (!down) {
        if (sprite_b >= 0) {
            if (sprite.jump_timer > 0) {
                if (sprite.jump_timer >= 90 + 10) {
                    Play_Sound(explosion_sound, 30, (int) sprite_x, (int) sprite_y, int(25050 - sprite.weight * 3000), true);
                    //New_Particle(	PARTICLE_DUST_CLOUD,sprite_x+rand()%5-rand()%5-10,sprite_bottom+rand()%3-rand()%3,0,-0.2,rand()%50+20,0,0);
                    if (rand() % 7 == 1) {
                        New_Particle(PARTICLE_SMOKE_CLOUD, sprite_x + rand() % 5 - rand() % 5 - 10, sprite_bottom + rand() % 3 - rand() % 3, 0.3, -0.1, 450, 0, 0);
                        New_Particle(PARTICLE_SMOKE_CLOUD, sprite_x + rand() % 5 - rand() % 5 - 10, sprite_bottom + rand() % 3 - rand() % 3, -0.3, -0.1, 450, 0, 0);
                    }
                    if (sprite.weight > 1)
                        earthquake = 34 + int(sprite.weight * 20);
                }
                sprite.jump_timer = 0;
            }
            sprite_b = 0;
        }
    }

    /*****************************************************************************************/
    /* Assignment of auxiliary variables to the sprite's variables                           */
    /*****************************************************************************************/

    if (sprite_b > 4.0)
        sprite_b = 4.0;
    if (sprite_b < -4.0)
        sprite_b = -4.0;
    if (sprite_a > max_speed)
        sprite_a = max_speed;
    if (sprite_a < -max_speed)
        sprite_a = -max_speed;
    if (sprite.health > sprite.type->health)
        sprite.health = sprite.type->health;
    if (sprite.hit_counter == 0 || sprite.player == 1) {
        sprite_x += sprite_a;
        sprite_y += sprite_b;
    }
    if (i == player_index || sprite.health < 1) {
        double friction = 1.04;
        if (map->climate == WEATHER_RAIN || map->climate == WEATHER_RAIN_FOREST)
            friction = 1.03;
        if (map->climate == WEATHER_SNOW)
            friction = 1.01;
        if (!down)
            sprite_a /= friction;
        else
            sprite_a /= 1.03;//1.02//1.05
        sprite_b /= 1.25;
    }

    sprite.x = sprite_x;
    sprite.y = sprite_y;
    sprite.movement_x = sprite_a;
    sprite.movement_y = sprite_b;

    sprite.can_move_right= right;
    sprite.can_move_left = left;
    sprite.can_move_down = down;
    sprite.can_move_up = up;
/*
	sprite.weight = sprite.type->weight;

	if (sprite.health < 1 && sprite.weight == 0)
		sprite.weight = 1;
*/
    if (sprite.jump_timer < 0)
        sprite.can_move_down = false;

    //sprite.is_crouching   = false;

    /*****************************************************************************************/
    /* Artificial Intelligence                                                               */
    /*****************************************************************************************/

    if (sprite.player == 0) {
        for (int ai = 0; ai < SPRITE_MAX_AI; ai++)
            switch (sprite.type->AI[ai]) {
                case AI_NONE:
                    ai = SPRITE_MAX_AI; // stopping
                    break;
                case AI_CHICKEN:
                    sprite.AI_Chicken();
                    break;
                case AI_SMALL_CHICKEN:
                    sprite.AI_Chicken();
                    break;
                case AI_FROG1:
                    sprite.AI_Frog1();
                    break;
                case AI_FROG2:
                    sprite.AI_Frog2();
                    break;
                case AI_BONUS:
                    sprite.AI_Bonus();
                    break;
                case AI_EGG:
                    sprite.AI_Egg();
                    break;
                case AI_PROJECTILE:
                    sprite.AI_Projectile();
                    break;
                case AI_JUMPING:
                    sprite.AI_Jumper();
                    break;
                case AI_BASIC:
                    sprite.AI_Default();
                    break;
                case AI_NONSTOP:
                    sprite.AI_NonStop();
                    break;
                case AI_TURN_ON_COLLISION_HORIZONTAL:
                    sprite.AI_Turns_From_Horizontal_Obstacle();
                    break;
                case AI_TURN_FROM_OBSTACLE_VERT:
                    sprite.AI_Turns_From_Vertical_Obstacle();
                    break;
                case AI_AVOID_PIT:
                    sprite.AI_Avoids_Pit();
                    break;
                case AI_RANDOM_DIRECTION_CHANGE_HORIZONTAL:
                    sprite.AI_Random_Horizontal_Direction_Change();
                    break;
                case AI_RANDOM_TURNING:
                    sprite.AI_Random_Turn();
                    break;
                case AI_RANDOM_JUMP:
                    sprite.AI_Random_Jump();
                    break;
                case AI_FOLLOW_PLAYER:
                    if (invisibility == 0)
                        sprite.AI_Follows_Player(sprites[player_index]);
                    break;
                case AI_FOLLOW_PLAYER_IF_VISIBLE:
                    if (invisibility == 0)
                        sprite.AI_Follows_Player_If_Seen(sprites[player_index]);
                    break;
                case AI_FOLLOWS_PLAYER_VERT_HORI:
                    if (invisibility == 0)
                        sprite.AI_Follows_Player_Vertical_Horizontal(sprites[player_index]);
                    break;
                case AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI:
                    if (invisibility == 0)
                        sprite.AI_Follows_Player_If_Seen_Vertical_Horizontal(sprites[player_index]);
                    break;
                case AI_FLEES_PLAYER_IF_SEEN:
                    if (invisibility == 0)
                        sprite.AI_Flees_From_Player_If_Seen(sprites[player_index]);
                    break;
                case AI_BOMB:
                    sprite.AI_Bomb();
                    break;
                case AI_ATTACK_1_IF_HIT:
                    sprite.AI_Attack_1_If_Hit();
                    break;
                case AI_ATTACK_2_IF_HIT:
                    sprite.AI_Attack_2_If_Hit();
                    break;
                case AI_ATTACK_1_NONSTOP:
                    sprite.AI_Attack_1_Nonstop();
                    break;
                case AI_ATTACK_2_NONSTOP:
                    sprite.AI_Attack_2_Nonstop();
                    break;
                case AI_ATTACK_1_IF_PLAYER_IN_FRONT:
                    if (invisibility == 0)
                        sprite.AI_Attack_1_If_Player_In_Front(sprites[player_index]);
                    break;
                case AI_ATTACK_2_IF_PLAYER_IN_FRONT:
                    if (invisibility == 0)
                        sprite.AI_Attack_2_If_Player_In_Front(sprites[player_index]);
                    break;
                case AI_ATTACK_1_IF_PLAYER_BELOW:
                    if (invisibility == 0)
                        sprite.AI_Attack_1_If_Player_Below(sprites[player_index]);
                    break;
                case AI_JUMP_IF_PLAYER_ABOVE:
                    if (invisibility == 0)
                        sprite.AI_Jump_If_Player_Above(sprites[player_index]);
                    break;
                case AI_DAMAGE_FROM_WATER:
                    sprite.AI_Takes_Damage_From_Water();
                    break;
                case AI_KILL_ALL:
                    sprite.AI_Kill_All();
                    break;
                case AI_FRICTION_AFFECTS:
                    sprite.AI_Friction_Affects();
                    break;
                case AI_HIDES:
                    sprite.AI_Hides();
                    break;
                case AI_RETURN_TO_START_X:
                    sprite.AI_Returns_To_Start_X();
                    break;
                case AI_RETURN_TO_START_Y:
                    sprite.AI_Returns_To_Start_Y();
                    break;
                case AI_MOVE_X_COS:
                    sprite.AI_Moves_X(cos_table[degree % 360]);
                    break;
                case AI_MOVE_Y_COS:
                    sprite.AI_Moves_Y(cos_table[degree % 360]);
                    break;
                case AI_MOVE_X_SIN:
                    sprite.AI_Moves_X(sin_table[degree % 360]);
                    break;
                case AI_MOVE_Y_SIN:
                    sprite.AI_Moves_Y(sin_table[degree % 360]);
                    break;
                case AI_MOVE_X_COS_FAST:
                    sprite.AI_Moves_X(cos_table[(degree * 2) % 360]);
                    break;
                case AI_MOVE_Y_SIN_FAST:
                    sprite.AI_Moves_Y(sin_table[(degree * 2) % 360]);
                    break;
                case AI_MOVES_X_COS_SLOW:
                    sprite.AI_Moves_X(cos_table[(degree / 2) % 360]);
                    break;
                case AI_MOVES_Y_SIN_SLOW:
                    sprite.AI_Moves_Y(sin_table[(degree / 2) % 360]);
                    break;
                case AI_MOVES_Y_SIN_FREE:
                    sprite.AI_Moves_Y(sin_table[(sprite.timer / 2) % 360]);
                    break;
                case AI_CHANGE_IF_ENERGY_LESS_THAN_2:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_If_Energy_Less_Than_2(prototypes[sprite.type->transformation]);
                    break;
                case AI_CHANGE_IF_ENERGY_MORE_THAN_1:
                    if (sprite.type->transformation > -1)
                        if (sprite.AI_Change_If_Energy_More_Than_1(prototypes[sprite.type->transformation]) == 1)
                            Create_Effects(DESTRUCTION_SMOKE_GRAY, (unsigned long) sprite.x, (unsigned long) sprite.y);
                    break;
                case AI_CHANGE_TIMER:
                    if (sprite.type->transformation > -1) {
                        sprite.AI_Change_Timer(prototypes[sprite.type->transformation]);
                    }
                    break;
                case AI_CHANGE_IF_HIT:
                    if (sprite.type->transformation > -1) {
                        sprite.AI_Change_If_Hit(prototypes[sprite.type->transformation]);
                    }
                    break;
                case AI_TELEPORT:
                    if (sprite.AI_Teleport(i, sprites, MAX_SPRITES, sprites[player_index]) == 1) {
                        camera_x = (int)sprites[player_index].x;
                        camera_y = (int)sprites[player_index].y;
                        dcamera_x = camera_x - MAP_DRAWING_WIDTH / 2;
                        dcamera_y = camera_y - MAP_DRAWING_HEIGHT / 2;
                        PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
                        if (sprite.type->sounds[SOUND_ATTACK2] != -1)
                            Play_Sound_Menu(sprite.type->sounds[SOUND_ATTACK2], 100);
                        //Play_Sound(, 100, camera_x, camera_y, SOUNDS_SAMPLERATE, false);
                    }
                    break;
                case AI_CLIMBER:
                    sprite.AI_Climber();
                    break;
                case AI_CLIMBER2:
                    sprite.AI_Climber2();
                    break;
                case AI_DESTROY_IF_PARENT_DESTROYED:
                    sprite.AI_Dies_If_Parent_Dies(sprites);
                    break;

                case AI_FALLS_FROM_STORY:
                    sprite.AI_Falls_From_Story(earthquake + story_switch);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 1, 0);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 1, 0);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 1, 0);
                    break;
                case AI_FALLS_IF_SWITCH1_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch1);
                    break;
                case AI_FALLS_IF_SWITCH2_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch2);
                    break;
                case AI_FALLS_IF_SWITCH3_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch3);
                    break;
                case AI_RANDOM_MOVEMENT_VERT_HORI:
                    sprite.AI_Random_Vertical_Horizontal_Move();
                    break;
                case AI_TURN_IF_HIT:
                    sprite.AI_Turns_If_Hit();
                    break;
                case AI_EVIL_ONE:
                    if (sprite.health < 1) music_volume = 0;
                    break;

                case AI_INFO1:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info01));
                    break;
                case AI_INFO2:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info02));
                    break;
                case AI_INFO3:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info03));
                    break;
                case AI_INFO4:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info04));
                    break;
                case AI_INFO5:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info05));
                    break;
                case AI_INFO6:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info06));
                    break;
                case AI_INFO7:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info07));
                    break;
                case AI_INFO8:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info08));
                    break;
                case AI_INFO9:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info09));
                    break;
                case AI_INFO10:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info10));
                    break;
                case AI_INFO11:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info11));
                    break;
                case AI_INFO12:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info12));
                    break;
                case AI_INFO13:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info13));
                    break;
                case AI_INFO14:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info14));
                    break;
                case AI_INFO15:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info15));
                    break;
                case AI_INFO16:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info16));
                    break;
                case AI_INFO17:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info17));
                    break;
                case AI_INFO18:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info18));
                    break;
                case AI_INFO19:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info19));
                    break;

                default:
                    break;
            }
    }

    //if (echo == 1 && sprite.type->type == TYPE_PROJECTILE && sprite.type->damage_type == DAMAGE_NOISE && sprite.type->sounds[SOUND_ATTACK1] > -1)
    //	Play_Sound(sprite.type->sounds[SOUND_ATTACK1],20, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

    /*****************************************************************************************/
    /* Don't let the sprite out of the game area                                             */
    /*****************************************************************************************/

    if (sprite.x < 0)
        sprite.x = 0;
    if (sprite.y < -sprite_height)
        sprite.y = -sprite_height;
    if (sprite.x > MAP_WIDTH * 32)
        sprite.x = MAP_WIDTH * 32;
    // If the sprite falls below the bottom of the map
    if (sprite.y > MAP_HEIGHT * 32 + sprite_height) {
        sprite.y = MAP_HEIGHT * 32 + sprite_height;
        sprite.health = 0;
        sprite.is_hidden = true;
        if (sprite.total_weight >= 1)
            earthquake = 50;
    }
    if (sprite.movement_x > max_speed)
        sprite.movement_x = max_speed;
    if (sprite.movement_x < -max_speed)
        sprite.movement_x = -max_speed;

    /*****************************************************************************************/
    /* Attacks 1 and 2                                                                       */
    /*****************************************************************************************/

    // sprite is ready for attack and not crouching
    if (sprite.charge_time == 0 && !sprite.is_crouching) {
        // attack time is "full," which means starting the attack
        if (sprite.attack_timer_1 == sprite.type->attack1_duration) {
            // set recovery time after which the sprite can attack again
            sprite.charge_time = sprite.type->reload_time;
            // if the sprite has not defined its own reload time ...
            if (sprite.projectile_1 > -1 && sprite.type->reload_time == 0)
                // ... and the projectile has, take the reload_time from the projectile
                if (prototypes[sprite.projectile_1].fire_rate > 0)
                    sprite.charge_time = prototypes[sprite.projectile_1].fire_rate;

            // play the attack sound
            Play_Sound(sprite.type->sounds[SOUND_ATTACK1], 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

            if (sprite.projectile_1 > -1) {
                Add_ProjectileSprite(prototypes[sprite.projectile_1], 0, sprite_x, sprite_y, i);
                //		if (prototypes[sprite.projectile_1].sounds[SOUND_ATTACK1] > -1)
                //			Play_Sound(prototypes[sprite.projectile_1].sounds[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            }
        }

        // Same as attack 1
        if (sprite.attack_timer_2 == sprite.type->attack2_duration) {
            sprite.charge_time = sprite.type->reload_time;
            if (sprite.projectile_2 > -1 && sprite.type->reload_time == 0)
                if (prototypes[sprite.projectile_2].fire_rate > 0)
                    sprite.charge_time = prototypes[sprite.projectile_2].fire_rate;

            Play_Sound(sprite.type->sounds[SOUND_ATTACK2], 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

            if (sprite.projectile_2 > -1) {
                Add_ProjectileSprite(prototypes[sprite.projectile_2], 0, sprite_x, sprite_y, i);
                //		if (prototypes[sprite.projectile_2].sounds[SOUND_ATTACK1] > -1)
                //			Play_Sound(prototypes[sprite.projectile_2].sounds[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            }
        }
    }

    // Random sounds
    if (sprite.type->sounds[SOUND_RANDOM] != -1 && rand() % 200 == 1 && sprite.health > 0)
        Play_Sound(sprite.type->sounds[SOUND_RANDOM], 80, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

    // DEVELOPMENT PHASE AUXILIARY FUNCTIONS

    unsigned char color;
    unsigned long block;

    if (PisteInput_KeyDown(DIK_B) && cheats) {
        if (i == player_index) {
            char number[50];
            itoa(blocks[1].top_bound, number, 10);
            //_gcvt(sprite_a,7,number);
            PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 310, 50);
        }

        if (sprite.type->collides_with_tiles)
            for (x = 0; x < blocks_x_count; x++) {
                for (y = 0; y < blocks_y_count; y++) {
                    color = 50 - x * 2 - y * 2;
                    block = x + y * blocks_x_count;
                    if (block > 299)
                        block = 299;
                    if (block < 0)
                        block = 0;
                    if (!blocks[block].background)
                        color += 32;
                    PisteDraw_Buffer_Clear(PD_BACKBUFFER, blocks[block].left_bound - camera_x, blocks[block].top_bound - camera_y, blocks[block].right_bound - camera_x, blocks[block].bottom_bound - camera_y, color);
                }
            }
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, (int) (sprite_left - camera_x), (int) (sprite_top - camera_y), (int) (sprite_right - camera_x), (int) (sprite_bottom - camera_y), 30);
    }
    return 0;
}

int Move_Sprite2(int i) {
    if (i >= MAX_SPRITES || i < 0)
        return -1;
    if (!sprites[i].type)
        return -1;
/*
	double	sprite_x,
			sprite_y,
			sprite_a,
			sprite_b,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width,
			sprite_height;

	int		map_left,
			map_top,

			x = 0,
			y = 0;

	bool	right,
			left,
			up,
			down,

			in_water;

	unsigned char   max_speed;
*/
    sprite_x = 0;
    sprite_y = 0;
    sprite_a = 0;
    sprite_b = 0;

    sprite_left = 0;
    sprite_right = 0;
    sprite_top = 0;
    sprite_bottom = 0;

    sprite_width = 0;
    sprite_height = 0;

    map_left = 0;
    map_top = 0;

    x = 0;
    y = 0;

    right = false;
    left = false;
    up = false;
    down = false;

    in_water = false;
    max_speed = 0;

    // Variables
/*
	PK2Sprite &sprite = sprites[i];

	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.movement_x,
			sprite_b = sprite.movement_y,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width  = sprite.type->width,
			sprite_height = sprite.type->height;

	int		map_left,
			map_top,

			blocks_x_count,
			blocks_y_count,

			x = 0,
			y = 0;

	bool	right	= true,
			left	= true,
			up		= true,
			down		= true;

	bool	in_water = sprite.in_water;

	unsigned char   max_speed = sprite.type->max_speed;
*/
    if (sprites[i].type == NULL)
        return 1;

    PK2Sprite &sprite = sprites[i];

    sprite_x = sprite.x;
    sprite_y = sprite.y;
    sprite_a = sprite.movement_x;
    sprite_b = sprite.movement_y;

    sprite_width = sprite.type->width;
    sprite_height = sprite.type->height;

    x = 0;
    y = 0;
    right = true, left = true, up = true, down = true;
    in_water = sprite.in_water;
    sprite.is_crouching = false;//sprite.hiding; // by default, the sprite is not crouching

    sprite.edge_left = false;
    sprite.edge_right = false;

    // Move the actual variables to auxiliary variables.
    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;
    sprite_top = sprite_y - sprite_height / 2;
    sprite_bottom = sprite_y + sprite_height / 2;

    max_speed = (unsigned char) sprite.type->max_speed;

    /* Add speed to stunned sprites */
    if (sprite.health < 1/* && sprite.type->max_speed == 0*/)
        max_speed = 3;

    // Calculate the remaining sprite attack
    if (sprite.attack_timer_1 > 0)
        sprite.attack_timer_1--;
    if (sprite.attack_timer_2 > 0)
        sprite.attack_timer_2--;
    if (sprite.charge_time > 0)    // duration between two shots (laying)
        sprite.charge_time--;

    /*****************************************************************************************/
    /* Player sprite and its control                                                      */
    /*****************************************************************************************/

    bool additional_speed = true;
    bool slow_down = false;

    if (sprite.player != 0 && sprite.health > 0) {
        /* WALKING */
        if (PisteInput_Read_Controller(control_run))
            additional_speed = false;

        /* ATTACK 1 */
        if (PisteInput_Read_Controller(control_attack1) && sprite.charge_time == 0 && sprite.projectile_1 != -1)
            sprite.attack_timer_1 = sprite.type->attack1_duration;

        /* ATTACK 2 */
        if (PisteInput_Read_Controller(control_attack2) && sprite.charge_time == 0 && sprite.projectile_2 != -1)
            sprite.attack_timer_2 = sprite.type->attack2_duration;

        /* CROUCHING */
        sprite.is_crouching = false;

        if (PisteInput_Read_Controller(control_down) && !sprite.can_move_down) {
            sprite.is_crouching = true;
            sprite_top += sprite_height / 1.5;
        }

        double a_addition = 0;

        /* MOVING RIGHT */
        if (PisteInput_Read_Controller(control_right)) {
            a_addition = 0.04;//0.08;

            if (additional_speed) {
                if (rand() % 20 == 1 && sprite.animation_index == ANIMATION_WALK) //rand()%30
                    New_Particle(PARTICLE_DUST_CLOUD, sprite_x - 8, sprite_bottom - 8, 0.25, -0.25, 40, 0, 0);
                a_addition += 0.09;//0.05
            }

            if (sprite.can_move_down)
                a_addition /= 1.5;//2.0

            sprite.flip_horizontal = false;
        }

        /* MOVING LEFT */
        if (PisteInput_Read_Controller(control_left)) {
            a_addition = -0.04;

            if (additional_speed) {
                if (rand() % 20 == 1 && sprite.animation_index == ANIMATION_WALK) {
                    New_Particle(PARTICLE_DUST_CLOUD, sprite_x - 8, sprite_bottom - 8, -0.25, -0.25, 40, 0, 0);
                }
                a_addition -= 0.09;
            }

            if (sprite.can_move_down)    // when the sprite touches the ground, friction affects
                a_addition /= 1.5;//2.0

            sprite.flip_horizontal = true;
        }

        if (sprite.is_crouching)    // the speed slows down when crouching
            a_addition /= 10;

        sprite_a += a_addition;

        /* JUMPING */
        if (sprite.type->weight > 0) {
            if (PisteInput_Read_Controller(control_jump)) {
                if (!sprite.is_crouching) {
                    if (sprite.jump_timer == 0)
                        Play_Sound(jump_sound, 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency,
                                   sprite.type->random_freq);
                    if (sprite.jump_timer <= 0)
                        sprite.jump_timer = 1;
                }
            } else {
                if (sprite.jump_timer > 0 && sprite.jump_timer < sprite.type->max_jump / 2) // /2
                    sprite.jump_timer = sprite.type->max_jump / 2;// /2
            }

            /* falling quietly downward */
            if (PisteInput_Read_Controller(control_jump) && sprite.jump_timer > sprite.type->max_jump * 2 + 20 &&
                sprite.type->can_float_down)
                slow_down = true;
            if (slow_down && sprite_b > 0) {
                sprite_b /= 4;//3
            }
        }
            /* MOVING UP AND DOWN */
        else { // if the player-sprite's weight is 0 - for example, as a bird
            if (PisteInput_Read_Controller(control_jump)) {
                sprite_b -= 0.15;
            }
            if (PisteInput_Read_Controller(control_down)) {
                sprite_b += 0.15;
            }
            sprite.jump_timer = 0;
        }

        /* AI that also applies to the player */
        for (int ai = 0; ai < SPRITE_MAX_AI; ai++)
            switch (sprite.type->AI[ai]) {
                case AI_CHANGE_IF_ENERGY_LESS_THAN_2:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_If_Energy_Less_Than_2(prototypes[sprite.type->transformation]);
                    break;
                case AI_CHANGE_IF_ENERGY_MORE_THAN_1:
                    if (sprite.type->transformation > -1) {
                        if (sprite.AI_Change_If_Energy_More_Than_1(prototypes[sprite.type->transformation]) == 1)
                            Create_Effects(DESTRUCTION_SMOKE_GRAY, (unsigned long) sprite.x, (unsigned long) sprite.y);
                    }
                    break;
                case AI_CHANGE_TIMER:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_Timer(prototypes[sprite.type->transformation]);
                    break;
                case AI_DAMAGE_FROM_WATER:
                    sprite.AI_Takes_Damage_From_Water();
                    break;
                default:
                    break;
            }

        /* It won't work if the player is something other than a game character */
        if (sprite.type->type != TYPE_GAME_CHARACTER)
            sprite.health = 0;
    }

    /*****************************************************************************************/
    /* Jump                                                                                  */
    /*****************************************************************************************/
/*
    // old jump
	bool jump_maximum = sprite.jump_timer >= sprite.type->max_jump*2;

	// If we have jumped / in the air:
	if (sprite.jump_timer > 0)
	{
		if (!jump_maximum)
		// sprite_b = (sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.0;//-4
		   sprite_b = (sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.5;

		if (sprite.jump_timer < 100)
			sprite.jump_timer ++;
	}

	if (sprite.jump_timer < 0)
		sprite.jump_timer++;

	if (sprite_b > 0 && !jump_maximum)
		sprite.jump_timer = sprite.type->max_jump*2;
*/
    bool jump_maximum = sprite.jump_timer >= 45;

    // If we have jumped / in the air:
    if (sprite.jump_timer > 0) {
        if (!jump_maximum)
            //sprite_b = (sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.0;//-4
            sprite_b -= sin_table[sprite.jump_timer] / 3;//(sprite.type->max_jump/2 - sprite.jump_timer/2)/-2.5;

        if (sprite.jump_timer < 90)
            sprite.jump_timer++;
    }

    if (sprite.jump_timer < 0)
        sprite.jump_timer++;
    if (sprite_b > 0 && !jump_maximum)
        sprite.jump_timer = 45;//sprite.type->max_jump*2;

    /*****************************************************************************************/
    /* Recovering from impact                                                                    */
    /*****************************************************************************************/

    if (sprite.hit_counter > 0)    // how long the sprite recovers from impact
        sprite.hit_counter--;

    /*****************************************************************************************/
    /* Effect of gravity                                                                     */
    /*****************************************************************************************/

    if (sprite.weight != 0)
        sprite_b += sprite.weight + sprite_b / 1.25;

    /*****************************************************************************************/
    /* By default, the sprite is not in water nor hiding                                     */
    /*****************************************************************************************/

    sprite.in_water = false;
    sprite.hiding = false;

    /*****************************************************************************************/
    /* Speed limits                                                                          */
    /*****************************************************************************************/

    if (sprite_b > 4.0)//4
        sprite_b = 4.0;//4
    if (sprite_b < -4.0)
        sprite_b = -4.0;
    if (sprite_a > max_speed)
        sprite_a = max_speed;
    if (sprite_a < -max_speed)
        sprite_a = -max_speed;

    /*****************************************************************************************/
    /* Collisions with blocks                                                                */
    /*****************************************************************************************/

    int blocks_x_count, blocks_y_count, blocks_count;
    unsigned long p;

    if (sprite.type->collides_with_tiles) {
        blocks_x_count = (int) ((sprite_width) / 32) + 4;
        blocks_y_count = (int) ((sprite_height) / 32) + 4;

        map_left = (int) (sprite_left) / 32;
        map_top = (int) (sprite_top) / 32;

        // READ BLOCKS FROM THE MAP AND EXAMINE THEM
        //memset(blocks,255,sizeof(blocks));
/*
		for (y=0;y<blocks_y_count;y++)
			for (x=0;x<blocks_x_count;x++)
			{
				p = x+y*blocks_x_count;

				if (p<300 && p>=0)
					blocks[p] = Examine_Block(map_left+x-1,map_top+y-1);
			}
*/
        for (int py = 0; py < blocks_y_count; py++)
            for (int px = 0; px < blocks_x_count; px++) {
                blocks[px + (py * blocks_x_count)] = Examine_Block(map_left + px - 1, map_top + py - 1);
            }
/**/
        /*****************************************************************************************/
        /* Go through the blocks surrounding the sprite                                          */
        /*****************************************************************************************/

        blocks_count = blocks_y_count * blocks_x_count;
        for (y = 0; y < blocks_y_count; y++)
            for (x = 0; x < blocks_x_count; x++) {
                p = x + y * blocks_x_count;

/* debugging...
				if (i==player_index) {
					char number[20];
					itoa(blocks[p].code,number,10);
					PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10+x*50,60+y*10);
				}
*/

                if (p < 300 && p >= 0)
                    Check_Wall(sprite, blocks[p]);
/*
				Check_Wall_Debug(sprite, blocks[p],
					sprite_x,
					sprite_y,
					sprite_a,
					sprite_b,
					sprite_left,
					sprite_right,
					sprite_top,
					sprite_bottom,
					sprite_width,
					sprite_height,
					map_left,
					map_top,
					right,
					left,
					up,
					down);
*/
            }
    }
    /*****************************************************************************************/
    /* If the sprite is underwater                                                           */
    /*****************************************************************************************/

    if (sprite.in_water) {
        if (!sprite.type->can_swim || sprite.health < 1) {
            if (sprite_b > 0)
                sprite_b /= 2.0;

            sprite_b -= (1.5 - sprite.weight) / 10;
            sprite_a /= 1.05;
        }
        if (rand() % 80 == 1)
            New_Particle(PARTICLE_SPARK, sprite_x - 4, sprite_y, 0, -0.5 - rand() % 2, rand() % 30 + 30, 0, 32);
    }
    if (in_water != sprite.in_water) {// the sprite gets into the water or comes out of the water
        Create_SplashEffect((int) sprite_x, (int) sprite_y, 32);
    }

    /*****************************************************************************************/
    /* Sprite's basic weight                                                                    */
    /*****************************************************************************************/

    sprite.weight = sprite.initial_weight;
    sprite.total_weight = sprite.weight;

    if (sprite.health < 1 && sprite.weight == 0)
        sprite.weight = 1;

    /*****************************************************************************************/
    /* Sprite's collision with other sprites                                                    */
    /*****************************************************************************************/

    int destruction;
    double sprite2_upper; // related to crouching
    Block sprite_block;

    PK2Sprite *sprite2;

    for (int sprite_index = 0; sprite_index < MAX_SPRITES; sprite_index++) {
        sprite2 = &sprites[sprite_index];
        if (sprite_index != i && !sprite2->is_hidden) {
            if (sprite2->is_crouching)
                sprite2_upper = sprite2->type->height / 3;//1.5;
            else
                sprite2_upper = 0;
            if (sprite2->type->obstacle && sprite.type->collides_with_tiles) {
                if (sprite_x - sprite_width / 2 + sprite_a <= sprite2->x + sprite2->type->width / 2
                    && sprite_x + sprite_width / 2 + sprite_a >= sprite2->x - sprite2->type->width / 2
                    && sprite_y - sprite_height / 2 + sprite_b <= sprite2->y + sprite2->type->height / 2
                    && sprite_y + sprite_height / 2 + sprite_b >= sprite2->y - sprite2->type->height / 2) {
                    sprite_block.code = 0;
                    sprite_block.bottom_bound = (int) sprite2->y + sprite2->type->height / 2;
                    sprite_block.right_bound = (int) sprite2->x + sprite2->type->width / 2;
                    sprite_block.left_bound = (int) sprite2->x - sprite2->type->width / 2;
                    sprite_block.top_bound = (int) sprite2->y - sprite2->type->height / 2;
                    sprite_block.water = false;
                    //sprite_block.code = BLOCK_ELEVATOR_VERT;
                    Disable_Block_Background(sprite_block);

                    if (!sprite.type->obstacle) {
                        if (!sprite2->type->blocks_down)
                            sprite_block.down = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_up)
                            sprite_block.up = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_right)
                            sprite_block.right = BLOCK_BACKGROUND;
                        if (!sprite2->type->blocks_left)
                            sprite_block.left = BLOCK_BACKGROUND;
                    }
                    if (sprite2->movement_x > 0)
                        sprite_block.code = BLOCK_ELEVATOR_HORI;
                    if (sprite2->movement_y > 0)
                        sprite_block.code = BLOCK_ELEVATOR_VERT;
                    Check_Wall(sprite, sprite_block, sprite_x, sprite_y, sprite_a, sprite_b, sprite_left, sprite_right, sprite_top, sprite_bottom, sprite_width, sprite_height, map_left, map_top, right, left, up, down);
                }
            }
            if (sprite_x <= sprite2->x + sprite2->type->width / 2 && sprite_x >= sprite2->x - sprite2->type->width / 2 && sprite_y/*top_bound*/ <= sprite2->y + sprite2->type->height / 2 && sprite_y/*bottom_bound*/ >= sprite2->y - sprite2->type->height / 2 + sprite2_upper) {
                // same-type sprites change direction upon collision
                if (sprite.type->index == sprite2->type->index && sprite2->health > 0 && sprite.player == 0) {
                    if (sprite.x < sprite2->x)
                        right = false;
                    if (sprite.x > sprite2->x)
                        left = false;
                    if (sprite.y < sprite2->y)
                        down = false;
                    if (sprite.y > sprite2->y)
                        up = false;
                }
                /* sprites can exchange information about the player's location */
/*
                if (sprite.last_seen_player_x != -1 && sprite2->last_seen_player_x == -1) {
					sprite2->last_seen_player_x = sprite.last_seen_player_x + rand()%30 - rand()%30;
					sprite.last_seen_player_x = -1;
				}
*/
                if (sprite.is_enemy != sprite2->is_enemy && sprite.parent_sprite != sprite_index) {
                    if (sprite2->type->type != TYPE_BACKGROUND && sprite.type->type != TYPE_BACKGROUND && sprite2->type->type != TYPE_TELEPORTER && sprite2->hit_counter == 0 && sprite.hit_counter == 0 && sprite2->health > 0 && sprite.health > 0 && sprite2->damage_received < 1) {
                        // Does another sprite drop on top?
                        if (sprite2->movement_y > 2 && sprite2->weight >= 0.5 && sprite2->y < sprite_y && !sprite.type->obstacle && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
                            //sprite.damage_received = (int)sprite2->weight;//1;
                            sprite.damage_received = (int) (sprite2->weight + sprite2->movement_y / 4);
                            sprite.damage_type_received = DAMAGE_FALL;
                            sprite2->jump_timer = 1;
                        }
                        // Is the other sprite harmful
                        if (sprite.type->damage > 0 && sprite2->type->type != TYPE_BONUS) {
                            sprite2->damage_received = sprite.type->damage;
                            sprite2->damage_type_received = sprite.type->damage_type;
                            sprite.attack_timer_1 = sprite.type->attack1_duration;
                            // Projectiles break upon collision
                            if (sprite2->type->type == TYPE_PROJECTILE) {
                                sprite.damage_received = 1;//sprite2->type->damage;
                                sprite.damage_type_received = sprite2->type->damage_type;
                            }
                            if (sprite.type->type == TYPE_PROJECTILE) {
                                sprite.damage_received = 1;//sprite2->type->damage;
                                sprite.damage_type_received = sprite2->type->damage_type;
                            }
                        }
                    }
                }
                // add the weight of the other sprite touching the sprite to the sprite's weight
                if (sprite.weight > 0)
                    sprite.total_weight += sprites[sprite_index].type->weight;
            }
        }
    }

    /*****************************************************************************************/
    /* If the sprite has suffered damage                                                       */
    /*****************************************************************************************/

    if (sprite.damage_received != 0 && sprite.health > 0 && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE) {
        if (sprite.type->protection != sprite.damage_type_received || sprite.type->protection == DAMAGE_NONE) {
            sprite.health -= sprite.damage_received;
            sprite.hit_counter = DAMAGE_TIME;
            if (sprite.damage_type_received == DAMAGE_ELECTRIC)
                sprite.hit_counter *= 6;
            Play_Sound(sprite.type->sounds[SOUND_DAMAGE], 100, (int) sprite.x, (int) sprite.y, sprite.type->sound_frequency, sprite.type->random_freq);
            if (sprite.type->destruction % 100 == DESTRUCTION_FEATHERS) {
                Create_Effects(DESTRUCTION_FEATHERS, (unsigned long) sprite.x, (unsigned long) sprite.y);
            }
            if (sprite.type->type != TYPE_PROJECTILE) {
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, -1, -1, 60, 0.01, 128);
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, 0, -1, 60, 0.01, 128);
                New_Particle(PARTICLE_STAR, sprite_x, sprite_y, 1, -1, 60, 0.01, 128);
            }
            if (sprite.Has_AI(AI_CHANGE_SKULLS_IF_HIT))
                Map_Change_SkullBlocks();
            if (sprite.Has_AI(AI_ATTACK_1_IF_HIT)) {
                sprite.attack_timer_1 = sprite.type->attack1_duration;
                sprite.charge_time = 0;
            }
            if (sprite.Has_AI(AI_ATTACK_2_IF_HIT)) {
                sprite.attack_timer_2 = sprite.type->attack2_duration;
                sprite.charge_time = 0;
            }
        }
        sprite.damage_received = 0;
        sprite.damage_type_received = DAMAGE_NONE;

        /*****************************************************************************************/
        /* If the sprite is destroyed                                                                   */
        /*****************************************************************************************/

        if (sprite.health < 1) {
            destruction = sprite.type->destruction;
            if (destruction != DESTRUCTION_INDESTRUCTIBLE) {
                if (sprite.type->bonus > -1 && sprite.type->bonus_count > 0)
                    if (/*sprite.Has_AI(AI_BONUS)*/sprite.type->always_drop_bonus || rand() % 4 == 1)
                        for (int bi = 0; bi < sprite.type->bonus_count; bi++)
                            Add_BonusSprite(prototypes[sprite.type->bonus], 0, sprite_x - 11 + (10 - rand() % 20), sprite_bottom - 16 - (10 + rand() % 20), i);
                if (sprite.Has_AI(AI_CHANGE_SKULLS_IF_KNOCKED_OUT) && !sprite.Has_AI(AI_CHANGE_SKULLS_IF_HIT))
                    Map_Change_SkullBlocks();
                if (destruction >= DESTRUCTION_ANIMATION)
                    destruction -= DESTRUCTION_ANIMATION;
                else
                    sprite.is_hidden = true;
                Create_Effects(destruction, (unsigned long)sprite.x, (unsigned long)sprite.y);
                Play_Sound(sprite.type->sounds[SOUND_DESTRUCTION], 100, (int)sprite.x, (int)sprite.y, sprite.type->sound_frequency, sprite.type->random_freq);
                if (sprite.type->type == TYPE_GAME_CHARACTER && sprite.type->points != 0) {
                    char number[10];
                    itoa(sprite.type->points, number, 10);
                    New_Fade_Text(font2, number, (int) sprites[i].x - 8, (int) sprites[i].y - 8, 80);
                    points_add += sprite.type->points;
                }
            } else
                sprite.health = 1;
        }
    }

    if (sprite.hit_counter == 0)
        sprite.damage_type_received = DAMAGE_NONE;

    /*****************************************************************************************/
    /* Checks                                                                          */
    /*****************************************************************************************/

    if (!right) {
        if (sprite_a > 0)
            sprite_a = 0;
    }

    if (!left) {
        if (sprite_a < 0)
            sprite_a = 0;
    }

    if (!up) {
        if (sprite_b < 0)
            sprite_b = 0;
        if (!jump_maximum)
            sprite.jump_timer = sprite.type->max_jump * 2;
    }

    if (!down) {
        if (sprite_b >= 0) {
            if (sprite.jump_timer > 0) {
                if (sprite.jump_timer > sprite.type->max_jump * 2 + 10) {
                    Play_Sound(explosion_sound, 30, (int) sprite_x, (int) sprite_y,
                               int(25050 - sprite.weight * 3000), true);
                    //New_Particle(	PARTICLE_DUST_CLOUD,sprite_x+rand()%5-rand()%5-10,sprite_bottom+rand()%3-rand()%3,0,-0.2,rand()%50+20,0,0);
                    if (rand() % 7 == 1) {
                        New_Particle(PARTICLE_SMOKE_CLOUD, sprite_x + rand() % 5 - rand() % 5 - 10,
                                     sprite_bottom + rand() % 3 - rand() % 3, 0.3, -0.1, 450, 0, 0);
                        New_Particle(PARTICLE_SMOKE_CLOUD, sprite_x + rand() % 5 - rand() % 5 - 10,
                                     sprite_bottom + rand() % 3 - rand() % 3, -0.3, -0.1, 450, 0, 0);
                    }
                    if (sprite.weight > 1)
                        earthquake = 34 + int(sprite.weight * 20);
                }
                sprite.jump_timer = 0;
            }
            sprite_b = 0;
        }
    }

    /*****************************************************************************************/
    /* Assignment of auxiliary variables to the sprite's variables                           */
    /*****************************************************************************************/

    if (sprite_b > 4.0)
        sprite_b = 4.0;
    if (sprite_b < -4.0)
        sprite_b = -4.0;
    if (sprite_a > max_speed)
        sprite_a = max_speed;
    if (sprite_a < -max_speed)
        sprite_a = -max_speed;
    if (sprite.health > sprite.type->health)
        sprite.health = sprite.type->health;
    if (sprite.hit_counter == 0 || sprite.player == 1) {
        sprite_x += sprite_a;
        sprite_y += sprite_b;
    }

    if (i == player_index || sprite.health < 1) {
        double friction = 1.04;//1.04
        if (map->climate == WEATHER_RAIN || map->climate == WEATHER_RAIN_FOREST)
            friction = 1.03;
        if (map->climate == WEATHER_SNOW)
            friction = 1.01;
        if (!down)
            sprite_a /= friction;
        else
            sprite_a /= 1.03;//1.02//1.05
        sprite_b /= 1.25;
    }

    sprite.x = sprite_x;
    sprite.y = sprite_y;
    sprite.movement_x = sprite_a;
    sprite.movement_y = sprite_b;

    sprite.can_move_right = right;
    sprite.can_move_left = left;
    sprite.can_move_down = down;
    sprite.can_move_up = up;
/*
	sprite.weight = sprite.type->weight;

	if (sprite.health < 1 && sprite.weight == 0)
		sprite.weight = 1;
*/
    if (sprite.jump_timer < 0)
        sprite.can_move_down = false;
    //sprite.is_crouching = false;

    /*****************************************************************************************/
    /* Artificial Intelligence                                                               */
    /*****************************************************************************************/

    if (sprite.player == 0) {
        for (int ai = 0; ai < SPRITE_MAX_AI; ai++)
            switch (sprite.type->AI[ai]) {
                case AI_NONE:
                    ai = SPRITE_MAX_AI; // stopping
                    break;
                case AI_CHICKEN:
                    sprite.AI_Chicken();
                    break;
                case AI_SMALL_CHICKEN:
                    sprite.AI_Chicken();
                    break;
                case AI_FROG1:
                    sprite.AI_Frog1();
                    break;
                case AI_FROG2:
                    sprite.AI_Frog2();
                    break;
                case AI_BONUS:
                    sprite.AI_Bonus();
                    break;
                case AI_EGG:
                    sprite.AI_Egg();
                    break;
                case AI_PROJECTILE:
                    sprite.AI_Projectile();
                    break;
                case AI_JUMPING:
                    sprite.AI_Jumper();
                    break;
                case AI_BASIC:
                    sprite.AI_Default();
                    break;
                case AI_NONSTOP:
                    sprite.AI_NonStop();
                    break;
                case AI_TURN_ON_COLLISION_HORIZONTAL:
                    sprite.AI_Turns_From_Horizontal_Obstacle();
                    break;
                case AI_TURN_FROM_OBSTACLE_VERT:
                    sprite.AI_Turns_From_Vertical_Obstacle();
                    break;
                case AI_AVOID_PIT:
                    sprite.AI_Avoids_Pit();
                    break;
                case AI_RANDOM_DIRECTION_CHANGE_HORIZONTAL:
                    sprite.AI_Random_Horizontal_Direction_Change();
                    break;
                case AI_RANDOM_TURNING:
                    sprite.AI_Random_Turn();
                    break;
                case AI_RANDOM_JUMP:
                    sprite.AI_Random_Jump();
                    break;
                case AI_FOLLOW_PLAYER:
                    sprite.AI_Follows_Player(sprites[player_index]);
                    break;
                case AI_FOLLOW_PLAYER_IF_VISIBLE:
                    sprite.AI_Follows_Player_If_Seen(sprites[player_index]);
                    break;
                case AI_FOLLOWS_PLAYER_VERT_HORI:
                    sprite.AI_Follows_Player_Vertical_Horizontal(sprites[player_index]);
                    break;
                case AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI:
                    sprite.AI_Follows_Player_If_Seen_Vertical_Horizontal(sprites[player_index]);
                    break;
                case AI_BOMB:
                    sprite.AI_Bomb();
                    break;
                case AI_ATTACK_1_IF_HIT:
                    sprite.AI_Attack_1_If_Hit();
                    break;
                case AI_ATTACK_2_IF_HIT:
                    sprite.AI_Attack_2_If_Hit();
                    break;
                case AI_ATTACK_1_NONSTOP:
                    sprite.AI_Attack_1_Nonstop();
                    break;
                case AI_ATTACK_2_NONSTOP:
                    sprite.AI_Attack_2_Nonstop();
                    break;
                case AI_ATTACK_1_IF_PLAYER_IN_FRONT:
                    sprite.AI_Attack_1_If_Player_In_Front(sprites[player_index]);
                    break;
                case AI_ATTACK_2_IF_PLAYER_IN_FRONT:
                    sprite.AI_Attack_2_If_Player_In_Front(sprites[player_index]);
                    break;
                case AI_ATTACK_1_IF_PLAYER_BELOW:
                    sprite.AI_Attack_1_If_Player_Below(sprites[player_index]);
                    break;
                case AI_JUMP_IF_PLAYER_ABOVE:
                    sprite.AI_Jump_If_Player_Above(sprites[player_index]);
                    break;
                case AI_DAMAGE_FROM_WATER:
                    sprite.AI_Takes_Damage_From_Water();
                    break;
                case AI_KILL_ALL:
                    sprite.AI_Kill_All();
                    break;
                case AI_FRICTION_AFFECTS:
                    sprite.AI_Friction_Affects();
                    break;
                case AI_HIDES:
                    sprite.AI_Hides();
                    break;
                case AI_RETURN_TO_START_X:
                    sprite.AI_Returns_To_Start_X();
                    break;
                case AI_RETURN_TO_START_Y:
                    sprite.AI_Returns_To_Start_Y();
                    break;
                case AI_MOVE_X_COS:
                    sprite.AI_Moves_X(cos_table[degree % 360]);
                    break;
                case AI_MOVE_Y_COS:
                    sprite.AI_Moves_Y(cos_table[degree % 360]);
                    break;
                case AI_MOVE_X_SIN:
                    sprite.AI_Moves_X(sin_table[degree % 360]);
                    break;
                case AI_MOVE_Y_SIN:
                    sprite.AI_Moves_Y(sin_table[degree % 360]);
                    break;
                case AI_MOVE_X_COS_FAST:
                    sprite.AI_Moves_X(cos_table[(degree * 2) % 360]);
                    break;
                case AI_MOVE_Y_SIN_FAST:
                    sprite.AI_Moves_Y(sin_table[(degree * 2) % 360]);
                    break;
                case AI_MOVES_X_COS_SLOW:
                    sprite.AI_Moves_X(cos_table[(degree / 2) % 360]);
                    break;
                case AI_MOVES_Y_SIN_SLOW:
                    sprite.AI_Moves_Y(sin_table[(degree / 2) % 360]);
                    break;
                case AI_MOVES_Y_SIN_FREE:
                    sprite.AI_Moves_Y(sin_table[(sprite.timer / 2) % 360]);
                    break;
                case AI_CHANGE_IF_ENERGY_LESS_THAN_2:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_If_Energy_Less_Than_2(prototypes[sprite.type->transformation]);
                    break;
                case AI_CHANGE_IF_ENERGY_MORE_THAN_1:
                    if (sprite.type->transformation > -1)
                        if (sprite.AI_Change_If_Energy_More_Than_1(prototypes[sprite.type->transformation]) == 1)
                            Create_Effects(DESTRUCTION_SMOKE_GRAY, (unsigned long) sprite.x, (unsigned long) sprite.y);
                    break;
                case AI_CHANGE_TIMER:
                    if (sprite.type->transformation > -1)
                        sprite.AI_Change_Timer(prototypes[sprite.type->transformation]);
                    break;
                case AI_TELEPORT:
                    if (sprite.AI_Teleport(i, sprites, MAX_SPRITES, sprites[player_index]) == 1) {
                        camera_x = (int) sprites[player_index].x;
                        camera_y = (int) sprites[player_index].y;
                        dcamera_x = camera_x - MAP_DRAWING_WIDTH / 2;
                        dcamera_y = camera_y - MAP_DRAWING_HEIGHT / 2;
                        PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
                        if (sprite.type->sounds[SOUND_ATTACK2] != -1)
                            Play_Sound_Menu(sprite.type->sounds[SOUND_ATTACK2], 100);
                        //Play_Sound(, 100, camera_x, camera_y, SOUNDS_SAMPLERATE, false);
                    }
                    break;
                case AI_DESTROY_IF_PARENT_DESTROYED:
                    sprite.AI_Dies_If_Parent_Dies(sprites);
                    break;

                case AI_FALLS_FROM_STORY:
                    sprite.AI_Falls_From_Story(earthquake + story_switch);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH1_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch1, 1, 0);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH2_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch2, 1, 0);
                    break;
                case AI_MOVES_DOWN_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 0, 1);
                    break;
                case AI_MOVES_UP_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 0, -1);
                    break;
                case AI_MOVES_LEFT_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, -1, 0);
                    break;
                case AI_MOVES_RIGHT_IF_SWITCH3_PRESSED:
                    sprite.AI_Moves_If_Switch_Pressed(switch3, 1, 0);
                    break;
                case AI_FALLS_IF_SWITCH1_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch1);
                    break;
                case AI_FALLS_IF_SWITCH2_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch2);
                    break;
                case AI_FALLS_IF_SWITCH3_PRESSED:
                    sprite.AI_Falls_If_Switch_Pressed(switch3);
                    break;
                case AI_RANDOM_MOVEMENT_VERT_HORI:
                    sprite.AI_Random_Vertical_Horizontal_Move();
                    break;

                case AI_INFO1:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info01));
                    break;
                case AI_INFO2:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info02));
                    break;
                case AI_INFO3:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info03));
                    break;
                case AI_INFO4:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info04));
                    break;
                case AI_INFO5:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info05));
                    break;
                case AI_INFO6:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info06));
                    break;
                case AI_INFO7:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info07));
                    break;
                case AI_INFO8:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info08));
                    break;
                case AI_INFO9:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info09));
                    break;
                case AI_INFO10:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info10));
                    break;
                case AI_INFO11:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info11));
                    break;
                case AI_INFO12:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info12));
                    break;
                case AI_INFO13:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info13));
                    break;
                case AI_INFO14:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info14));
                    break;
                case AI_INFO15:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info15));
                    break;
                case AI_INFO16:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info16));
                    break;
                case AI_INFO17:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info17));
                    break;
                case AI_INFO18:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info18));
                    break;
                case AI_INFO19:
                    if (sprite.AI_Info(sprites[player_index])) New_Notification(texts->Get_Text(txt_info19));
                    break;

                default:
                    break;
            }
    }

    //if (echo == 1 && sprite.type->type == TYPE_PROJECTILE && sprite.type->damage_type == DAMAGE_NOISE && sprite.type->sounds[SOUND_ATTACK1] > -1)
    //	Play_Sound(sprite.type->sounds[SOUND_ATTACK1],20, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

    /*****************************************************************************************/
    /* Don't let the sprite out of the game area                                                 */
    /*****************************************************************************************/

    if (sprite.x < 0)
        sprite.x = 0;
    if (sprite.y < -sprite_height)
        sprite.y = -sprite_height;
    if (sprite.x > MAP_WIDTH * 32)
        sprite.x = MAP_WIDTH * 32;

    // If the sprite falls below the bottom of the map
    if (sprite.y > MAP_HEIGHT * 32 + sprite_height) {
        sprite.y = MAP_HEIGHT * 32 + sprite_height;
        sprite.health = 0;
        sprite.is_hidden = true;
        if (sprite.total_weight >= 1)
            earthquake = 50;
    }
    if (sprite.movement_x > max_speed)
        sprite.movement_x = max_speed;
    if (sprite.movement_x < -max_speed)
        sprite.movement_x = -max_speed;

    /*****************************************************************************************/
    /* Attacks 1 and 2                                                                    */
    /*****************************************************************************************/

    // sprite is ready for attack and not crouching
    if (sprite.charge_time == 0 && !sprite.is_crouching) {
        // attack time is "full," which means starting the attack
        if (sprite.attack_timer_1 == sprite.type->attack1_duration) {
            // set recovery time after which the sprite can attack again
            sprite.charge_time = sprite.type->reload_time;
            // if the sprite has not defined its own reload time ...
            if (sprite.projectile_1 > -1 && sprite.type->reload_time == 0)
                // ... and the projectile has, take the reload_time from the projectile
                if (prototypes[sprite.projectile_1].fire_rate > 0)
                    sprite.charge_time = prototypes[sprite.projectile_1].fire_rate;
            // play the attack sound
            Play_Sound(sprite.type->sounds[SOUND_ATTACK1], 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            if (sprite.projectile_1 > -1) {
                Add_ProjectileSprite(prototypes[sprite.projectile_1], 0, sprite_x, sprite_y, i);
                //		if (prototypes[sprite.projectile_1].sounds[SOUND_ATTACK1] > -1)
                //			Play_Sound(prototypes[sprite.projectile_1].sounds[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            }
        }
        // Same as attack 1
        if (sprite.attack_timer_2 == sprite.type->attack2_duration) {
            sprite.charge_time = sprite.type->reload_time;
            if (sprite.projectile_2 > -1 && sprite.type->reload_time == 0)
                if (prototypes[sprite.projectile_2].fire_rate > 0)
                    sprite.charge_time = prototypes[sprite.projectile_2].fire_rate;
            Play_Sound(sprite.type->sounds[SOUND_ATTACK2], 100, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            if (sprite.projectile_2 > -1) {
                Add_ProjectileSprite(prototypes[sprite.projectile_2], 0, sprite_x, sprite_y, i);
                //		if (prototypes[sprite.projectile_2].sounds[SOUND_ATTACK1] > -1)
                //			Play_Sound(prototypes[sprite.projectile_2].sounds[SOUND_ATTACK1],100, (int)sprite_x, (int)sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);
            }
        }
    }

    // Random sounds
    if (sprite.type->sounds[SOUND_RANDOM] != -1 && rand() % 200 == 1 && sprite.health > 0)
        Play_Sound(sprite.type->sounds[SOUND_RANDOM], 80, (int) sprite_x, (int) sprite_y, sprite.type->sound_frequency, sprite.type->random_freq);

    // DEVELOPMENT PHASE AUXILIARY FUNCTIONS

    unsigned char color;
    unsigned long block;

    if (PisteInput_KeyDown(DIK_B) && cheats) {
        if (i == player_index) {
            char number[50];
            itoa(blocks[1].top_bound, number, 10);
            //_gcvt(sprite_a,7,number);
            PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 310, 50);
        }
        if (sprite.type->collides_with_tiles)
            for (x = 0; x < blocks_x_count; x++) {
                for (y = 0; y < blocks_y_count; y++) {
                    color = 50 - x * 2 - y * 2;
                    block = x + y * blocks_x_count;
                    if (block > 299)
                        block = 299;
                    if (block < 0)
                        block = 0;
                    if (!blocks[block].background)
                        color += 32;
                    PisteDraw_Buffer_Clear(PD_BACKBUFFER, blocks[block].left_bound - camera_x, blocks[block].top_bound - camera_y, blocks[block].right_bound - camera_x, blocks[block].bottom_bound - camera_y, color);
                }
            }
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, (int) (sprite_left - camera_x), (int) (sprite_top - camera_y), (int) (sprite_right - camera_x), (int) (sprite_bottom - camera_y), 30);
    }
    return 0;
}

int Move_Bonus_Sprite(int i) {
    // Variables
/*
	double	sprite_x,
			sprite_y,
			sprite_a,
			sprite_b,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width,
			sprite_height;

	int		map_left,
			map_top,

	//		blocks_x_count,
	//		blocks_y_count,

			x = 0,
			y = 0;

	bool	right,
			left,
			up,
			down,

			in_water;

	unsigned char   max_speed;
*/
    sprite_x = 0;
    sprite_y = 0;
    sprite_a = 0;
    sprite_b = 0;

    sprite_left = 0;
    sprite_right = 0;
    sprite_top = 0;
    sprite_bottom = 0;

    sprite_width = 0;
    sprite_height = 0;

    map_left = 0;
    map_top = 0;

/*
	PK2Sprite &sprite = sprites[i];

	double	sprite_x = sprite.x,
			sprite_y = sprite.y,
			sprite_a = sprite.movement_x,
			sprite_b = sprite.movement_y,

			sprite_left,
			sprite_right,
			sprite_top,
			sprite_bottom;

	int		sprite_width  = sprite.type->width,
			sprite_height = sprite.type->height;

	int		map_left,
			map_top,

			blocks_x_count,
			blocks_y_count,

			x = 0,
			y = 0;

	bool	right	= true,
			left	= true,
			up		= true,
			down		= true;

	bool	in_water = sprite.in_water;
*/
    PK2Sprite &sprite = sprites[i];

    sprite_x = sprite.x;
    sprite_y = sprite.y;
    sprite_a = sprite.movement_x;
    sprite_b = sprite.movement_y;

    sprite_width = sprite.type->width;
    sprite_height = sprite.type->height;

    x = 0;
    y = 0;
    right = true, left = true, up = true, down = true;
    in_water = sprite.in_water;
    max_speed = (int) sprite.type->max_speed;

    // Move the actual variables to auxiliary variables.
    sprite_left = sprite_x - sprite_width / 2;
    sprite_right = sprite_x + sprite_width / 2;
    sprite_top = sprite_y - sprite_height / 2;
    sprite_bottom = sprite_y + sprite_height / 2;

    if (sprite.hit_counter > 0)
        sprite.hit_counter--;
    if (sprite.charge_time > 0)
        sprite.charge_time--;
    if (sprite.transformation_timer > 0)    // duration for changes
        sprite.transformation_timer--;

    // Factors related to the jump
    if (story_switch + earthquake > 0 && sprite.jump_timer == 0)
        sprite.jump_timer = sprite.type->max_jump / 2;
    if (sprite.jump_timer > 0 && sprite.jump_timer < sprite.type->max_jump) {
        sprite.jump_timer++;
        sprite_b = (sprite.type->max_jump - sprite.jump_timer) / -4.0;//-2
    }

    if (sprite_b > 0)
        sprite.jump_timer = sprite.type->max_jump;
    if (sprite.weight != 0) {   // if the bonus has weight, examine the environment
        // o
        //
        // |  Gravity
        // V

        sprite_b += sprite.weight + sprite_b / 1.25;

        if (sprite.in_water) {
            if (sprite_b > 0)
                sprite_b /= 2.0;
            if (rand() % 80 == 1)
                New_Particle(PARTICLE_SPARK, sprite_x - 4, sprite_y, 0, -0.5 - rand() % 2, rand() % 30 + 30, 0, 32);
        }

        sprite.in_water = false;
        sprite.total_weight = sprite.weight;

        /* OTHER SPRITES */
        Block sprite_block;

        for (int sprite_index = 0; sprite_index < MAX_SPRITES; sprite_index++) {
            if (sprite_index != i && !sprites[sprite_index].is_hidden) {
                if (sprites[sprite_index].type->obstacle && sprite.type->collides_with_tiles) {
                    if (sprite_x - sprite_width / 2 + sprite_a <= sprites[sprite_index].x + sprites[sprite_index].type->width / 2
                        && sprite_x + sprite_width / 2 + sprite_a >= sprites[sprite_index].x - sprites[sprite_index].type->width / 2
                        && sprite_y - sprite_height / 2 + sprite_b <= sprites[sprite_index].y + sprites[sprite_index].type->height / 2
                        && sprite_y + sprite_height / 2 + sprite_b >= sprites[sprite_index].y - sprites[sprite_index].type->height / 2) {
                        sprite_block.code = 0;
                        sprite_block.bottom_bound = (int) sprites[sprite_index].y + sprites[sprite_index].type->height / 2;
                        sprite_block.right_bound = (int) sprites[sprite_index].x + sprites[sprite_index].type->width / 2;
                        sprite_block.left_bound = (int) sprites[sprite_index].x - sprites[sprite_index].type->width / 2;
                        sprite_block.top_bound = (int) sprites[sprite_index].y - sprites[sprite_index].type->height / 2;

                        sprite_block.down = BLOCK_WALL;
                        sprite_block.up = BLOCK_WALL;
                        sprite_block.right = BLOCK_WALL;
                        sprite_block.left = BLOCK_WALL;

                        if (!sprites[sprite_index].type->blocks_down)
                            sprite_block.down = BLOCK_BACKGROUND;
                        if (!sprites[sprite_index].type->blocks_up)
                            sprite_block.up = BLOCK_BACKGROUND;
                        if (!sprites[sprite_index].type->blocks_right)
                            sprite_block.right = BLOCK_BACKGROUND;
                        if (!sprites[sprite_index].type->blocks_left)
                            sprite_block.left = BLOCK_BACKGROUND;
                        sprite_block.water = false;
                        Disable_Block_Background(sprite_block);
                        Check_Wall(sprite, sprite_block, sprite_x, sprite_y, sprite_a, sprite_b, sprite_left, sprite_right, sprite_top, sprite_bottom, sprite_width, sprite_height, map_left, map_top, right, left, up, down);
                    }
                }
                if (sprite_x < sprites[sprite_index].x + sprites[sprite_index].type->width / 2 &&
                    sprite_x > sprites[sprite_index].x - sprites[sprite_index].type->width / 2 &&
                    sprite_y < sprites[sprite_index].y + sprites[sprite_index].type->height / 2 &&
                    sprite_y > sprites[sprite_index].y - sprites[sprite_index].type->height / 2 && sprite.hit_counter == 0) {
                    if (sprites[sprite_index].type->type != TYPE_BONUS &&
                        !(sprite_index == player_index && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE))
                        sprite_a += sprites[sprite_index].movement_x * (rand() % 4);
                    // add the weight of the other sprite touching the sprite to the sprite's weight
                    sprite.total_weight += sprites[sprite_index].type->weight;
                    // same-type sprites change direction upon collision
                    if (sprite.type->index == sprites[sprite_index].type->index &&
                        sprites[sprite_index].health > 0) {
                        if (sprite.x < sprites[sprite_index].x) {
                            sprites[sprite_index].movement_x += sprite.movement_x / 3.0;
                            right = false;
                        }
                        if (sprite.x > sprites[sprite_index].x) {
                            sprites[sprite_index].movement_x += sprite.movement_x / 3.0;
                            left = false;
                        }
/*
						if (sprite.y < sprites[sprite_index].y)
							down = false;
						if (sprite.y > sprites[sprite_index].y)
							up = false;
*/
                    }
                }
            }
        }
        // Check that we don't go too fast in any direction.
        if (sprite_b > 4)
            sprite_b = 4;
        if (sprite_b < -4)
            sprite_b = -4;
        if (sprite_a > 3)
            sprite_a = 3;
        if (sprite_a < -3)
            sprite_a = -3;

        // Calculate
        int blocks_x_count = 0, blocks_y_count = 0;

        if (sprite.type->collides_with_tiles) {
            blocks_x_count = (int) ((sprite_width) / 32) + 4;
            blocks_y_count = (int) ((sprite_height) / 32) + 4;
            map_left = (int) (sprite_left) / 32;
            map_top = (int) (sprite_top) / 32;
            for (y = 0; y < blocks_y_count; y++)
                for (x = 0; x < blocks_x_count; x++) {
                    blocks[x + y * blocks_x_count] = Examine_Block(map_left + x - 1, map_top + y - 1);
                }
            // Investigate if it collides with the block
            for (y = 0; y < blocks_y_count; y++)
                for (x = 0; x < blocks_x_count; x++)
                    Check_Wall(sprite, blocks[x + y * blocks_x_count]);
/*
			Check_Wall_Debug(sprite, blocks[x+y*blocks_x_count],
					sprite_x,
					sprite_y,
					sprite_a,
					sprite_b,
					sprite_left,
					sprite_right,
					sprite_top,
					sprite_bottom,
					sprite_width,
					sprite_height,
					map_left,
					map_top,
					right,
					left,
					up,
					down);
*/
        }

        if (in_water != sprite.in_water)
            Create_SplashEffect((int) sprite_x, (int) sprite_y, 32);
        if (!right) {
            if (sprite_a > 0)
                sprite_a = -sprite_a / 1.5;
        }
        if (!left) {
            if (sprite_a < 0)
                sprite_a = -sprite_a / 1.5;
        }
        if (!up) {
            if (sprite_b < 0)
                sprite_b = 0;

            sprite.jump_timer = sprite.type->max_jump;
        }
        if (!down) {
            if (sprite_b >= 0) {
                if (sprite.jump_timer > 0) {
                    sprite.jump_timer = 0;
                    //	if (/*sprite_b == 4*/!on_ground)
                    //		Play_Sound(explosion_sound,20,(int)sprite_x, (int)sprite_y,
                    //				      int(25050-sprite.type->weight*4000),true);
                }
                if (sprite_b > 2)
                    sprite_b = -sprite_b / (3 + rand() % 2);
                else
                    sprite_b = 0;
            }
            //sprite_a /= friction;
            sprite_a /= 1.07;
        } else {
            sprite_a /= 1.02;
        }
        sprite_b /= 1.5;
        if (sprite_b > 4)
            sprite_b = 4;
        if (sprite_b < -4)
            sprite_b = -4;
        if (sprite_a > 4)
            sprite_a = 4;
        if (sprite_a < -4)
            sprite_a = -4;

        sprite_x += sprite_a;
        sprite_y += sprite_b;

        sprite.x = sprite_x;
        sprite.y = sprite_y;
        sprite.movement_x = sprite_a;
        sprite.movement_y = sprite_b;

        sprite.can_move_right = right;
        sprite.can_move_left = left;
        sprite.can_move_down = down;
        sprite.can_move_up = up;
    } else {   // if sprite's weight is zero, make the sprite "floating"
        sprite.y = sprite.start_y + cos_table[int(degree + (sprite.start_x + sprite.start_y)) % 360] / 3.0;
        sprite_y = sprite.y;
    }
    sprite.weight = sprite.initial_weight;
    int destruction;
    ///////////////////////////////////////////
    // check if the bonus touches the player //
    ///////////////////////////////////////////
    if (sprite_x < sprites[player_index].x + sprites[player_index].type->width / 2 && sprite_x > sprites[player_index].x - sprites[player_index].type->width / 2 && sprite_y < sprites[player_index].y + sprites[player_index].type->height / 2 && sprite_y > sprites[player_index].y - sprites[player_index].type->height / 2 && sprite.hit_counter == 0) {
        if (sprite.health > 0 && sprites[player_index].health > 0) {
            if (sprite.type->points != 0) {
                points_add += sprite.type->points;
                char number[6];
                itoa(sprite.type->points, number, 10);
                if (sprite.type->points >= 50)
                    New_Fade_Text(font2, number, (int) sprite.x - 8, (int) sprite.y - 8, 100);
                else
                    New_Fade_Text(font1, number, (int) sprite.x - 8, (int) sprite.y - 8, 100);
            }
            if (sprite.Has_AI(AI_BONUS_TIME))
                game_time += sprite.type->reload_time;
            if (sprite.Has_AI(AI_BONUS_INVISIBILITY))
                invisibility = sprite.type->reload_time;
            //map->sprites[(int)(sprite.start_x/32) + (int)(sprite.start_y/32)*MAP_WIDTH] = 255;
            if (sprite.type->damage != 0 && sprite.type->destruction != DESTRUCTION_INDESTRUCTIBLE)
                sprites[player_index].health -= sprite.type->damage;
            destruction = sprite.type->destruction;
            if (destruction != DESTRUCTION_INDESTRUCTIBLE) {
                if (destruction >= DESTRUCTION_ANIMATION)
                    destruction -= DESTRUCTION_ANIMATION;
                else {
                    if (sprite.type->key) {
                        num_keys--;
                        if (num_keys < 1)
                            Unlock_Map();
                    }
                    sprite.is_hidden = true;
                }
                if (sprite.Has_AI(AI_NEW_IF_DESTROYED)) {
                    double ax, ay;
                    ax = sprite.start_x;//-sprite.type->width;
                    ay = sprite.start_y - sprite.type->height / 2.0;
                    Add_Sprite(*sprite.type, 0, ax - 17, ay, i);
                    for (int r = 1; r < 6; r++)
                        New_Particle(PARTICLE_SPARK, ax + rand() % 10 - rand() % 10, ay + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
                }
                if (sprite.type->bonus != -1)
                    Add_Item(&prototypes[sprite.type->bonus]);
                if (sprite.type->transformation != -1) {
                    if (prototypes[sprite.type->transformation].AI[0] != AI_BONUS) {
                        sprites[player_index].type = &prototypes[sprite.type->transformation];
                        sprites[player_index].projectile_1 = sprites[player_index].type->projectile1;
                        sprites[player_index].projectile_2 = sprites[player_index].type->projectile2;
                        sprites[player_index].initial_weight = sprites[player_index].type->weight;
                        sprites[player_index].y -= sprites[player_index].type->height / 2;
                        //New_Notification("pekka has been transformed!");
                    }
                }
                if (sprite.type->projectile1 != -1) {
                    sprites[player_index].projectile_1 = sprite.type->projectile1;
                    New_Notification(texts->Get_Text(txt_game_newegg));
                    //ammo1 += sprite.type->bonus_count;
                }
                if (sprite.type->projectile2 != -1) {
                    sprites[player_index].projectile_2 = sprite.type->projectile2;
                    New_Notification(texts->Get_Text(txt_game_newdoodle));
                    //ammo2 += sprite.type->bonus_count;
                }
                Play_Sound(sprite.type->sounds[SOUND_DESTRUCTION], 100, (int)sprite.x, (int)sprite.y, sprite.type->sound_frequency, sprite.type->random_freq);
                Create_Effects(destruction, (unsigned long) sprite_x, (unsigned long) sprite_y);
            }
        }
    }
    for (i = 0; i < SPRITE_MAX_AI; i++) {
        if (sprite.type->AI[i] == AI_NONE)
            break;

        switch (sprite.type->AI[i]) {
            case AI_BONUS:
                sprite.AI_Bonus();
                break;
            case AI_BASIC:
                sprite.AI_Default();
                break;
            case AI_CHANGE_TIMER:
                if (sprite.type->transformation > -1)
                    sprite.AI_Change_Timer(prototypes[sprite.type->transformation]);
                break;
            case AI_FALLS_FROM_STORY:
                sprite.AI_Falls_From_Story(earthquake + story_switch);
                break;

            default:
                break;
        }
    }
    /* It is not worth the effort to check if the player has movement_x bonus item */
    if (sprite.player != 0)
        sprite.health = 0;

    return 0;
}

void Set_Margins(bool hori, bool vert, int width, int height) {
    if (LIMIT_MAP_DRAWING_AREA) {
        if (SCREEN_WIDTH > width && hori)
            MARGIN_HORI = (SCREEN_WIDTH - width) / 2;
        else
            MARGIN_HORI = 0;
        if (SCREEN_HEIGHT > height && vert)
            MARGIN_VERT = (SCREEN_HEIGHT - height) / 2;
        else
            MARGIN_VERT = 0;
        PisteDraw_SetMarginLeft(MARGIN_HORI);
        PisteDraw_SetMarginTop(MARGIN_VERT);
    }
}

// TODO: =>
int Initialize_States() {
    if (next_game_state != game_state) {
        // INITIALIZE THE BASIC SETUP
        if (next_game_state == STATE_BASIC_INITIALIZATION) {
            PisteLog_Write("- Initializing basic stuff \n");
            //Load_Language("language\\suomi.txt");
            strcpy(player_name, texts->Get_Text(txt_player_default_name));
            srand((unsigned) time(NULL));
            _getcwd(working_directory, _MAX_PATH);
            PisteLog_Write("  - Work directory: ");
            PisteLog_Write(working_directory);
            PisteLog_Write("\n");
            strcpy(message, "no message");
            strcpy(episode, "");
            //strcpy(episode_directory,"");
            strcpy(next_map, "untitle1.map");
            stage = 1;
            if (!tables_calculated) {
                PisteLog_Write("  - Precalculating... \n");
                Calculate_Tables();
                Map_Cos_Sin(cos_table, sin_table);
                tables_calculated = true;
            }
            Initialize_Fade_Texts();
            if (MAP_DRAWING_WIDTH > SCREEN_WIDTH)
                MAP_DRAWING_WIDTH = SCREEN_WIDTH;
            if (MAP_DRAWING_HEIGHT > SCREEN_HEIGHT)
                MAP_DRAWING_HEIGHT = SCREEN_HEIGHT;
            if (!LIMIT_MAP_DRAWING_AREA) {
                MAP_DRAWING_WIDTH = SCREEN_WIDTH;
                MAP_DRAWING_HEIGHT = SCREEN_HEIGHT;
            } else {
/*
				if (SCREEN_WIDTH > 640)
					MAP_DRAWING_WIDTH = 640;
				else
					MAP_DRAWING_WIDTH = SCREEN_WIDTH;

				if (SCREEN_HEIGHT > 480)
					MAP_DRAWING_HEIGHT = 480;
				else
					MAP_DRAWING_HEIGHT = SCREEN_HEIGHT;
*/
            }
            Map_Set_Screen_Dimensions(MAP_DRAWING_WIDTH, MAP_DRAWING_HEIGHT);
            PisteLog_Write("  - Initializing PisteSound \n");
            if ((PisteSound_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, SOUNDS_STEREO, SOUNDS_SAMPLERATE, SOUNDS_BITRATE)) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Initializing PisteSound failed!\n");
            }
            PisteLog_Write("  - Initializing Midas \n");
            Midas_Init();
            PisteLog_Write("  - Initializing PisteInput \n");
            if ((PisteInput_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app)) == PI_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Initializing PisteInput failed!\n");
            }
            PisteLog_Write("  - Initializing PisteDraw \n");
            if ((PisteDraw_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE)) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Initializing PisteDraw failed!\n");
            }
            PisteLog_Write("  - Creating an empty map class \n");
            map = new PK2Map();
            PisteLog_Write("  - Creating movement_x buffer for game's stuff bitmap \n");
            if ((game_image = PisteDraw_Buffer_Create(640, 480, true, 255)) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Creating buffer failed!\n");
                return -1;
            }
            if ((game_image_sysmem = PisteDraw_Buffer_Create(640, 480, false, 255)) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Creating buffer failed!\n");
                return -1;
            }
            PisteLog_Write("  - Loading pk2stuff.pcx \n");
            if (PisteDraw_Image_Load(game_image, "gfx\\pk2stuff.bmp", true) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Loading pk2stuff.bmp failed!\n");
                return -1;
            }
            if (PisteDraw_Image_Load(game_image_sysmem, "gfx\\pk2stuff.bmp", true) == PD_ERROR) {
                PK2_error = true;
                PisteLog_Write("  - Loading pk2stuff.bmp failed!\n");
                return -1;
            }
            // FONTS
            //if ((font1 = PisteDraw_Font_Create(game_image,1,456,8,8,52)) == PD_ERROR)
            //	PK2_error = true;
            PisteLog_Write("  - Loading fonts \n");
            int index_font = 0, index_path = 0;
            index_path = texts->Get_Index("font path");
            index_font = texts->Get_Index("font small font");
            if (index_path == -1 || index_font == -1) {
                if ((font1 = PisteDraw_Font_Create("language\\fonts\\", "ScandicSmall.txt")) == PD_ERROR) {
                    PK2_error = true;
                }
            } else {
                if ((font1 = PisteDraw_Font_Create(texts->Get_Text(index_path), texts->Get_Text(index_font))) == PD_ERROR) {
                    PK2_error = true;
                    PisteLog_Write("    - Loading font ");
                    PisteLog_Write(texts->Get_Text(index_path));
                    PisteLog_Write(texts->Get_Text(index_font));
                    PisteLog_Write(" failed!\n");
                }
            }
            index_font = texts->Get_Index("font big font normal");
            if (index_path == -1 || index_font == -1) {
                if ((font2 = PisteDraw_Font_Create("language\\fonts\\", "ScandicBig1.txt")) == PD_ERROR) {
                    PK2_error = true;
                }
            } else {
                if ((font2 = PisteDraw_Font_Create(texts->Get_Text(index_path), texts->Get_Text(index_font))) == PD_ERROR) {
                    PK2_error = true;
                    PisteLog_Write("    - Loading font ");
                    PisteLog_Write(texts->Get_Text(index_path));
                    PisteLog_Write(texts->Get_Text(index_font));
                    PisteLog_Write(" failed!\n");
                }
            }
            index_font = texts->Get_Index("font big font hilite");
            if (index_path == -1 || index_font == -1) {
                if ((font3 = PisteDraw_Font_Create("language\\fonts\\", "ScandicBig2.txt")) == PD_ERROR) {
                    PK2_error = true;
                }
            } else {
                if ((font3 = PisteDraw_Font_Create(texts->Get_Text(index_path), texts->Get_Text(index_font))) == PD_ERROR) {
                    PK2_error = true;
                    PisteLog_Write("    - Loading font ");
                    PisteLog_Write(texts->Get_Text(index_path));
                    PisteLog_Write(texts->Get_Text(index_font));
                    PisteLog_Write(" failed!\n");
                }
            }
            index_font = texts->Get_Index("font big font shadow");
            if (index_path == -1 || index_font == -1) {
                if ((font4 = PisteDraw_Font_Create("language\\fonts\\", "ScandicBig3.txt")) == PD_ERROR) {
                    PK2_error = true;
                }
            } else {
                if ((font4 = PisteDraw_Font_Create(texts->Get_Text(index_path), texts->Get_Text(index_font))) == PD_ERROR) {
                    PK2_error = true;
                    PisteLog_Write("    - Loading font ");
                    PisteLog_Write(texts->Get_Text(index_path));
                    PisteLog_Write(texts->Get_Text(index_font));
                    PisteLog_Write(" failed!\n");
                }
            }
/*
			if ((font2 = PisteDraw_Font_Create("language\\fonts\\","ScandicBig1.txt")) == PD_ERROR){
				PK2_error = true;
			}
			if ((font3 = PisteDraw_Font_Create("language\\fonts\\","ScandicBig2.txt")) == PD_ERROR){
				PK2_error = true;
			}
			if ((font4 = PisteDraw_Font_Create("language\\fonts\\","ScandicBig3.txt")) == PD_ERROR){
				PK2_error = true;
			}
*/
/*
			if ((font1 = PisteDraw_Font_Create(game_image,1,413,8,10,52)) == PD_ERROR)
				PK2_error = true;
			//if ((font2 = PisteDraw_Font_Create(game_image,1,425,15,18,42)) == PD_ERROR)
			//	PK2_error = true;
			if ((font2 = PisteDraw_Font_Create(game_image,0,268,15,21,42)) == PD_ERROR)
				PK2_error = true;
			if ((font3 = PisteDraw_Font_Create(game_image,0,290,15,21,42)) == PD_ERROR)
				PK2_error = true;
			if ((font4 = PisteDraw_Font_Create(game_image,0,312,15,21,42)) == PD_ERROR)
				PK2_error = true;
*/
            Clear_Sprites();

            Find_Episodes();
            Initialize_Levels();
            Find_Levels();

            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteLog_Write("  - Loading basic sound fx \n");

            if ((switch_sound = PisteSound_SFX_New("sfx\\switch3.wav")) == PS_ERROR)
                PK2_error = true;
            if ((jump_sound = PisteSound_SFX_New("sfx\\jump4.wav")) == PS_ERROR)
                PK2_error = true;
            if ((splash_sound = PisteSound_SFX_New("sfx\\splash.wav")) == PS_ERROR)
                PK2_error = true;
            if ((unlock_sound = PisteSound_SFX_New("sfx\\openlock.wav")) == PS_ERROR)
                PK2_error = true;
            if ((menu_sound = PisteSound_SFX_New("sfx\\menu2.wav")) == PS_ERROR)
                PK2_error = true;
            if ((shoot_sound = PisteSound_SFX_New("sfx\\moo.wav")) == PS_ERROR)
                PK2_error = true;
            if ((crow_sound = PisteSound_SFX_New("sfx\\doodle.wav")) == PS_ERROR)
                PK2_error = true;
            if ((explosion_sound = PisteSound_SFX_New("sfx\\pump.wav")) == PS_ERROR)
                PK2_error = true;
            if ((score_counter_sound = PisteSound_SFX_New("sfx\\counter.wav")) == PS_ERROR)
                PK2_error = true;
            if (PK2_error)
                PisteLog_Write("  - Loading one or more basic sound fx failed!\n");

            PisteWait_Start();
            PisteDraw_Fade_Palette_In(PD_FADE_SLOW);

            PisteLog_Write("  - Calculating tiles. \n");
            Calculate_Blocks();
            Initialize_Items();

            PisteLog_Write("  - Loading background picture \n");
            background_image = PisteDraw_Buffer_Create(640, 480, true, 255);
            PisteDraw_Image_Load(background_image, "gfx\\menu.bmp", true);

            Clear_Saves();

            PisteLog_Write("  - Loading saves \n");
            Find_All_Saves("data\\saves.dat");

            PisteLog_Write("  - PisteSound sounds on \n");
            PisteSound_ToggleSoundOn(settings.sounds);

            PisteLog_Write("- Initializing basic stuff completed \n");
        }

        // KARTAN ALUSTUS
        if (next_game_state == STATE_MAP) {
            PisteLog_Write("- Initializing map screen \n");
            Set_Margins(true, true, 640, 480);

            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_UpdateScreen();
            //PisteDraw_Font_Write(font2,"loading...",PD_BACKBUFFER,SCREEN_WIDTH/2-82,SCREEN_HEIGHT/2-9);

            if (!game_in_progress) {
                if (load_game != -1) {
                    PisteLog_Write("  - Loading movement_x saved game \n");
                    Find_Levels();
                    for (int j = 0; j < EPISODE_MAX_STAGES; j++)
                        stages[j].completed = saves[load_game].stage_completed[j];
                    load_game = -1;
                    game_in_progress = true;
                    game_over = true;
                    stage_completed = true;
                    end_timer = 0;
                } else {
                    PisteLog_Write("  - Starting movement_x new game \n");
                    Initialize_Levels();    // jos ladataan peli, asetetaan l�p�istyarvot jaksoille aikaisemmin
                    Find_Levels();
                }

                PisteLog_Write("  - Loading top scores \n");
                char topscore_file[_MAX_PATH] = "scores.dat";
                Load_Episode_Scores(topscore_file);
            }

            /* Loading the map background_image ...*/
            char map_image[_MAX_PATH] = "map.bmp";
            Add_Episode_Directory(map_image);
            PisteLog_Write("  - Loading map picture ");
            PisteLog_Write(map_image);
            PisteLog_Write(" from episode folder \n");

            if (PisteDraw_Image_Load(background_image, map_image, true) == PD_ERROR) { // Jos episodilla ei omaa...
                PisteLog_Write("  - Loading map picture map.bmp from gfx folder \n");
                if (PisteDraw_Image_Load(background_image, "gfx\\map.bmp", true) == PD_ERROR) { // ladataan oletuskuva.
                    PisteLog_Write("  - Loading map picture failed!\n");
                }
            }

            /* Loading the map music ...*/
            char map_music[_MAX_PATH] = "map.xm";
            Add_Episode_Directory(map_music);

            if (!Is_File(map_music)) {
                strcpy(map_music, "map.mod");
                Add_Episode_Directory(map_music);

                if (!Is_File(map_music)) {
                    strcpy(map_music, "map.it");
                    Add_Episode_Directory(map_music);

                    if (!Is_File(map_music)) {
                        strcpy(map_music, "map.s3m");
                        Add_Episode_Directory(map_music);
                    }
                }
            }

            PisteLog_Write("  - Loading map music ");
            PisteLog_Write(map_music);
            PisteLog_Write(" from episode folder \n");

            if (Midas_Load_Music(map_music) != 0) {
                PisteLog_Write("  - Loading map music map.xm from music folder \n");
                if (Midas_Load_Music("music\\map.xm") != 0) {
                    PisteLog_Write("  - Loading map music failed!\n");
                    PK2_error = true;
                }
            }

            music_volume = music_max_volume;
            switch_from_map_to_game = false;
            PisteDraw_Fade_Palette_In(PD_FADE_SLOW);
            PisteWait_Start();
        }

        // MENU INITIALIZATION
        if (next_game_state == STATE_MENUS) {
            PisteLog_Write("- Initializing menu screen \n");
            Set_Margins(true, true, 640, 480);
            //PisteDraw_Buffer_Flip_Fast(background_image,PD_BACKBUFFER,0,0);
            PisteLog_Write("  - Copying game situation as background image \n");
            Copy_Game_State_To_Background();
            Blur_Image(background_image, 640, 480);
            //Copy_Game_State_To_Background();
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_UpdateScreen();
            PisteLog_Write("  - Loading episodes \n");
            Find_Episodes();

            if (!game_in_progress) {
                PisteLog_Write("  - Loading menu background image: gfx\\menu.bmp\n");
                PisteDraw_Image_Load(background_image, "gfx\\menu.bmp", true);
                //Blur_Image(background_image, 640, 480);

                PisteLog_Write("  - Loading menu background music: music\\song09.xm \n");
                Midas_Load_Music("music\\song09.xm");//theme.xm
                music_volume = music_max_volume;
            }

            //PisteDraw_Image_Load(background_image,"gfx\\menu.bmp",false);
            menu_rectangle.left = 320 - 5;
            menu_rectangle.top = 240 - 5;
            menu_rectangle.right = 320 + 5;
            menu_rectangle.bottom = 240 + 5;

            PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
            PisteWait_Start();
            selected_menu_id = 1;
            PisteLog_Write("- Initializing menu screen completed\n");
        }

        // NEW LEVEL INITIALIZATION
        if (next_game_state == STATE_GAME) {
            PisteLog_Write("- Initializing movement_x new level \n");
            Set_Margins(false, false, 0, 0);

            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_Font_Write(font2, texts->Get_Text(txt_game_loading), PD_BACKBUFFER, SCREEN_WIDTH / 2 - 82, SCREEN_HEIGHT / 2 - 9);
            PisteDraw_UpdateScreen();
            PisteDraw_Reset_Palette();

            if (stages[current_stage_index].completed)
                retry = true;
            else
                retry = false;

            if (!game_in_progress) {
                stage_completed = false;

                Clear_Sprites();
                PisteLog_Write("  - Loading map \n");
                if (Load_Map(next_map) == 1) {
                    PK2_error = true;
                    PisteLog_Write("  - Loading map failed!\n");
                }

                PisteLog_Write("  - Doing tile calcualtions...\n");
                Calculate_Blocks();

                PisteLog_Write("  - Initializing items \n");
                Initialize_Items();
                game_in_progress = true;
                music_volume = music_max_volume;
                degree = 0;
                item_panel_x = -215;
                points_add = 0;
            } else {
                degree = degree_temp;
            }

            PisteWait_Start();
            fps_time = PisteWait_Get();
            Set_Margins(true, true, 800, 480);
            PisteLog_Write("- Initializing movement_x new level complete\n");
        }

        // INITIALIZATION OF SCORE COUNTING
        if (next_game_state == STATE_SCORE_CALCULATION) {
            Set_Margins(true, true, 640, 480);

            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_UpdateScreen();

            PisteDraw_Image_Load(background_image, "gfx\\menu.bmp", true);
            Blur_Image(background_image, 640, 480);

            if (Midas_Load_Music("music\\hiscore.xm") != 0)
                PK2_error = true;

            stage_new_record = false;
            stage_new_record_time = false;
            episode_new_record = false;

            // Calculate the player's total points in advance
            unsigned long temp_pisteet = 0;
            temp_pisteet += stage_points;
            temp_pisteet += game_time * 5;
            temp_pisteet += sprites[player_index].health * 300;
            for (int i = 0; i < MAX_ITEMS; i++)
                if (items[i] != NULL)
                    temp_pisteet += items[i]->points + 500;

            //if (stages[current_stage_index].completed)
            //if (stages[current_stage_index].order == stage-1)
            points += temp_pisteet;

            if (retry)
                points -= temp_pisteet;

            fake_points = 0;
            score_calculation_phase = 0;
            score_calculation_delay = 30;
            bonus_points = 0, time_points = 0, energy_points = 0, item_points = 0, rescue_points = 0;

            char score_file[_MAX_PATH] = "scores.dat";
            int comparison_result;

            /* Check if the player has broken the level's score or speed record */
            comparison_result = Compare_Episode_Scores(current_stage_index, temp_pisteet, map->time - game_time,
                                                     false);
            if (comparison_result > 0) {
                Save_Episode_Scores(score_file);
            }

            /* Check if the player has broken the episode's score record */
            comparison_result = Compare_Episode_Scores(0, points, 0, true);
            if (comparison_result > 0)
                Save_Episode_Scores(score_file);

            music_volume = music_max_volume;
            switch_from_score_to_map = false;
            //PisteDraw_Fade_Palette_In(PD_FADE_FAST);
            PisteWait_Start();
        }

        // INTRO INITIALIZATION
        if (next_game_state == STATE_INTRO) {
            PisteLog_Write("- Initializing intro screen\n");
            Set_Margins(true, true, 640, 480);
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_UpdateScreen();
            PisteLog_Write("  - Loading picture: gfx\\intro.bmp\n");
            PisteDraw_Image_Load(background_image, "gfx\\intro.bmp", true);
            PisteLog_Write("  - Loading music: music\\intro.xm\n");
            if (Midas_Load_Music("music\\intro.xm") != 0)
                PK2_error = true;
            music_volume = music_max_volume;
            intro_counter = 0;
            switch_from_score_to_map = false;
            //PisteDraw_Fade_Palette_In(PD_FADE_FAST);
            PisteWait_Start();
            PisteLog_Write("- Initializing intro screen complete\n");
        }

        if (next_game_state == STATE_END) {
            Set_Margins(true, true, 640, 480);
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
            PisteDraw_UpdateScreen();
            PisteDraw_Image_Load(background_image, "gfx\\ending.bmp", true);
            if (Midas_Load_Music("music\\intro.xm") != 0)
                PK2_error = true;
            music_volume = music_max_volume;
            end_counter = 0;
            switch_from_end_to_menu = false;
            game_in_progress = false;
            //PisteDraw_Fade_Palette_In(PD_FADE_FAST);
            PisteWait_Start();
        }

        PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
        game_state = next_game_state;
    }

    return 0;
}

/*
int PK_Init(void) {
	_getcwd(working_directory, _MAX_PATH);
	strcpy(message,"no message");
	stage = 1;
	strcpy(episode,"demo");
	strcpy(episode_directory,"episodes\\demo");
	srand( (unsigned)time( NULL ) );
	Calculate_Tables();
	Map_Cos_Sin(cos_table, sin_table);
	Initialize_Fade_Texts();
	if (LIMIT_MAP_DRAWING_AREA) {
		if (SCREEN_WIDTH > 640)
			MAP_DRAWING_WIDTH = 640;
		else
			MAP_DRAWING_WIDTH = SCREEN_WIDTH;
		if (SCREEN_HEIGHT > 480)
			MAP_DRAWING_HEIGHT = 480;
		else
			MAP_DRAWING_HEIGHT = SCREEN_HEIGHT;
	} else {
		MAP_DRAWING_WIDTH = SCREEN_WIDTH;
		MAP_DRAWING_HEIGHT = SCREEN_HEIGHT;
	}
	//PK2Map_Set_Screen_Dimensions(SCREEN_WIDTH,SCREEN_HEIGHT);
	Map_Set_Screen_Dimensions(MAP_DRAWING_WIDTH,MAP_DRAWING_HEIGHT);
	if ((PisteSound_Init((HWND &)window_handle, (HINSTANCE &) hinstance_app, SOUNDS_STEREO, SOUNDS_SAMPLERATE, SOUNDS_BITRATE)) == PD_ERROR)
		PK2_error = true;
	Midas_Init();
	if ((PisteInput_Init((HWND &)window_handle, (HINSTANCE &)hinstance_app)) == PI_ERROR)
		PK2_error = true;
	if ((PisteDraw_Init((HWND &)window_handle, (HINSTANCE &) hinstance_app, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE)) == PD_ERROR)
		PK2_error = true;
	map = new PK2Map();
	if ((game_image = PisteDraw_Buffer_Uusi(640,480, true, 255)) == PD_ERROR)
		PK2_error = true;
	if (PisteDraw_Load_Image(game_image,"gfx\\pk2stuff.bmp", true) == PD_ERROR)
		PK2_error = true;
	if ((font1 = PisteDraw_Font_Create(game_image,1,456,8,8,52)) == PD_ERROR)
		PK2_error = true;
	//if ((font2 = PisteDraw_Font_Create(game_image,1,446,14,9,45)) == PD_ERROR)
	//	PK2_error = true;
	if ((font2 = PisteDraw_Font_Create(game_image,1,425,15,18,42)) == PD_ERROR)
		PK2_error = true;
	Clear_Sprites();
	Initialize_Levels();
	Find_Levels();
	if (Load_Map("untitle1.map") == 1)
		PK2_error = true;
	PisteDraw_Buffer_Clear(PD_BACKBUFFER,0);
	if ((switch_sound = PisteSound_SFX_Uusi("sfx\\switch.wav"))==PS_ERROR)
		PK2_error = true;
	if ((jump_sound = PisteSound_SFX_Uusi("sfx\\jump2.wav"))==PS_ERROR)
		PK2_error = true;
	if ((splash_sound = PisteSound_SFX_Uusi("sfx\\splash.wav"))==PS_ERROR)
		PK2_error = true;
	if ((unlock_sound = PisteSound_SFX_New("sfx\\openlock.wav"))==PS_ERROR)
		PK2_error = true;
	//PisteDraw_SetClipper(PD_BACKBUFFER,0,30,639,450);
//	if ((background_extra = PisteDraw_Buffer_Create(640,480, true, 255)) == PD_ERROR)
//		PK2_error = true;
//	if ((PisteDraw_Image_Load(background_extra,"pilvet2.bmp",false)) == PD_ERROR)
//		PK2_error = true;
	PisteWait_Start();
	PisteDraw_Fade_Palette_In(PD_FADE_SLOW);
	Calculate_Blocks();
	Initialize_Items();
	return 0;
}
*/

int Sprites(void) {
    info_sprites_active = 0;
    //bool is_active;
    for (int i = 0; i < MAX_SPRITES; i++) {
        // Is the sprite close to the center of events? If so, activate it.
        if (sprites[i].x < camera_x + 640 + 320 &&//MAP_DRAWING_WIDTH+MAP_DRAWING_WIDTH/2 &&
            sprites[i].x > camera_x - 320 &&//MAP_DRAWING_WIDTH/2 &&
            sprites[i].y < camera_y + 480 + 240 &&//MAP_DRAWING_HEIGHT+MAP_DRAWING_HEIGHT/2 &&
            sprites[i].y > camera_y - 240)//MAP_DRAWING_HEIGHT/2)
            sprites[i].is_active = true;
        else
            sprites[i].is_active = false;
        if (sprites[i].is_hidden == true)
            sprites[i].is_active = false;
    }
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (sprites[i].is_active && sprites[i].type->type != TYPE_BACKGROUND) {
            if (sprites[i].type->type == TYPE_BONUS)
                Move_Bonus_Sprite(i);
            else
                Move_Sprite(i);
            info_sprites_active++;
        }
    }
    return 0;
}

int Draw_Sprites_Backgrounds() {
    double xl, yl, start_x, start_y, yk;
    int i;
    for (int in = 0; in < MAX_SPRITES; in++) {
        i = background_sprites[in];
        // Check if the sprite is visible
        if (sprites[i].type != NULL && i != -1) {
            if (!sprites[i].is_hidden && sprites[i].type->type == TYPE_BACKGROUND) {
                //Check if the sprite or part of it is in the picture
                start_x = sprites[i].start_x;
                start_y = sprites[i].start_y;
                if (sprites[i].type->parallax_coefficient != 0) {
                    xl = start_x - camera_x - MAP_DRAWING_WIDTH / 2 - sprites[i].type->width / 2;
                    xl /= sprites[i].type->parallax_coefficient;
                    yl = start_y - camera_y - MAP_DRAWING_HEIGHT / 2 - sprites[i].type->height / 2;
                    yk = sprites[i].type->parallax_coefficient;///1.5;
                    if (yk != 0)
                        yl /= yk;
                } else
                    xl = yl = 0;
                switch (sprites[i].type->AI[0]) {
                    case AI_BACKGROUND_MOON:
                        yl += MAP_DRAWING_HEIGHT / 3 + 50;
                        break;
/*
                    case AI_BACKGROUND_MOVES_LEFT:
                        if (sprites[i].movement_x == 0)
                            sprites[i].movement_x = rand()%3;
                        sprites[i].start_x -= sprites[i].movement_x;
                        if (sprites[i].hiding && sprites[i].start_x < camera_x) {
                          sprites[i].start_x = camera_x+SCREEN_WIDTH+sprites[i].type->width*2;
                          sprites[i].movement_x = rand()%3;
                        }
                        break;
*/
                    case AI_MOVE_X_COS:
                        sprites[i].AI_Moves_X(cos_table[degree % 360]);
                        start_x = sprites[i].x;
                        start_y = sprites[i].y;
                        break;
                    case AI_MOVE_Y_COS:
                        sprites[i].AI_Moves_Y(cos_table[degree % 360]);
                        start_x = sprites[i].x;
                        start_y = sprites[i].y;
                        break;
                    case AI_MOVE_X_SIN:
                        sprites[i].AI_Moves_X(sin_table[degree % 360]);
                        start_x = sprites[i].x;
                        start_y = sprites[i].y;
                        break;
                    case AI_MOVE_Y_SIN:
                        sprites[i].AI_Moves_Y(sin_table[degree % 360]);
                        start_x = sprites[i].x;
                        start_y = sprites[i].y;
                        break;

                    default:
                        break;
                }
                sprites[i].x = start_x - xl;
                sprites[i].y = start_y - yl;

                //Check if the sprite or part of it is in the picture
                if (sprites[i].x - sprites[i].type->width / 2 < camera_x + SCREEN_WIDTH && sprites[i].x + sprites[i].type->width / 2 > camera_x && sprites[i].y - sprites[i].type->height / 2 < camera_y + SCREEN_HEIGHT && sprites[i].y + sprites[i].type->height / 2 > camera_y) {
                    sprites[i].Draw(camera_x, camera_y);
                    sprites[i].hiding = false;
                    info_sprites_drawn++;
                } else {
                    if (!pause)
                        sprites[i].Animate();
                    sprites[i].hiding = true;
                }
                info_sprites++;
            }
        }
    }

    return 0;
}

int Draw_Sprites() {
    info_sprites = 0;
    info_sprites_drawn = 0;
    int stars, sx;
    double star_x, star_y;
    for (int i = 0; i < MAX_SPRITES; i++) {
        // Is the sprite visible
        if (!sprites[i].is_hidden && sprites[i].type->type != TYPE_BACKGROUND) {
            // Check if the sprite or part of it is in the picture
            if (sprites[i].x - sprites[i].type->width / 2 < camera_x + SCREEN_WIDTH && sprites[i].x + sprites[i].type->width / 2 > camera_x && sprites[i].y - sprites[i].type->height / 2 < camera_y + SCREEN_HEIGHT && sprites[i].y + sprites[i].type->height / 2 > camera_y) {
                if (sprites[i].hit_counter > 0 && sprites[i].type->type != TYPE_BONUS && sprites[i].health < 1)
                    Draw_ImpactEffect((unsigned long) sprites[i].x - 8, (unsigned long) sprites[i].y - 8);
                if (invisibility == 0 || degree % 2 == 0 || i != player_index)
                    sprites[i].Draw(camera_x, camera_y);
                //else
                //	sprites[i].Draw(camera_x,camera_y);
                if (sprites[i].health < 1 && sprites[i].type->type != TYPE_PROJECTILE) {
                    sx = (int) sprites[i].x;
                    for (stars = 0; stars < 3; stars++) {
                        star_x = sprites[i].x - 8 + (sin_table[((stars * 120 + degree) * 2) % 359]) / 3;
                        star_y = sprites[i].y - 18 + (cos_table[((stars * 120 + degree) * 2 + sx) % 359]) / 8;
                        Draw_Star((unsigned long) star_x, (unsigned long) star_y, 100, 128);
                    }
                }
                info_sprites_drawn++;
            } else {
                if (!pause)
                    sprites[i].Animate();
                if (sprites[i].health < 1)
                    sprites[i].is_hidden = true;
            }
            info_sprites++;
        }
    }

    return 0;
}

int Camera(void) {
    camera_x = (int) sprites[player_index].x - MAP_DRAWING_WIDTH / 2;
    camera_y = (int) sprites[player_index].y - MAP_DRAWING_HEIGHT / 2;
/*
	if (!PisteInput_Mouse_Left()) {
		camera_x = (int)sprites[player_index].x - MAP_DRAWING_WIDTH / 2;
		camera_y = (int)sprites[player_index].y - MAP_DRAWING_HEIGHT / 2;
	} else {
		camera_x += PisteInput_Mouse_X(0) * 5;
		camera_y += PisteInput_Mouse_Y(0) * 5;
	}
*/
    if (earthquake > 0) {
        dcamera_x += (rand() % earthquake - rand() % earthquake) / 5;
        dcamera_y += (rand() % earthquake - rand() % earthquake) / 5;
        earthquake--;
    }
    if (story_switch > 0) {
        dcamera_x += (rand() % 9 - rand() % 9);//3
        dcamera_y += (rand() % 9 - rand() % 9);
        story_switch--;
    }
    if (dcamera_x != camera_x)
        dcamera_a = (camera_x - dcamera_x) / 15;
    if (dcamera_y != camera_y)
        dcamera_b = (camera_y - dcamera_y) / 15;
    if (dcamera_a > 6)
        dcamera_a = 6;
    if (dcamera_a < -6)
        dcamera_a = -6;
    if (dcamera_b > 6)
        dcamera_b = 6;
    if (dcamera_b < -6)
        dcamera_b = -6;

    dcamera_x += dcamera_a;
    dcamera_y += dcamera_b;
    camera_x = (int) dcamera_x;
    camera_y = (int) dcamera_y;

    if (camera_x < 0)
        camera_x = 0;
    if (camera_y < 0)
        camera_y = 0;
    if (camera_x > int(MAP_WIDTH - MAP_DRAWING_WIDTH / 32) * 32)
        camera_x = int(MAP_WIDTH - MAP_DRAWING_WIDTH / 32) * 32;
    if (camera_y > int(MAP_HEIGHT - MAP_DRAWING_HEIGHT / 32) * 32)
        camera_y = int(MAP_HEIGHT - MAP_DRAWING_HEIGHT / 32) * 32;

    return 0;
}

int Draw_Info(void) {
    int fy, temp;
    char number[20];
    fy = 35;

    temp = PisteDraw_Font_Write(font1, "sprite: ", PD_BACKBUFFER, 10, fy);
    itoa(info_sprites, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 10 + temp, fy);
    fy += 10;

    temp = PisteDraw_Font_Write(font1, "active: ", PD_BACKBUFFER, 10, fy);
    itoa(info_sprites_active, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 10 + temp, fy);
    fy += 10;

    temp = PisteDraw_Font_Write(font1, "drawn: ", PD_BACKBUFFER, 10, fy);
    itoa(info_sprites_drawn, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 10 + temp, fy);
    fy += 10;
/*
	char file[255];
	strcpy(file, "sprites\\esim.spr");
	Add_Episode_Directory(file);
	PisteDraw_Font_Write(font1, file, PD_BACKBUFFER, 30, 450);
*/
    for (int i = 0; i < 40; i++) {
        itoa(prototypes[i].index, number, 10);
        PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 410, 10 + i * 10);
        PisteDraw_Font_Write(font1, prototypes[i].file, PD_BACKBUFFER, 430, 10 + i * 10);
        PisteDraw_Font_Write(font1, prototypes[i].bonus_sprite, PD_BACKBUFFER, 545, 10 + i * 10);
    }
    PisteDraw_Font_Write(font1, message, PD_BACKBUFFER, 10, 80);
    for (int i = 0; i < EPISODE_MAX_STAGES; i++) {
        if (strcmp(stages[i].name, "") != 0)
            PisteDraw_Font_Write(font1, stages[i].name, PD_BACKBUFFER, 30, 240 + i * 10);
    }
    PisteDraw_Font_Write(font1, next_map, PD_BACKBUFFER, 10, 450);

    char drawn_number[50];
    _gcvt(sprites[player_index].x, 7, drawn_number);
    PisteDraw_Font_Write(font1, drawn_number, PD_BACKBUFFER, 10, 420);
    _gcvt(sprites[player_index].y, 7, drawn_number);
    PisteDraw_Font_Write(font1, drawn_number, PD_BACKBUFFER, 10, 430);
    _gcvt(sprites[player_index].movement_y, 7, drawn_number);
    PisteDraw_Font_Write(font1, drawn_number, PD_BACKBUFFER, 10, 440);
    //_gcvt(sprites[player_index].movement_y,7,drawn_number);
    //PisteDraw_Font_Write(font1,drawn_number,PD_BACKBUFFER,10,450);
    //itoa(sprites[player_index].type->height,number,10);
    //PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10,440);
/*
	x = (int)((sprites[player_index].x+sprites[player_index].movement_x)/32);
	itoa(x,number,10);
	PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10,10);
	y = (int)(sprites[player_index].y/32);
	itoa(y,number,10);
	PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10,25);
	p = map->walls[x+y*MAP_WIDTH];
	itoa(p,number,10);
	PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10,40);
*/
    itoa(sprites[player_index].jump_timer, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 270, 450);
/*
	temp = 0;
	for (int p=177;p<185;p++)	{
		itoa(p,number,10);
		PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,10+temp,450) ;
		_gcvt(sin_table[p],2,drawn_number);
		temp += PisteDraw_Font_Write(font1,drawn_number,PD_BACKBUFFER,10+temp,460) + 4;
	}
*/
    char file_path[_MAX_PATH] = "";
    Add_Episode_Directory(file_path);
    PisteDraw_Font_Write(font1, file_path, PD_BACKBUFFER, 10, 470);
    itoa(invisibility, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 610, 470);
    return 0;
}

int Draw_Background(void) {
    int parallax_x = (camera_x % 1920) / 3;
    int parallax_y = (camera_y % 1440) / 3;
    int parallax_x2 = (camera_x % 1280) / 2;
    int parallax_y2 = (camera_y % 960) / 2;

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 34);//0
    Set_Bounds();
    //if (LIMIT_MAP_DRAWING_AREA)
    //	PisteDraw_SetClipper(PD_BACKBUFFER,MARGIN_HORI,MARGIN_VERT,MAP_DRAWING_WIDTH+MARGIN_HORI,MAP_DRAWING_HEIGHT+MARGIN_VERT);
    //else
    //	PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
/*
	if (LIMIT_MAP_DRAWING_AREA)
		PisteDraw_SetClipper(PD_BACKBUFFER,16,16,MAP_DRAWING_WIDTH-16,MAP_DRAWING_HEIGHT-16);
	else
		PisteDraw_SetClipper(PD_BACKBUFFER,16,16,SCREEN_WIDTH-16,SCREEN_HEIGHT-16);
*/
    if (map->background == BACKGROUND_STATIC) {
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 0);
    }
    if (map->background == BACKGROUND_PARALLAX_HORI) {
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0 - parallax_x, 0);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640 - parallax_x, 0);
        if (MAP_DRAWING_WIDTH > 640)
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 1280 - parallax_x, 0);
    }
    if (map->background == BACKGROUND_PARALLAX_VERT) {
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 0 - parallax_y);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0, 480 - parallax_y);
        if (MAP_DRAWING_WIDTH > 640) {
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 0 - parallax_y);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640, 480 - parallax_y);
        }
    }
    if (map->background == BACKGROUND_PARALLAX_VERT_AND_HORI) {
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0 - parallax_x, 0 - parallax_y);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640 - parallax_x, 0 - parallax_y);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 0 - parallax_x, 480 - parallax_y);
        PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 640 - parallax_x, 480 - parallax_y);
        if (MAP_DRAWING_WIDTH > 640) {
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 1280 - parallax_x, 0 - parallax_y);
            PisteDraw_Buffer_Flip_Fast(map->background_image_buffer, PD_BACKBUFFER, 1280 - parallax_x, 480 - parallax_y);
        }
    }
    //clouds 1
/*
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,0-parallax_x2,0-parallax_y2);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,640-parallax_x2,0-parallax_y2);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,0-parallax_x2,480-parallax_y2);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,640-parallax_x2,480-parallax_y2);
*/
    //clouds 2
/*
	parallax_x2 = (int)((camera_x%1600)/2.5);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,0-parallax_x2,95,0,332,640,382);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,640-parallax_x2,95,0,332,640,382);
	parallax_x2 = (int)((camera_x%1280)/2);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,0-parallax_x2,65,0,186,640,263);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,640-parallax_x2,65,0,186,640,263);
	parallax_x2 = (int)((camera_x%960)/1.5);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,0-parallax_x2,0,0,0,640,109);
	PisteDraw_Buffer_Flip_Fast(background_extra,PD_BACKBUFFER,640-parallax_x2,0,0,0,640,109);
*/
    return 0;
}

int Draw_Bottom_Panel() {
    Set_Bounds();
    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    if (num_items > 0 && item_panel_x < 10)
        item_panel_x++;
    if (num_items == 0 && item_panel_x > -215)
        item_panel_x--;
    if (item_panel_x > -215)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, item_panel_x, MAP_DRAWING_HEIGHT - 60, 1, 216, 211, 266);
    if (item_panel_x > 5)
        PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_items), PD_BACKBUFFER, 15, MAP_DRAWING_HEIGHT - 65);
    char number[15];
    int interval = 0;
    int x, y;
    //////////////
    // draw duration
    //////////////
    if (game_time > 0) {
        int minutes = game_time / 60, seconds = game_time % 60;
        x = SCREEN_WIDTH / 2 - 546 / 2 + 342;
        y = MAP_DRAWING_HEIGHT - 39;
        PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_time), PD_BACKBUFFER, x, y - 20);
        itoa(minutes, number, 10);
        PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, x + 1, y + 1);
        interval += PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, x, y);
        interval += PisteDraw_Font_Write(font1, ":", PD_BACKBUFFER, x + interval, y);
        if (seconds < 10) {
            PisteDraw_Font_Write(font4, "0", PD_BACKBUFFER, x + interval + 1, y + 1);
            interval += PisteDraw_Font_Write(font2, "0", PD_BACKBUFFER, x + interval, y);
        }
        itoa(seconds, number, 10);
        PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, x + interval + 1, y + 1);
        interval += PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, x + interval, y);
    }
    /////////////////
    // draw keys
    /////////////////
    if (num_keys > 0) {
        x = SCREEN_WIDTH / 2 - 546 / 2 + 483;
        y = MAP_DRAWING_HEIGHT - 39;
        PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_keys), PD_BACKBUFFER, x, y - 20);
        itoa(num_keys, number, 10);
        PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, x + 1, y + 1);
        PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, x, y);
    }
    /////////////////
    // draw items
    /////////////////
    Draw_Items();
    return 0;
}

// TODO: =>
int Draw_Game_Top(void) {
    char number[15];
    int interval = 20;

    interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_energy), PD_BACKBUFFER, 40, 10);
    ltoa(sprites[player_index].health, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 40 + interval + 1, 10 + 1);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 40 + interval, 10);

    interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_score), PD_BACKBUFFER, 230, 10);
    ltoa(stage_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 230 + interval + 1, 10 + 1);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 230 + interval, 10);
/*
	interval = PisteDraw_Font_Write(font1,"level: ",PD_BACKBUFFER,450,10);
	ltoa(map->stage,number,10);
	PisteDraw_Font_Write(font4,number,PD_BACKBUFFER,450+interval+1,10+1);
	PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,450+interval,10);
*/
    if (sprites[player_index].projectile_2 != -1) {
        //itoa(ammo2,number,10);
        PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_attack1), PD_BACKBUFFER, MAP_DRAWING_WIDTH - 170, 10);
        prototypes[sprites[player_index].projectile_2].Draw(MAP_DRAWING_WIDTH - 170/*+MARGIN_HORI*/,/*MARGIN_VERT+*/20,0);
        //PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,SCREEN_WIDTH-35,50);
    }
    if (sprites[player_index].projectile_1 != -1) {
        //itoa(ammo1,number,10);
        PisteDraw_Font_Write(font1, texts->Get_Text(txt_game_attack2), PD_BACKBUFFER, MAP_DRAWING_WIDTH - 90, 10 + 15);
        prototypes[sprites[player_index].projectile_1].Draw(MAP_DRAWING_WIDTH - 90/*+MARGIN_HORI*/,/*MARGIN_VERT+*/20 + 15, 0);
        //PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,SCREEN_WIDTH-35,10);
    }
    if (notification_timer > 0) {
        int ilm_pituus = strlen(notification) * 8 + 8; // 300
        RECT alue = {SCREEN_WIDTH / 2 - (ilm_pituus / 2), 60, SCREEN_WIDTH / 2 + (ilm_pituus / 2), 60 + 20};
        if (notification_timer < 20) {
            alue.top += (20 - notification_timer) / 2;
            alue.bottom -= (20 - notification_timer) / 2;
        }
        if (notification_timer > MAX_NOTIFICATION_DISPLAY_TIME - 20) {
            alue.top += 10 - (MAX_NOTIFICATION_DISPLAY_TIME - notification_timer) / 2;
            alue.bottom -= 10 - (MAX_NOTIFICATION_DISPLAY_TIME - notification_timer) / 2;
        }
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, alue.left - 1, alue.top - 1, alue.right + 1, alue.bottom + 1, 51);
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, alue.left, alue.top, alue.right, alue.bottom, 38);
        if (notification_timer >= 100)
            PisteDraw_Font_Write(font1, notification, PD_BACKBUFFER, alue.left + 4, alue.top + 4);
        else
            PisteDraw_Font_Write_Transparent(font1, notification, PD_BACKBUFFER, alue.left + 4, alue.top + 4, notification_timer);
    }
    return 0;
}

int Draw_Game(void) {
    char number[15];
    int interval = 20;
    if (!skip_frame)
        Draw_Background();
    Map_Animate(degree, block_animation / 7, switch1, switch2, switch3, false);
    if (!skip_frame)
        if (settings.background_sprites)
            Draw_Sprites_Backgrounds();
    if (settings.weather_effects)
        Process_BackgroundParticles();
    Set_Bounds();
    if (!skip_frame)
        map->Draw_Backgrounds(camera_x, camera_y, false);
    if (!pause) {
        Process_Particles();
        if (!stage_completed && (!time_limit || game_time > 0))
            Sprites();
        Fade_Texts();
    }
    if (!skip_frame)
        Draw_Sprites();
    //Draw_Bonuses();
    if (!skip_frame)
        Draw_Particles();
    Set_Bounds();
/*
	if (LIMIT_MAP_DRAWING_AREA)
		PisteDraw_SetClipper(PD_BACKBUFFER,MARGIN_HORI,MARGIN_VERT,MAP_DRAWING_WIDTH+MARGIN_HORI,MAP_DRAWING_HEIGHT+MARGIN_VERT);
	else
		PisteDraw_SetClipper(PD_BACKBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
*/
    if (!skip_frame)
        map->Draw_Walls(camera_x, camera_y, false);
    Draw_Fade_Texts();
    if (settings.show_items)
        Draw_Bottom_Panel();
    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //PisteDraw_SetClipper(PD_BACKBUFFER,0,0,639,479);
    Draw_Game_Top();
/*
	interval = PisteDraw_Font_Write(font2,"episode ",PD_BACKBUFFER,20,SCREEN_HEIGHT-16);
	PisteDraw_Font_Write(font2,episode,PD_BACKBUFFER,20+interval,SCREEN_HEIGHT-16);
*/
/*
	//////////////
	// draw duration
	//////////////
	int minutes = duration/60,
		seconds = duration%60;
	interval = PisteDraw_Font_Write(font2,"time ",PD_BACKBUFFER,272,SCREEN_HEIGHT-20);
	itoa(minutes,number,10);
	interval += PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,272+interval,SCREEN_HEIGHT-20);
	interval += PisteDraw_Font_Write(font1,":",PD_BACKBUFFER,272+interval,SCREEN_HEIGHT-15);
	if (seconds < 10)
		interval += PisteDraw_Font_Write(font2,"0",PD_BACKBUFFER,272+interval,SCREEN_HEIGHT-20);
	itoa(seconds,number,10);
	interval += PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,272+interval,SCREEN_HEIGHT-20);
	/////////////////
	// draw keys
	/////////////////
	interval = PisteDraw_Font_Write(font2,"keys ",PD_BACKBUFFER,450,SCREEN_HEIGHT-20);
	itoa(num_keys,number,10);
	PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,450+interval,SCREEN_HEIGHT-20);
	/////////////////
	// draw items
	/////////////////
	Draw_Items();
*/
    if (draw_info)
        Draw_Info();
    ///////////////////
    // draw framerate
    ///////////////////
    if (show_fps) {
        interval = PisteDraw_Font_Write(font1, "fps: ", PD_BACKBUFFER, 570, 48);
        itoa(fps, number, 10);
        PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 570 + interval, 48);
    }
    ///////////////////
    // draw pause
    ///////////////////
    if (pause)
        PisteDraw_Font_Write(font2, texts->Get_Text(txt_game_paused), PD_BACKBUFFER, MAP_DRAWING_WIDTH / 2 - 82, MAP_DRAWING_HEIGHT / 2 - 9);
    ////////////////////////
    // draw stage completed
    ////////////////////////
    if (stage_completed) {
        Draw_Wave_Text(texts->Get_Text(txt_game_clear), font2, MAP_DRAWING_WIDTH / 2 - 120, MAP_DRAWING_HEIGHT / 2 - 9);
/*
		if (time_limit)
		{
			interval = PK_Piirra_LaineTeksti(texts->Get_Text(txt_game_timebonus),font2,MAP_DRAWING_WIDTH/2-120,MAP_DRAWING_HEIGHT/2+20);
			itoa(fake_points,number,10);
			Draw_Wave_Text(number,font2,MAP_DRAWING_WIDTH/2-120+interval,MAP_DRAWING_HEIGHT/2+20);
		}
*/
    } else if (game_over) {
        if (sprites[player_index].health < 1) {
            Draw_Wave_Text(texts->Get_Text(txt_game_ko), font2, MAP_DRAWING_WIDTH / 2 - 90, MAP_DRAWING_HEIGHT / 2 - 9 - 10);
        } else if (game_time < 1 && time_limit)
            Draw_Wave_Text(texts->Get_Text(txt_game_timeout), font2, MAP_DRAWING_WIDTH / 2 - 67, MAP_DRAWING_HEIGHT / 2 - 9 - 10);
        Draw_Wave_Text(texts->Get_Text(txt_game_tryagain), font2, MAP_DRAWING_WIDTH / 2 - 75, MAP_DRAWING_HEIGHT / 2 - 9 + 10);
    }
    //PisteDraw_Font_Write(font2,"game over!",PD_BACKBUFFER,SCREEN_WIDTH/2-75,SCREEN_HEIGHT/2-9);
/*
	if (PisteInput_KeyDown(DIK_S))
		Blur_Image(PD_BACKBUFFER, 640, 480);
*/
    if (double_speed)
        skip_frame = !skip_frame;
    else
        skip_frame = false;
    if (!skip_frame) {
        PisteWait_Wait(0);//10
        PisteDraw_UpdateScreen();
        PisteWait_Start();
    }
    block_animation = 1 + block_animation % 34;
    return 0;
}

void Draw_Cursor(int x, int y) {
    PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x, y, 621, 461, 640, 480);
}

int Draw_Menu_Rectangle(int left, int top, int right, int bottom, unsigned char color) {
    if (game_in_progress)
        return 0;
    //color = 224;
    if (menu_rectangle.left < left)
        menu_rectangle.left++;
    if (menu_rectangle.left > left)
        menu_rectangle.left--;
    if (menu_rectangle.right < right)
        menu_rectangle.right++;
    if (menu_rectangle.right > right)
        menu_rectangle.right--;
    if (menu_rectangle.top < top)
        menu_rectangle.top++;
    if (menu_rectangle.top > top)
        menu_rectangle.top--;
    if (menu_rectangle.bottom < bottom)
        menu_rectangle.bottom++;
    if (menu_rectangle.bottom > bottom)
        menu_rectangle.bottom--;
    left = (int)menu_rectangle.left;
    right = (int)menu_rectangle.right;
    top = (int)menu_rectangle.top;
    bottom = (int)menu_rectangle.bottom;

    left += (int)(sin_table[(degree * 2) % 359] / 2.0);
    right += (int)(cos_table[(degree * 2) % 359] / 2.0);
    top += (int)(sin_table[((degree * 2) + 20) % 359] / 2.0);
    bottom += (int)(cos_table[((degree * 2) + 40) % 359] / 2.0);
    //PisteDraw_Buffer_Clear(PD_BACKBUFFER,left_bound,top_bound,right_bound,bottom_bound,38);
/*
	double coefficient_y = (bottom_bound - top_bound) / 6.0;
	double dbl_y = top_bound;
	for (int y=0;y<6;y++) {
		PisteDraw_Buffer_Clear(PD_BACKBUFFER,left_bound,int(dbl_y),right_bound,int(dbl_y+coefficient_y),224+y);
		dbl_y += coefficient_y;
	}
*/
    double coefficient_y = (bottom - top) / 19.0;
    double coefficient_x = (right - left) / 19.0;
    double dbl_y = top;
    double dbl_x = left;
    bool dark = true;
    int col = 0; // TODO: local color variable
    for (int y = 0; y < 19; y++) {
        dbl_x = left;
        for (int x = 0; x < 19; x++) {
            //color = (x+y) / 6;
            col = (x / 4) + (y / 3);
            if (dark)
                col /= 2;
            PisteDraw_Buffer_Clear(PD_BACKBUFFER, int(dbl_x), int(dbl_y), int(dbl_x + coefficient_x), int(dbl_y + coefficient_y), color + col);
            dbl_x += coefficient_x;
            dark = !dark;
        }
        dbl_y += coefficient_y;
    }
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1, top - 1, right + 1, top + 2, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1, top - 1, left + 2, bottom + 1, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1, bottom - 2, right + 1, bottom + 1, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, right - 2, top - 1, right + 1, bottom + 1, 0);

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1 + 1, top - 1 + 1, right + 1 + 1, top + 2 + 1, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1 + 1, top - 1 + 1, left + 2 + 1, bottom + 1 + 1, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left - 1 + 1, bottom - 2 + 1, right + 1 + 1, bottom + 1 + 1, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, right - 2 + 1, top - 1 + 1, right + 1 + 1, bottom + 1 + 1, 0);

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left, top, right, top + 1, 153);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left, top, left + 1, bottom, 144);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, left, bottom - 1, right, bottom, 138);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, right - 1, top, right, bottom, 138);

    return 0;
}

bool Draw_Menus_Selection(char *text, int x, int y) {
    int length = strlen(text) * 15;
    if ((mouse_x > x && mouse_x < x + length && mouse_y > y && mouse_y < y + 15) ||
        (selected_menu_id == choice_menu_id)) {
        selected_menu_id = choice_menu_id;
        if (((PisteInput_Mouse_Left() && mouse_x > x && mouse_x < x + length && mouse_y > y && mouse_y < y + 15)
            || PisteInput_KeyDown(DIK_SPACE) || PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1))
            && key_delay == 0) {
            Play_Sound_Menu(menu_sound, 100);
            key_delay = 20;
            return true;
        }
        Draw_Wave_Text(text, font3, x, y);
    } else
        Draw_Wave_Text_Slow(text, font2, x, y);
    //PisteDraw_Font_Write(font2,text,PD_BACKBUFFER,x,y);
    choice_menu_id++;
    return false;
}

int Draw_Menus_Selection_Box(int x, int y, bool variable) {
    if (!variable)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x, y, 473, 124, 473 + 31, 124 + 31);
    else
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x, y, 504, 124, 504 + 31, 124 + 31);
    if ((mouse_x > x && mouse_x < x + 30 && mouse_y > y && mouse_y < y + 31)) {
        /* || (selected_menu_id == choice_menu_id))*/
        if ((PisteInput_Mouse_Left() || PisteInput_KeyDown(DIK_SPACE) || PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1)) && key_delay == 0) {
            Play_Sound_Menu(menu_sound, 100);
            key_delay = 20;
            return true;
        }
    }
    //choice_menu_id++;
    return false;
}

int Draw_Menus_Adjustment_Box(int x, int y) {
    int val = 45;
    int randx = rand() % 3 - rand() % 3;
    int randy = rand() % 3 - rand() % 3;

    if (selected_menu_id == choice_menu_id)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x + randx, y + randy, 566, 124, 566 + 31, 124 + 31);
    else
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x, y, 566, 124, 566 + 31, 124 + 31);
    if (selected_menu_id == choice_menu_id + 1)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x + val + randx, y + randy, 535, 124, 535 + 31, 124 + 31);
    else
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x + val, y, 535, 124, 535 + 31, 124 + 31);
    if ((mouse_x > x && mouse_x < x + 30 && mouse_y > y && mouse_y < y + 31) || (selected_menu_id == choice_menu_id)) {
        if ((PisteInput_Mouse_Left() || PisteInput_KeyDown(DIK_SPACE) || PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1)) && key_delay == 0) {
            Play_Sound_Menu(menu_sound, 100);
            key_delay = 20;
            return 1;
        }
    }
    x += val;
    if ((mouse_x > x && mouse_x < x + 30 && mouse_y > y && mouse_y < y + 31) || (selected_menu_id == choice_menu_id + 1)) {
        if ((PisteInput_Mouse_Left() || PisteInput_KeyDown(DIK_SPACE) || PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1)) && key_delay == 0) {
            Play_Sound_Menu(menu_sound, 100);
            key_delay = 20;
            return 2;
        }
    }
    choice_menu_id += 2;

    return 0;
}

int Draw_Menus_Main_Menu() {
    int my = 0;
    Draw_Menu_Rectangle(160, 200, 640 - 180, 410, 224);
    if (game_in_progress) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_continue), 180, 250 + my)) {
            if ((!game_over && !stage_completed) || end_timer > 1)
                next_game_state = STATE_GAME;
            else
                next_game_state = STATE_MAP;
        }
        my += 20;
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_new_game), 180, 250 + my)) {
        name_edit = true;
        menu_name_index = strlen(player_name);//   0;
        menu_name_prev_char = ' ';
        current_menu = MENU_NAME;
        key_delay = 30;
    }
    my += 20;

    if (game_in_progress) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_save_game), 180, 250 + my)) {
            current_menu = MENU_SAVE;
        }
        my += 20;
    }

    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_load_game), 180, 250 + my)) {
        current_menu = MENU_LOAD;
    }
    my += 20;

    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_controls), 180, 250 + my)) {
        current_menu = MENU_CONTROLS;
    }
    my += 20;

    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_graphics), 180, 250 + my)) {
        current_menu = MENU_GRAPHICS;
    }
    my += 20;

    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_sounds), 180, 250 + my)) {
        current_menu = MENU_SOUNDS;
    }
    my += 20;

    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_exit), 180, 250 + my)) {
        end_game = true;
        PisteDraw_Fade_Palette_Out(PD_FADE_NORMAL);
        music_volume = 0;
    }
    my += 20;

    return 0;
}

// TODO: =>
int Draw_Menus_Name() {
    int my = 0, mx = menu_name_index * 15 + 180 + rand() % 2;
    char character;
    bool mouse_in_area = false;

    Draw_Menu_Rectangle(90, 150, 640 - 90, 480 - 100, 224);

    if (mouse_x > 180 && mouse_x < 180 + 15 * 20 && mouse_y > 255 && mouse_y < 255 + 18)
        mouse_in_area = true;
    if (mouse_in_area && PisteInput_Mouse_Left() && key_delay == 0) {
        name_edit = true;
        menu_name_index = (mouse_x - 180) / 15;
        key_delay = 10;
    }
    if (name_edit && key_delay == 0) {
        if (PisteInput_KeyDown(DIK_LEFT)) {
            menu_name_index--;
            key_delay = 8;
        }
        if (PisteInput_KeyDown(DIK_RIGHT)) {
            menu_name_index++;
            key_delay = 8;
        }
    }
    if (menu_name_index >= 19)
        menu_name_index = 18;
    if (menu_name_index >= (int) strlen(player_name))
        menu_name_index = (int) strlen(player_name);
    if (menu_name_index < 0)
        menu_name_index = 0;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_playermenu_type_name), PD_BACKBUFFER, 180, 224);

    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 180 - 2, 255 - 2, 180 + 20 * 15 + 4, 255 + 18 + 4, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 180, 255, 180 + 20 * 15, 255 + 18, 50);
    if (name_edit) {
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, mx - 2, 254, mx + 16 + 3, 254 + 20 + 3, 0);
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, mx - 1, 254, mx + 16, 254 + 20, 96 + 16);
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, mx + 12, 254, mx + 16, 254 + 20, 96 + 8);
    }

    Draw_Wave_Text_Slow(player_name, font2, 180, 255);
    PisteDraw_Font_Write_Transparent(font3, player_name, PD_BACKBUFFER, 180, 255, 15);
    character = PisteInput_Read_Keyboard();

    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1) && key_delay == 0 && name_edit) {
        name_edit = false;
    }
    if (character != '\0' && (character != menu_name_prev_char || key_delay == 0) && name_edit) {
        menu_name_prev_char = character;
        if (character == '\n') {
            character = '\0';
            name_edit = false;
            selected_menu_id = 1;
        }
        key_delay = 15;
        player_name[menu_name_index] = character;
        menu_name_index++;
        //if (mouse_in_area)
        //	mouse_x += 15;
    }
    if (key_delay == 0) {
        if (PisteInput_KeyDown(DIK_DELETE) || PisteInput_Read_Controller() == PI_KB_BACKSPACE) {
            for (int c = menu_name_index; c < 19; c++)
                player_name[c] = player_name[c + 1];
            player_name[19] = '\0';
            key_delay = 10;
        }
        if (PisteInput_Read_Controller() == PI_KB_BACKSPACE) {
            key_delay = 10;
            menu_name_index--;
            //if (mouse_in_area)
            //	mouse_x -= 15;
        }
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_playermenu_continue), 180, 300)) {
        current_menu = MENU_EPISODES;
        menu_name_index = 0;
        name_edit = false;
        selected_menu_id = choice_menu_id = 1;
        if (num_episodes == 1) {
            strcpy(episode, episodes[2]);
            next_game_state = STATE_MAP;
            game_in_progress = false;
            New_Game();
        }
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_playermenu_clear), 340, 300)) {
        memset(player_name, '\0', sizeof(player_name));
        strcpy(player_name, " ");
        menu_name_index = 0;
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_exit), 180, 400)) {
        current_menu = MENU_MAIN;
        menu_name_index = 0;
        name_edit = false;
    }

    return 0;
}

// TODO: =>
int Draw_Menus_Load() {
    int my = 0, interval = 0;
    char location[100];
    char episode[8];
    char index[4];

    Draw_Menu_Rectangle(40, 70, 640 - 40, 410, 70);

    PisteDraw_Font_Write(font2, texts->Get_Text(txt_loadgame_title), PD_BACKBUFFER, 50, 90);
    PisteDraw_Font_Write(font1, texts->Get_Text(txt_loadgame_info), PD_BACKBUFFER, 50, 110);
    my = -20;

    for (int i = 0; i < MAX_SAVES; i++) {
        itoa(i + 1, index, 10);
        strcpy(location, index);
        strcat(location, ". ");
        strcat(location, saves[i].name);
        if (Draw_Menus_Selection(location, 100, 150 + my))
            Load_Game(i);
        if (strcmp(saves[i].episode, " ") != 0) {
            interval = 0;
            interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_loadgame_episode), PD_BACKBUFFER, 400, 150 + my);
            interval += PisteDraw_Font_Write(font1, saves[i].episode, PD_BACKBUFFER, 400 + interval, 150 + my);
            interval = 0;
            interval += PisteDraw_Font_Write(font1, texts->Get_Text(txt_loadgame_level), PD_BACKBUFFER, 400 + interval, 160 + my);
            itoa(saves[i].stage, episode, 10);
            interval += PisteDraw_Font_Write(font1, episode, PD_BACKBUFFER, 400 + interval, 160 + my);
        }
        my += 22;
    }
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400))
        current_menu = MENU_MAIN;

    return 0;
}

// TODO: =>
int Draw_Menus_Save() {
    int my = 0, interval = 0;
    char location[100];
    char episode[8];
    char index[4];
    Draw_Menu_Rectangle(40, 70, 640 - 40, 410, 224);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_savegame_title), PD_BACKBUFFER, 50, 90);
    PisteDraw_Font_Write(font1, texts->Get_Text(txt_savegame_info), PD_BACKBUFFER, 50, 110);
    my = -20;
    for (int i = 0; i < MAX_SAVES; i++) {
        itoa(i + 1, index, 10);
        strcpy(location, index);
        strcat(location, ". ");
        strcat(location, saves[i].name);
        if (Draw_Menus_Selection(location, 100, 150 + my))
            Save_Game(i);
        if (strcmp(saves[i].episode, " ") != 0) {
            interval = 0;
            interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_savegame_episode), PD_BACKBUFFER, 400, 150 + my);
            interval += PisteDraw_Font_Write(font1, saves[i].episode, PD_BACKBUFFER, 400 + interval, 150 + my);
            interval = 0;
            interval += PisteDraw_Font_Write(font1, texts->Get_Text(txt_savegame_level), PD_BACKBUFFER, 400 + interval, 160 + my);
            itoa(saves[i].stage, episode, 10);
            interval += PisteDraw_Font_Write(font1, episode, PD_BACKBUFFER, 400 + interval, 160 + my);
        }
        my += 22;
    }
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400))
        current_menu = MENU_MAIN;

    return 0;
}

int Draw_Menus_Graphics() {
    int my = 0;
    Draw_Menu_Rectangle(40, 70, 640 - 40, 410, 224);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_gfx_title), PD_BACKBUFFER, 50, 90);
    my += 150;
    if (settings.transparent_objects) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_tfx_on), 180, my))
            settings.transparent_objects = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_tfx_off), 180, my))
            settings.transparent_objects = true;
    }
    if (Draw_Menus_Selection_Box(100, my, settings.transparent_objects)) {
        settings.transparent_objects = !settings.transparent_objects;
    }
    my += 30;
    if (settings.transparent_menu_texts) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_tmenus_on), 180, my))
            settings.transparent_menu_texts = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_tmenus_off), 180, my))
            settings.transparent_menu_texts = true;
    }
    if (Draw_Menus_Selection_Box(100, my, settings.transparent_menu_texts)) {
        settings.transparent_menu_texts = !settings.transparent_menu_texts;
    }
    my += 30;
    if (settings.show_items) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_items_on), 180, my))
            settings.show_items = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_items_off), 180, my))
            settings.show_items = true;
    }
    if (Draw_Menus_Selection_Box(100, my, settings.show_items)) {
        settings.show_items = !settings.show_items;
    }
    my += 30;
    if (settings.weather_effects) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_weather_on), 180, my))
            settings.weather_effects = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_weather_off), 180, my))
            settings.weather_effects = true;
    }
    if (Draw_Menus_Selection_Box(100, my, settings.weather_effects)) {
        settings.weather_effects = !settings.weather_effects;
    }
    my += 30;
    if (settings.background_sprites) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_bgsprites_on), 180, my))
            settings.background_sprites = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_bgsprites_off), 180, my))
            settings.background_sprites = true;
    }
    if (Draw_Menus_Selection_Box(100, my, settings.background_sprites)) {
        settings.background_sprites = !settings.background_sprites;
    }
    my += 30;
    if (double_speed) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_speed_double), 180, my))
            double_speed = false;
    } else {
        if (Draw_Menus_Selection(texts->Get_Text(txt_gfx_speed_normal), 180, my))
            double_speed = true;
    }
    if (Draw_Menus_Selection_Box(100, my, double_speed)) {
        double_speed = !double_speed;
    }
    my += 30;
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400))
        current_menu = MENU_MAIN;

    return 0;
}

int Draw_Menus_Sounds() {
    int my = 0;
    Draw_Menu_Rectangle(40, 70, 640 - 40, 410, 224);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_sound_title), PD_BACKBUFFER, 50, 90);
    my += 20;
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 404, 224 + my, 404 + sound_volume, 244 + my, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 400, 220 + my, 400 + sound_volume, 240 + my, 81);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_sound_sfx_volume), PD_BACKBUFFER, 180, 200 + my);
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_sound_less), 180, 200 + my))
        if (sound_volume > 0)
            sound_volume -= 5;
    if (Draw_Menus_Selection(texts->Get_Text(txt_sound_more), 180 + 8 * 15, 200 + my))
        if (sound_volume < 100)
            sound_volume += 5;
    if (sound_volume < 0)
        sound_volume = 0;
    if (sound_volume > 100)
        sound_volume = 100;
    my += 40;
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 404, 224 + my, 404 + int(music_max_volume * 1.56), 244 + my, 0);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 400, 220 + my, 400 + int(music_max_volume * 1.56), 240 + my, 112);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_sound_music_volume), PD_BACKBUFFER, 180, 200 + my);
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_sound_less), 180, 200 + my))
        if (music_max_volume > 0)
            music_max_volume -= 4;
    if (Draw_Menus_Selection(texts->Get_Text(txt_sound_more), 180 + 8 * 15, 200 + my))
        if (music_max_volume < 64)
            music_max_volume += 4;
    if (music_max_volume < 0)
        music_max_volume = 0;
    if (music_max_volume > 64)
        music_max_volume = 64;
    music_volume = music_max_volume;
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400))
        current_menu = MENU_MAIN;

    return 0;
}

int Draw_Menus_Controls() {
    int my = 0;
    Draw_Menu_Rectangle(40, 70, 640 - 40, 410, 224);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_title), PD_BACKBUFFER, 50, 90);
    my = 40;
    if (read_controls_menu > 0) {
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, 299, 74 + my + read_controls_menu * 20, 584, 94 + my + read_controls_menu * 20, 0);
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, 295, 70 + my + read_controls_menu * 20, 580, 90 + my + read_controls_menu * 20, 50);
    }
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_moveleft), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_moveright), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_jump), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_duck), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_walkslow), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_eggattack), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_doodleattack), PD_BACKBUFFER, 100, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_controls_useitem), PD_BACKBUFFER, 100, 90 + my);
    my += 20;

    my = 40;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_left), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_right), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_jump), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_down), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_run), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_attack1), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_attack2), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
    PisteDraw_Font_Write(font2, PisteInput_Read_Controller_Name(control_use_item), PD_BACKBUFFER, 310, 90 + my);
    my += 20;
/*
	if (mouse_x > 310 && mouse_x < 580 && mouse_y > 130 && mouse_y < my-20) {
		read_controls_menu = (mouse_y - 120) / 20;

		if (read_controls_menu < 0 || read_controls_menu > 8)
			read_controls_menu = 0;
		else
			key_delay = 25;
	}
*/
    if (read_controls_menu == 0) {
        if (Draw_Menus_Selection(texts->Get_Text(txt_controls_edit), 100, 90 + my))
            read_controls_menu = 1;
    }
    my += 30;
    if (Draw_Menus_Selection(texts->Get_Text(txt_controls_kbdef), 100, 90 + my)) {
        control_left = PI_KB_LEFT;
        control_right = PI_KB_RIGHT;
        control_jump = PI_KB_UP;
        control_down = PI_KB_DOWN;
        control_run = PI_KB_RALT;
        control_attack1 = PI_KB_RCONTROL;
        control_attack2 = PI_KB_RSHIFT;
        control_use_item = PI_KB_SPACE;
        read_controls_menu = 0;
    }
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_controls_gp4def), 100, 90 + my)) {
        control_left = PI_CONTROLLER_1_MOVE_LEFT;
        control_right = PI_CONTROLLER_1_MOVE_RIGHT;
        control_jump = PI_CONTROLLER_1_MOVE_UP;
        control_down = PI_CONTROLLER_1_MOVE_DOWN;
        control_run = PI_CONTROLLER_1_BUTTON_2;
        control_attack1 = PI_CONTROLLER_1_BUTTON_1;
        control_attack2 = PI_CONTROLLER_1_BUTTON_3;
        control_use_item = PI_CONTROLLER_1_BUTTON_4;
        read_controls_menu = 0;
    }
    my += 20;
    if (Draw_Menus_Selection(texts->Get_Text(txt_controls_gp6def), 100, 90 + my)) {
        control_left = PI_CONTROLLER_1_MOVE_LEFT;
        control_right = PI_CONTROLLER_1_MOVE_RIGHT;
        control_jump = PI_CONTROLLER_1_MOVE_UP;//PI_CONTROLLER_1_BUTTON_1;
        control_down = PI_CONTROLLER_1_MOVE_DOWN;
        control_run = PI_CONTROLLER_1_BUTTON_2;
        control_attack1 = PI_CONTROLLER_1_BUTTON_1;
        control_attack2 = PI_CONTROLLER_1_BUTTON_4;
        control_use_item = PI_CONTROLLER_1_BUTTON_6;
        read_controls_menu = 0;
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400)) {
        current_menu = MENU_MAIN;
        read_controls_menu = 0;
    }
    unsigned char k = 0;
    if (key_delay == 0 && read_controls_menu > 0) {
        k = PisteInput_Read_Controller();
        if (k != 0) {
            switch (read_controls_menu) {
                case 1:
                    control_left = k;
                    break;
                case 2:
                    control_right = k;
                    break;
                case 3:
                    control_jump = k;
                    break;
                case 4:
                    control_down = k;
                    break;
                case 5:
                    control_run = k;
                    break;
                case 6:
                    control_attack1 = k;
                    break;
                case 7:
                    control_attack2 = k;
                    break;
                case 8:
                    control_use_item = k;
                    break;
                default:
                    Play_Sound_Menu(shoot_sound, 100);
                    break;
            }
            key_delay = 20;
            read_controls_menu++;
        }
        if (read_controls_menu > 8) {
            read_controls_menu = 0;
            selected_menu_id = 1;
        }
    }
    my += 20;
    return 0;
}

int Draw_Menus_Episodes() {
    int my = 0;
    Draw_Menu_Rectangle(110, 130, 640 - 110, 450, 224);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_episodes_choose_episode), PD_BACKBUFFER, 50, 90);
    my += 80;
    if (num_episodes - 2 > 10) {
        char number[20];
        int interval = 90;
        int x = 50,//500,
        y = 50;//300;
        //interval += PisteDraw_Font_Write(font1,"page:",PD_BACKBUFFER,x,y+40);
        itoa(episode_page + 1, number, 10);
        interval += PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, x + interval, y + 20);
        interval += PisteDraw_Font_Write(font1, "/", PD_BACKBUFFER, x + interval, y + 20);
        itoa((num_episodes / 10) + 1, number, 10);
        interval += PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, x + interval, y + 20);
        int button = Draw_Menus_Adjustment_Box(x, y);
        if (button == 1 && episode_page > 0)
            episode_page--;
        if (button == 2 && (episode_page * 10) + 10 < num_episodes)
            episode_page++;
    }
    for (int i = (episode_page * 10) + 2; i < (episode_page * 10) + 12; i++) {
        if (strcmp(episodes[i], "") != 0) {
            if (Draw_Menus_Selection(episodes[i], 220, 90 + my)) {
                strcpy(episode, episodes[i]);
                Load_Additional_Information();
                next_game_state = STATE_MAP;
                game_in_progress = false;
                New_Game();
                //PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
            }
            my += 20;
        }
    }
    /* page / all */
/*
    if (num_episodes-2 > 10) {
		char number[20];
		int interval = 0;
		int x = 500,
			y = 300;
		//interval += PisteDraw_Font_Write(font1,"page:",PD_BACKBUFFER,x,y+40);
		itoa(episode_page+1,number,10);
		interval += PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,x+interval,y+40);
		interval += PisteDraw_Font_Write(font1,"/",PD_BACKBUFFER,x+interval,y+40);
		itoa((num_episodes/10)+1,number,10);
		interval += PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,x+interval,y+40);
		itoa((episode_page*10)+1,number,10);
		interval += PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,500,340);
		interval += PisteDraw_Font_Write(font1,"-",PD_BACKBUFFER,500+interval,340);
		if ((episode_page*10)+10 <= num_episodes-2)
			itoa((episode_page*10)+10,number,10);
		else
			itoa(num_episodes-2,number,10);
		interval += PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,500+interval,340);
		interval += PisteDraw_Font_Write(font1,"/",PD_BACKBUFFER,500+interval,340);
		itoa(num_episodes-2,number,10);
		interval += PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,500+interval,340);
		int button = Draw_Menus_Adjustment_Box(x,y);
		if (button == 1 && episode_page > 0)
			episode_page--;
		if (button == 2 && (episode_page*10)+10 < num_episodes)
			episode_page++;
	}
*/
/*
	char number[20];
	itoa(num_episodes,number,10);
	PisteDraw_Font_Write(font1,number,PD_BACKBUFFER,100,240);
*/
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 180, 400)) {
        current_menu = MENU_MAIN;
        my += 20;
    }
    PisteDraw_Font_Write(font1, texts->Get_Text(txt_episodes_get_more), PD_BACKBUFFER, 140, 440);

    return 0;
}

int Draw_Menus() {
    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
    PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 0, 0);
    choice_menu_id = 1;

    switch (current_menu) {
        case MENU_MAIN:
            Draw_Menus_Main_Menu();
            break;
        case MENU_EPISODES:
            Draw_Menus_Episodes();
            break;
        case MENU_GRAPHICS:
            Draw_Menus_Graphics();
            break;
        case MENU_SOUNDS:
            Draw_Menus_Sounds();
            break;
        case MENU_CONTROLS:
            Draw_Menus_Controls();
            break;
        case MENU_NAME:
            Draw_Menus_Name();
            break;
        case MENU_LOAD:
            Draw_Menus_Load();
            break;
        case MENU_SAVE:
            Draw_Menus_Save();
            break;
        default:
            Draw_Menus_Main_Menu();
            break;
    }
    //Draw_Particles();
    Draw_Cursor(mouse_x, mouse_y);
    PisteWait_Wait(0);//10
    //if (PisteDraw_Fade_Palette_Done())
    //	PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
    PisteDraw_UpdateScreen();
    PisteWait_Start();

    return 0;
}

// TODO: =>
int Draw_Button(int x, int y, int t) {
    int count = 0;
    t = t * 25;
    int blink = 50 + (int) (sin_table[degree % 360] * 2);
    if (blink < 0)
        blink = 0;
    //PisteDraw_Buffer_Flip_Fast(game_image,PD_BACKBUFFER,x,y,1+t,58,23+t,80);
    if (mouse_x > x && mouse_x < x + 17 && mouse_y > y && mouse_y < y + 17) {
        if (key_delay == 0 && (PisteInput_Mouse_Left() || PisteInput_KeyDown(DIK_SPACE) || PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1))) {
            key_delay = 30;
            return 2;
        }
        if (t == 25)
            Draw_Transparent_Object(game_image_sysmem, 247, 1, 25, 25, x - 2, y - 2, 60, 96);
        if (t == 0)
            Draw_Transparent_Object(game_image_sysmem, 247, 1, 25, 25, x - 4, y - 4, 60, 32);
        if (t == 50)
            Draw_Transparent_Object(game_image_sysmem, 247, 1, 25, 25, x - 4, y - 4, 60, 64);
        count = 1;
    }
    if (t == 25)
        Draw_Transparent_Object(game_image_sysmem, 247, 1, 25, 25, x - 2, y - 2, blink, 96);
    if (((degree / 45) + 1) % 4 == 0 || t == 0)
        PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, x, y, 1 + t, 58, 23 + t, 80);

    return count;
}

// TODO: =>
int Draw_Map() {
    char number[20];
    int interval = 20;

    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
    PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 0, 0);
    PisteDraw_Font_Write(font4, episode, PD_BACKBUFFER, 100 + 2, 72 + 2);
    PisteDraw_Font_Write(font2, episode, PD_BACKBUFFER, 100, 72);
    PisteDraw_Font_Write(font4, texts->Get_Text(txt_map_total_score), PD_BACKBUFFER, 100 + 2, 92 + 2);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_map_total_score), PD_BACKBUFFER, 100, 92);//250,80
    ltoa(points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 100 + interval + 2 + 15, 92 + 2);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 100 + interval + 15, 92);

    if (episode_scores.episode_top_scores > 0) {
        interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_map_episode_best_player), PD_BACKBUFFER, 360, 72);
        PisteDraw_Font_Write(font1, episode_scores.episode_top_player, PD_BACKBUFFER, 360 + interval + 10, 72);
        interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_map_episode_hiscore), PD_BACKBUFFER, 360, 92);
        ltoa(episode_scores.episode_top_scores, number, 10);
        PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 360 + interval + 15, 92);
    }
    interval = PisteDraw_Font_Write(font1, texts->Get_Text(txt_map_next_level), PD_BACKBUFFER, 100, 120);
    ltoa(stage, number, 10);
    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, 100 + interval + 15, 120);
    //Draw_Particles();
    if (num_stages == 0) {
        PisteDraw_Font_Write(font2, texts->Get_Text(txt_episodes_no_maps), PD_BACKBUFFER, 180, 290);
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_mainmenu_return), 100, 430)) {
        next_game_state = STATE_MENUS;
        current_menu = MENU_MAIN;
    }
    int button_x = 0, button_y = 0;
    int type = 0;
    int count;
    int minutes = 0, seconds = 0;
    int icon;
    int sinx = 0, cosy = 0;
    int pekka_frame = 0;

    for (int i = 0; i <= num_stages; i++) {
        if (strcmp(stages[i].name, "") != 0 && stages[i].order > 0) {
            type = 0;                            //0 gray
            if (stages[i].order == stage)
                type = 1;                        //1 green
            if (stages[i].order > stage)
                type = 2;                        //2 orange
            if (stages[i].completed)
                type = 0;

            if (stages[i].x == 0)
                stages[i].x = 142 + i * 30;
            if (stages[i].y == 0)
                stages[i].y = 270;
            icon = stages[i].icon;

            //PisteDraw_Buffer_Flip_Fast(game_image,PD_BACKBUFFER,stages[i].x-4,stages[i].y-4-30,1+(icon*27),452,27+(icon*27),478);
            PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, stages[i].x - 9, stages[i].y - 14, 1 + (icon * 28), 452, 28 + (icon * 28), 479);
            if (type == 1) {
                //PisteDraw_Buffer_Flip_Fast(game_image,PD_BACKBUFFER,stages[i].x-30,stages[i].y-4,157,46,181,79);
                sinx = (int) (sin_table[degree % 360] / 2);
                cosy = (int) (cos_table[degree % 360] / 2);
                pekka_frame = 28 * ((degree % 360) / 120);
                PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, stages[i].x + sinx - 12, stages[i].y - 17 + cosy, 157 + pekka_frame, 46, 181 + pekka_frame, 79);
            }
            count = Draw_Button(stages[i].x - 5, stages[i].y - 10, type);

            // if clicked
            if (count == 2) {
                if (type < 2 || (cheats && type > 0)) {//== 1
                    strcpy(next_map, stages[i].file);
                    //stage = i;
                    current_stage_index = i;
                    switch_from_map_to_game = true;
                    PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
                    music_volume = 0;
                    Play_Sound_Menu(crow_sound, 90);
                } else
                    Play_Sound_Menu(shoot_sound, 100);
            }
            itoa(stages[i].order, number, 10);
            PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, stages[i].x - 12 + 2, stages[i].y - 29 + 2);
            //PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,stages[i].x+3,stages[i].y-27);
            if (count > 0) {
                int info_x = 489 + 3, info_y = 341 - 26;
/*
				PisteDraw_Buffer_Tayta(PD_BACKBUFFER,stages[i].x-3, stages[i].y+26,stages[i].x + 130, stages[i].y+26+120,1);
				PisteDraw_Buffer_Clear(PD_BACKBUFFER,stages[i].x-2, stages[i].y+27,stages[i].x + 129, stages[i].y+25+120,43);
				PisteDraw_Buffer_Clear(PD_BACKBUFFER,stages[i].x-1, stages[i].y+28,stages[i].x + 128, stages[i].y+24+120,38);
*/
                PisteDraw_Buffer_Flip_Fast(game_image, PD_BACKBUFFER, info_x - 3, info_y + 26, 473, 0, 607, 121);
                PisteDraw_Font_Write(font1, stages[i].name, PD_BACKBUFFER, info_x, info_y + 30);

                if (episode_scores.scores[i] > 0) {
                    PisteDraw_Font_Write_Transparent(font1, texts->Get_Text(txt_map_level_best_player), PD_BACKBUFFER, info_x, info_y + 50, 75);
                    PisteDraw_Font_Write(font1, episode_scores.top_players[i], PD_BACKBUFFER, info_x, info_y + 62);
                    interval = 8 + PisteDraw_Font_Write_Transparent(font1, texts->Get_Text(txt_map_level_hiscore), PD_BACKBUFFER, info_x, info_y + 74, 75);
                    ltoa(episode_scores.scores[i], number, 10);
                    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, info_x + interval, info_y + 75);
                }
                if (episode_scores.times[i] > 0) {
                    PisteDraw_Font_Write_Transparent(font1, texts->Get_Text(txt_map_level_fastest_player), PD_BACKBUFFER, info_x, info_y + 98, 75);
                    PisteDraw_Font_Write(font1, episode_scores.fastest_players[i], PD_BACKBUFFER, info_x, info_y + 110);

                    interval = 8 + PisteDraw_Font_Write_Transparent(font1, texts->Get_Text(txt_map_level_best_time), PD_BACKBUFFER, info_x, info_y + 122, 75);
                    minutes = episode_scores.times[i] / 60;
                    seconds = episode_scores.times[i] % 60;

                    itoa(minutes, number, 10);
                    interval += PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, info_x + interval, info_y + 122);
                    interval += PisteDraw_Font_Write(font1, ":", PD_BACKBUFFER, info_x + interval, info_y + 122);
                    itoa(seconds, number, 10);
                    PisteDraw_Font_Write(font1, number, PD_BACKBUFFER, info_x + interval, info_y + 122);
                }
            }
        }
    }
    Draw_Cursor(mouse_x, mouse_y);
    PisteWait_Wait(0);//10
    PisteDraw_UpdateScreen();
    PisteWait_Start();

    return 0;
}

// TODO: =>
int Draw_Scoreboard() {
    char number[20];
    int interval = 20;
    int my = 0, x, y;
    int cube, degree;
    int color = 0, factor;

    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
    PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 0, 0);

    for (degree = 0; degree < 18; degree++) {
        factor = (int) (cos_table[(degree + degree * 3) % 180]);
        x = (int) (sin_table[(degree + degree * 10) % 360] * 2) + factor;
        y = (int) (cos_table[(degree + degree * 10) % 360] * 2);//10 | 360 | 2
        //PisteDraw_Buffer_Flip_Fast(game_image,PD_BACKBUFFER,320+x,240+y,157,46,181,79);
        cube = (int) (sin_table[(degree + degree * 3) % 360]);
        if (cube < 0) cube = -cube;
        PisteDraw_Buffer_Clear(PD_BACKBUFFER, 320 - x, 240 - y, 320 - x + cube, 240 - y + cube, COLOR_TURQUOISE + 8);
    }
//	unsigned char *buffer_source = NULL,
//		  *buffer_target = NULL;
//	unsigned long source_buffer_width,
//		  target_buffer_width;

//	PisteDraw_Draw_Begin(game_image,    *&buffer_source, (unsigned long &)source_buffer_width);
//	PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer_target, (unsigned long &)target_buffer_width);
    for (degree = 0; degree < 18; degree++) {
        x = (int) (sin_table[(degree + degree * 10) % 360] * 3);
        y = (int) (cos_table[(degree + degree * 10) % 360] * 3);//10 | 360 | 3
        //PisteDraw_Buffer_Flip_Fast(game_image,PD_BACKBUFFER,320+x,240+y,157,46,181,79);
        cube = (int) (sin_table[(degree + degree * 2) % 360]) + 18;
        if (cube < 0) cube = -cube;//0;//
        if (cube > 100) cube = 100;
        //PisteDraw_Buffer_Clear(PD_BACKBUFFER,320+x,240+y,320+x+cube,240+y+cube,COLOR_TURQUOISE+10);
        Draw_Transparent_Object(game_image_sysmem, 247, 1, 25, 25, 320 + x, 240 + y, cube, 32);
//		Draw_Transparent_Object3(game_image, 247, 1, 25, 25, 320+x, 240+y, cube, 32);
//		Draw_Transparent_Object4(247, 1, 25, 25, 320+x, 240+y, cube, 32, buffer_source, buffer_target, source_buffer_width, target_buffer_width);
    }
//	PisteDraw_Draw_End(PD_BACKBUFFER);
//	PisteDraw_Draw_End(game_image);
    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_title), PD_BACKBUFFER, 100 + 2, 72 + 2);
    PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_title), PD_BACKBUFFER, 100, 72);
    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_level_score), PD_BACKBUFFER, 100 + 2, 102 + 2);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_level_score), PD_BACKBUFFER, 100, 102);//250,80
    fake_points = bonus_points + time_points + energy_points + item_points + rescue_points;
    ltoa(fake_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 102 + 2);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 102);
    my = 0;

    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_bonus_score), PD_BACKBUFFER, 100 + 2, 192 + 2 + my);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_bonus_score), PD_BACKBUFFER, 100, 192 + my);
    ltoa(bonus_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 192 + 2 + my);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 192 + my);
    my += 30;

    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_time_score), PD_BACKBUFFER, 100 + 2, 192 + 2 + my);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_time_score), PD_BACKBUFFER, 100, 192 + my);
    ltoa(time_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 192 + 2 + my);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 192 + my);
    my += 30;

    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_energy_score), PD_BACKBUFFER, 100 + 2, 192 + 2 + my);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_energy_score), PD_BACKBUFFER, 100, 192 + my);
    ltoa(energy_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 192 + 2 + my);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 192 + my);
    my += 30;

    PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_item_score), PD_BACKBUFFER, 100 + 2, 192 + 2 + my);
    interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_item_score), PD_BACKBUFFER, 100, 192 + my);
    ltoa(item_points, number, 10);
    PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 192 + 2 + my);
    PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 192 + my);
    my += 30;
/*
	ltoa(fake_points,number,10);
	PisteDraw_Font_Write(font4,number,PD_BACKBUFFER,400+2,192+2);
	PisteDraw_Font_Write(font2,number,PD_BACKBUFFER,400,192);
*/
    x = 110;
    y = 192 + my;

    for (int i = 0; i < MAX_ITEMS; i++)
        if (items[i] != NULL) {
            items[i]->Draw(x - items[i]->width / 2, y - items[i]->height / 2, 0);
            x += 38;
        }
    my += 10;

    if (score_calculation_phase >= 4) {
        PisteDraw_Font_Write(font4, texts->Get_Text(txt_score_screen_total_score), PD_BACKBUFFER, 100 + 2, 192 + 2 + my);
        interval = PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_total_score), PD_BACKBUFFER, 100, 192 + my);//250,80
        ltoa(points, number, 10);
        PisteDraw_Font_Write(font4, number, PD_BACKBUFFER, 400 + 2, 192 + 2 + my);
        PisteDraw_Font_Write(font2, number, PD_BACKBUFFER, 400, 192 + my);
        my += 25;
        if (stage_new_record) {
            PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_new_level_hiscore), PD_BACKBUFFER, 100 + rand() % 2, 192 + my + rand() % 2);
            my += 25;
        }
        if (stage_new_record_time) {
            PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_new_level_best_time), PD_BACKBUFFER, 100 + rand() % 2, 192 + my + rand() % 2);
            my += 25;
        }
        if (episode_new_record) {
            PisteDraw_Font_Write(font2, texts->Get_Text(txt_score_screen_new_episode_hiscore), PD_BACKBUFFER, 100 + rand() % 2, 192 + my + rand() % 2);
            my += 25;
        }
    }
    if (Draw_Menus_Selection(texts->Get_Text(txt_score_screen_continue), 100, 430)) {
        music_volume = 0;
        switch_from_score_to_map = true;
        PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
        //next_game_state = STATE_MAP;
    }
    Draw_Cursor(mouse_x, mouse_y);
    PisteWait_Wait(0);//10
    PisteDraw_UpdateScreen();
    PisteWait_Start();

    return 0;
}

void Draw_Intro_Text(char *text, int font, int x, int y, unsigned long start_time, unsigned long end_time) {
    int percent = 100;
    if (intro_counter > start_time && intro_counter < end_time) {
        if (intro_counter - start_time < 100)
            percent = intro_counter - start_time;
        if (end_time - intro_counter < 100)
            percent = end_time - intro_counter;
        if (percent > 0) {
            if (percent < 100)
                PisteDraw_Font_Write_Transparent(font, text, PD_BACKBUFFER, x, y, percent);
            else
                PisteDraw_Font_Write(font, text, PD_BACKBUFFER, x, y);
        }
    }
}

// TODO: =>
int Draw_Intro() {
    unsigned long score_logo_start = 300;
    unsigned long score_logo_end = score_logo_start + 500;
    unsigned long credits_start = score_logo_end + 80;
    unsigned long credits_end = credits_start + 700;
    unsigned long testers_start = credits_end + 80;
    unsigned long testers_end = testers_start + 700;
    unsigned long translator_start = testers_end + 100;
    unsigned long translator_end = translator_start + 300;
    unsigned long midas_start = translator_end + 100;
    unsigned long midas_end = midas_start + 300;

    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
    PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 280, 80, 280, 80, 640, 480);

    if ((intro_counter / 10) % 50 == 0)
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 353, 313, 242, 313, 275, 432);
    if (intro_counter > score_logo_start && intro_counter < score_logo_end) {
        //int x = intro_counter - score_logo_start - 194;
        int factor = 120 / (intro_counter - score_logo_start);
        int x = 120 - factor;
        if (x > 120)
            x = 120;
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER,/*120*/x, 230, 37, 230, 194, 442);
        Draw_Intro_Text(texts->Get_Text(txt_intro_presents), font1, 230, 400, score_logo_start, score_logo_end - 20);
    }
    if (intro_counter > credits_start) {
        Draw_Intro_Text(texts->Get_Text(txt_intro_a_game_by), font1, 120, 230, credits_start, credits_end);
        Draw_Intro_Text("janne kivilahti", font1, 120, 250, credits_start + 20, credits_end + 20);
        Draw_Intro_Text(texts->Get_Text(txt_intro_original), font1, 120, 275, credits_start + 40, credits_end + 40);
        Draw_Intro_Text("antti suuronen 1998", font1, 120, 295, credits_start + 50, credits_end + 50);
    }

    if (intro_counter > testers_start) {
        Draw_Intro_Text(texts->Get_Text(txt_intro_tested_by), font1, 120, 230, testers_start, testers_end);
        Draw_Intro_Text("antti suuronen", font1, 120, 250, testers_start + 10, testers_end + 10);
        Draw_Intro_Text("toni hurskainen", font1, 120, 260, testers_start + 20, testers_end + 20);
        Draw_Intro_Text("juho rytk�nen", font1, 120, 270, testers_start + 30, testers_end + 30);
        Draw_Intro_Text("annukka korja", font1, 120, 280, testers_start + 40, testers_end + 40);
        Draw_Intro_Text(texts->Get_Text(txt_intro_thanks_to), font1, 120, 300, testers_start + 70,
                        testers_end + 70);
        Draw_Intro_Text("oskari raunio", font1, 120, 310, testers_start + 70, testers_end + 70);
        Draw_Intro_Text("assembly organization", font1, 120, 320, testers_start + 70, testers_end + 70);
    }
    if (intro_counter > translator_start) {
        Draw_Intro_Text(texts->Get_Text(txt_intro_translation), font1, 120, 230, translator_start, translator_end);
        Draw_Intro_Text(texts->Get_Text(txt_intro_translator), font1, 120, 250, translator_start + 20, translator_end + 20);
    }
    if (intro_counter > midas_start) {
        Draw_Intro_Text("housemarque sound system", font1, 120, 230, midas_start, midas_end);
        Draw_Intro_Text("petteri kangaslampi", font1, 120, 250, midas_start + 20, midas_end + 20);
        Draw_Intro_Text("jarno paananen", font1, 120, 270, midas_start + 40, midas_end + 40);
    }
    PisteWait_Wait(0);
    PisteDraw_UpdateScreen();
    PisteWait_Start();

    return 0;
}

// TODO: =>
int Draw_Ending_Character(int x, int y, int type, int plus, int flutter) {
    int frm = 0;
    int yk = 0;
    if (type == 1) { // Pekka
        frm = 1;
        if ((degree / 10) % 10 == flutter) frm = 0;
        yk = (int) sin_table[(degree % 360)];
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x + 3, y + 56, 4, 63, 29, 69);
        if (yk < 0) {
            y += yk;
            frm = 2;
        }
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y, 1 + frm * 35, 1, 32 + frm * 35, 59);
    }
    if (type == 2) { // chicken (looking right)
        frm = 0;
        if ((degree / 10) % 10 == flutter) frm = 1;
        yk = (int) cos_table[((degree + plus) % 360)];
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x + 3, y + 36, 4, 63, 29, 69);
        if (yk < 0) {
            y += yk;
            frm = 2;
        }
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y, 106 + frm * 37, 1, 139 + frm * 37, 38);
    }
    if (type == 3) { // chicken (looking left)
        frm = 0;
        if ((degree / 10) % 10 == flutter) frm = 1;
        yk = (int) cos_table[((degree + plus) % 360)];
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x + 3, y + 36, 4, 63, 29, 69);
        if (yk < 0) {
            y += yk;
            frm = 2;
        }
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y, 106 + frm * 37, 41, 139 + frm * 37, 77);
    }
    if (type == 4) { // little chicken (looking right)
        frm = 0;
        if ((degree / 10) % 10 == flutter) frm = 1;
        yk = (int) sin_table[(((degree * 2) + plus) % 360)];
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x + 3, y + 27, 4, 63, 29, 69);
        if (yk < 0) {
            y += yk;
            frm = 2;
        }
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y, 217 + frm * 29, 1, 243 + frm * 29, 29);
    }
    if (type == 5) { // little chicken (looking left)
        frm = 0;
        if ((degree / 10) % 10 == flutter) frm = 1;
        yk = (int) sin_table[(((degree * 2) + plus) % 360)];
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y + 27, 4, 63, 29, 69);
        if (yk < 0) {
            y += yk;
            frm = 2;
        }
        PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, x, y, 217 + frm * 29, 33, 243 + frm * 29, 61);
    }

    return 0;
}

// TODO: =>
int Draw_Ending() {
    unsigned long congratulations_start = 300;
    unsigned long congratulations_end = congratulations_start + 1000;
    unsigned long the_end_start = congratulations_end + 80;
    unsigned long the_end_end = the_end_start + 3000;
/*
	unsigned long testers_start	= credits_end + 80;
	unsigned long testers_end	= testers_start + 700;
	unsigned long translator_start		= testers_end + 100;
	unsigned long translator_end	= translator_start + 300;
*/
    PisteDraw_SetClipper(PD_BACKBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0);
    PisteDraw_Buffer_Flip_Fast(background_image, PD_BACKBUFFER, 320 - 233 / 2, 240 - 233 / 2, 6, 229, 239, 462);

    Draw_Ending_Character(345, 244, 3, 30, 2);
    Draw_Ending_Character(276, 230, 2, 50, 3);
    Draw_Ending_Character(217, 254, 4, 0, 4);

    Draw_Ending_Character(305, 240, 1, 0, 1);
    Draw_Ending_Character(270, 284, 2, 20, 1);
    Draw_Ending_Character(360, 284, 5, 60, 2);

    if (end_counter > congratulations_start) {
        Draw_Intro_Text(texts->Get_Text(txt_end_congratulations), font2, 220, 380, congratulations_start,
                        congratulations_end);
        Draw_Intro_Text(texts->Get_Text(txt_end_chickens_saved), font1, 220, 402, congratulations_start + 30,
                        congratulations_end + 30);
    }
    if (end_counter > the_end_start) {
        Draw_Intro_Text(texts->Get_Text(txt_end_the_end), font2, 280, 190, the_end_start, the_end_end);
    }
/*
	if (intro_counter > credits_start) {
		PK_Piirra_Intro_Teksti(texts->Hae_Teksti(txt_intro_a_game_by),font1, 120, 230, credits_start, credits_end);
		PK_Piirra_Intro_Teksti("janne kivilahti",			font1, 120, 250, credits_start+20, credits_end+20);
		PK_Piirra_Intro_Teksti(texts->Hae_Teksti(txt_intro_original), font1, 120, 275, credits_start+40, credits_end+40);
		PK_Piirra_Intro_Teksti("antti suuronen 1998",		font1, 120, 295, credits_start+50, credits_end+50);
	}
	if (intro_counter > testers_start) {
		PK_Piirra_Intro_Teksti(texts->Hae_Teksti(txt_intro_tested_by),font1, 120, 230, testers_start, testers_end);
		PK_Piirra_Intro_Teksti("antti suuronen",			font1, 120, 250, testers_start+10, testers_end+10);
		PK_Piirra_Intro_Teksti("toni hurskainen",			font1, 120, 260, testers_start+20, testers_end+20);
		PK_Piirra_Intro_Teksti("juho rytk�nen",				font1, 120, 270, testers_start+30, testers_end+30);
		PK_Piirra_Intro_Teksti("annukka korja",				font1, 120, 280, testers_start+40, testers_end+40);
		PK_Piirra_Intro_Teksti(texts->Hae_Teksti(txt_intro_thanks_to),font1, 120, 300, testers_start+70, testers_end+70);
		PK_Piirra_Intro_Teksti("oskari raunio",				font1, 120, 310, testers_start+70, testers_end+70);
		PK_Piirra_Intro_Teksti("assembly organization",		font1, 120, 320, testers_start+70, testers_end+70);
	}
	if (intro_counter > translator_start) {
		PK_Piirra_Intro_Teksti(texts->Hae_Teksti(txt_intro_translation), font1, 120, 230, translator_start, translator_end);
		Draw_Intro_Text(texts->Get_Text(txt_intro_translator),  font1, 120, 250, translator_start+20, translator_end+20);
	}
	if ((intro_counter / 10) % 50 == 0)
		PisteDraw_Buffer_Flip_Fast(background_image,PD_BACKBUFFER,353, 313, 242, 313, 275, 432);
*/
    PisteWait_Wait(0);
    PisteDraw_UpdateScreen();
    PisteWait_Start();

    return 0;
}

/* OTHER ROUTINES -------------------------------------------------------*/
int Main_Intro(void) {
    mouse_x += PisteInput_Mouse_X(0);
    mouse_y += PisteInput_Mouse_Y(0);
    mouse_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 30;
    mouse_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 30;

    if (PisteInput_KeyDown(DIK_LEFT))
        mouse_x -= 3;
    if (PisteInput_KeyDown(DIK_RIGHT))
        mouse_x += 3;
    if (PisteInput_KeyDown(DIK_UP))
        mouse_y -= 3;
    if (PisteInput_KeyDown(DIK_DOWN))
        mouse_y += 3;

    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 640 - 19)
        mouse_x = 640 - 19;
    if (mouse_y > 480 - 19)
        mouse_y = 480 - 19;

    Draw_Intro();
    degree = 1 + degree % 360;
    intro_counter++;

    if (switch_from_intro_to_menu && PisteDraw_Fade_Palette_Done()) {
        next_game_state = STATE_MENUS;
        game_in_progress = false;
    }
    if (key_delay > 0)
        key_delay--;
    if (key_delay == 0) {
        if (PisteInput_KeyDown(DIK_RETURN) || PisteInput_KeyDown(DIK_SPACE) || intro_counter == 3500) {
            switch_from_intro_to_menu = true;
            PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
        }
    }

    return 0;
}

//TODO: =>
int Main_Scoreboard(void) {
    mouse_x += PisteInput_Mouse_X(0);
    mouse_y += PisteInput_Mouse_Y(0);
    mouse_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 30;
    mouse_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 30;

    if (PisteInput_KeyDown(DIK_LEFT))
        mouse_x -= 3;
    if (PisteInput_KeyDown(DIK_RIGHT))
        mouse_x += 3;
    if (PisteInput_KeyDown(DIK_UP))
        mouse_y -= 3;
    if (PisteInput_KeyDown(DIK_DOWN))
        mouse_y += 3;
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 640 - 19)
        mouse_x = 640 - 19;
    if (mouse_y > 480 - 19)
        mouse_y = 480 - 19;

    Draw_Scoreboard();
    degree = 1 + degree % 360;

    // SCORE COUNTING
    int energy = sprites[player_index].health; //TODO: energy_points?
    if (score_calculation_delay == 0)
        if (bonus_points < stage_points) {
            score_calculation_phase = 1;
            score_calculation_delay = 0;
            bonus_points += 10;

            if (degree % 7 == 1)
                Play_Sound_Menu(score_counter_sound, 70);
            if (bonus_points >= stage_points) {
                bonus_points = stage_points;
                score_calculation_delay = 50;
            }
        } else if (game_time > 0) {
            score_calculation_phase = 2;
            score_calculation_delay = 0;
            time_points += 5;
            game_time--;
            if (degree % 10 == 1)
                Play_Sound_Menu(score_counter_sound, 70);
            if (game_time == 0)
                score_calculation_delay = 50;
        } else if (sprites[player_index].health > 0) {
            score_calculation_phase = 3;
            score_calculation_delay = 10;
            energy_points += 300;
            sprites[player_index].health--;
            Play_Sound_Menu(score_counter_sound, 70);
        } else if (num_items > 0) {
            score_calculation_phase = 4;
            score_calculation_delay = 30;
            for (int i = 0; i < MAX_ITEMS; i++)
                if (items[i] != NULL) {
                    item_points += items[i]->points + 500;
                    items[i] = NULL;
                    Play_Sound_Menu(jump_sound, 100);
                    break;
                }
        } else
            score_calculation_phase = 5;
    if (score_calculation_delay > 0)
        score_calculation_delay--;
    if (switch_from_score_to_map && PisteDraw_Fade_Palette_Done()) {
        /*checking if it was the last stage*/
        if (current_stage_index == EPISODE_MAX_STAGES - 1) { // just as if someone made this many episodes...
            next_game_state = STATE_END;
        } else if (stages[current_stage_index + 1].order == -1)
            next_game_state = STATE_END;
        else { // if not, return to the map...
            next_game_state = STATE_MAP;
            //game_in_progress = false;
            current_menu = MENU_MAIN;
        }
    }
    if (key_delay > 0)
        key_delay--;
    if (key_delay == 0) {
        if (PisteInput_KeyDown(DIK_RETURN) && score_calculation_phase == 5) {
            switch_from_score_to_map = true;
            music_volume = 0;
            PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_RETURN) && score_calculation_phase < 5) {
            score_calculation_phase = 5;
            bonus_points = stage_points;
            time_points += game_time * 5;
            game_time = 0;
            energy_points += sprites[player_index].health * 300;
            sprites[player_index].health = 0;
            for (int i = 0; i < MAX_ITEMS; i++)
                if (items[i] != NULL)
                    item_points += items[i]->points + 500;
            num_items = 0;
            key_delay = 20;
        }
    }

    return 0;
}

int Main_Map(void) {
    mouse_x += PisteInput_Mouse_X(0);
    mouse_y += PisteInput_Mouse_Y(0);
    mouse_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 30;
    mouse_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 30;

    if (PisteInput_KeyDown(DIK_LEFT))
        mouse_x -= 3;
    if (PisteInput_KeyDown(DIK_RIGHT))
        mouse_x += 3;
    if (PisteInput_KeyDown(DIK_UP))
        mouse_y -= 3;
    if (PisteInput_KeyDown(DIK_DOWN))
        mouse_y += 3;
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 640 - 19)
        mouse_x = 640 - 19;
    if (mouse_y > 480 - 19)
        mouse_y = 480 - 19;
    Draw_Map();
    degree = 1 + degree % 360;

    if (switch_from_map_to_game && PisteDraw_Fade_Palette_Done()) {
        next_game_state = STATE_GAME;
        //strcpy(next_map,stages[i].file);
        game_in_progress = false;//true;
        //PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);
    }
    if (key_delay > 0)
        key_delay--;
    if (key_delay == 0) {
/*
		if (PisteInput_KeyDown(DIK_RETURN))
		{
			switch_from_map_to_game = true;
			//next_game_state = STATE_GAME;
			PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
		}
*/
    }

    return 0;
}

int Main_Menus(void) {
    mouse_x += PisteInput_Mouse_X(0);
    mouse_y += PisteInput_Mouse_Y(0);
/*
	mouse_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1)/30;
	mouse_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1)/30;
*/
    if (!name_edit && key_delay == 0 && read_controls_menu == 0) {
/*
		if (PisteInput_Keydown(DIK_LEFT));
			//mouse_x -= 3;

		if (PisteInput_KeyDown(DIK_RIGHT));
			//mouse_x += 3;
*/
        if (PisteInput_KeyDown(DIK_UP) || PisteInput_KeyDown(DIK_LEFT) || PisteInput_Controller_MoveX(PI_CONTROLLER_1) < 0 || PisteInput_Controller_MoveY(PI_CONTROLLER_1) < 0) {
            selected_menu_id--;
            if (selected_menu_id < 1)
                selected_menu_id = choice_menu_id - 1;
            key_delay = 15;
            //mouse_y -= 3;
        }
        if (PisteInput_KeyDown(DIK_DOWN) || PisteInput_KeyDown(DIK_RIGHT) ||
                PisteInput_Controller_MoveX(PI_CONTROLLER_1) > 0 || PisteInput_Controller_MoveY(PI_CONTROLLER_1) > 0) {
            selected_menu_id++;
            if (selected_menu_id > choice_menu_id - 1)
                selected_menu_id = 1;
            key_delay = 15;
            //mouse_y += 3;
        }
    }
    if (name_edit || read_controls_menu > 0) {
        selected_menu_id = 0;
    }
    if (current_menu != MENU_NAME) {
        name_edit = false;
    }
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 640 - 19)
        mouse_x = 640 - 19;
    if (mouse_y > 480 - 19)
        mouse_y = 480 - 19;
    int menu_before = current_menu;
    Draw_Menus();
    if (current_menu != menu_before)
        selected_menu_id = 1;
    degree = 1 + degree % 360;
    if (double_speed)
        degree = 1 + degree % 360;
    if (key_delay > 0)
        key_delay--;

    return 0;
}

int Main_Game(void) {
    Camera();
    Draw_Game();
    Update_Calculated_Blocks();
    if (!pause) {
        degree = 1 + degree % 359;
        if (switch1 > 0)
            switch1--;
        if (switch2 > 0)
            switch2--;
        if (switch3 > 0)
            switch3--;
        if (notification_timer > 0)
            notification_timer--;
        if (points_add > 0) {
            stage_points++;
            points_add--;
        }
        if (time_limit && !stage_completed) {
            if (seconds > 0) {
                seconds -= 10;
            } else {
                seconds = 1000;
                if (game_time > 0) {
                    game_time--;
                } else {
                    game_over = true;
                    //PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
                }
            }
        }
        if (invisibility > 0)
            invisibility--;
    }
    // frame counters
    fps_counter++;
    if (PisteWait_Get() >= fps_time + 1000) {
        fps = fps_counter;
        fps_counter = 0;
        fps_time = PisteWait_Get();
    }
    if (sprites[player_index].health < 1 && !game_over) {
        game_over = true;
        key_delay = 50;
    }
    if (key_delay > 0)
        key_delay--;
    if (stage_completed || game_over) {
        if (end_timer > 1)
            end_timer--;
        if (end_timer == 0)
            end_timer = 800;//2000;
        if (PisteInput_Read_Controller(control_attack1) || PisteInput_Read_Controller(control_attack2) ||
            PisteInput_Read_Controller(control_jump) || PisteInput_KeyDown(DIK_RETURN))
            if (end_timer > 2 && end_timer < 600/*1900*/ && key_delay == 0)
                end_timer = 2;
        if (end_timer == 2) {
            PisteDraw_Fade_Palette_Out(PD_FADE_NORMAL);
            //music_volume = 0;
        }
    }
/*
	if (stage_completed) {
		if (duration > 0) {
			fake_points+=5;
			duration--;
		}
	}
*/
    if (end_timer == 1 && PisteDraw_Fade_Palette_Done()) {
        if (stage_completed)
            next_game_state = STATE_SCORE_CALCULATION;//STATE_MAP;
        else
            next_game_state = STATE_MAP;
    }
    if (key_delay == 0) {
        if (PisteInput_Read_Controller(control_use_item) && sprites[player_index].health > 0) {
            Use_Item();
            key_delay = 10;
        }
        if (PisteInput_KeyDown(DIK_P) && !stage_completed) {
            pause = !pause;
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_DELETE))
            sprites[player_index].health = 0;
        if (PisteInput_KeyDown(DIK_I)) {
            show_fps = !show_fps;
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_TAB)) {
            Switch_Item();
            key_delay = 10;
        }
    }
    if (key_delay == 0 && cheats) {
        if (PisteInput_KeyDown(DIK_Z)) {
            switch1 = SWITCH_START_VALUE;
            switch2 = SWITCH_START_VALUE;
            switch3 = SWITCH_START_VALUE;
            key_delay = 10;
        }
        if (PisteInput_KeyDown(DIK_G)) {
            settings.transparent_objects = !settings.transparent_objects;
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_L)) {
            Unlock_Map();
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_K)) {
            Map_Change_SkullBlocks();
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_F)) {
            show_fps = !show_fps;
            key_delay = 20;
        }
        if (PisteInput_KeyDown(DIK_I)) {
            draw_info = !draw_info;
            key_delay = 10;
        }
        if (PisteInput_KeyDown(DIK_R)) {
            Find_Start();
            sprites[player_index].health = 10;
            key_delay = 10;
        }
        if (PisteInput_KeyDown(DIK_END)) {
            if (Midas_Load_Music("music\\hiscore.xm") != 0)
                PK2_error = true;
            key_delay = 20;
            stage_completed = true;
            stages[current_stage_index].completed = true;
            stage++;
        }
        if (PisteInput_KeyDown(DIK_E))
            sprites[player_index].health = 3;
        if (PisteInput_KeyDown(DIK_LSHIFT)) {
            for (int r = 1; r < 6; r++)
                New_Particle(PARTICLE_SPARK, sprites[player_index].x + rand() % 10 - rand() % 10, sprites[player_index].y + rand() % 10 - rand() % 10, 0, 0, rand() % 100, 0.1, 32);
            sprites[player_index] = PK2Sprite(&prototypes[PROTOTYPE_CHICKEN], 1, false, sprites[player_index].x, sprites[player_index].y);
        }
    }

    return 0;
}

int Main_Ending(void) {
    mouse_x += PisteInput_Mouse_X(0);
    mouse_y += PisteInput_Mouse_Y(0);
    mouse_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 30;
    mouse_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 30;

    if (PisteInput_KeyDown(DIK_LEFT))
        mouse_x -= 3;
    if (PisteInput_KeyDown(DIK_RIGHT))
        mouse_x += 3;
    if (PisteInput_KeyDown(DIK_UP))
        mouse_y -= 3;
    if (PisteInput_KeyDown(DIK_DOWN))
        mouse_y += 3;

    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y < 0)
        mouse_y = 0;
    if (mouse_x > 640 - 19)
        mouse_x = 640 - 19;
    if (mouse_y > 480 - 19)
        mouse_y = 480 - 19;

    Draw_Ending();
    degree = 1 + degree % 360;
    end_counter++;
    intro_counter = end_counter; // for intro texts

    if (switch_from_end_to_menu && PisteDraw_Fade_Palette_Done()) {
        next_game_state = STATE_MENUS;
        current_menu = MENU_MAIN;
        game_in_progress = false;
    }
    if (key_delay > 0)
        key_delay--;
    if (key_delay == 0) {
        if (PisteInput_KeyDown(DIK_RETURN) || PisteInput_KeyDown(DIK_SPACE)) {
            switch_from_end_to_menu = true;
            music_volume = 0;
            PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
        }
    }

    return 0;
}

int Main(void) {
    if (window_closed)
        return (0);
    /* GETTING THE CURRENT STATES OF KEYBOARD, MOUSE, AND CONTROLLERS */
    // Keyboard
    if (!PisteInput_Update_Keyboard())        // Get the keyboard states
        PK2_error = true;
    // Mouse
    if (!PisteInput_Update_Mouse())            // Get the mouse state
        PK2_error = true;
    if (!PisteInput_Update_Controllers()) {            // Get the controllers states
        //PK2_error = true;
    }
    switch (game_state) {
        case STATE_GAME:
            Main_Game();
            break;
        case STATE_MENUS:
            Main_Menus();
            break;
        case STATE_MAP:
            Main_Map();
            break;
        case STATE_SCORE_CALCULATION:
            Main_Scoreboard();
            break;
        case STATE_INTRO:
            Main_Intro();
            break;
        case STATE_END:
            Main_Ending();
            break;
        default:
            end_game = true;
            break;
    }
    // MUSIC VOLUME CONTROL
    bool send = false;

    if (music_volume != music_current_volume)
        send = true;
    if (music_max_volume > 64)
        music_max_volume = 64;
    if (music_max_volume < 0)
        music_max_volume = 0;
    if (music_volume > music_max_volume)
        music_volume = music_max_volume;
    if (music_current_volume < music_volume)
        music_current_volume++;
    if (music_current_volume > music_volume)
        music_current_volume--;
    if (music_current_volume > 64)
        music_current_volume = 64;
    if (music_current_volume < 0)
        music_current_volume = 0;
    if (send && settings.music)
        MIDASsetMusicVolume(playHandle, music_current_volume);

    if (PisteInput_KeyDown(DIK_ESCAPE) && key_delay == 0) {
        if (current_menu != MENU_MAIN || game_state != STATE_MENUS) {
            next_game_state = STATE_MENUS;
            current_menu = MENU_MAIN;
            key_delay = 70;
            degree_temp = degree;
        } else if (game_state == STATE_MENUS) {
            end_game = true;
            music_volume = 0;
            PisteDraw_Fade_Palette_Out(PD_FADE_FAST);
        }
    }
    if (PisteInput_KeyDown(DIK_LALT) && PisteInput_KeyDown(DIK_TAB)) {
        PisteLog_Write("- ALT+TAB pressed. \n");
        if (window_active)
            SendMessage(window_handle, WM_ACTIVATE, HIunsigned short(true), 0);
        //SendMessage(window_handle, WM_CLOSE,0,0);
        //window_closed = true;
    }
/*
	if (PisteInput_Keydown(DIK_LALT) && PisteInput_KeyDown(DIK_Q))
	{
		if (window_active)
			SendMessage(window_handle, WM_ACTIVATE,HIunsigned short(true),0);
		PisteLog_Write("- ALT+Q pressed. \n");
		//SendMessage(window_handle, WM_CLOSE,0,0);
		//window_closed = true;
	}
*/
    if (end_game && PisteDraw_Fade_Palette_Done()) {
        SendMessage(window_handle, WM_CLOSE, 0, 0);
        window_closed = true;
    }
/*
	if (PisteInput_KeyDown(DIK_X))
	{
		SendMessage(window_handle, WM_CLOSE,0,0);
		window_closed = true;
	}
*/
    return 0;
}

int Reload(void) {
    if (unload) {
        PisteLog_Write("  - Reloading all devices. \n");
/*
		PisteLog_Write("    - Reloading Midas config. \n");
		if ( !MIDASconfig() )
		{
			MIDASerror = true;
			settings.music = false;
		}
*/
        next_game_state = STATE_BASIC_INITIALIZATION;
        game_state = STATE_NOT_INITIALIZED;
        if (Initialize_States() != 0)
            return 1;
        next_game_state = STATE_MENUS;
        //Initialize_States();
        unload = false;
        PisteLog_Write("  - Reloading all DirectX devices done. \n");
    }
    return 0;
}

int Unload(void) {
    if (!unload) {
        PisteLog_Write("  - Unloading all DirectX devices. \n");
        PisteLog_Write("    - Unloading PisteDraw \n");
        PisteDraw_Quit();
        PisteLog_Write("    - Unloading PisteInput. \n");
        PisteInput_Quit();
        PisteLog_Write("    - Stopping music. \n");
        if (module_loaded && settings.music) {
            Midas_Stop();
            MIDASfreeModule(module);
            module_loaded = false;
        }
        PisteLog_Write("    - Deleting map. \n");
        delete map;
        PisteLog_Write("    - Unloading Midas. \n");
        Midas_End();
        PisteLog_Write("    - Unloading Direct Sound. \n");
        PisteSound_Stop();
        unload = true;
    }

    return 0;
}

int Quit(void) {
    Unload();
    PisteLog_Write("    - Deleting game texts. \n");
    delete texts;
    // If an error occurred, show the text box.
    if (PK2_error) {
        char error_message[255];
        strcpy(error_message, PisteDraw_GetError());
        strcat(error_message, "\n");
        strcat(error_message, PisteSound_GetError());
        MessageBox(window_handle, error_message, "Pekka Kana 2", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}

/* WINDOWS -------------------------------------------------------------------------------------*/
int Language_Files_Get() {
    char directory[_MAX_PATH];
    strcpy(directory, "language\\");
    strcat(directory, "*.txt");

    struct _finddata_t map_file;
    long hFile;
    int i = 0;
    if ((hFile = _findfirst(directory, &map_file)) == -1L)
        return 1;
    else {
        SendMessage(hCMB_Languages, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)
        map_file.name);
        SendMessage(hCMB_Languages, CB_SETCURSEL, 0, 0);
        i++;
    }
    while (_findnext(hFile, &map_file) == 0) {
        SendMessage(hCMB_Languages, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)
        map_file.name);
/*
		if (strcmp(settings.language,map_file.name)==0)
			SendMessage(hCMB_Languages,CB_SETCURSEL, i-1, 0);
*/
        i++;
    }
    SendMessage(hCMB_Languages, CB_SETCURSEL, 0, 0);
    int count = i;
    char language_file[_MAX_PATH];
    for (i = 0; i < count; i++) {
        SendMessage(hCMB_Languages, CB_GETLBTEXT, i, (LPARAM)(LPCSTR)language_file);
        if (strcmp(language_file, settings.language) == 0)
            SendMessage(hCMB_Languages, CB_SETCURSEL, i, 0);
    }
    _findclose(hFile);
    return 0;
}

// Follows the window code. The below is the window's "event listener".
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch(msg) {
        case WM_ACTIVATE: {
            //PisteLog_Write("- Main window's active state has changed.\n");
            if ((BOOL)HIunsigned short(wparam) != 0) {
                PisteLog_Write("- Main window has been minimized.\n");
                window_active = false;
                PostQuitMessage(0);
                window_closed = true;
/*
                if (Unload() != 0) {
                    PisteLog_Write("- Unload failed.\n");
                    PostQuitMessage(0);
                    window_closed = true;
                }
                else {
                    ShowCursor(true);
                }
*/
            }
            if (LOunsigned short(wparam) == WA_INACTIVE) {
                PisteLog_Write("- Main window is set inactive.\n");
                window_active = false;
                PostQuitMessage(0);
                window_closed = true;
            }
            if (LOunsigned short(wparam) == WA_ACTIVE || LOunsigned short(wparam) == WA_CLICKACTIVE) {
/*
                PisteLog_Write("- Activating game's main window.\n");
                if (!window_active) {
                    PisteLog_Write("  - Trying to recover.\n");
                    if (Reload() != 0) {
                        PisteLog_Write("  - Recovery failed! \n");
                        PostQuitMessage(0);
                        window_closed = true;
                    }
                    else {
                        window_active = true;
                        ShowCursor(false);
                    }
                    //if (PisteDraw_GetFrameCount() == PD_ERROR) {
                    //    PisteLog_Write("  - Reallocation of surfaces failed.\n");
                    //    PostQuitMessage(0);
                    //    //PK2_error = true;
                    //}
                }
*/
            }
/*
            if (HIunsigned short(wparam) == 0) {
                PisteLog_Write("- Activating game's main window.\n");
                if (!window_active) {
                    PisteLog_Write("  - Trying to revieve surfaces.\n");
                    if (PisteDraw_GetFrameCount() == PD_ERROR) {
                        PisteLog_Write("  - Reallocation of surfaces failed.\n");
                        PK2_error = true;
                    }
                }
                window_active = true;
            }
            else {
                PisteLog_Write("- ALT+TAB pressed!\n");
                window_active = false;
                PostQuitMessage(0);
            }
*/
            return(0);
        } break;
        case WM_CREATE: {
            return(0);
        } break;
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return(0);
        } break;
        case WM_DESTROY: {
            PisteLog_Write("- Main window receives exit message.\n");
            PostQuitMessage(0);
            return(0);
        } break;

        default:
            break;
    }

    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

// TODO: =>
LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    PAINTSTRUCT ps;
    HDC hdc;
    switch(msg) {
        case WM_CREATE: {
            return(0);
        } break;
        case WM_COMMAND: {
            switch(LOunsigned short(wparam)) {
                case ID_BT_Exit: {
                    SendMessage(hSetupWindow, WM_CLOSE, 0,0);
                    SendMessage(hwnd, WM_CLOSE, 0,0);
                } break;
                case ID_BT_Music: {
                    PisteLog_Write("    - Setting Midas config. \n");
                    if (!MIDASconfig()) {
                        MIDASerror = true;
                        settings.music = false;
                    } else {
                        MIDASsaveConfig("data\\msetup.ini");
                        settings.music = true;
                    }
                } break;
                case ID_BT_Play: {
                    initial_setup = false;
                    if (SendMessage(hRB_1024x768, BM_GETSTATE, 0, 0) == BST_CHECKED) {
                        SCREEN_WIDTH = 1024;
                        SCREEN_HEIGHT = 768;
                    }
                    if (SendMessage(hRB_640x480, BM_GETSTATE, 0, 0) == BST_CHECKED) {
                        SCREEN_WIDTH = 640;
                        SCREEN_HEIGHT = 480;
                    }
                    if (SendMessage(hRB_800x600, BM_GETSTATE, 0, 0) == BST_CHECKED) {
                        SCREEN_WIDTH = 800;
                        SCREEN_HEIGHT = 600;
                    }
                    if (SendMessage(hCB_Music, BM_GETSTATE, 0, 0) == BST_CHECKED) {
                        settings.music = true;
                    } else
                        settings.music = false;
                    if (SendMessage(hCB_SFX, BM_GETSTATE, 0, 0) == BST_CHECKED) {
                        settings.sounds = true;
                    } else
                        settings.sounds = false;
                    // Get the selected language
                    char language_file[_MAX_PATH];
                    char directory[_MAX_PATH];
                    long selected = SendMessage(hCMB_Languages, CB_GETCURSEL, 0, 0);
                    SendMessage(hCMB_Languages, CB_GETLBTEXT, selected, (LPARAM)(LPCSTR)language_file);
                    //texts->Read_File(language_file);
                    if (strcmp(language_file, settings.language) != 0) {
                        strcpy(settings.language, language_file);
                        strcpy(directory, "language\\");
                        strcat(directory, language_file);
                        Load_Language(directory);
                    }
                    //DestroyWindow(hSetupWindow);
                    //SendMessage(hSetupWindow, WM_CLOSE,0,0);
                } break;

                default:
                    break;
            }
            return 0;
        }
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return(0);
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            return(0);
        } break;
/*
        case WM_CTLCOLORSTATIC: {
            HDC  hdcButton  = (HDC) wparam;
            HWND hwndButton = (HWND) lparam;
            return((LRESULT)(HBRUSH)GetStockObject(LTGRAY_BRUSH));
            break;
        }
        case WM_CTLCOLORBTN: {
            HDC  hdcButton  = (HDC) wparam;
            HWND hwndButton = (HWND) lparam;
            return((LRESULT)(HBRUSH)GetStockObject(LTGRAY_BRUSH));
            break;
        }
*/
        default:
            break;
    }

    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int PK_SetupWindow(HWND & hwnd, HINSTANCE & hinstance, int & ncmdshow) {
    WNDCLASSEX wc; // new window class (struct)
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    //wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
    //wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    //wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "SetupWindow";

    if (!RegisterClassEx(&wc)) // register the window
        return 1;
    if (!(hSetupWindow = CreateWindowEx(WS_EX_LEFT,
        "SetupWindow",
        _T("Pekka Kana 2 v1.21 Setup"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        560,                    // width and
        440,                    // height
        NULL,                   // parent window if child window
        NULL,                   // menu
        hinstance,              // program identifier
        NULL                    // data for creating the window(?)
    )))
        return 1;

    /* STATIC TEXTS */
    CreateWindowEx(NULL, "static", " ", WS_CHILD | WS_VISIBLE | SS_LEFT,
        //320,360,
        0, 0, 560, 440, hSetupWindow, (HMENU)(10), hinstance, NULL);
    CreateWindowEx(NULL, "static",
        //"Setup options",
        texts->Get_Text(txt_setup_options), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY, 10, 12, 100, 24, hSetupWindow, (HMENU)(10), hinstance, NULL);
    CreateWindowEx(NULL, "static", "www.pistegamez.net", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY, 10, 390, 200, 24, hSetupWindow, (HMENU)(10), hinstance, NULL);
/*
	CreateWindowEx(	NULL,
        "static",
        "Pekka Kana 2 (C) 2002-2003 Piste Gamez \nhttp://pistegamez.cjb.net",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        300,275,
        300,60,
        hSetupWindow,
        (HMENU)(10),
        hinstance,
        NULL);
*/
    /* IMAGE */
    CreateWindowEx(NULL, "STATIC", "SETUPBMP", WS_CHILD | WS_VISIBLE | SS_BITMAP, 295, 0, 266, 400, hSetupWindow, (HMENU)(11), hinstance, NULL);
    CreateWindowEx(NULL, "static", texts->Get_Text(txt_setup_language), WS_CHILD | WS_VISIBLE | SS_LEFT,
        //320,360,
        295, 345, 266, 90, hSetupWindow, (HMENU)(10), hinstance, NULL);

    /* GROUPBOXES */
    CreateWindowEx(NULL, "button",
        //"Video modes:",
        texts->Get_Text(txt_setup_videomodes), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 40, 250, 150, hSetupWindow, (HMENU)(20), hinstance, NULL);
    CreateWindowEx(NULL, "button",
        //"Music and sound effects:",
        texts->Get_Text(txt_setup_music_and_sounds), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 200, 250, 100, hSetupWindow, (HMENU)(30), hinstance, NULL);

    /* RADIOBUTTONS */
    hRB_640x480 = CreateWindowEx(NULL, "button", "640x480", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 65, 100, 24, hSetupWindow, (HMENU)(ID_RB), hinstance, NULL);
    hRB_800x600 = CreateWindowEx(NULL, "button", "800x600", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 90, 100, 24, hSetupWindow, (HMENU)(ID_RB), hinstance, NULL);
    hRB_1024x768 = CreateWindowEx(NULL, "button", "1024x768", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 115, 100, 24, hSetupWindow, (HMENU)(ID_RB), hinstance, NULL);

    /* CHECK BOXES */
    hCB_Music = CreateWindowEx(NULL, "button",
        //"Music",
        texts->Get_Text(txt_setup_music), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 225, 100, 24, hSetupWindow, (HMENU)(ID_CB_Music), hinstance, NULL);
    hCB_SFX = CreateWindowEx(NULL, "button",
        //"Sound Effects",
        texts->Get_Text(txt_setup_sounds), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 255, 120, 24, hSetupWindow, (HMENU)(ID_CB_SFX), hinstance, NULL);

    /* COMBOBOXES */
    hCMB_Languages = CreateWindowEx(NULL, "combobox", "Kielet",
        //texts->Get_Text(txt_setup_languages),
        WS_CHILD | WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT, 300, 370, 250, 224, hSetupWindow, (HMENU)(ID_CMB_Languages), hinstance, NULL);

    /* BUTTONS */
    hBT_Play = CreateWindowEx(NULL, "button", "Music Setup...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 225, 120, 24, hSetupWindow, (HMENU)(ID_BT_Music), hinstance, NULL);
    hBT_Play = CreateWindowEx(NULL, "button",
        //"Play",
        texts->Get_Text(txt_setup_play), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 320, 250, 24, hSetupWindow, (HMENU)(ID_BT_Play), hinstance, NULL);
    hBT_Exit = CreateWindowEx(NULL, "button",
        //"Exit",
        texts->Get_Text(txt_setup_exit), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 355, 250, 24, hSetupWindow, (HMENU)(ID_BT_Exit), hinstance, NULL);

    switch (settings.screen_width) {
        case 800:
            SendMessage(hRB_800x600, BM_SETCHECK, true, 0);
            break;
        case 640:
            SendMessage(hRB_640x480, BM_SETCHECK, true, 0);
            break;
        case 1024:
            SendMessage(hRB_1024x768, BM_SETCHECK, true, 0);
            break;
        default:
            SendMessage(hRB_800x600, BM_SETCHECK, true, 0);
            break;
    }
    if (settings.music)
        SendMessage(hCB_Music, BM_SETCHECK, true, 0);
    else
        SendMessage(hCB_Music, BM_SETCHECK, false, 0);
    if (settings.sounds)
        SendMessage(hCB_SFX, BM_SETCHECK, true, 0);
    else
        SendMessage(hCB_SFX, BM_SETCHECK, false, 0);
    Language_Files_Get();
/*
	SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"suomi.txt");
	SendMessage(hCMB_Languages,CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"english.txt");
*/
    if (!ShowWindow(hSetupWindow, ncmdshow))
        return 1;
    if (!UpdateWindow(hSetupWindow))
        return 1;

    return 0;
}

// The beginning and the end: WinMain. Everything starts here and the game runs within this.
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
    WNDCLASSEX winclass;
    HWND hwnd;
    MSG msg;
    char *commands = lpcmdline;
    if (strcmp(commands, "log" ) == 0) {
        PisteLog_EnableWriting();
    }
    PisteLog_Write("PK2 STARTUP\n");
    PisteLog_Write("========================\n");

    PisteLog_Write("- Command string: ");
    PisteLog_Write(commands);
    PisteLog_Write("\n");

    if (strcmp(commands, "developer" ) == 0) {
        PisteLog_Write("- Developer mode set. Cheats are on.\n");
        cheats = true;
    }
    if (strcmp(commands, "nolimits" ) == 0) {
        PisteLog_Write("- No limits mode set. Game area is drawn full screen.\n");
        LIMIT_MAP_DRAWING_AREA = false;
    }
    /* START SETUP */
    Load_Settings("data\\settings.ini");
    texts = new PisteLanguage();
    char directory[_MAX_PATH];

    strcpy(directory, "language\\");
    strcat(directory, settings.language);
    if (!Load_Language(directory)) {
        MessageBox(window_handle, "Could not find the default language file!", "Pekka Kana 2", MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }
    if (!PK_SetupWindow(hwnd, hinstance, ncmdshow))
        return 0;
    while (initial_setup) {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    if (initial_setup)
        return(msg.wParam);

    /* GAME WINDOW */
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
    winclass.hCursor = LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
    winclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;
    winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//IDI_APPLICATION

    hinstance_app = hinstance;
    if (!RegisterClassEx(&winclass))
        return(0);
    if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, GAME_NAME, WS_POPUP | WS_VISIBLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hinstance, NULL)))
        return(0);
    window_handle = hwnd;
    Initialize_States();
    //PK_Init();
    ShowCursor(false);

    if (cheats)
        next_game_state = STATE_MENUS;
    else
        next_game_state = STATE_INTRO;

    while(!PK2_error) {
        Initialize_States();
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (window_active)
            Main();
    }

    if (PK2_error)
        PisteLog_Write("- PK2 Error! Stopping.\n");
    PisteLog_Write("- Saving settings.\n");
    Save_Settings("data\\settings.ini");

    Quit();

    PisteLog_Write("========================\n");
    PisteLog_Write("PK2 EXITS\n");
    PisteLog_Write("========================\n");
    ShowCursor(true);

    return(msg.wParam);
}
