#ifndef PK2SPRITE
#define PK2SPRITE

const char LATEST_VERSION_PK2SPRITE[4] = "1.3";

const int MAX_PROTOTYPES = 100;

const int SPRITE_MAX_FRAMES = 50;
const int SPRITE_MAX_ANIMATIONS = 20;
const int SPRITE_MAX_AI = 10;
const int ANIMATION_MAX_SEQUENCES = 10;
const int MAX_SOUNDS = 7;

const int ANIMATION_IDLE = 0;
const int ANIMATION_WALK = 1;
const int ANIMATION_JUMP_UP = 2;
const int ANIMATION_JUMP_DOWN = 3;
const int ANIMATION_CROUCH = 4;
const int ANIMATION_DAMAGE = 5;
const int ANIMATION_DEATH = 6;
const int ANIMATION_ATTACK1 = 7;
const int ANIMATION_ATTACK2 = 8;

const unsigned char SOUND_DAMAGE = 0;
const unsigned char SOUND_DESTRUCTION = 1;
const unsigned char SOUND_ATTACK1 = 2;
const unsigned char SOUND_ATTACK2 = 3;
const unsigned char SOUND_RANDOM = 4;
const unsigned char SOUND_SPECIAL1 = 5;
const unsigned char SOUND_SPECIAL2 = 6;

const unsigned char AI_NONE = 0;
const unsigned char AI_CHICKEN = 1;
const unsigned char AI_EGG = 2;
const unsigned char AI_SMALL_CHICKEN = 3;
const unsigned char AI_BONUS = 4;                                       //B
const unsigned char AI_JUMPING = 5;
const unsigned char AI_BASIC = 6;                                       //B
const unsigned char AI_TURN_ON_COLLISION_HORIZONTAL = 7;
const unsigned char AI_AVOID_PIT = 8;
const unsigned char AI_RANDOM_DIRECTION_CHANGE_HORIZONTAL = 9;
const unsigned char AI_RANDOM_JUMP = 10;
const unsigned char AI_FOLLOW_PLAYER = 11;
const unsigned char AI_RANDOM_START_DIRECTION_HORIZONTAL = 12;
const unsigned char AI_FOLLOW_PLAYER_IF_VISIBLE = 13;
const unsigned char AI_CHANGE_IF_ENERGY_LESS_THAN_2 = 14;               //P
const unsigned char AI_CHANGE_IF_ENERGY_MORE_THAN_1 = 15;               //P
const unsigned char AI_START_DIRECTION_TOWARDS_PLAYER = 16;
const unsigned char AI_PROJECTILE = 17;
const unsigned char AI_NONSTOP = 18;
const unsigned char AI_ATTACK_1_IF_HIT = 19;
const unsigned char AI_BOMB = 20;
const unsigned char AI_ATTACK_1_IF_PLAYER_IN_FRONT = 21;
const unsigned char AI_ATTACK_1_IF_PLAYER_BELOW = 22;
const unsigned char AI_DAMAGE_FROM_WATER = 23;        //P
const unsigned char AI_ATTACK_2_IF_PLAYER_IN_FRONT = 24;
const unsigned char AI_KILL_ALL = 25;
const unsigned char AI_FRICTION_AFFECTS = 26;
const unsigned char AI_HIDES = 27;
const unsigned char AI_RETURN_TO_START_X = 28;
const unsigned char AI_RETURN_TO_START_Y = 29;
const unsigned char AI_TELEPORT = 30;
const unsigned char AI_THROWING_WEAPON = 35;
const unsigned char AI_FALLS_FROM_STORY = 36;                           //B
const unsigned char AI_CHANGE_SKULLS_IF_KNOCKED_OUT = 37;
const unsigned char AI_CHANGE_SKULLS_IF_HIT = 38;
const unsigned char AI_DESTROY_IF_PARENT_DESTROYED = 39;

const unsigned char AI_MOVE_X_COS = 41;
const unsigned char AI_MOVE_Y_COS = 42;
const unsigned char AI_MOVE_X_SIN = 43;
const unsigned char AI_MOVE_Y_SIN = 44;
const unsigned char AI_MOVE_X_COS_FAST = 45;
const unsigned char AI_MOVE_Y_SIN_FAST = 46;
const unsigned char AI_MOVES_X_COS_SLOW = 47;
const unsigned char AI_MOVES_Y_SIN_SLOW = 48;
const unsigned char AI_MOVES_Y_SIN_FREE = 49;

const unsigned char AI_RANDOM_TURNING = 50;
const unsigned char AI_JUMP_IF_PLAYER_ABOVE = 51;
const unsigned char AI_CHANGE_TIMER = 52;                               //B

const unsigned char AI_FALLS_IF_SWITCH1_PRESSED = 53;                   //B
const unsigned char AI_FALLS_IF_SWITCH2_PRESSED = 54;                   //B
const unsigned char AI_FALLS_IF_SWITCH3_PRESSED = 55;                   //B

const unsigned char AI_MOVES_DOWN_IF_SWITCH1_PRESSED = 56;
const unsigned char AI_MOVES_UP_IF_SWITCH1_PRESSED = 57;
const unsigned char AI_MOVES_LEFT_IF_SWITCH1_PRESSED = 58;
const unsigned char AI_MOVES_RIGHT_IF_SWITCH1_PRESSED = 59;
const unsigned char AI_MOVES_DOWN_IF_SWITCH2_PRESSED = 60;
const unsigned char AI_MOVES_UP_IF_SWITCH2_PRESSED = 61;
const unsigned char AI_MOVES_LEFT_IF_SWITCH2_PRESSED = 62;
const unsigned char AI_MOVES_RIGHT_IF_SWITCH2_PRESSED = 63;
const unsigned char AI_MOVES_DOWN_IF_SWITCH3_PRESSED = 64;
const unsigned char AI_MOVES_UP_IF_SWITCH3_PRESSED = 65;
const unsigned char AI_MOVES_LEFT_IF_SWITCH3_PRESSED = 66;
const unsigned char AI_MOVES_RIGHT_IF_SWITCH3_PRESSED = 67;

const unsigned char AI_TURN_FROM_OBSTACLE_VERT = 70;                    //v1.2
const unsigned char AI_RANDOM_START_DIRECTION_VERT = 71;                //v1.2
const unsigned char AI_START_DIRECTION_TOWARDS_PLAYER_VERT = 72;        //v1.2
const unsigned char AI_CLIMBER = 73;                                    //v1.2
const unsigned char AI_CLIMBER2 = 74;                                   //v1.2
const unsigned char AI_FLEES_PLAYER_IF_SEEN = 75;                       //v1.2
const unsigned char AI_NEW_IF_DESTROYED = 76;                           //v1.2, B

const unsigned char AI_ARROW_LEFT = 77;                                 //v1.2
const unsigned char AI_ARROW_RIGHT = 78;                                //v1.2
const unsigned char AI_ARROW_UP = 79;                                   //v1.2
const unsigned char AI_ARROW_DOWN = 80;                                 //v1.2
const unsigned char AI_ARROWS_AFFECT = 81;                              //v1.2

const unsigned char AI_BACKGROUND_MOON = 101;
const unsigned char AI_BACKGROUND_MOVES_LEFT = 102;
const unsigned char AI_BACKGROUND_MOVES_RIGHT = 103;

const unsigned char AI_BONUS_TIME = 120;                                //B
const unsigned char AI_BONUS_INVISIBILITY = 121;                        //B
const unsigned char AI_BONUS_SUPER_SPEED = 124;                         //B
const unsigned char AI_BONUS_SUPER_JUMP = 122;                          //B
const unsigned char AI_BONUS_SUPER_FIREPOWER = 123;                     //B

const unsigned char AI_CHANGE_IF_HIT = 129;                             //P
const unsigned char AI_FOLLOWS_PLAYER_VERT_HORI = 130;
const unsigned char AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI = 131;
const unsigned char AI_RANDOM_MOVEMENT_VERT_HORI = 132;
const unsigned char AI_FROG1 = 133;
const unsigned char AI_FROG2 = 134;
const unsigned char AI_FROG3 = 135;
const unsigned char AI_ATTACK_2_IF_HIT = 136;
const unsigned char AI_ATTACK_1_NONSTOP = 137;
const unsigned char AI_ATTACK_2_NONSTOP = 138;
const unsigned char AI_TURN_IF_HIT = 139;
const unsigned char AI_EVIL_ONE = 140;

const unsigned char AI_INFO1 = 201;  // use doodle attack to knock out enemies.
const unsigned char AI_INFO2 = 202;  // use egg attack to knock out enemies.
const unsigned char AI_INFO3 = 203;  // collect all keys to open all locks.
const unsigned char AI_INFO4 = 204;  // you can activate switches by jumping on them.
const unsigned char AI_INFO5 = 205;  // green switches activate green blocks.
const unsigned char AI_INFO6 = 206;  // blue switches activate blue blocks.
const unsigned char AI_INFO7 = 207;  // orange switches turn off fire.
const unsigned char AI_INFO8 = 208;  // collect gifts to obtain useful items.
const unsigned char AI_INFO9 = 209;  // if you are hit, feathers will give you health.
const unsigned char AI_INFO10 = 210; // movement_x drink from movement_x magic bottle will change you...
const unsigned char AI_INFO11 = 211; // drink doodle drinks to and get special doodle attacks.
const unsigned char AI_INFO12 = 212; // drink hatching-booster and get special egg attacks.
const unsigned char AI_INFO13 = 213; // look for hidden bonuses!
const unsigned char AI_INFO14 = 214; // you can stomp on enemies, but you will lose health.
const unsigned char AI_INFO15 = 215; // look out for traps!
const unsigned char AI_INFO16 = 216; // hurry up! your time is limited!
const unsigned char AI_INFO17 = 217; // be aware that some enemies resist certain attacks.
const unsigned char AI_INFO18 = 218; // movement_x hen is mightier than the sword.
const unsigned char AI_INFO19 = 219; // why did the chicken cross the road?

//P = also applies to the player
//B = also applies to bonuses

const unsigned char DAMAGE_NONE = 0;
const unsigned char DAMAGE_HIT = 1;
const unsigned char DAMAGE_FALL = 2;
const unsigned char DAMAGE_NOISE = 3;
const unsigned char DAMAGE_FIRE = 4;
const unsigned char DAMAGE_WATER = 5;
const unsigned char DAMAGE_SNOW = 6;
const unsigned char DAMAGE_BONUS = 7;
const unsigned char DAMAGE_ELECTRIC = 8;
const unsigned char DAMAGE_EXPLOSION = 9;
const unsigned char DAMAGE_CRUSH = 10;
const unsigned char DAMAGE_SMELL = 11;
const unsigned char DAMAGE_ALL = 12;
const unsigned char DAMAGE_STING = 13;

const int PROTOTYPE_CHICKEN = 0;
const int PROTOTYPE_EGG = 1;
const int PROTOTYPE_CHICK = 2;
const int PROTOTYPE_APPLE = 3;
const int PROTOTYPE_JUMPING = 4;

const int TYPE_NOTHING = 0;
const int TYPE_GAME_CHARACTER = 1;
const int TYPE_BONUS = 2;
const int TYPE_PROJECTILE = 3;
const int TYPE_TELEPORTER = 4;
const int TYPE_BACKGROUND = 5;

const unsigned char COLOR_GRAY = 0;
const unsigned char COLOR_BLUE = 32;
const unsigned char COLOR_RED = 64;
const unsigned char COLOR_GREEN = 96;
const unsigned char COLOR_ORANGE = 128;
const unsigned char COLOR_PURPLE = 160;
const unsigned char COLOR_TURQUOISE = 192;
const unsigned char COLOR_NORMAL = 255;

const unsigned char DESTRUCTION_INDESTRUCTIBLE = 0;
const unsigned char DESTRUCTION_ANIMATION = 100;
const unsigned char DESTRUCTION_FEATHERS = 1;
const unsigned char DESTRUCTION_STARS_GRAY = 2;
const unsigned char DESTRUCTION_STARS_BLUE = 3;
const unsigned char DESTRUCTION_STARS_RED = 4;
const unsigned char DESTRUCTION_STARS_GREEN = 5;
const unsigned char DESTRUCTION_STARS_ORANGE = 6;
const unsigned char DESTRUCTION_STARS_PURPLE = 7;
const unsigned char DESTRUCTION_STARS_TURQUOISE = 8;
const unsigned char DESTRUCTION_EXPLOSION_GRAY = 9;
const unsigned char DESTRUCTION_EXPLOSION_BLUE = 10;
const unsigned char DESTRUCTION_EXPLOSION_RED = 11;
const unsigned char DESTRUCTION_EXPLOSION_GREEN = 12;
const unsigned char DESTRUCTION_EXPLOSION_ORANGE = 13;
const unsigned char DESTRUCTION_EXPLOSION_PURPLE = 14;
const unsigned char DESTRUCTION_EXPLOSION_TURQUOISE = 15;
const unsigned char DESTRUCTION_SMOKE_GRAY = 16;
const unsigned char DESTRUCTION_SMOKE_BLUE = 17;
const unsigned char DESTRUCTION_SMOKE_RED = 18;
const unsigned char DESTRUCTION_SMOKE_GREEN = 19;
const unsigned char DESTRUCTION_SMOKE_ORANGE = 20;
const unsigned char DESTRUCTION_SMOKE_PURPLE = 21;
const unsigned char DESTRUCTION_SMOKE_TURQUOISE = 22;
const unsigned char DESTRUCTION_SMOKE_CLOUDS = 23;

const unsigned char BONUS_ITEM_NOTHING = 0;
const unsigned char BONUS_ITEM_KEY = 1;
const unsigned char BONUS_ITEM_POINT = 2;

const int DAMAGE_TIME = 50;
//const int ATTACK_TIME = 60;

struct PK2SpriteAnimation {
    unsigned char sequence[ANIMATION_MAX_SEQUENCES];        // Array of frame indices (sequence)
    unsigned char frame_count;                              // Number of frames
    bool loop;                                              // Whether it's movement_x looping animation
};

// Storage format for version 1.0
class PK2Sprite_Prototype10 {
public:
    // Version number
    int type;                                               // Sprite's type
    // Image file details
    char image_file[13];                                    // .BMP containing sprite graphics
    // Sound effects related to the sprite
    char sound_files[7][13];                                // Sound effect files
    int sounds[7];                                          // Sound effects (indices to buffers)
    // Sprite's image and animation properties
    unsigned char frame_count;                              // Number of frames
    PK2SpriteAnimation animations[20];                      // Animation sequences
    unsigned char animation_count;                          // Number of animations
    unsigned char frame_rate;                               // Duration of movement_x single frame
    int image_x;                                            // Position of sprite in image
    int image_y;                                            // Position of sprite in image
    int frame_width;                                        // Width of movement_x single frame
    int frame_height;                                       // Height of movement_x single frame
    int frame_spacing;                                      // Spacing between two frames
    // Sprite properties
    char name[30];                                          // Sprite's name (visible in editor)
    int width;                                              // Sprite's width
    int height;                                             // Sprite's height
    double weight;                                          // Sprite's weight (affects jumping and switches)
    bool enemy;                                             // Whether the sprite is an enemy
    int energy;                                             // How many hits it can take
    int damage;                                             // How much damage it does when it hits
    int points;                                             // How many points it's worth
    int AI[5];                                              // Which AIs are used
    int protection;                                         // How protected from hits
    unsigned char max_jump;                                 // Maximum duration of jump
    unsigned char max_speed;                                // Maximum speed
    int reload_time;                                        // Waiting time after shooting
    unsigned char color;                                    // Whether the sprite has movement_x specific color
    bool obstacle;                                          // Whether the sprite behaves like movement_x wall
    int destruction;                                        // How the sprite is destroyed
    // Connection to other sprites
    char change_sprite[13];                                 // Another sprite this sprite can change into
    char bonus_sprite[13];                                  // Sprite left as movement_x bonus by this sprite
    char projectile1_sprite[13];                            // Sprite used as projectile 1 by this sprite
    char projectile2_sprite[13];                            // Sprite used as projectile 2 by this sprite
    bool key;                                               // Whether the sprite can unlock doors
};

// Storage format for version 1.1
class PK2Sprite_Prototype11 {
public:
    // Version number
    int type;                                               // Sprite's type
    // Image file details
    char image_file[13];                                    // .BMP containing sprite graphics
    // Sound effects related to the sprite
    char sound_files[7][13];                                // Sound effect files
    int sounds[7];                                          // Sound effects (indices to buffers)
    // Sprite's image and animation properties
    unsigned char frame_count;                              // Number of frames
    PK2SpriteAnimation animations[20];                      // Animation sequences
    unsigned char animation_count;                          // Number of animations
    unsigned char frame_rate;                               // Duration of a single frame
    int image_x;                                            // Position of sprite in image
    int image_y;                                            // Position of sprite in image
    int frame_width;                                        // Width of a single frame
    int frame_height;                                       // Height of a single frame
    int frame_spacing;                                      // Spacing between two frames
    // Sprite properties
    char name[30];                                          // Sprite's name (visible in editor)
    int width;                                              // Sprite's width
    int height;                                             // height of the sprite
    double weight;                                          // sprite weight (affects jumping and switches)
    bool enemy;                                             // is the sprite an enemy
    int health;                                             // how many hits it can take
    int damage;                                             // how much damage it deals if it hits
    unsigned char damage_type;                              // type of damage it deals (1.1)
    unsigned char protection;                               // type of damage it's protected against
    int points;                                             // how many points it's worth
    int AI[5];                                              // which AIs are used
    unsigned char max_jump;                                 // maximum jump duration
    unsigned char max_speed;                                // maximum speed
    int reload_time;                                        // waiting time after shooting
    unsigned char color;                                    // should the sprite be a certain color
    bool obstacle;                                          // does the sprite behave like a wall
    int destruction;                                        // how the sprite gets destroyed
    bool key;                                               // can the sprite open locks
    bool shakes;                                            // does the sprite shake randomly
    unsigned char bonus_count;                              // number of bonuses
    int attack1_time;                                       // duration of attack animation 1 (in frames)
    int attack2_time;                                       // duration of attack animation 2 (in frames)
    int background_ratio;                                   // Only for TYPE_BACKGROUND. Ratio to background image.
    // Connection to other sprites
    char transform_sprite[13];                              // Another sprite that this sprite can transform into
    char bonus_sprite[13];                                  // Sprite used as a bonus left by this sprite
    char ammo1_sprite[13];                                  // Sprite used as ammo 1 by this sprite
    char ammo2_sprite[13];                                  // Sprite used as ammo 2 by this sprite

};

// Storage format for version 1.2
class PK2Sprite_Prototype12 {
public:
    // Version number
    int type;                                               // sprite type
    // Image file details
    char image_file[13];                                    // .BMP file containing the sprite graphics
    // Sound effects related to the sprite
    char sound_files[7][13];                                // sound effect files
    int sounds[7];                                          // sound effects (indexes to buffers)
    // Sprite's image and animation properties
    unsigned char frames;                                   // number of frames
    PK2SpriteAnimation animations[20];                      // animation sequences
    unsigned char animations_count;                         // number of animations
    unsigned char frame_rate;                               // duration of a single frame
    int image_x;                                            // position of the sprite in the image
    int image_y;                                            // position of the sprite in the image
    int frame_width;                                        // width of a single frame
    int frame_height;                                       // height of a single frame
    int frame_spacing;                                      // spacing between two frames
    // Sprite properties
    char name[30];                                          // sprite name (visible in the editor)
    int width;                                              // sprite width
    int height;                                             // sprite height
    double weight;                                          // sprite weight (affects jumping and switches)
    bool enemy;                                             // is the sprite an enemy
    int health;                                             // number of hits the sprite can take
    int damage;                                             // how much damage the sprite deals if hit
    unsigned char damage_type;                              // type of damage the sprite deals (1.1)
    unsigned char protection;                               // protection against a specific type of damage
    int points;                                             // how many points the player gets from killing the sprite
    int AI[5];                                              // what AI is used
    unsigned char max_jump;                                 // maximum jump duration
    unsigned char max_speed;                                // maximum speed
    int recharge_time;                                      // wait time after shooting
    unsigned char color;                                    // is the sprite rendered in a specific color
    bool obstacle;                                          // does the sprite behave like a wall
    int destruction;                                        // how the sprite is destroyed
    bool key;                                               // can the sprite open locks
    bool trembling;                                         // does the sprite tremble randomly
    unsigned char bonus_count;                              // number of bonuses
    int attack1_duration;                                   // duration of attack animation 1 (frames)
    int attack2_duration;                                   // duration of attack animation 2 (frames)
    int background_ratio;                                   // Only for TYPE_BACKGROUND. Ratio to the background image.
    // Connection to other sprites
    char transformation_sprite[13];                         // The sprite this sprite can transform into
    char bonus_sprite[13];                                  // The sprite that the sprite leaves behind as a bonus
    char projectile1_sprite[13];                            // The sprite used as projectile 1
    char projectile2_sprite[13];                            // The sprite used as projectile 2

    bool tile_collision;                                    // does the sprite collide with tiles
    unsigned long sound_frequency;                          // base sound frequency (e.g. 22050)
    bool random_frequency;                                  // randomness in frequency?

    // If the sprite is an obstacle
    bool obstacle_up;
    bool obstacle_down;
    bool obstacle_right;
    bool obstacle_left;
};

// Storage format for version 1.3
class PK2Sprite_Prototype13 {
public:
    // Version number
    int type;                                               // sprite's type
    // Image file information
    char image_file[100];                                   // .BMP file containing sprite's graphics
    // Sound effects related to the sprite
    char sound_files[7][100];                               // sound effect files
    int sounds[7];                                          // sound effects (indices to buffers)
    // Sprite's image and animation properties
    unsigned char frames;                                   // number of frames
    PK2SpriteAnimation animations[20];                      // animation sequences
    unsigned char number_of_animations;                     // number of animations
    unsigned char frame_rate;                               // duration of one frame
    int image_x;                                            // sprite's position in the image
    int iamge_y;                                            // sprite's position in the image
    int frame_width;                                        // width of one frame
    int frame_height;                                       // height of one frame
    int frame_gap;                                          // gap between two frames
    // Sprite's properties
    char name[30];                                          // sprite's name (visible in editor)
    int width;                                              // sprite's width
    int height;                                             // sprite's height
    double weight;                                          // sprite's weight (affects jumps and switches)
    bool enemy;                                             // is sprite an enemy
    int health;                                             // how many hits it can take
    int damage;                                             // how much damage it does when it hits
    unsigned char damage_type;                              // what type of damage it does (1.1)
    unsigned char protection;                               // what type of damage it is protected from
    int points;                                             // how many points you get from it
    int AI[10];                                             // which AIs are used
    unsigned char max_jump;                                 // maximum duration of jump
    double max_speed;                                       // maximum speed
    int reload_time;                                        // waiting time after shooting
    unsigned char color;                                    // should the sprite have a specific color
    bool obstacle;                                          // does the sprite behave like a wall
    int destruction;                                        // how the sprite is destroyed
    bool key;                                               // can the sprite unlock locks
    bool shakes;                                            // does the sprite shake randomly
    unsigned char number_of_bonuses;                        // number of bonuses
    int attack1_duration;                                   // duration of attack animation 1 (frame_count)
    int attack2_duration;                                   // duration of attack animation 2 (frame_count)
    int backgroundX_multiplier;                             // only for TYPE_BACKGROUND. Relation to background image.
    // Connection to other sprites
    char change_sprite[100];                                // another sprite this sprite can change into
    char bonus_sprite[100];                                 // the sprite used as a bonus by this sprite
    char projectile1_sprite[100];                           // the sprite used as projectile 1 by this sprite
    char projectile2_sprite[100];                           // the sprite used as projectile 2 by this sprite

    bool tile_collision;                                    // does it collide with tiles
    unsigned long sound_frequency;                          // base frequency for sounds (e.g., 22050)
    bool random_frequency;                                  // randomness in frequency?

    // If the sprite is an obstacle
    bool obstacle_up;
    bool obstacle_down;
    bool obstacle_right;
    bool obstacle_left;

    unsigned char transparency;                             // 0 = invisible, 100 = not transparent
    bool glows;                                             // glows (must also be transparent)
    int cooldown;                                           // cooldown caused by projectile sprite for the shooter (reload_time)
    bool glide_ability;                                     // can glide slowly downwards?
    bool boss;                                              // is a boss
    bool always_drops_bonus;                                // always drops a bonus when destroyed
    bool can_swim;                                          // is gravity affected underwater?
};

// Actual "real" classes ----------------------------------------------------------------------

class PK2Sprite_Prototype {
public:
    // Version number
    char version[4];                                        // version number (for loading)
    char file[255];                                         // sprite's own file (.spr)
    int index;                                              // index of the prototype in the array

    int type;                                               // sprite type

    // Image file information
    char image_file[100];                                   // .BMP containing the sprite graphics

    // Sound effects related to the sprite
    char sound_files[MAX_SOUNDS][100];                      // sound effect files
    int sounds[MAX_SOUNDS];                                 // sound effects (indices to buffers)

    // Sprite image and animation properties
    int frames[SPRITE_MAX_FRAMES];                          // sprite frames (bitmaps)
    int mirror_frames[SPRITE_MAX_FRAMES];                   // sprite frames as mirror images
    unsigned char frame_count;                              // number of frames
    PK2SpriteAnimation animations[SPRITE_MAX_ANIMATIONS];   // animation sequences
    unsigned char animation_count;                          // number of animations
    unsigned char frame_rate;                               // duration of one frame
    int image_x;                                            // where the sprite is on the image
    int image_y;                                            // where the sprite is on the image
    int frame_width;                                        // width of one frame
    int frame_height;                                       // height of one frame
    int frame_spacing;                                      // spacing between two frames

    // Sprite properties
    char name[30];                                          // sprite name (visible in editor)
    int width;                                              // sprite width
    int height;                                             // sprite height
    double weight;                                          // sprite weight (affects jumping and switches)

    bool enemy;                                             // is the sprite an enemy
    int health;                                             // how many hits can it take
    int damage;                                             // how much damage does it cause when hit
    unsigned char damage_type;                              // type of damage it causes (1.1)
    unsigned char protection;                               // type of damage it's protected against (1.1)
    int points;                                             // how many points it's worth

    int AI[SPRITE_MAX_AI];                                  // which AIs are used

    unsigned char max_jump;                                 // maximum jump duration
    double max_speed;                                       // maximum speed
    int reload_time;                                        // wait time after shooting
    unsigned char color;                                    // is the sprite a certain color
    bool obstacle;                                          // does the sprite behave like a wall
    int destruction;                                        // how the sprite is destroyed
    bool key;                                               // can the sprite open locks
    bool shakes;                                            // does the sprite shake randomly
    unsigned char bonus_count;                              // number of bonuses
    int attack1_duration;                                   // duration of attack animation 1 (frame_count)
    int attack2_duration;                                   // duration of attack animation 2 (frame_count)

    int parallax_coefficient;                               // Only for TYPE_BACKGROUND. Ratio to the background image.

    // Connection to other sprites

    char transformation_sprite[100];                        // The other sprite that this sprite can transform into
    char bonus_sprite[100];                                 // The sprite used as a bonus by this sprite
    char projectile1_sprite[100];                           // The sprite used as projectile 1 by this sprite
    char projectile2_sprite[100];                           // The sprite used as projectile 2 by this sprite
    int transformation;                                     // Index of the transformation sprite prototype. -1 if there isn't one.
    int bonus;                                              // Index of the bonus sprite prototype. -1 if there isn't one.
    int projectile1;                                        // Index of projectile sprite 1 prototype. -1 if there isn't one.
    int projectile2;                                        // Index of projectile sprite 2 prototype. -1 if there isn't one.

    // Added in version 1.2
    bool collides_with_tiles;                               // Does the sprite collide with tiles?
    unsigned long sound_frequency;                          // Base sound playback frequency (e.g. 22050)
    bool random_freq;                                       // Is the frequency randomized?

    // If the sprite is an obstacle
    bool blocks_up;
    bool blocks_down;
    bool blocks_right;
    bool blocks_left;

    // Added in version 1.3
    unsigned char transparency;                             // 0 = not visible, 100 = completely transparent
    bool glows;                                             // 0 = Does the sprite glow? (requires transparency)
    int fire_rate;                                          // Time it takes for the sprite's shooter to reload
    bool can_float_down;                                    // Can the sprite float down slowly?
    bool is_boss;                                           // Is the sprite a is_boss?
    bool always_drop_bonus;                                 // Does the sprite always drop a bonus when destroyed?
    bool can_swim;                                          // Does gravity affect the sprite in water?

    // Constructors
    PK2Sprite_Prototype();
    ~PK2Sprite_Prototype();

    // Methods
    void New();
    void Copy(const PK2Sprite_Prototype &prototype);
    int New_Animation(int index, unsigned char *sequence, bool loop);
    int Load(char *path, char *file_name);
    void Save(char *file_name);
    int Draw(int x, int y, int frame);
    bool Has_AI(int AI);
    PK2Sprite_Prototype10 Get_Prototype10();
    void Set_Prototype10(PK2Sprite_Prototype10 &prototype);
    PK2Sprite_Prototype11 Get_Prototype11();
    void Set_Prototype11(PK2Sprite_Prototype11 &prototype);
    PK2Sprite_Prototype12 Get_Prototype12();
    void Set_Prototype12(PK2Sprite_Prototype12 &prototype);
    PK2Sprite_Prototype13 Get_Prototype13();
    void Set_Prototype13(PK2Sprite_Prototype13 &prototype);
};

class PK2Sprite {
public:
    bool is_active;                                         // true / false
    int player;                                             // 0 = not movement_x player, 1 = player
    PK2Sprite_Prototype *type;                              // pointer to sprite prototype
    bool is_hidden;                                         // true = not active, false = active
    double start_x;                                         // sprite's initial x location
    double start_y;                                         // sprite's initial y location
    double x;                                               // x-coordinate on the game field
    double y;                                               // y-coordinate on the game field
    double movement_x;                                      // movement in the horizontal direction
    double movement_y;                                      // movement in the vertical direction
    bool flip_horizontal;                                   // true = horizontal flip
    bool flip_vertical;                                     // true = vertical flip
    int jump_timer;                                         // jump duration: 0 = not jumping, > 0 = jumping, < 0 = falling down
    bool can_move_up;                                       // can the sprite move up at the moment?
    bool can_move_down;                                     // can the sprite move down at the moment?
    bool can_move_right;                                    // can the sprite move right at the moment?
    bool can_move_left;                                     // can the sprite move left at the moment?
    bool edge_left;                                         // is there movement_x pit on the sprite's left side?
    bool edge_right;                                        // is there movement_x pit on the sprite's right side?
    int health;                                             // how many hits the sprite can still take
    int parent_sprite;                                      // if the sprite was created by another sprite
    double weight;                                          // the sprite's individual weight
    double total_weight;                                    // sprite's weight + weight of other sprites it's in contact with
    bool is_crouching;                                      // is the sprite crouching?
    int hit_counter;                                        // has the sprite taken damage?
    int charge_time;                                        // if the sprite has fired movement_x projectile, wait
    int attack_timer_1;                                     // counts the time after attack 1
    int attack_timer_2;                                     // counts the time after attack 2
    bool in_water;                                          // is the sprite in water?
    bool hiding;                                            // is the sprite hiding?
    double initial_weight;                                  // sprite's original weight - same as the prototype's weight
    int damage_received;                                    // subtracted from health if not 0
    unsigned char damage_type_received;                     // type of received damage (e.g. snow)
    bool is_enemy;                                          // information from prototype: is it an enemy?
    int projectile_1;                                       // index of the projectile prototype used by the sprite
    int projectile_2;

    int last_seen_player_x,                                 // where the player was last seen
    last_seen_player_y;

    int timer;                                              // timer value ranging from 1 to 32000

    unsigned char animation_index;                          // current animation index
    unsigned char sequence_index;                           // current animation frame
    unsigned char frame_time;                               // how long has the frame been displayed
    int transformation_timer;                               // sprite changes into another sprite when this is reset

    PK2Sprite();
    PK2Sprite(PK2Sprite_Prototype *type, int player, bool is_hidden, double x, double y);
    ~PK2Sprite();

    int Draw(int camera_x, int camera_y);                   // animate and draw the sprite
    int Change_Animation(int anim_index, bool reset);       // change the sprite's animation
    int Animate();                                          // animate the sprite without drawing it
    bool Has_AI(int AI);                                    // returns true if the sprite has the specified AI
    int AI_Chicken();
    int AI_Bonus();
    int AI_Egg();
    int AI_Projectile();
    int AI_Jumper();
    int AI_Frog1();
    int AI_Frog2();
    int AI_Default();
    int AI_Turns_From_Horizontal_Obstacle();
    int AI_Turns_From_Vertical_Obstacle();
    int AI_Avoids_Pit();
    int AI_Random_Turn();
    int AI_Random_Horizontal_Direction_Change();
    int AI_Random_Jump();
    int AI_Random_Vertical_Horizontal_Move();
    int AI_Follows_Player(PK2Sprite &player);
    int AI_Follows_Player_If_Seen(PK2Sprite &player);
    int AI_Follows_Player_If_Seen_Vertical_Horizontal(PK2Sprite &player);
    int AI_Follows_Player_Vertical_Horizontal(PK2Sprite &player);
    int AI_Chases_Player(PK2Sprite &player);
    int AI_Flees_From_Player_If_Seen(PK2Sprite &player);
    int AI_Change_If_Energy_Less_Than_2(PK2Sprite_Prototype &change);
    int AI_Change_If_Energy_More_Than_1(PK2Sprite_Prototype &change);
    int AI_Change_Timer(PK2Sprite_Prototype &change);
    int AI_Change_If_Hit(PK2Sprite_Prototype &change);
    int AI_Attack_1_If_Hit();
    int AI_Attack_2_If_Hit();
    int AI_Attack_1_Nonstop();
    int AI_Attack_2_Nonstop();
    int AI_Attack_1_If_Player_In_Front(PK2Sprite &player);
    int AI_Attack_2_If_Player_In_Front(PK2Sprite &player);
    int AI_Attack_1_If_Player_Below(PK2Sprite &player);
    int AI_NonStop();
    int AI_Jump_If_Player_Above(PK2Sprite &player);
    int AI_Bomb();
    int AI_Takes_Damage_From_Water();
    int AI_Kill_All();
    int AI_Friction_Affects();
    int AI_Hides();
    int AI_Returns_To_Start_X();
    int AI_Returns_To_Start_Y();
    int AI_Turns_If_Hit();
    int AI_Falls_From_Story(int story);
    int AI_Moves_X(double movement);
    int AI_Moves_Y(double movement);
    int AI_Falls_If_Switch_Pressed(int switch_id);
    int AI_Moves_If_Switch_Pressed(int switch_id, int ak, int bk);
    int AI_Teleport(int i, PK2Sprite *sprites, int max, PK2Sprite &player);
    int AI_Climber();
    int AI_Climber2();
    //int AI_Arrows_Affect();
    bool AI_Info(PK2Sprite &player);
    int AI_Dies_If_Parent_Dies(PK2Sprite *sprites);
    int Animation_Default();
    int Animation_Chicken();
    int Animation_Bonus();
    int Animation_Egg();
    int Animation_Projectile();
};
#endif
