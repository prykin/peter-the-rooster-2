//#define WIN32_LEAN_AND_MEAN
//#define INITGUID

//#include "ddraw.h" //TODO: DDRAW
#include <string.h>
//#include <istream.h>
#include <stdlib.h>
#include <stdio.h>
//#include <mmsystem.h>

#include "../sprites.h"

void Animation_New(PK2Sprite_Prototype &proto, int anim_i, unsigned char *sequence, bool loop) {
    unsigned char frame_i = 0;
    if (anim_i < SPRITE_MAX_ANIMATIONS) {
        proto.animations[anim_i].frame_count = 0;
        while (frame_i < ANIMATION_MAX_SEQUENCES && sequence[frame_i] != 0) {
            proto.animations[anim_i].sequence[frame_i] = sequence[frame_i];
            proto.animations[anim_i].frame_count++;
            frame_i++;
        }
        proto.animations[anim_i].loop = loop;
    }
}

/*
void Animation_New(PK2Sprite_Prototype &proto, int anim_i, char *sequence, bool loop) {
	unsigned char frame_i = 0;
	int frame = 1;

	if (anim_i < SPRITE_MAX_ANIMATIONS) {
		proto.animations[anim_i].frame_count = 0;
    while (frame_i < ANIMATION_MAX_SEQUENCES && frame != 0) {
			switch (sequence[frame_i]) {
                case '1':
                    frame = 1;
                    break;
                case '2':
                    frame = 2;
                    break;
                case '3':
                    frame = 3;
                    break;
                case '4':
                    frame = 4;
                    break;
                case '5':
                    frame = 5;
                    break;
                case '6':
                    frame = 6;
                    break;
                case '7':
                    frame = 7;
                    break;
                case '8':
                    frame = 8;
                    break;
                case '9':
                    frame = 9;
                    break;
                case '0':
                    frame = 0;
                    break;
                case ',':
                    frame = 10;
                    break;

                default:
                    frame = 0;
                    break;
			}
			proto.animations[anim_i].sequence[frame_i] = frame;
			proto.animations[anim_i].frame_count++;
			frame_i++;
		}
		proto.animations[anim_i].loop = loop;
	}
}
*/

void create_prototype_rooster(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "rooster");
    strcpy(proto.transformation_sprite, "r_naked.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");//egg.spr
    strcpy(proto.projectile2_sprite, "");//doodle.spr

    strcpy(proto.sound_files[SOUND_DAMAGE], "p_damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "p_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "p_layegg.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "p_doodle.wav");
    proto.AI[0] = AI_CHICKEN;
    proto.AI[1] = AI_CHANGE_IF_ENERGY_LESS_THAN_2;
    for (int ai = 2; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;
    proto.sound_frequency = 22050;
    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 21;
    proto.frame_rate = 4;
    proto.attack1_duration = 30;
    proto.attack2_duration = 60;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 32;
    proto.frame_height = 59;
    proto.height = 59;//58
    proto.reload_time = 0;//60
    proto.width = 30;
    proto.max_jump = 26;//24
    proto.max_speed = 3;
    proto.weight = 1;
    proto.random_freq = true;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.transparency = false;
    proto.glows = false;
    proto.fire_rate = 0;
    proto.can_float_down = true;
    proto.is_boss = false;
    proto.always_drop_bonus = false;

    unsigned char seq1[8] = {1, 1, 1, 1, 1, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    unsigned char seq2[5] = {3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    unsigned char seq4[5] = {8, 8, 9, 9, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);
    unsigned char seq5[5] = {1, 10, 11, 12, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);
    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);
    unsigned char seq7[8] = {15, 15, 16, 16, 17, 17, 18, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);
    unsigned char seq8[5] = {19, 19, 19, 19, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    unsigned char seq9[5] = {20, 20, 21, 21, 0};
    Animation_New(proto, ANIMATION_ATTACK2, seq9, true);

    proto.Save(file);
}

void create_prototype_naked_rooster(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "naked rooster");
    strcpy(proto.transformation_sprite, "rooster.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
/*
	for (int sound=0;sound<MAX_SOUNDS;sound++)
	{
		strcpy(proto.sound_files[sound], "");
		proto.sounds[sound] = -1;
	}
 */
    strcpy(proto.sound_files[SOUND_DAMAGE], "p_damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "p_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "p_layegg.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "p_doodle.wav");

    proto.AI[0] = AI_CHICKEN;
    proto.AI[1] = AI_CHANGE_IF_ENERGY_MORE_THAN_1;
    proto.AI[2] = AI_HIDES;
    for (int ai = 2; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;
    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 22;
    proto.frame_rate = 4;
    proto.attack1_duration = 30;
    proto.attack2_duration = 60;
    proto.image_x = 1;
    proto.image_y = 125;
    proto.frame_width = 32;
    proto.frame_height = 59;
    proto.height = 58;
    proto.reload_time = 0;//60
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_float_down = true;

    unsigned char seq1[9] = {1, 1, 21, 21, 22, 22, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    unsigned char seq2[5] = {3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    unsigned char seq4[5] = {8, 8, 9, 9, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);
    unsigned char seq5[5] = {1, 10, 11, 12, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);
    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);
    unsigned char seq7[8] = {15, 15, 16, 16, 17, 17, 18, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);
    unsigned char seq8[5] = {19, 19, 19, 19, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    unsigned char seq9[5] = {20, 20, 20, 20, 0};
    Animation_New(proto, ANIMATION_ATTACK2, seq9, false);

    proto.Save(file);
}

void create_prototype_baby_chicken(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "baby chicken");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }
    strcpy(proto.sound_files[SOUND_DAMAGE], "chick.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    proto.AI[0] = AI_SMALL_CHICKEN;
    proto.AI[1] = AI_FOLLOW_PLAYER;
    proto.animation_count = 7;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 249;
    proto.frame_width = 26;
    proto.frame_height = 29;
    proto.height = 29;
    proto.reload_time = 5;
    proto.width = 26;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.8;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_float_down = false;

    unsigned char seq1[9] = {1, 1, 1, 1, 1, 1, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    unsigned char seq2[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);
    unsigned char seq5[2] = {8, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);
    unsigned char seq6[2] = {7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);
    unsigned char seq7[7] = {7, 4, 4, 5, 5, 6, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);
    unsigned char seq8[7] = {1, 3, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hen(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "a hen");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "feather.spr");
    strcpy(proto.projectile1_sprite, "egg.spr");//egg.spr
    strcpy(proto.projectile2_sprite, "");//doodle.spr
    strcpy(proto.sound_files[SOUND_DAMAGE], "p_damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "p_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "p_layegg.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "p_layegg.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "hen.wav");

    proto.AI[0] = AI_CHICKEN;
    proto.sound_frequency = 26050;
    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 7;
    proto.attack1_duration = 30;
    proto.attack2_duration = 60;
    proto.image_x = 106;
    proto.image_y = 63;
    proto.frame_width = 34;
    proto.frame_height = 37;
    proto.height = 37;//58
    proto.reload_time = 60;//40
    proto.width = 30;
    proto.max_jump = 20;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.random_freq = true;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.transparency = false;
    proto.glows = false;
    proto.fire_rate = 0;
    proto.can_float_down = true;
    proto.is_boss = false;
    proto.always_drop_bonus = false;

    unsigned char seq1[8] = {1, 1, 1, 1, 1, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    unsigned char seq2[5] = {3, 3, 4, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[5] = {5, 5, 6, 6, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    unsigned char seq4[3] = {5, 6, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);
    unsigned char seq5[2] = {7, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);
    unsigned char seq6[3] = {8, 9, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);
    unsigned char seq7[6] = {8, 8, 8, 7, 10, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);
    unsigned char seq8[5] = {5, 5, 5, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    unsigned char seq9[5] = {9, 9, 9, 9, 0};
    Animation_New(proto, ANIMATION_ATTACK2, seq9, true);

    proto.Save(file);
}

void create_prototype_candy(char *file, char *name, unsigned char color, int points) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bon2.bmp");
    strcpy(proto.name, name);
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus8.wav");
    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;//1
    proto.frame_rate = 4;
    proto.image_x = 1;//1
    proto.image_y = 59;
    proto.frame_width = 27;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 27;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = points;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_GRAY + color / 32;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = color;
    proto.enemy = false;
    //unsigned char seq1[2] = {1,0};
    unsigned char seq1[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_jewel(char *file, char *name, unsigned char color, int points) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bon2.bmp");
    strcpy(proto.name, name);
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");
    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;//1
    proto.frame_rate = 4;
    proto.image_x = 1;//1
    proto.image_y = 89;
    proto.frame_width = 22;
    proto.frame_height = 24;
    proto.height = 24;
    proto.reload_time = 0;
    proto.width = 22;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = points;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_GRAY + color / 32;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = color;
    proto.enemy = false;
    //unsigned char seq1[2] = {1,0};
    unsigned char seq1[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_apple(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "apple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus9.wav");//bonus9.wav/apple.wav
    proto.AI[0] = AI_BONUS;
    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;//1
    proto.frame_rate = 4;
    proto.image_x = 380;//1
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 23;
    proto.height = 23;
    proto.reload_time = 0;
    proto.width = 19;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 10;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    //unsigned char seq1[2] = {1,0};
    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_falling_apple(char *file) {
    PK2Sprite_Prototype proto;
    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "falling apple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");
    proto.AI[0] = AI_BONUS;
    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;//1
    proto.frame_rate = 4;
    proto.image_x = 380;//1
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 23;
    proto.height = 23;
    proto.reload_time = 0;
    proto.width = 19;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 10;
    proto.random_freq = true;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_changing_apple(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "changing apple");
    strcpy(proto.transformation_sprite, "apple4.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus13.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_CHANGE_TIMER;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 380;
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 23;
    proto.height = 23;
    proto.reload_time = 400;
    proto.width = 19;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 12;
    proto.random_freq = true;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_ORANGE;
    proto.enemy = false;

    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_rotten_apple(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "rotten apple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 380;
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 23;
    proto.height = 23;
    proto.reload_time = 500;
    proto.width = 19;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 0;
    proto.random_freq = true;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_apple5(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "newton apple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus9.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FALLS_FROM_STORY;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 380;
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 23;
    proto.height = 23;
    proto.reload_time = 500;
    proto.width = 19;
    proto.max_jump = 20;
    proto.max_speed = 4;
    proto.weight = 0;
    proto.points = 14;
    proto.random_freq = true;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_PURPLE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_PURPLE;
    proto.enemy = false;

    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_big_apple(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "big apple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "app_bite.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 134;
    proto.image_y = 1;
    proto.frame_width = 26;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 26;
    proto.max_jump = 20;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 100;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_big_apple2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "big apple 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "app_bite.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 134;
    proto.image_y = 1;
    proto.frame_width = 26;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 26;
    proto.max_jump = 20;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 200;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_ORANGE;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_carrot(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "carrot");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus8.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 97;
    proto.image_y = 1;
    proto.frame_width = 10;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 10;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_cheese(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "carrot");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus11.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 14;
    proto.image_x = 336;
    proto.image_y = 1;
    proto.frame_width = 24;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 24;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 30;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_pepper(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "hot pepper");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodlef2.spr");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus6.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 186;
    proto.image_y = 1;
    proto.frame_width = 15;
    proto.frame_height = 20;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 15;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 40;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_feather(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "feather");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus4.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 77;
    proto.image_y = 1;
    proto.frame_width = 17;
    proto.frame_height = 17;
    proto.height = 17;
    proto.reload_time = 0;
    proto.width = 17;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_FEATHERS;
    proto.type = TYPE_BONUS;
    proto.damage = -1;
    proto.damage_type = DAMAGE_BONUS;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_feather2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "super feather");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus4.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 77;
    proto.image_y = 1;
    proto.frame_width = 17;
    proto.frame_height = 17;
    proto.height = 17;
    proto.reload_time = 0;
    proto.width = 17;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_FEATHERS;
    proto.type = TYPE_BONUS;
    proto.damage = -3;
    proto.damage_type = DAMAGE_BONUS;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

/*
void create_prototype_orange(char *file)
{
	PK2Sprite_Prototype proto;

	strcpy(proto.version,LATEST_VERSION_PK2SPRITE);
	strcpy(proto.image_file,"spr_bons.bmp");
	strcpy(proto.name, "orange");
	strcpy(proto.transformation_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.projectile1_sprite,"");
	strcpy(proto.projectile2_sprite,"");

	for (int sound=0;sound<MAX_SOUNDS;sound++)
	{
		strcpy(proto.sound_files[sound], "");
		proto.sounds[sound] = -1;
	}

	strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

	proto.AI[0]			= AI_BONUS;

	proto.animation_count	= 1;
	proto.key			= false;
	proto.health		= 1;
	proto.obstacle			= false;
	proto.frame_count		= 1;
	proto.frame_rate	= 4;
	proto.image_x		= 23;
	proto.image_y		= 1;
	proto.frame_width  = 19;
	proto.frame_height = 19;
	proto.height		= 19;
	proto.reload_time	= 0;
	proto.width		= 19;
	proto.max_jump		= 22;
	proto.max_speed	= 3;
	proto.weight			= 0.4;
	proto.points		= 30;
	proto.protection		= DAMAGE_NONE;
	proto.shakes       = false;
	proto.destruction	= DESTRUCTION_STARS_ORANGE;
	proto.type		= TYPE_BONUS;
	proto.damage		= 0;
	proto.damage_type= DAMAGE_NONE;
	proto.color			= COLOR_NORMAL;
	proto.enemy	= false;

	unsigned char seq1[2] = {1,0};
	Animation_New(proto, ANIMATION_IDLE,	 seq1, false);

	proto.Save(file);
}
*/
void create_prototype_orange(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "orange");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");

    proto.AI[0] = AI_BASIC;//AI_BONUS;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 204;
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 0;
    proto.width = 19;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 30;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[9] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq2, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, true);
    Animation_New(proto, ANIMATION_CROUCH, seq2, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, true);
    Animation_New(proto, ANIMATION_DEATH, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_grapefruit(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "orange");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");

    proto.AI[0] = AI_BASIC;//AI_BONUS;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 4;
    proto.image_x = 204;
    proto.image_y = 1;
    proto.frame_width = 19;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 0;
    proto.width = 19;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 45;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[2] = {4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[9] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq2, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, true);
    Animation_New(proto, ANIMATION_CROUCH, seq2, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, true);
    Animation_New(proto, ANIMATION_DEATH, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_grape(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "grape");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");

    proto.AI[0] = AI_BASIC;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 585;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 11;
    proto.height = 11;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 22;
    proto.max_speed = 4;
    proto.weight = 0.5;
    proto.points = 5;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    Animation_New(proto, ANIMATION_WALK, seq1, false);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, false);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, false);
    Animation_New(proto, ANIMATION_CROUCH, seq1, false);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, false);
    Animation_New(proto, ANIMATION_DEATH, seq1, false);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, false);

    proto.Save(file);
}

void create_prototype_pineapple1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bon2.bmp");
    strcpy(proto.name, "pineapple");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus9.wav");//bonus9.wav/apple.wav

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;//1
    proto.frame_rate = 4;
    proto.image_x = 1;//1
    proto.image_y = 1;
    proto.frame_width = 21;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 18;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    //unsigned char seq1[2] = {1,0};
    unsigned char seq1[7] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_ghost_powder1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bon2.bmp");
    strcpy(proto.name, "ghost powder");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");

    proto.AI[0] = AI_BASIC;//AI_BONUS;
    proto.AI[1] = AI_BONUS_INVISIBILITY;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 20;
    proto.frame_height = 24;
    proto.height = 24;
    proto.reload_time = 1500;
    proto.width = 20;
    proto.max_jump = 10;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_BLUE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = false;

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq2, false);
    Animation_New(proto, ANIMATION_WALK, seq2, false);
    Animation_New(proto, ANIMATION_JUMP_UP, seq2, false);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, false);
    Animation_New(proto, ANIMATION_CROUCH, seq2, false);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, false);
    Animation_New(proto, ANIMATION_DEATH, seq2, false);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, false);

    proto.Save(file);
}

void create_prototype_ghost_powder2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bon2.bmp");
    strcpy(proto.name, "ghost powder renew");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus12.wav");

    proto.AI[0] = AI_BASIC;//AI_BONUS;
    proto.AI[1] = AI_BONUS_INVISIBILITY;
    proto.AI[2] = AI_NEW_IF_DESTROYED;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 20;
    proto.frame_height = 24;
    proto.height = 24;
    proto.reload_time = 1500;
    proto.width = 20;
    proto.max_jump = 10;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_TURQUOISE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_TURQUOISE;
    proto.enemy = false;

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq2, false);
    Animation_New(proto, ANIMATION_WALK, seq2, false);
    Animation_New(proto, ANIMATION_JUMP_UP, seq2, false);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, false);
    Animation_New(proto, ANIMATION_CROUCH, seq2, false);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, false);
    Animation_New(proto, ANIMATION_DEATH, seq2, false);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, false);

    proto.Save(file);
}

void create_prototype_easter_egg(char *file, char *name, char *sprite, int points, unsigned char color, unsigned char destruction) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, name);
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, sprite);
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");//bonus9.wav/apple.wav

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 4;
    proto.image_x = 380;
    proto.image_y = 27;
    proto.frame_width = 20;
    proto.frame_height = 21;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = points;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = destruction;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = color;
    proto.enemy = false;

    //unsigned char seq1[2] = {1,0};
    unsigned char seq1[9] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

// TODO: =>
void create_prototype_key1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "key type 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus3.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = true;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 14;
    proto.image_x = 45;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 20;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_key2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "key type 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = true;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 45;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 20;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_key3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "spoo-key 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus3.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FALLS_FROM_STORY;

    proto.animation_count = 1;
    proto.key = true;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 14;
    proto.image_x = 45;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 20;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_TURQUOISE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_TURQUOISE;
    proto.enemy = false;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_jumping_key(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "jumping key");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus3.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_JUMPING;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_NONSTOP;

    proto.animation_count = 1;
    proto.key = true;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 45;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 20;
    proto.height = 20;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 10;
    proto.max_speed = 2;
    proto.weight = 0.3;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    Animation_New(proto, ANIMATION_WALK, seq1, false);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, false);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, false);
    Animation_New(proto, ANIMATION_CROUCH, seq1, false);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, false);
    Animation_New(proto, ANIMATION_DEATH, seq1, false);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, false);

    proto.Save(file);
}

void create_prototype_clock20s(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "clock 20 sec");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_BONUS_TIME;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 79;
    proto.image_y = 62;
    proto.frame_width = 28;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 20;//sekunteja
    proto.width = 28;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 10;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_clock10s(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "clock 10 sec");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_BONUS_TIME;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 79;
    proto.image_y = 62;
    proto.frame_width = 28;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 10;//sekunteja
    proto.width = 28;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 20;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_clock30s(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "clock 30 sec");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_BONUS_TIME;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 79;
    proto.image_y = 62;
    proto.frame_width = 28;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 30;//sekunteja
    proto.width = 28;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 20;
    proto.random_freq = true;//false
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_GREEN;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_egg(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "eggcrack.wav");

    proto.AI[0] = AI_EGG;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 5;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 281;
    proto.frame_width = 11;
    proto.frame_height = 13;
    proto.height = 13;
    proto.reload_time = 10;
    proto.width = 11;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0.3;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 50;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq2, false);

    proto.Save(file);
}

void create_prototype_smelly_egg(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "eggcrack.wav");

    proto.AI[0] = AI_EGG;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 5;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 281;
    proto.frame_width = 11;
    proto.frame_height = 13;
    proto.height = 13;
    proto.reload_time = 10;
    proto.width = 11;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0.3;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_GREEN;
    proto.enemy = false;
    proto.fire_rate = 10;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq2, false);

    proto.Save(file);
}

void create_prototype_fire_egg(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "fire egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "flame.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "eggcrack.wav");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_ATTACK_1_IF_HIT;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 5;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 281;
    proto.frame_width = 11;
    proto.frame_height = 13;
    proto.height = 13;
    proto.reload_time = 40;
    proto.width = 11;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0.3;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;
    proto.fire_rate = 50;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq2, false);

    proto.Save(file);
}

void create_prototype_ice_egg(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "ice egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "icecube.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "eggcrack.wav");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_ATTACK_1_IF_HIT;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 5;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 281;
    proto.frame_width = 11;
    proto.frame_height = 13;
    proto.height = 13;
    proto.reload_time = 40;
    proto.width = 11;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0.3;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_STARS_BLUE;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = false;
    proto.fire_rate = 50;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq2, false);

    proto.Save(file);
}

void create_prototype_bomb(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "bomb");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "flame.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "explot.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_BOMB;
    proto.AI[2] = AI_ATTACK_1_IF_HIT;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 81;
    proto.frame_width = 23;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 60;
    proto.width = 23;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0.4;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.fire_rate = 0;

    unsigned char seq1[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_cockadoodledoo(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "cockadoodledoo");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;

    for (int ai = 2; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 297;
    proto.frame_width = 25;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 50;
    proto.width = 25;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_NOISE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 90;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_fire_doodle_flame(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "fire doodle flame");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "flame.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_ATTACK_1_IF_HIT;
    proto.AI[3] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 319;
    proto.frame_width = 25;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 50;
    proto.width = 25;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_SMOKE_CLOUDS;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;//2
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_RED;
    proto.enemy = false;
    proto.fire_rate = 60;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_fire_doodle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "fire doodle");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 319;
    proto.frame_width = 25;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 50;
    proto.width = 25;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_FIRE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_SMOKE_CLOUDS;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;//2
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 50;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_ice_doodle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "ice doodle");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "icecube.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_ATTACK_1_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 341;
    proto.frame_width = 25;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 50;
    proto.width = 25;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_SNOW;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_SMOKE_BLUE;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;//2
    proto.damage_type = DAMAGE_SNOW;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 60;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bad_breath(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "bad breath");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 363;
    proto.frame_width = 27;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 30;
    proto.width = 27;
    proto.max_jump = 0;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_SMOKE_CLOUDS;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_SMELL;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 10;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_sonic_doodle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "sonic doodle");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_NONSTOP;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 387;
    proto.frame_width = 27;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 50;
    proto.width = 27;
    proto.max_jump = 0;
    proto.max_speed = 4;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.collides_with_tiles = false;
    proto.damage = 1;
    proto.damage_type = DAMAGE_NOISE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.fire_rate = 20;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_ammo1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "ammo type 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 44;
    proto.frame_width = 7;
    proto.frame_height = 7;
    proto.height = 7;
    proto.reload_time = 40;
    proto.width = 7;
    proto.max_jump = 0;
    proto.max_speed = 4;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_ELECTRIC;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.fire_rate = 40;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_ammo2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "ammo type 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_PROJECTILE;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 1;
    proto.image_x = 1;
    proto.image_y = 44;
    proto.frame_width = 7;
    proto.frame_height = 7;
    proto.height = 7;
    proto.reload_time = 140;
    proto.width = 7;
    proto.max_jump = 0;
    proto.max_speed = 4;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_PROJECTILE;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;
    proto.fire_rate = 40;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_ice_cube(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "ice cube");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_BOMB;
    proto.AI[2] = AI_FRICTION_AFFECTS;
    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = true;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 29;
    proto.image_y = 341;
    proto.frame_width = 19;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 300;
    proto.width = 19;
    proto.max_jump = 5;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_SNOW;
    proto.destruction = DESTRUCTION_SMOKE_BLUE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_SNOW;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);

    proto.Save(file);
}

void create_prototype_stone_cube(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hens.bmp");
    strcpy(proto.name, "stone cube");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = true;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 29;
    proto.image_y = 341;
    proto.frame_width = 19;
    proto.frame_height = 19;
    proto.height = 19;
    proto.reload_time = 300;
    proto.width = 19;
    proto.max_jump = 5;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_GRAY;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GRAY;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);

    proto.Save(file);
}

void create_prototype_stone_cube_item(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "stone cube item");
    strcpy(proto.transformation_sprite, "stncube3.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FRICTION_AFFECTS;
    proto.AI[2] = AI_CHANGE_TIMER;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 110;
    proto.image_y = 62;
    proto.frame_width = 25;
    proto.frame_height = 17;
    proto.height = 17;
    proto.reload_time = 150;
    proto.width = 25;
    proto.max_jump = 5;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);

    proto.Save(file);
}

void create_prototype_stone_cube_bonus(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "stone cube bonus");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "stncube2.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 110;
    proto.image_y = 62;
    proto.frame_width = 25;
    proto.frame_height = 17;
    proto.height = 17;
    proto.reload_time = 150;
    proto.width = 25;
    proto.max_jump = 5;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_GRAY;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_tennis_ball(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mous.bmp");
    strcpy(proto.name, "tennis ball");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_THROWING_WEAPON;
    proto.AI[2] = AI_BOMB;
    proto.AI[3] = AI_JUMPING;
    proto.AI[4] = AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 71;
    proto.frame_width = 15;
    proto.frame_height = 15;
    proto.height = 15;
    proto.reload_time = 300;
    proto.width = 15;
    proto.max_jump = 25;
    proto.max_speed = 4;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);

    proto.Save(file);
}
/*
void create_prototype_bunny(char *file)
{
	PK2Sprite_Prototype proto;

	strcpy(proto.version,LATEST_VERSION_PK2SPRITE);
	strcpy(proto.image_file,"pk2spr01.bmp");
	strcpy(proto.name, "red jumper");
	strcpy(proto.transformation_sprite,"");
	strcpy(proto.bonus_sprite,"apple.spr");
	strcpy(proto.projectile1_sprite,"");
	strcpy(proto.projectile2_sprite,"");

	strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
	strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

	proto.AI[0]			= AI_JUMPING;
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_NONSTOP;
	proto.AI[3]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_NONE;

	proto.animation_count	= 3;
	proto.key			= false;
	proto.health		= 3;
	proto.obstacle			= false;
	proto.frame_count		= 12;
	proto.frame_rate	= 6;
	proto.image_x		= 1;
	proto.image_y		= 120;
	proto.frame_width  = 40;
	proto.frame_height = 44;
	proto.height		= 44;
	proto.reload_time	= 15;
	proto.width		= 40;
	proto.max_jump		= 20;//20
	proto.max_speed	= 2;
	proto.weight			= 1;
	proto.points		= 20;
	proto.protection		= DAMAGE_NONE;
	proto.shakes       = false;
	proto.destruction	= DESTRUCTION_SMOKE_RED;//DESTRUCTION_ANIMATION;
	proto.type		= TYPE_GAME_CHARACTER;
	proto.damage		= 1;
	proto.damage_type= DAMAGE_HIT;
	proto.color			= COLOR_NORMAL;
	proto.enemy	= true;

	unsigned char seq1[2] = {1,0};
	Animation_New(proto, ANIMATION_IDLE,	seq1, false);

	unsigned char seq2[2] = {1,0};
	Animation_New(proto, ANIMATION_WALK,		seq2, true);

	unsigned char seq3[2] = {2,0};
	Animation_New(proto, ANIMATION_JUMP_UP,	seq3, true);

	unsigned char seq4[2] = {3,0};
	Animation_New(proto, ANIMATION_JUMP_DOWN,	seq4, true);

	unsigned char seq5[2] = {1,0};
	Animation_New(proto, ANIMATION_CROUCH,		seq5, false);

	unsigned char seq6[3] = {4,5,0};
	Animation_New(proto, ANIMATION_DAMAGE,	seq6, true);

	unsigned char seq7[10] = {6,6,7,7,8,9,10,11,12,0};
	Animation_New(proto, ANIMATION_DEATH,	seq7, false);

	unsigned char seq8[5] = {2,0};
	Animation_New(proto, ANIMATION_ATTACK1,	seq8, true);

	proto.Tallenna(file);
}

void create_prototype_bunny2(char *file)
{
	PK2Sprite_Prototype proto;

	strcpy(proto.version,LATEST_VERSION_PK2SPRITE);
	strcpy(proto.image_file,"pk2spr01.bmp");
	strcpy(proto.name, "green jumper");
	strcpy(proto.transformation_sprite,"");
	strcpy(proto.bonus_sprite,"grape.spr");
	strcpy(proto.projectile1_sprite,"");
	strcpy(proto.projectile2_sprite,"");

	strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
	strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

	proto.AI[0]			= AI_JUMPING;
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_FOLLOW_PLAYER;
	for (int ai=3;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_NONE;

	proto.animation_count	= 3;
	proto.key			= false;
	proto.health		= 3;
	proto.obstacle			= false;
	proto.frame_count		= 12;
	proto.frame_rate	= 6;
	proto.image_x		= 1;
	proto.image_y		= 120;
	proto.frame_width  = 40;
	proto.frame_height = 44;
	proto.height		= 44;
	proto.reload_time	= 15;
	proto.width		= 40;
	proto.max_jump		= 20;//20
	proto.max_speed	= 0;
	proto.weight			= 1;
	proto.points		= 10;
	proto.protection		= DAMAGE_NONE;
	proto.shakes       = false;
	proto.destruction	= DESTRUCTION_SMOKE_GREEN;
	proto.type		= TYPE_GAME_CHARACTER;
	proto.damage		= 2;
	proto.damage_type= DAMAGE_HIT;
	proto.color			= COLOR_GREEN;
	proto.enemy	= true;

	unsigned char seq1[2] = {1,0};
	Animation_New(proto, ANIMATION_IDLE,	seq1, false);

	unsigned char seq2[2] = {1,0};
	Animation_New(proto, ANIMATION_WALK,		seq2, true);

	unsigned char seq3[2] = {2,0};
	Animation_New(proto, ANIMATION_JUMP_UP,	seq3, true);

	unsigned char seq4[2] = {3,0};
	Animation_New(proto, ANIMATION_JUMP_DOWN,	seq4, true);

	unsigned char seq5[2] = {1,0};
	Animation_New(proto, ANIMATION_CROUCH,		seq5, false);

	unsigned char seq6[3] = {4,5,0};
	Animation_New(proto, ANIMATION_DAMAGE,	seq6, true);

	unsigned char seq7[10] = {6,6,7,7,8,9,10,11,12,0};
	Animation_New(proto, ANIMATION_DEATH,	seq7, false);

	unsigned char seq8[5] = {2,0};
	Animation_New(proto, ANIMATION_ATTACK1,	seq8, true);

	proto.Save(file);
}*/
/*
void create_prototype_boxing_bunny(char *file)
{
	PK2Sprite_Prototype proto;

	strcpy(proto.version,LATEST_VERSION_PK2SPRITE);
	strcpy(proto.image_file,"pk2spr01.bmp");
	strcpy(proto.name, "rabbit");
	strcpy(proto.transformation_sprite,"");
	strcpy(proto.bonus_sprite,"carrot.spr");
	strcpy(proto.projectile1_sprite,"");
	strcpy(proto.projectile2_sprite,"");


	strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
	strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

	proto.AI[0]			= AI_BASIC;
	proto.AI[1]			= AI_RANDOM_START_DIRECTION_HORIZONTAL;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;

	for (int ai=4;ai<SPRITE_MAX_AI;ai++)
		proto.AI[ai]	= AI_NONE;

	proto.animation_count	= 6;
	proto.key			= false;
	proto.health		= 2;
	proto.obstacle			= false;
	proto.frame_count		= 19;
	proto.frame_rate	= 5;
	proto.image_x		= 1;
	proto.image_y		= 167;
	proto.frame_width  = 38;
	proto.frame_height = 50;
	proto.height		= 50;
	proto.reload_time	= 15;
	proto.width		= 31;
	proto.max_jump		= 35;
	proto.weight			= 1;
	proto.points		= 20;
	proto.protection		= SUOJA_EI_MITAAN;
	proto.destruction	= DESTRUCTION_ANIMATION;
	proto.type		= TYPE_GAME_CHARACTER;
	proto.damage		= 1;
	proto.color			= COLOR_NORMAL;
	proto.enemy	= true;

	unsigned char seq1[10] = {1,1,2,2,1,1,2,2,1,0};
	Animation_New(proto, ANIMATION_IDLE,	seq1, false);

	unsigned char seq2[7] = {7,8,9,10,11,12,0};
	Animation_New(proto, ANIMATION_WALK,		seq2, true);

	unsigned char seq3[2] = {3,0};
	Animation_New(proto, ANIMATION_JUMP_UP,	seq3, true);

	unsigned char seq4[2] = {4,0};
	Animation_New(proto, ANIMATION_JUMP_DOWN,	seq4, true);

	unsigned char seq5[4] = {1,5,6,0};
	Animation_New(proto, ANIMATION_CROUCH,		seq5, false);

	unsigned char seq6[5] = {13,13,14,14,0};
	Animation_New(proto, ANIMATION_DAMAGE,	seq6, true);

	unsigned char seq7[10] = {15,15,16,16,17,17,18,18,19,0};
	Animation_New(proto, ANIMATION_DEATH,	seq7, false);

	proto.Save(file);
}
*/
void create_prototype_wolf(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wolf.bmp");
    strcpy(proto.name, "wolf");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "wolf_ko.wav");

    proto.AI[0] = AI_BASIC;
    //proto.AI[0]			= AI_HIDES;
    proto.AI[1] = AI_RANDOM_TURNING;
    proto.AI[2] = AI_FLEES_PLAYER_IF_SEEN;
    proto.AI[3] = AI_AVOID_PIT;
    proto.AI[4] = AI_TURN_ON_COLLISION_HORIZONTAL;
    //proto.AI[4]			= AI_RETURN_TO_START_X;
    /*
	proto.AI[0]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[1]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[2]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[3]			= AI_AVOID_PIT;
	proto.AI[4]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 5;
    proto.obstacle = false;
    proto.frame_count = 16;
    proto.frame_rate = 8;
    proto.image_x = 2;
    proto.image_y = 2;
    proto.frame_width = 47;
    proto.frame_height = 52;
    proto.height = 52;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 25;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.points = 60;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 2;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[8] = {5, 5, 5, 5, 5, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {8, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {9, 9, 10, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[10] = {13, 13, 13, 14, 14, 14, 14, 15, 16, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {11, 11, 12, 12, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);


    proto.Save(file);
}

void create_prototype_evil_wolf(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wolf.bmp");
    strcpy(proto.name, "evil wolf");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "wolf_ko.wav");

    proto.AI[0] = AI_SMALL_CHICKEN;
    proto.AI[1] = AI_FOLLOW_PLAYER;
    proto.AI[2] = AI_HIDES;
    /*
	proto.AI[0]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[1]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[2]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[3]			= AI_AVOID_PIT;
	proto.AI[4]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 5;
    proto.obstacle = false;
    proto.frame_count = 16;
    proto.frame_rate = 8;
    proto.image_x = 2;
    proto.image_y = 2;
    proto.frame_width = 47;
    proto.frame_height = 52;
    proto.height = 52;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 30;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.points = 60;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 2;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[8] = {5, 5, 5, 5, 5, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {8, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {9, 9, 10, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[10] = {13, 13, 13, 14, 14, 14, 14, 15, 16, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {11, 11, 12, 12, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);


    proto.Save(file);
}

void create_prototype_smart_wolf(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wolf.bmp");
    strcpy(proto.name, "smart wolf");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "wolf_ko.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_HIDES;
    proto.AI[2] = AI_SMALL_CHICKEN;
    proto.AI[3] = AI_FOLLOW_PLAYER;
    proto.AI[4] = AI_JUMP_IF_PLAYER_ABOVE;
    /*
	proto.AI[0]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[1]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[2]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[3]			= AI_AVOID_PIT;
	proto.AI[4]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 5;
    proto.obstacle = false;
    proto.frame_count = 16;
    proto.frame_rate = 8;
    proto.image_x = 2;
    proto.image_y = 2;
    proto.frame_width = 47;
    proto.frame_height = 52;
    proto.height = 52;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 25;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.points = 60;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 2;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[8] = {5, 5, 5, 5, 5, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {8, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {9, 9, 10, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[10] = {13, 13, 13, 14, 14, 14, 14, 15, 16, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {11, 11, 12, 12, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);


    proto.Save(file);
}

void create_prototype_frog1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_frog.bmp");
    strcpy(proto.name, "frog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "frog_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "frog_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "frog_att.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "frog_att.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FROG1;
    proto.AI[3] = AI_FOLLOW_PLAYER;
    proto.AI[4] = AI_FRICTION_AFFECTS;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 30;
    proto.frame_height = 31;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 0;
    proto.weight = 1;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 1, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 4, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[6] = {4, 4, 4, 4, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {8, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_frog2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_frog.bmp");
    strcpy(proto.name, "frog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "frog_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "frog_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "frog_att.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "frog_att.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FROG2;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_FRICTION_AFFECTS;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 30;
    proto.frame_height = 31;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 4;
    proto.weight = 1;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 1, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {5, 5, 5, 1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, false);

    unsigned char seq3[6] = {4, 4, 4, 4, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {8, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_frog3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_frog.bmp");
    strcpy(proto.name, "frog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "frog_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "frog_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "frog_att.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "frog_att.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FROG1;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_NONSTOP;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 30;
    proto.frame_height = 31;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_BLUE;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 1, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 4, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[6] = {4, 4, 4, 4, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {8, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_evil_frog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_frog.bmp");
    strcpy(proto.name, "evil frog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "frog_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "frog_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "frog_att.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "frog_att.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOW_PLAYER;
    proto.AI[2] = AI_FROG2;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_FRICTION_AFFECTS;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 30;
    proto.frame_height = 31;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_PURPLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 1, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 4, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[6] = {4, 4, 4, 4, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {8, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_player_frog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_frog.bmp");
    strcpy(proto.name, "player frog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "frog_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "frog_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "frog_att.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "frog_att.wav");

    proto.AI[0] = AI_BASIC;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 8;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 30;
    proto.frame_height = 31;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 30;
    proto.max_jump = 26;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_float_down = false;

    unsigned char seq1[5] = {1, 1, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 4, 3, 4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[6] = {4, 4, 4, 4, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {8, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bunny(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bun.bmp");
    strcpy(proto.name, "jumping bunny");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "carrot.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

//	strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
//	strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "bunny_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bunny_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "bunny_at.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "bunny_at.wav");

    proto.AI[0] = AI_JUMPING;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    for (int ai = 4; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 19;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 47;
    proto.height = 47;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 5;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 2, 2, 1, 1, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[7] = {9, 10, 11, 12, 13, 14, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {19, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[5] = {15, 15, 16, 16, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {5, 5, 6, 6, 7, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {17, 17, 18, 18, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bunny2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bun.bmp");
    strcpy(proto.name, "juping bunny 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "carrot.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    strcpy(proto.sound_files[SOUND_DAMAGE], "bunny_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bunny_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "bunny_at.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "bunny_at.wav");


    proto.AI[0] = AI_JUMPING;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FOLLOW_PLAYER;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 19;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 47;
    proto.height = 47;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 24;//20
    proto.max_speed = 0;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GREEN;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 2, 2, 1, 1, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[7] = {9, 10, 11, 12, 13, 14, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {19, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[5] = {15, 15, 16, 16, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {5, 5, 6, 6, 7, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {17, 17, 18, 18, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_boxing_bunny(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bun.bmp");
    strcpy(proto.name, "boxing bunny");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "carrot.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "bunny_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bunny_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "bunny_at.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "bunny_at.wav");

    proto.AI[0] = AI_BASIC;
    //proto.AI[1]			= AI_HIDES;
    proto.AI[1] = AI_RANDOM_TURNING;
    proto.AI[2] = AI_FLEES_PLAYER_IF_SEEN;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_TURN_IF_HIT;
    //proto.AI[4]			= AI_RETURN_TO_START_X;
    /*
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[4]			= AI_AVOID_PIT;
	proto.AI[5]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 19;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 47;
    proto.height = 47;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 35;
    proto.max_speed = 3;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 2, 2, 1, 1, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[7] = {9, 10, 11, 12, 13, 14, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {19, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[5] = {15, 15, 16, 16, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {5, 5, 6, 6, 7, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {17, 17, 18, 18, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_jumping_bunny(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bun.bmp");
    strcpy(proto.name, "jumping bunny");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "carrot.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "bunny_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bunny_k2.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "bunny_at.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "bunny_at.wav");


    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    //proto.AI[3]			= AI_RANDOM_DIRECTION_CHANGE_HORIZONTAL;
    proto.AI[3] = AI_JUMP_IF_PLAYER_ABOVE;
    proto.AI[4] = AI_RANDOM_JUMP;
    proto.AI[5] = AI_TURN_IF_HIT;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 19;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 47;
    proto.height = 47;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 35;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 30;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 2, 2, 1, 1, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[7] = {9, 10, 11, 12, 13, 14, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {19, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[5] = {15, 15, 16, 16, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {5, 5, 6, 6, 7, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {17, 17, 18, 18, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_turtle_walking(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_turt.bmp");
    strcpy(proto.name, "turtle (walking)");
    strcpy(proto.transformation_sprite, "turtle2.spr");
    strcpy(proto.bonus_sprite, "jewel3.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_NONSTOP;//AI_CHANGE_IF_ENERGY_LESS_THAN_2;

    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 3;
    proto.frame_count = 13;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 44;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 1000;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 80;
    proto.protection = DAMAGE_FALL;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {6, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {11, 12, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[7] = {12, 12, 12, 11, 1, 13, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_turtle_shell(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_turt.bmp");
    strcpy(proto.name, "turtle (shell)");
    strcpy(proto.transformation_sprite, "turtle3.spr");
    strcpy(proto.bonus_sprite, "jewel4.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_FRICTION_AFFECTS;

    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = true;
    proto.blocks_left = true;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.frame_count = 8;
    proto.frame_rate = 6;
    proto.image_x = 236;
    proto.image_y = 1;
    proto.frame_width = 44;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 800;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 80;
    proto.protection = DAMAGE_FALL;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {5, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, false);

    unsigned char seq3[2] = {5, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {5, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, false);

    unsigned char seq5[2] = {5, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[6] = {6, 7, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_turtle_waking(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_turt.bmp");
    strcpy(proto.name, "turtle (waking)");
    strcpy(proto.transformation_sprite, "turtle1.spr");
    strcpy(proto.bonus_sprite, "jewel4.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_FRICTION_AFFECTS;

    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.frame_count = 8;
    proto.frame_rate = 6;
    proto.image_x = 236;
    proto.image_y = 1;
    proto.frame_width = 44;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 200;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 80;
    proto.protection = DAMAGE_FALL;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[10] = {5, 5, 4, 4, 3, 3, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, false);

    unsigned char seq3[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, false);

    unsigned char seq4[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, false);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[6] = {6, 7, 6, 7, 8, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_red_turtle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_turt.bmp");
    strcpy(proto.name, "red turtle");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "jewel2.spr");
    strcpy(proto.projectile1_sprite, "");
    //strcpy(proto.projectile2_sprite,"doodlef2.spr");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    //proto.AI[1]			= AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    //proto.AI[3]			= AI_AVOID_PIT;
    proto.AI[3] = AI_NONSTOP;//AI_CHANGE_IF_ENERGY_LESS_THAN_2;

    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 5;
    proto.frame_count = 13;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 44;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 300;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 40;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_RED;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {6, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {11, 12, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[7] = {12, 12, 12, 11, 1, 13, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_stupid_hedgehog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hedg.bmp");
    strcpy(proto.name, "stupid hedgehog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    /*
	for (int sound=0;sound<MAX_SOUNDS;sound++)
	{
		strcpy(proto.sound_files[sound], "");
		proto.sounds[sound] = -1;
	}*/

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;

    for (int ai = 4; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 30;
    proto.reload_time = 12;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[3] = {3, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_evil_hedgehog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hedg.bmp");
    strcpy(proto.name, "evil hedgehog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
/*
	for (int sound=0;sound<MAX_SOUNDS;sound++)
	{
		strcpy(proto.sound_files[sound], "");
		proto.sounds[sound] = -1;
	}
*/
    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_RANDOM_START_DIRECTION_HORIZONTAL;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;
    proto.AI[4] = AI_FOLLOW_PLAYER;
    proto.AI[5] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 30;
    proto.reload_time = 12;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 4;
    proto.weight = 1;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[3] = {3, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_deaf_hedgehog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hedg.bmp");
    strcpy(proto.name, "deaf hedgehog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");
    /*
	for (int sound=0;sound<MAX_SOUNDS;sound++)
	{
		strcpy(proto.sound_files[sound], "");
		proto.sounds[sound] = -1;
	}*/

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;

    for (int ai = 4; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 30;
    proto.reload_time = 12;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NOISE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[3] = {3, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_stoned_hedgehog(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_hedg.bmp");
    strcpy(proto.name, "stoned hedgehog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;

    for (int ai = 4; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 30;
    proto.reload_time = 12;
    proto.width = 30;
    proto.max_jump = 15;
    proto.max_speed = 0;
    proto.weight = 1;
    proto.points = 20;
    proto.protection = DAMAGE_NOISE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GRAY;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[3] = {3, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bat(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "bat");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    for (int ai = 4; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 41;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 15;
    proto.width = 41;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_GRAY;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {4, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bomber_bat(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "bomber bat");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "bomb.spr");
    strcpy(proto.projectile1_sprite, "bomb.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_FLEES_PLAYER_IF_SEEN;
    proto.AI[4] = AI_ATTACK_1_IF_PLAYER_BELOW;
    proto.AI[5] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 34;
    proto.frame_width = 41;
    proto.frame_height = 44;
    proto.height = 44;
    proto.reload_time = 40;
    proto.width = 41;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 30;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_SMOKE_PURPLE;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {4, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_evil_bomber_bat(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "evil bomber bat");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "bomb.spr");
    strcpy(proto.projectile1_sprite, "bomb.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_FOLLOW_PLAYER;
    proto.AI[4] = AI_ATTACK_1_IF_PLAYER_BELOW;
    proto.AI[5] = AI_NONE;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 34;
    proto.frame_width = 41;
    proto.frame_height = 44;
    proto.height = 44;
    proto.reload_time = 40;
    proto.width = 41;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 30;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_SMOKE_PURPLE;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {4, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bird(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bird.bmp");
    strcpy(proto.name, "bad birdy");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "chick.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_ATTACK_1_IF_PLAYER_BELOW;

    proto.animation_count = 4;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 9;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 29;
    proto.frame_height = 26;
    proto.height = 26;
    proto.reload_time = 50;
    proto.width = 29;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    Animation_New(proto, ANIMATION_WALK, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);

    Animation_New(proto, ANIMATION_CROUCH, seq1, true);

    unsigned char seq6[3] = {7, 8, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {7, 8, 9, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {4, 5, 6, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bird2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bird.bmp");
    strcpy(proto.name, "evil birdy");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "chick.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    //proto.AI[2]			= AI_NONSTOP;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_ATTACK_1_IF_PLAYER_BELOW;
    proto.AI[2] = AI_FOLLOW_PLAYER;

    proto.animation_count = 4;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 9;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 29;
    proto.frame_height = 26;
    proto.height = 26;
    proto.reload_time = 50;
    proto.width = 29;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 30;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    Animation_New(proto, ANIMATION_WALK, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);

    Animation_New(proto, ANIMATION_CROUCH, seq1, true);

    unsigned char seq6[3] = {7, 8, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {7, 8, 9, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {4, 5, 6, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bird3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bird.bmp");
    strcpy(proto.name, "still birdy");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "grape.spr");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "chick.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_1_IF_PLAYER_BELOW;
    proto.AI[2] = AI_FOLLOW_PLAYER;

    proto.animation_count = 4;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 9;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 29;
    proto.frame_height = 26;
    proto.height = 26;
    proto.reload_time = 50;
    proto.width = 29;
    proto.max_jump = 20;//20
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GREEN;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    Animation_New(proto, ANIMATION_WALK, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);

    Animation_New(proto, ANIMATION_CROUCH, seq1, true);

    unsigned char seq6[3] = {7, 8, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {7, 8, 9, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {4, 5, 6, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bird4(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bird.bmp");
    strcpy(proto.name, "friendly birdy");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "chick.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "chick.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_1_IF_PLAYER_BELOW;
    proto.AI[2] = AI_FOLLOW_PLAYER;

    proto.animation_count = 4;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 9;
    proto.frame_rate = 6;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 29;
    proto.frame_height = 26;
    proto.height = 26;
    proto.reload_time = 50;
    proto.width = 29;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_FEATHERS;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_TURQUOISE;
    proto.enemy = false;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    Animation_New(proto, ANIMATION_WALK, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);

    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);

    Animation_New(proto, ANIMATION_CROUCH, seq1, true);

    unsigned char seq6[3] = {7, 8, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {7, 8, 9, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[5] = {4, 5, 6, 5, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_fish1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "fish");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 36;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 15;
    proto.width = 36;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 5;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {2, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {4, 4, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[3] = {5, 6, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_fish2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "evil fish");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOW_PLAYER;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 36;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 15;
    proto.width = 36;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 9;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {2, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {4, 4, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[3] = {5, 6, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_fish3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "guard fish");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FLEES_PLAYER_IF_SEEN;
    proto.AI[2] = AI_RETURN_TO_START_X;
    proto.AI[3] = AI_RANDOM_TURNING;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 36;
    proto.frame_height = 28;
    proto.height = 28;
    proto.reload_time = 15;
    proto.width = 36;
    proto.max_jump = 20;//20
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 7;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;//DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {2, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[3] = {4, 4, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[3] = {5, 6, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_bee(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "bee");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bee_ko.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "bee.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI;
    proto.AI[2] = AI_RETURN_TO_START_X;
    proto.AI[3] = AI_RETURN_TO_START_Y;
    proto.AI[4] = AI_RANDOM_MOVEMENT_VERT_HORI;
    proto.AI[5] = AI_DAMAGE_FROM_WATER;
    /*
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[4]			= AI_AVOID_PIT;
	proto.AI[5]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 10;
    proto.frame_height = 10;
    proto.height = 10;
    proto.reload_time = 15;
    proto.width = 10;
    proto.max_jump = 10;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 1;
    proto.protection = DAMAGE_STING;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_STING;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);

    unsigned char seq2[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_bee2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "angry bee");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bee_ko.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "bee.wav");


    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_VERT_HORI;
    proto.AI[2] = AI_DAMAGE_FROM_WATER;
    proto.AI[3] = AI_KILL_ALL;
    /*
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[4]			= AI_AVOID_PIT;
	proto.AI[5]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 10;
    proto.frame_height = 10;
    proto.height = 10;
    proto.reload_time = 15;
    proto.width = 10;
    proto.max_jump = 10;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 1;
    proto.protection = DAMAGE_STING;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_STING;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);

    unsigned char seq2[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_bee3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "friendly bee");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bee_ko.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "bee.wav");


    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_VERT_HORI;
    //proto.AI[2]			= AI_DAMAGE_FROM_WATER;
    /*
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[4]			= AI_AVOID_PIT;
	proto.AI[5]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1000;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 10;
    proto.frame_height = 10;
    proto.height = 10;
    proto.reload_time = 15;
    proto.width = 10;
    proto.max_jump = 10;
    proto.max_speed = 3;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_STING;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_STING;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);

    unsigned char seq2[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_beehive(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "beehive");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "bee2.spr");
    strcpy(proto.projectile1_sprite, "bee2.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "beehive.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "beehive.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_ATTACK_1_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 5;
    proto.health = 4;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 45;
    proto.frame_width = 27;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 30;
    proto.width = 27;
    proto.max_jump = 17;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 100;
    proto.protection = DAMAGE_STING;
    proto.collides_with_tiles = false;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_beehive2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "beehive 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "bee2.spr");
    strcpy(proto.projectile1_sprite, "bee2.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "beehive.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "beehive.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 5;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 45;
    proto.frame_width = 27;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 30;
    proto.width = 27;
    proto.max_jump = 17;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_STING;
    proto.collides_with_tiles = false;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

/*
void create_prototype_giant_bee(char *file)
{
	PK2Sprite_Prototype proto;

	strcpy(proto.version,LATEST_VERSION_PK2SPRITE);
	strcpy(proto.image_file,"spr_fisb.bmp");
	strcpy(proto.name, "giant bee");
	strcpy(proto.transformation_sprite,"");
	strcpy(proto.bonus_sprite,"");
	strcpy(proto.projectile1_sprite,"");
	strcpy(proto.projectile2_sprite,"");

	strcpy(proto.sound_files[SOUND_DAMAGE],	  "damage.wav");
	strcpy(proto.sound_files[SOUND_DESTRUCTION],"bee_ko.wav");
	strcpy(proto.sound_files[SOUND_RANDOM],	  "bee.wav");

	proto.AI[0]			= AI_BASIC;
	proto.AI[1]			= AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI;
	proto.AI[2]			= AI_RETURN_TO_START_X;
	proto.AI[3]			= AI_RETURN_TO_START_Y;
	proto.AI[4]			= AI_RANDOM_MOVEMENT_VERT_HORI;
	proto.AI[5]			= AI_DAMAGE_FROM_WATER;

	proto.sound_frequency		= 15000;
	proto.animation_count	= 8;
	proto.key			= false;
	proto.health		= 3;
	proto.obstacle			= false;
	proto.frame_count		= 4;
	proto.frame_rate	= 2;
	proto.image_x		= 53;
	proto.image_y		= 32;
	proto.frame_width  = 42;
	proto.frame_height = 27;
	proto.height		= 27;
	proto.reload_time	= 15;
	proto.width		= 42;
	proto.max_jump		= 10;
	proto.max_speed	= 1;
	proto.weight			= 0;
	proto.points		= 40;
	proto.protection		= DAMAGE_STING;
	proto.destruction	= DESTRUCTION_STARS_ORANGE;
	proto.type		= TYPE_GAME_CHARACTER;
	proto.damage		= 1;
	proto.damage_type= DAMAGE_STING;
	proto.color			= COLOR_NORMAL;
	proto.enemy	= true;

	unsigned char seq1[3] = {1,2,0};
	Animation_New(proto, ANIMATION_IDLE,	seq1, true);
	Animation_New(proto, ANIMATION_WALK,		seq1, true);
	Animation_New(proto, ANIMATION_JUMP_UP,	seq1, true);
	Animation_New(proto, ANIMATION_JUMP_DOWN,	seq1, true);
	Animation_New(proto, ANIMATION_CROUCH,		seq1, true);
	Animation_New(proto, ANIMATION_DAMAGE,	seq1, true);
	Animation_New(proto, ANIMATION_ATTACK1,	seq1, true);
	Animation_New(proto, ANIMATION_ATTACK2,	seq1, true);

	proto.Save(file);
}
*/
void create_prototype_giant_bee(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_fisb.bmp");
    strcpy(proto.name, "giant bee");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bee_ko.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "bee.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI;
    proto.AI[2] = AI_RETURN_TO_START_X;
    proto.AI[3] = AI_RETURN_TO_START_Y;
    proto.AI[4] = AI_RANDOM_MOVEMENT_VERT_HORI;
    proto.AI[5] = AI_DAMAGE_FROM_WATER;
    /*
	proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;
	proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[3]			= AI_FLEES_PLAYER_IF_SEEN;
	proto.AI[4]			= AI_AVOID_PIT;
	proto.AI[5]			= AI_RETURN_TO_START_X;//AI_NONSTOP;
	*/
    proto.sound_frequency = 15000;
    proto.animation_count = 8;
    proto.key = false;
    proto.health = 10;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 2;
    proto.image_x = 274;
    proto.image_y = 1;
    proto.frame_width = 87;
    proto.frame_height = 70;
    proto.height = 80;
    proto.reload_time = 15;
    proto.width = 60;
    proto.max_jump = 10;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 100;
    proto.protection = DAMAGE_STING;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 2;
    proto.damage_type = DAMAGE_STING;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    unsigned char seq2[5] = {1, 1, 2, 2, 0};
    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq3, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_giant_mouse(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mous.bmp");
    strcpy(proto.name, "giant mouse");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "tennisb.spr");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "tennis.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "tennis.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOW_PLAYER;
    proto.AI[2] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[3] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
    proto.AI[4] = AI_RANDOM_JUMP;

    proto.sound_frequency = 11050;
    proto.animation_count = 5;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 20;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 7;
    proto.attack1_duration = 9;
    proto.attack2_duration = 9;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 71;
    proto.frame_height = 67;
    proto.height = 67;//58
    proto.reload_time = 80;//40
    proto.width = 71;
    proto.max_jump = 30;
    proto.max_speed = 0;
    proto.weight = 3;
    proto.random_freq = true;
    proto.points = 100;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[8] = {1, 1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[5] = {6, 6, 7, 7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[9] = {3, 3, 4, 4, 4, 4, 4, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_giant_mouse2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mous.bmp");
    strcpy(proto.name, "giant mouse 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "tennisb.spr");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FROG2;
    proto.AI[2] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[3] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
    proto.AI[4] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[5] = AI_TURN_IF_HIT;
    proto.AI[6] = AI_FRICTION_AFFECTS;

    proto.sound_frequency = 11050;
    proto.animation_count = 5;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 20;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 7;
    proto.attack1_duration = 9;
    proto.attack2_duration = 9;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 71;
    proto.frame_height = 67;
    proto.height = 67;//58
    proto.reload_time = 120;//40
    proto.width = 71;
    proto.max_jump = 43;
    proto.max_speed = 2;
    proto.weight = 1;
    proto.random_freq = true;
    proto.points = 100;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[8] = {1, 1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[5] = {6, 6, 7, 7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[9] = {3, 3, 4, 4, 4, 4, 4, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_giant_mouse3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mous.bmp");
    strcpy(proto.name, "giant ice mouse");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "tennisb.spr");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOW_PLAYER;
    proto.AI[2] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[3] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
    proto.AI[4] = AI_RANDOM_JUMP;

    proto.sound_frequency = 11050;
    proto.animation_count = 5;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 20;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 7;
    proto.attack1_duration = 9;
    proto.attack2_duration = 9;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 71;
    proto.frame_height = 67;
    proto.height = 67;//58
    proto.reload_time = 80;//40
    proto.width = 71;
    proto.max_jump = 30;
    proto.max_speed = 0;
    proto.weight = 3;
    proto.random_freq = true;
    proto.points = 100;
    proto.protection = DAMAGE_SNOW;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_SNOW;
    proto.color = COLOR_BLUE;
    proto.enemy = true;

    unsigned char seq1[8] = {1, 1, 1, 1, 2, 2, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[5] = {6, 6, 7, 7, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[9] = {3, 3, 4, 4, 4, 4, 4, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_jattirobokana_muoto1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mach.bmp");
    strcpy(proto.name, "walking metal hen");
    strcpy(proto.transformation_sprite, "roboss2.spr");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "robohead.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "boss2_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boss2_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
    proto.AI[3] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[4] = AI_NONSTOP;
    proto.AI[5] = AI_FROG1;
    //proto.AI[4]			= AI_ATTACK_1_NONSTOP;

    /*
	proto.AI[1]			= AI_FROG2;
	proto.AI[2]			= AI_ATTACK_2_IF_PLAYER_IN_FRONT;
	proto.AI[3]			= AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
	proto.AI[4]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[5]			= AI_TURN_IF_HIT;
	proto.AI[6]			= AI_FRICTION_AFFECTS;*/

    proto.sound_frequency = 11050;
    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 40;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.attack1_duration = 100;
    proto.attack2_duration = 100;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 78;
    proto.frame_height = 78;
    proto.height = 78;
    proto.reload_time = 400;
    proto.width = 78;
    proto.max_jump = 43;
    proto.max_speed = 4;
    proto.weight = 2;
    proto.random_freq = true;
    proto.points = 500;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {2, 4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_jattirobokana_muoto2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mach.bmp");
    strcpy(proto.name, "flying metal hen");
    strcpy(proto.transformation_sprite, "roboss3.spr");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "robohead.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "boss2_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boss2_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
    proto.AI[3] = AI_FOLLOW_PLAYER;

    //proto.AI[3]			= AI_ATTACK_1_IF_PLAYER_IN_FRONT;
    /*
	proto.AI[4]			= AI_CHANGE_TIMER;
	/*
	proto.AI[1]			= AI_FROG2;
	proto.AI[2]			= AI_ATTACK_2_IF_PLAYER_IN_FRONT;
	proto.AI[3]			= AI_CHANGE_SKULLS_IF_KNOCKED_OUT;
	proto.AI[4]			= AI_TURN_ON_COLLISION_HORIZONTAL;
	proto.AI[5]			= AI_TURN_IF_HIT;
	proto.AI[6]			= AI_FRICTION_AFFECTS;*/

    proto.sound_frequency = 11050;
    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 40;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.attack1_duration = 20;
    proto.attack2_duration = 20;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 78;
    proto.frame_height = 78;
    proto.height = 78;
    proto.reload_time = 300;
    proto.width = 78;
    proto.max_jump = 20;
    proto.max_speed = 3;
    proto.weight = -0.1;
    proto.random_freq = true;
    proto.points = 500;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {2, 4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_jattirobokana_muoto3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mach.bmp");
    strcpy(proto.name, "static metal hen");
    strcpy(proto.transformation_sprite, "roboss1.spr");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "boss2_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boss2_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;
    proto.AI[2] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;

    proto.sound_frequency = 11050;
    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 40;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.attack1_duration = 20;
    proto.attack2_duration = 20;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 78;
    proto.frame_height = 78;
    proto.height = 78;
    proto.reload_time = 130;
    proto.width = 78;
    proto.max_jump = 20;
    proto.max_speed = 0;
    proto.weight = 2;
    proto.random_freq = true;
    proto.points = 500;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_jattirobokana_muoto4(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mach.bmp");
    strcpy(proto.name, "metal hen 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "robohen.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "boss2_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boss2_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_1_NONSTOP;

    proto.sound_frequency = 11050;
    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.attack1_duration = 600;
    proto.attack2_duration = 20;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 78;
    proto.frame_height = 78;
    proto.height = 78;
    proto.reload_time = 600;
    proto.width = 78;
    proto.max_jump = 20;
    proto.max_speed = 0;
    proto.weight = 2;
    proto.random_freq = true;
    proto.points = 500;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GREEN;
    proto.enemy = true;

    unsigned char seq1[2] = {2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_jattirobokana_muoto5(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_mach.bmp");
    strcpy(proto.name, "metal hen 3");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "boss2_dm.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boss2_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;

    proto.sound_frequency = 11050;
    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 5;
    proto.attack1_duration = 20;
    proto.attack2_duration = 20;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 78;
    proto.frame_height = 78;
    proto.height = 78;
    proto.reload_time = 130;
    proto.width = 78;
    proto.max_jump = 20;
    proto.max_speed = 0;
    proto.weight = 2;
    proto.random_freq = true;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_liekki(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "flame");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_BOMB;
    proto.AI[2] = AI_DAMAGE_FROM_WATER;
    proto.AI[3] = AI_KILL_ALL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 112;
    proto.frame_width = 28;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 200;
    proto.width = 28;
    proto.max_jump = 5;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    proto.Save(file);
}

void create_prototype_liekki2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "flame 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_DAMAGE_FROM_WATER;
    proto.AI[2] = AI_KILL_ALL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 112;
    proto.frame_width = 28;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 200;
    proto.width = 28;
    proto.max_jump = 5;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    proto.Save(file);
}

void create_prototype_liekki3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "jumping flame");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FROG2;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_FRICTION_AFFECTS;
    proto.AI[4] = AI_DAMAGE_FROM_WATER;
    proto.AI[5] = AI_KILL_ALL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 112;
    proto.frame_width = 28;
    proto.frame_height = 30;
    proto.height = 30;
    proto.reload_time = 200;
    proto.width = 28;
    proto.max_jump = 5;
    proto.max_speed = 2;
    proto.weight = 0.5;
    proto.points = 0;
    proto.protection = DAMAGE_FIRE;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_FIRE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    proto.Save(file);
}

void create_prototype_possu(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_pigs.bmp");
    strcpy(proto.name, "chilli pig");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "apple.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodlef2.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "pig2.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "pig2.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "pig.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "pig_ko.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_AVOID_PIT;
//	proto.AI[5]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 30;
    proto.width = 31;
    proto.max_jump = 28;
    proto.max_speed = 2;
    proto.weight = 1.5;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_float_down = false;

    unsigned char seq1[9] = {1, 1, 1, 1, 2, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {8, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {9, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {15, 15, 16, 16, 17, 17, 18, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[10] = {10, 11, 12, 12, 12, 12, 11, 10, 1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_possu2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_pigs.bmp");
    strcpy(proto.name, "hot chilli pig");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "apple2.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodle_f.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "pig2.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "pig2.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "pig.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "pig_ko.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_AVOID_PIT;
//	proto.AI[5]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 5;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 34;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 30;
    proto.width = 31;
    proto.max_jump = 28;
    proto.max_speed = 2;
    proto.weight = 1.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;
    proto.can_float_down = false;

    unsigned char seq1[9] = {1, 1, 1, 1, 2, 2, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {8, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {9, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {13, 14, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[8] = {15, 15, 16, 16, 17, 17, 18, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[10] = {10, 11, 12, 12, 12, 12, 11, 10, 1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_lentava_possu(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_pigs.bmp");
    strcpy(proto.name, "flying pig");
    strcpy(proto.transformation_sprite, "pig.spr");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodlef2.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "pig2.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "pig2.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "pig.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_IF_ENERGY_LESS_THAN_2;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[4] = AI_NONSTOP;
//	proto.AI[5]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 87;
    proto.frame_width = 34;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 30;
    proto.width = 34;
    proto.max_jump = 15;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {5, 6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_tankki1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "tank squirel");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "cheese.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "ammo1.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "tankshot.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    //proto.AI[1]			= AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[1] = AI_ATTACK_2_NONSTOP;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_DAMAGE_FROM_WATER;
    proto.AI[6] = AI_START_DIRECTION_TOWARDS_PLAYER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 43;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 100;
    proto.width = 43;
    proto.max_jump = 28;
    proto.max_speed = 1;
    proto.weight = 1.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {4, 5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {6, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_tankki2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "tank squirel 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "cheese.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "ammo1.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "tankshot.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_2_NONSTOP;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_AVOID_PIT;
    proto.AI[5] = AI_DAMAGE_FROM_WATER;
    proto.AI[6] = AI_START_DIRECTION_TOWARDS_PLAYER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 43;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 100;
    proto.width = 43;
    proto.max_jump = 28;
    proto.max_speed = 1;
    proto.weight = 1.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {4, 5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {6, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_tankki3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "tank squirel 3");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "cheese.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "ammo2.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "tankshot.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_2_NONSTOP;
    proto.AI[2] = AI_FOLLOW_PLAYER;
    proto.AI[3] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 43;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 100;
    proto.width = 43;
    proto.max_jump = 28;
    proto.max_speed = 0;
    proto.weight = 1.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GREEN;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {4, 5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {6, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_tankki4(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tank.bmp");
    strcpy(proto.name, "evil tank 3");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "cheese.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "ammo1.spr");

    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "tankshot.wav");
    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ATTACK_2_IF_PLAYER_IN_FRONT;
    proto.AI[2] = AI_FOLLOW_PLAYER;
    proto.AI[3] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 3;
    proto.obstacle = false;
    proto.frame_count = 18;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 43;
    proto.frame_height = 40;
    proto.height = 40;
    proto.reload_time = 100;
    proto.width = 43;
    proto.max_jump = 28;
    proto.max_speed = 1;
    proto.weight = 1.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {3, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[3] = {4, 5, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {6, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);

    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_aave1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_batb.bmp");
    strcpy(proto.name, "ghost 1");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_MOVES_Y_SIN_FREE;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 7;
    proto.image_x = 1;
    proto.image_y = 145;
    proto.frame_width = 38;
    proto.frame_height = 33;
    proto.height = 33;
    proto.reload_time = 10;
    proto.width = 38;
    proto.max_jump = 1;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_ELECTRIC;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[3] = {1, 2, 0};
    unsigned char seq2[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);
    proto.Save(file);
}

void create_prototype_pahakana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_evil.bmp");
    strcpy(proto.name, "the evil one");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "crow.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "crow_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "crow.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "crow.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "crow.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FOLLOW_PLAYER;
    proto.AI[3] = AI_RANDOM_JUMP;
    proto.AI[4] = AI_EVIL_ONE;
    //proto.AI[3]			= AI_CHANGE_SKULLS_IF_KNOCKED_OUT;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 38;
    proto.frame_height = 46;
    proto.height = 46;
    proto.reload_time = 15;
    proto.width = 31;
    proto.max_jump = 16;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.points = 1000;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[9] = {1, 1, 1, 2, 1, 2, 1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {4, 4, 3, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, true);
    Animation_New(proto, ANIMATION_CROUCH, seq2, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    unsigned char seq4[3] = {6, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq4, true);

    proto.Save(file);
}

void create_prototype_pahakana2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_evil.bmp");
    strcpy(proto.name, "the evil one 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "robohen.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "crow.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "crow_ko.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "crow.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "crow.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "crow.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_FOLLOW_PLAYER;
    proto.AI[3] = AI_RANDOM_JUMP;
    proto.AI[4] = AI_EVIL_ONE;
    proto.AI[5] = AI_ATTACK_1_NONSTOP;
    //proto.AI[3]			= AI_CHANGE_SKULLS_IF_KNOCKED_OUT;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 7;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 38;
    proto.frame_height = 46;
    proto.height = 46;
    proto.attack1_duration = 600;
    proto.reload_time = 600;
    proto.width = 31;
    proto.max_jump = 16;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.points = 1000;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[9] = {1, 1, 1, 2, 1, 2, 1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {4, 4, 3, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);
    unsigned char seq3[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq2, true);
    Animation_New(proto, ANIMATION_CROUCH, seq2, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    unsigned char seq4[3] = {6, 5, 0};
    Animation_New(proto, ANIMATION_DEATH, seq4, true);

    proto.Save(file);
}

void create_prototype_luut1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bone.bmp");
    strcpy(proto.name, "chicken bones 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");//egg.spr
    strcpy(proto.projectile2_sprite, "");//doodle.spr

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;

    proto.sound_frequency = 22050;
    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 13;
    proto.frame_rate = 8;
    proto.attack1_duration = 30;
    proto.attack2_duration = 60;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 37;
    proto.frame_height = 45;
    proto.height = 45;//58
    proto.reload_time = 0;//60
    proto.width = 30;
    proto.max_jump = 26;//24
    proto.max_speed = 1;
    proto.weight = 1;
    proto.random_freq = true;
    proto.points = 25;
    proto.protection = DAMAGE_NOISE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_SMOKE_CLOUDS;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_ELECTRIC;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.transparency = false;
    proto.glows = false;
    proto.fire_rate = 0;
    proto.can_float_down = true;
    proto.is_boss = false;
    proto.always_drop_bonus = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[5] = {6, 6, 7, 7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[5] = {6, 7, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {8, 8, 9, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {10, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[6] = {11, 11, 12, 12, 13, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);

    unsigned char seq9[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_ATTACK2, seq9, true);

    proto.Save(file);
}

void create_prototype_luut2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bone.bmp");
    strcpy(proto.name, "chicken bones 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");//egg.spr
    strcpy(proto.projectile2_sprite, "");//doodle.spr

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[5] = AI_TURN_FROM_OBSTACLE_VERT;

    proto.sound_frequency = 22050;
    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 13;
    proto.frame_rate = 10;
    proto.attack1_duration = 30;
    proto.attack2_duration = 60;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 37;
    proto.frame_height = 45;
    proto.height = 45;//58
    proto.reload_time = 0;//60
    proto.width = 30;
    proto.max_jump = 26;//24
    proto.max_speed = 1;
    proto.weight = 0;
    proto.random_freq = true;
    proto.points = 25;
    proto.protection = DAMAGE_NOISE;
    proto.destruction = DESTRUCTION_ANIMATION + DESTRUCTION_SMOKE_CLOUDS;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_ELECTRIC;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.transparency = false;
    proto.glows = false;
    proto.fire_rate = 0;
    proto.can_float_down = true;
    proto.is_boss = false;
    proto.always_drop_bonus = false;

    unsigned char seq1[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[5] = {6, 6, 7, 7, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[5] = {6, 7, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {8, 8, 9, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {10, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[7] = {10, 11, 11, 12, 12, 13, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);

    unsigned char seq9[3] = {6, 7, 0};
    Animation_New(proto, ANIMATION_ATTACK2, seq9, true);

    proto.Save(file);
}

void create_prototype_robokana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_robo.bmp");
    strcpy(proto.name, "robot hen");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "robohead.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "robo_dmg.wav");
    strcpy(proto.sound_files[SOUND_RANDOM], "robo_rnd.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "explot.wav");

    proto.AI[0] = AI_SMALL_CHICKEN;
    proto.AI[1] = AI_FLEES_PLAYER_IF_SEEN;
    proto.AI[2] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[3] = AI_AVOID_PIT;
    proto.AI[4] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 7;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 11;
    proto.frame_rate = 5;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 39;
    proto.frame_height = 36;
    proto.height = 36;
    proto.reload_time = 5;
    proto.width = 30;
    proto.max_jump = 22;
    proto.max_speed = 3;
    proto.weight = 0.8;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_float_down = false;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[5] = {2, 3, 4, 5, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {6, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {7, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[4] = {8, 9, 10, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[2] = {11, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq7[2] = {1, 0};
    Animation_New(proto, ANIMATION_DEATH, seq7, false);

    unsigned char seq8[3] = {3, 6, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_robokanan_paa(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_robo.bmp");
    strcpy(proto.name, "head of robot hen");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "robo_dmg.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "explot.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_VERT_HORI;
    proto.AI[2] = AI_DAMAGE_FROM_WATER;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 40;
    proto.frame_width = 25;
    proto.frame_height = 24;
    proto.height = 24;
    proto.reload_time = 15;
    proto.width = 25;
    proto.max_jump = 10;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 5;
    proto.protection = DAMAGE_HIT;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);

    unsigned char seq2[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_piikkipaa1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (slow circle)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_MOVES_X_COS_SLOW;
    proto.AI[2] = AI_MOVES_Y_SIN_SLOW;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_piikkipaa2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (up and down)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_piikkipaa3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (left and right)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_piikkipaa4(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (bouncer)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[3] = AI_NONSTOP;
    proto.AI[4] = AI_NONE;
    proto.AI[5] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[6] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_piikkipaa5(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (wall climber)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CLIMBER;
    //proto.AI[2]			= AI_NONSTOP;
    proto.AI[2] = AI_NONE;
    proto.AI[3] = AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {3, 4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_piikkipaa6(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (static)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 5;
    proto.image_x = 145;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[9] = {4, 3, 2, 1, 1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);

    unsigned char seq2[2] = {4, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq2, true);

    unsigned char seq3[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq3, true);

    unsigned char seq8[9] = {1, 1, 1, 1, 1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_piikkipaa7(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (pulser 1)");
    strcpy(proto.transformation_sprite, "spkhead8.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 5;
    proto.image_x = 145;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 100;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[5] = {4, 3, 2, 1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[9] = {1, 1, 1, 1, 1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_piikkipaa8(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_shed.bmp");
    strcpy(proto.name, "spike head (pulser 2)");
    strcpy(proto.transformation_sprite, "spkhead7.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 5;
    proto.image_x = 145;
    proto.image_y = 1;
    proto.frame_width = 33;
    proto.frame_height = 33;
    proto.height = 31;
    proto.reload_time = 100;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    unsigned char seq2[2] = {4, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {4, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {4, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {4, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[2] = {4, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_hamahakki1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spid.bmp");
    strcpy(proto.name, "spider");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 38;
    proto.frame_height = 38;
    proto.height = 26;
    proto.reload_time = 12;
    proto.width = 26;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_ORANGE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hamahakki2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spid.bmp");
    strcpy(proto.name, "evil spider");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_VERT_HORI;
    proto.AI[2] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[3] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 38;
    proto.frame_height = 38;
    proto.height = 26;
    proto.reload_time = 12;
    proto.width = 26;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_PURPLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_PURPLE;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hamahakki3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spid.bmp");
    strcpy(proto.name, "guard spider");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_FOLLOWS_PLAYER_IF_SEEN_VERT_HORI;
    proto.AI[2] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[3] = AI_RETURN_TO_START_Y;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 3;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 38;
    proto.frame_height = 38;
    proto.height = 26;
    proto.reload_time = 12;
    proto.width = 26;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_GRAY;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_GRAY;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[5] = {1, 2, 3, 2, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hissi1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_plat.bmp");
    strcpy(proto.name, "moving platform (left and right)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[3] = AI_ARROWS_AFFECT;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;

    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;

    proto.frame_count = 3;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hissi2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_plat.bmp");
    strcpy(proto.name, "moving platform (up and down)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_FROM_OBSTACLE_VERT;
    proto.AI[2] = AI_START_DIRECTION_TOWARDS_PLAYER_VERT;
    proto.AI[3] = AI_ARROWS_AFFECT;
    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;
    proto.frame_count = 2;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_hissi3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_plat.bmp");
    strcpy(proto.name, "moving platform (wall climber)");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CLIMBER;
    //proto.AI[2]			= AI_NONSTOP;
    proto.AI[2] = AI_NONE;
    proto.AI[3] = AI_START_DIRECTION_TOWARDS_PLAYER;
    proto.AI[4] = AI_ARROWS_AFFECT;

    //proto.AI[1]			= AI_START_DIRECTION_TOWARDS_PLAYER;

    //proto.AI[2]			= AI_TURN_ON_COLLISION_HORIZONTAL;

    proto.animation_count = 8;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = true;
    proto.blocks_up = false;
    proto.blocks_down = true;
    proto.blocks_right = false;
    proto.blocks_left = false;
    proto.frame_count = 3;
    proto.frame_rate = 3;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 31;
    proto.frame_height = 32;
    proto.height = 31;
    proto.reload_time = 12;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 2;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 1;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[4] = {1, 2, 3, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, true);

    unsigned char seq6[2] = {1, 0};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, true);

    unsigned char seq8[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, true);

    proto.Save(file);
}

void create_prototype_nuoli_oikealle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow right");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_RIGHT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 19;
    proto.image_x = 1;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_ylos(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow up");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_UP;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 19;
    proto.image_x = 69;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_vasemmalle(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow left");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_LEFT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 19;
    proto.image_x = 137;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_alas(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow down");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_DOWN;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 19;
    proto.image_x = 205;
    proto.image_y = 36;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;
    proto.can_swim = true;

    unsigned char seq1[3] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_oikealle2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow right 2");
    strcpy(proto.transformation_sprite, "arrow2_d.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_RIGHT;
    proto.AI[2] = AI_CHANGE_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 10000;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 10;
    proto.image_x = 1;
    proto.image_y = 70;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {4, 3, 2, 1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_ylos2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow up 2");
    strcpy(proto.transformation_sprite, "arrow2_r.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_UP;
    proto.AI[2] = AI_CHANGE_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 10000;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 10;
    proto.image_x = 1;
    proto.image_y = 172;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {4, 3, 2, 1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_vasemmalle2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow left 2");
    strcpy(proto.transformation_sprite, "arrow2_u.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_LEFT;
    proto.AI[2] = AI_CHANGE_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 10000;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 10;
    proto.image_x = 1;
    proto.image_y = 138;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {4, 3, 2, 1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_nuoli_alas2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_arrw.bmp");
    strcpy(proto.name, "arrow down 2");
    strcpy(proto.transformation_sprite, "arrow2_l.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    //strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    //strcpy(proto.sound_files[SOUND_DESTRUCTION], "ko1.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_ARROW_DOWN;
    proto.AI[2] = AI_CHANGE_IF_HIT;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 10000;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 10;
    proto.image_x = 1;
    proto.image_y = 104;
    proto.frame_width = 31;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 0;
    proto.width = 31;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.can_swim = true;

    unsigned char seq1[7] = {4, 3, 2, 1, 2, 3, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_kallokytkin1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wall.bmp");
    strcpy(proto.name, "skull switch 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "tennis.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_SKULLS_IF_KNOCKED_OUT;

    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 11;
    proto.frame_rate = 5;
    proto.attack1_duration = 9;
    proto.attack2_duration = 9;
    proto.image_x = 1;
    proto.image_y = 100;
    proto.frame_width = 26;
    proto.frame_height = 31;
    proto.height = 31;//58
    proto.reload_time = 80;//40
    proto.width = 26;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.random_freq = true;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 1, 1, 1, 1, 11, 11, 11, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, false);

    unsigned char seq7[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_kallokytkin2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wall.bmp");
    strcpy(proto.name, "skull switch 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");//tennisb.spr
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "tennis.wav");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_SKULLS_IF_HIT;

    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 30000;
    proto.obstacle = false;
    proto.frame_count = 11;
    proto.frame_rate = 5;
    proto.attack1_duration = 9;
    proto.attack2_duration = 9;
    proto.image_x = 1;
    proto.image_y = 100;
    proto.frame_width = 26;
    proto.frame_height = 31;
    proto.height = 31;//58
    proto.reload_time = 80;//40
    proto.width = 26;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.random_freq = true;
    proto.points = 30000;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_ANIMATION;
    proto.shakes = false;
    proto.collides_with_tiles = true;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_HIT;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;

    unsigned char seq1[10] = {1, 1, 1, 1, 1, 1, 11, 11, 11, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    unsigned char seq2[2] = {1, 0};
    Animation_New(proto, ANIMATION_WALK, seq2, true);

    unsigned char seq3[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_UP, seq3, true);

    unsigned char seq4[2] = {1, 0};
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq4, true);

    unsigned char seq5[2] = {1, 0};
    Animation_New(proto, ANIMATION_CROUCH, seq5, false);

    unsigned char seq6[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_DAMAGE, seq6, false);

    unsigned char seq7[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_DEATH, seq7, true);

    unsigned char seq8[2] = {1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq8, false);
    Animation_New(proto, ANIMATION_ATTACK2, seq8, false);

    proto.Save(file);
}

void create_prototype_laatikko_appelsiini(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of orange");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "orange.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 4;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_megafooni(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of mega phone");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "megaphon.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_juoma_muna(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of easter egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "eegg_egg.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_GRAY;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GRAY;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_juoma_tulimuna(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of fire egg");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "eegg_fir.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_hoyhen(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of feather");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "feather.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GRAY;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_kana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of chicken");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "hen.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_RANDOM_JUMP;
    proto.AI[2] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 2;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 18;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 25;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;//true;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GRAY;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_pikkukana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of chick");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "smallhen.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_RANDOM_JUMP;
    proto.AI[2] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 13;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_pikkukanoja(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of chicks");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "smallhen.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_RANDOM_JUMP;
    proto.AI[2] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 3;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 17;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 300;
    proto.protection = DAMAGE_NONE;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_siili(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of hedgehog");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "hedgehog.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[2] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 18;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;//true;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GRAY;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_laatikko_perhoset(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "box of butterflys");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "bttrfly2.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "damage.wav");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "boxcrack.wav");

    proto.AI[0] = AI_BONUS;
    proto.AI[1] = AI_RANDOM_JUMP;
    proto.AI[2] = AI_FRICTION_AFFECTS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 9;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 62;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 17;
    proto.max_speed = 1;
    proto.weight = 1;
    proto.points = 300;
    proto.protection = DAMAGE_NONE;
    proto.shakes = false;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_nippu_omenoita(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "5 apples");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "apple3.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "damage.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 5;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 36;
    proto.image_y = 62;
    proto.frame_width = 40;
    proto.frame_height = 38;
    proto.height = 38;
    proto.reload_time = 0;
    proto.width = 40;
    proto.max_jump = 17;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.collides_with_tiles = false;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_rypaleterttu(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "8 grapes");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "wgrape.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "damage.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 8;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 556;
    proto.image_y = 1;
    proto.frame_width = 26;
    proto.frame_height = 41;
    proto.height = 41;
    proto.reload_time = 0;
    proto.width = 26;
    proto.max_jump = 17;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 10;
    proto.protection = DAMAGE_NONE;
    proto.collides_with_tiles = false;
    proto.shakes = true;
    proto.destruction = DESTRUCTION_STARS_GREEN;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_lahja_hoyhen(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "feather gift");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "feather.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja_siili(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "hedgehog gift");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "hedgehog.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja_pikkukana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "gift small hen");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "smallhen.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja_kivikuutio(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "gift stone cube");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "stncube.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja(char *file, char *csprite, char *snimi) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, snimi);
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, csprite);
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja_doodle_tuli(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "chili-doodle gift");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "can_chil.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 4;
    proto.image_x = 138;
    proto.image_y = 90;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_lahja_doodle_lumi(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "ice-doodle gift");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "can_icet.spr");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus2.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 4;
    proto.image_x = 138;// 110;
    proto.image_y = 90;//1;
    proto.frame_width = 21;
    proto.frame_height = 21;
    proto.height = 21;
    proto.reload_time = 0;
    proto.width = 21;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_BLUE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_taikajuoma_kana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "rooster potion");
    strcpy(proto.transformation_sprite, "rooster.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 163;
    proto.image_y = 1;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_BLUE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_taikajuoma_pikkukana(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "little hen potion");
    strcpy(proto.transformation_sprite, "smallhen.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 163;
    proto.image_y = 1;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_ORANGE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_ORANGE;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_taikajuoma_possu(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "pig potion");
    strcpy(proto.transformation_sprite, "pig.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 163;
    proto.image_y = 1;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_taikajuoma_lintu(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "bird potion");
    strcpy(proto.transformation_sprite, "bird4.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 163;
    proto.image_y = 1;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 20;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_TURQUOISE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_TURQUOISE;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_frog_potion(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "frog potion");
    strcpy(proto.transformation_sprite, "frog5.spr");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 163;
    proto.image_y = 1;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 0;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 60;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_GREEN;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_egg_potion(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "egg potion");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 125;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 20;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 70;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_GRAY;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_fire_egg_potion(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "fire egg potion");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "egg_fire.spr");
    strcpy(proto.projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], "");
        proto.sounds[sound] = -1;
    }

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus7.wav");

    proto.AI[0] = AI_BONUS;

    for (int ai = 1; ai < SPRITE_MAX_AI; ai++)
        proto.AI[ai] = AI_NONE;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 125;
    proto.frame_width = 20;
    proto.frame_height = 22;
    proto.height = 22;
    proto.reload_time = 0;
    proto.width = 20;
    proto.max_jump = 20;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 70;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_SMOKE_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_loud_speaker(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "loud speaker");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodle.spr");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 10;
    proto.frame_rate = 5;
    proto.image_x = 138;//1;
    proto.image_y = 62;//97;
    proto.frame_width = 29;
    proto.frame_height = 25;
    proto.height = 25;
    proto.reload_time = 0;
    proto.width = 29;
    proto.max_jump = 22;
    proto.max_speed = 0;
    proto.weight = 0.5;
    proto.points = 15;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_BLUE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_doodle_chili(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "fire doodle drink");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodle_f.spr");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 27;
    proto.image_y = 32;
    proto.frame_width = 23;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 23;
    proto.max_jump = 22;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 40;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_RED;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_doodle_jaatee(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "ice doodle drink");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "egg.spr");
    strcpy(proto.projectile2_sprite, "doodle_i.spr");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 53;
    proto.image_y = 32;
    proto.frame_width = 23;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 23;
    proto.max_jump = 22;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 40;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_BLUE;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_doodle_valkosipuli(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "garlic doodle drink");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "bad_brth.spr");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 79;
    proto.image_y = 32;
    proto.frame_width = 23;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 23;
    proto.max_jump = 22;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_GREEN;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_doodle_sonic(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, "sonic doodle drink");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "doodle_s.spr");

    strcpy(proto.sound_files[SOUND_DESTRUCTION], "bonus1.wav");

    proto.AI[0] = AI_BONUS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 32;
    proto.frame_width = 23;
    proto.frame_height = 27;
    proto.height = 27;
    proto.reload_time = 0;
    proto.width = 23;
    proto.max_jump = 22;
    proto.max_speed = 1;
    proto.weight = 0.5;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_EXPLOSION_GRAY;
    proto.type = TYPE_BONUS;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_teleportti1(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tele.bmp");
    strcpy(proto.name, "teleporter 1");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "teleport.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "teleport.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TELEPORT;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 2;
    proto.attack1_duration = 200;
    proto.attack2_duration = 200;
    proto.image_x = 1;
    proto.image_y = 68;//1
    proto.frame_width = 32;
    proto.frame_height = 64;
    proto.height = 62;//58
    proto.reload_time = 200;
    proto.width = 30;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_TELEPORTER;
    proto.collides_with_tiles = false;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;//COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    unsigned char seq2[7] = {6, 5, 4, 3, 2, 1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_teleportti2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tele.bmp");
    strcpy(proto.name, "teleporter 2");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "teleport.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "teleport.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TELEPORT;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 2;
    proto.attack1_duration = 200;
    proto.attack2_duration = 200;
    proto.image_x = 1;
    proto.image_y = 68;//1
    proto.frame_width = 32;
    proto.frame_height = 64;
    proto.height = 62;//58
    proto.reload_time = 200;
    proto.width = 30;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_TELEPORTER;
    proto.collides_with_tiles = false;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_RED;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    unsigned char seq2[7] = {6, 5, 4, 3, 2, 1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_teleportti3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_tele.bmp");
    strcpy(proto.name, "teleporter 3");
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "teleport.wav");
    strcpy(proto.sound_files[SOUND_ATTACK2], "teleport.wav");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TELEPORT;

    proto.animation_count = 9;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 6;
    proto.frame_rate = 2;
    proto.attack1_duration = 200;
    proto.attack2_duration = 200;
    proto.image_x = 1;
    proto.image_y = 68;//1
    proto.frame_width = 32;
    proto.frame_height = 64;
    proto.height = 62;//58
    proto.reload_time = 200;
    proto.width = 30;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_TELEPORTER;
    proto.collides_with_tiles = false;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_GREEN;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 3, 4, 5, 6, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    unsigned char seq2[7] = {6, 5, 4, 3, 2, 1, 0};
    Animation_New(proto, ANIMATION_ATTACK1, seq2, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq2, true);

    proto.Save(file);
}

void create_prototype_bg_tree(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg tree");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 246;
    proto.frame_height = 244;
    proto.height = 244;
    proto.reload_time = 0;
    proto.width = 246;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 14;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_tree2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg tree 2");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 248;
    proto.frame_width = 178;
    proto.frame_height = 174;
    proto.height = 150;//174;
    proto.reload_time = 0;
    proto.width = 178;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 8;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_bush(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg bush");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 211;
    proto.image_y = 385;//400
    proto.frame_width = 330;
    proto.frame_height = 93;
    proto.height = 80;
    proto.reload_time = 0;
    proto.width = 330;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 19;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_water_bush(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg water bush");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 425;
    proto.frame_width = 207;
    proto.frame_height = 53;
    proto.height = 300;//260
    proto.reload_time = 0;
    proto.width = 207;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 2;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_cloud(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg cloud");

    //proto.AI[0] = AI_TAUSTA_LIIKKUU_VASEMMALLE;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 250;
    proto.image_y = 1;
    proto.frame_width = 234;
    proto.frame_height = 134;
    proto.height = 134;
    proto.reload_time = 0;
    proto.width = 234;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 3; //2
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_moon(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg moon");

    proto.AI[0] = AI_BACKGROUND_MOON;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 487;
    proto.image_y = 1;
    proto.frame_width = 63;
    proto.frame_height = 63;
    proto.height = 63;
    proto.reload_time = 0;
    proto.width = 63;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 1;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_flower(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg flower1");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 8;
    proto.image_x = 553;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 15;
    proto.height = 15;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 0;
    proto.points = 100;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_bg_flower2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg flower2");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 13;
    proto.image_x = 553;
    proto.image_y = 1;
    proto.frame_width = 13;
    proto.frame_height = 15;
    proto.height = 15;
    proto.reload_time = 0;
    proto.width = 13;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = false;

    unsigned char seq1[5] = {1, 2, 3, 4, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_bg_chain(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg chain 1");

    proto.AI[0] = AI_MOVE_Y_SIN;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 250;
    proto.image_y = 138;
    proto.frame_width = 32;
    proto.frame_height = 96;
    proto.height = 96;
    proto.reload_time = 0;
    proto.width = 32;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;//TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_chain2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg chain 2");

    proto.AI[0] = AI_MOVE_X_COS;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 285;
    proto.image_y = 138;
    proto.frame_width = 96;
    proto.frame_height = 32;
    proto.height = 32;
    proto.reload_time = 0;
    proto.width = 96;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;//TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;


    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_pile_of_stones(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg pile of stones");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 384;
    proto.image_y = 138;//400
    proto.frame_width = 179;
    proto.frame_height = 87;
    proto.height = 87;
    proto.reload_time = 0;
    proto.width = 179;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 16;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_bg_stump(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg stump");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 487;
    proto.image_y = 89;
    proto.frame_width = 99;
    proto.frame_height = 46;
    proto.height = 46;
    proto.reload_time = 0;
    proto.width = 99;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_bg_metal_hen_ad(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "bg metal hen ad");

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 182;
    proto.image_y = 248;
    proto.frame_width = 176;
    proto.frame_height = 123;
    proto.height = 123;
    proto.reload_time = 0;
    proto.width = 176;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.parallax_coefficient = 60;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_BACKGROUND;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);

    proto.Save(file);
}

void create_prototype_worm(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "worm");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_AVOID_PIT;

    proto.animation_count = 9;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 4;
    proto.frame_rate = 7;
    proto.image_x = 553;
    proto.image_y = 31;
    proto.frame_width = 10;
    proto.frame_height = 4;
    proto.height = 4;
    proto.reload_time = 10;
    proto.width = 10;
    proto.max_jump = 15;
    proto.max_speed = 1;
    proto.weight = 0.4;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 3, 4, 3, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, false);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    proto.Save(file);
}

void create_prototype_butterfly(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "butterfly");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_NONSTOP;
    proto.AI[3] = AI_MOVES_Y_SIN_FREE;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 7;
    proto.image_x = 553;
    proto.image_y = 19;
    proto.frame_width = 11;
    proto.frame_height = 9;
    proto.height = 9;
    proto.reload_time = 10;
    proto.width = 11;
    proto.max_jump = 1;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 50;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    proto.Save(file);
}

void create_prototype_butterfly2(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "butterfly 2");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_MOVES_Y_SIN_SLOW;
    proto.AI[3] = AI_NONSTOP;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 8;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 7;
    proto.image_x = 553;
    proto.image_y = 19;
    proto.frame_width = 11;
    proto.frame_height = 9;
    proto.height = 9;
    proto.reload_time = 10;
    proto.width = 11;
    proto.max_jump = 1;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 150;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_ORANGE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_ORANGE;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    proto.Save(file);
}

void create_prototype_butterfly3(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "butterfly 3");
    strcpy(proto.bonus_sprite, "key2.spr");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_TURN_ON_COLLISION_HORIZONTAL;
    proto.AI[2] = AI_MOVES_Y_SIN_SLOW;
    proto.AI[3] = AI_NONSTOP;

    proto.animation_count = 2;
    proto.key = false;
    proto.bonus_count = 1;
    proto.health = 2;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 7;
    proto.image_x = 553;
    proto.image_y = 19;
    proto.frame_width = 11;
    proto.frame_height = 9;
    proto.height = 9;
    proto.reload_time = 10;
    proto.width = 11;
    proto.max_jump = 1;
    proto.max_speed = 1;
    proto.weight = 0;
    proto.points = 250;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_STARS_BLUE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_BLUE;
    proto.enemy = true;
    proto.always_drop_bonus = true;

    unsigned char seq1[7] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    proto.Save(file);
}

void create_prototype_fly(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bg1.bmp");
    strcpy(proto.name, "fly");

    strcpy(proto.sound_files[SOUND_DAMAGE], "");
    strcpy(proto.sound_files[SOUND_DESTRUCTION], "");
    strcpy(proto.sound_files[SOUND_ATTACK1], "");
    strcpy(proto.sound_files[SOUND_ATTACK2], "");

    proto.AI[0] = AI_BASIC;
//	proto.AI[1]			= AI_LIIKKUU_X_COS_NOPEA;
//	proto.AI[2]			= AI_LIIKKUU_Y_SIN_NOPEA;

    proto.AI[1] = AI_RETURN_TO_START_X;
    proto.AI[2] = AI_RETURN_TO_START_Y;
    proto.AI[3] = AI_RANDOM_MOVEMENT_VERT_HORI;

    proto.animation_count = 2;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;
    proto.frame_count = 2;
    proto.frame_rate = 7;
    proto.image_x = 581;
    proto.image_y = 19;
    proto.frame_width = 7;
    proto.frame_height = 5;
    proto.height = 5;
    proto.reload_time = 10;
    proto.width = 7;
    proto.max_jump = 1;
    proto.max_speed = 5;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.collides_with_tiles = false;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[7] = {1, 2, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);
    proto.Save(file);
}

void create_prototype_trap(char *file, char *name, int AI, bool up, bool down, bool right, bool left, int image_x, double weight, int color) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spks.bmp");
    strcpy(proto.name, name);
    strcpy(proto.transformation_sprite, "");
    strcpy(proto.bonus_sprite, "");
    strcpy(proto.projectile1_sprite, "");
    strcpy(proto.projectile2_sprite, "");

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI;
    proto.AI[2] = AI_KILL_ALL;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = true;
    proto.blocks_up = up;
    proto.blocks_down = down;
    proto.blocks_right = right;
    proto.blocks_left = left;

    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 1 + (image_x - 1) * 35;
    proto.image_y = 1;
    proto.frame_width = 32;
    proto.frame_height = 32;
    proto.height = 31;
    proto.reload_time = 300;
    proto.width = 31;
    proto.max_jump = 5;
    proto.max_speed = 3;
    proto.weight = weight;
    proto.points = 0;
    proto.protection = DAMAGE_HIT;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 2;
    proto.damage_type = DAMAGE_HIT;
    proto.color = color;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);

    proto.Save(file);
}

void create_prototype_big_wall(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wall.bmp");
    strcpy(proto.name, "big wall");

    //proto.AI[0] = AI_MOVE_Y_SIN;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = true;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 96;
    proto.frame_height = 96;
    proto.height = 96;
    proto.reload_time = 0;
    proto.width = 96;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 3;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_big_wall_elevator_vert(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_wall.bmp");
    strcpy(proto.name, "big wall elevator vert");

    proto.AI[0] = AI_MOVE_Y_SIN;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = true;
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 96;
    proto.frame_height = 96;
    proto.height = 96;
    proto.reload_time = 0;
    proto.width = 96;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 3;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.collides_with_tiles = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);

    proto.Save(file);
}

void create_prototype_big_trap_falling(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spk2.bmp");
    strcpy(proto.name, "big trap (falling)");

    strcpy(proto.transformation_sprite, "bigtrap2.spr");

    //proto.AI[0] = AI_MOVE_Y_SIN;

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;

    proto.animation_count = 1;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = true;
    //proto.blocks_up
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 96;
    proto.frame_height = 96;
    proto.height = 96;
    proto.reload_time = 100;
    proto.width = 96;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 3;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;


    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);

    proto.Save(file);
}

void create_prototype_big_trap_rising(char *file) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_spk2.bmp");
    strcpy(proto.name, "big trap (rising)");

    strcpy(proto.transformation_sprite, "bigtrap1.spr");

    //proto.AI[0] = AI_MOVE_Y_SIN;

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI_CHANGE_TIMER;

    proto.animation_count = 8;
    proto.key = false;
    proto.bonus_count = 0;
    proto.health = 2;
    proto.obstacle = true;
    //proto.blocks_up
    proto.frame_count = 1;
    proto.frame_rate = 4;
    proto.image_x = 1;
    proto.image_y = 1;
    proto.frame_width = 96;
    proto.frame_height = 96;
    proto.height = 96;
    proto.reload_time = 100;
    proto.width = 96;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = -0.5;
    proto.parallax_coefficient = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.shakes = false;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 3;
    proto.damage_type = DAMAGE_FALL;
    proto.color = COLOR_NORMAL;
    proto.enemy = true;


    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, false);
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);

    proto.Save(file);
}

void create_prototype_info(char *file, char *name, int AI) {
    PK2Sprite_Prototype proto;

    strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    strcpy(proto.image_file, "spr_bons.bmp");
    strcpy(proto.name, name);

    proto.AI[0] = AI_BASIC;
    proto.AI[1] = AI;

    proto.animation_count = 1;
    proto.key = false;
    proto.health = 1;
    proto.obstacle = false;

    proto.frame_count = 1;
    proto.frame_rate = 8;
    proto.image_x = 1;
    proto.image_y = 150;
    proto.frame_width = 23;
    proto.frame_height = 31;
    proto.height = 31;
    proto.reload_time = 300;
    proto.width = 23;
    proto.max_jump = 0;
    proto.max_speed = 0;
    proto.weight = 0;
    proto.points = 0;
    proto.protection = DAMAGE_NONE;
    proto.collides_with_tiles = false;
    proto.destruction = DESTRUCTION_INDESTRUCTIBLE;
    proto.type = TYPE_GAME_CHARACTER;
    proto.damage = 0;
    proto.damage_type = DAMAGE_NONE;
    proto.color = COLOR_NORMAL;
    proto.enemy = false;

    unsigned char seq1[2] = {1, 0};
    Animation_New(proto, ANIMATION_IDLE, seq1, true);
    Animation_New(proto, ANIMATION_WALK, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_UP, seq1, true);
    Animation_New(proto, ANIMATION_JUMP_DOWN, seq1, true);
    Animation_New(proto, ANIMATION_CROUCH, seq1, true);
    Animation_New(proto, ANIMATION_DAMAGE, seq1, true);
    Animation_New(proto, ANIMATION_DEATH, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK1, seq1, true);
    Animation_New(proto, ANIMATION_ATTACK2, seq1, true);

    proto.Save(file);
}

int main() {
/*
	// PELIHAHMOT
	create_prototype_rooster(				"sprites\\rooster.spr");	// p�ivitetty - update 1!
	create_prototype_naked_rooster(		"sprites\\r_naked.spr");	// p�ivitetty - update 1!
	create_prototype_hen(				"sprites\\hen.spr");
	create_prototype_baby_chicken(			"sprites\\smallhen.spr");
	create_prototype_frog1(			"sprites\\frog1.spr");
	create_prototype_frog2(			"sprites\\frog2.spr");
	create_prototype_frog3(			"sprites\\frog3.spr");
	create_prototype_evil_frog(			"sprites\\frog4.spr");		//uusi! - update 2!
	create_prototype_player_frog(			"sprites\\frog5.spr");		//uusi! - update 2!
	create_prototype_bunny(			"sprites\\jumper.spr");
	create_prototype_bunny2(			"sprites\\jumper2.spr");
	create_prototype_boxing_bunny(				"sprites\\rabbit.spr");		//��net p�ivitetty - update 2!
	create_prototype_jumping_bunny(				"sprites\\rabbit2.spr");	//��net p�ivitetty - update 2!
	create_prototype_stupid_hedgehog(				"sprites\\hedgehog.spr");
	create_prototype_evil_hedgehog(				"sprites\\hedgeho2.spr");
	create_prototype_deaf_hedgehog(				"sprites\\hedgeho3.spr");
	create_prototype_stoned_hedgehog(				"sprites\\hedgeho4.spr");
	create_prototype_bat(			"sprites\\bat.spr");
	create_prototype_bomber_bat(			"sprites\\bat2.spr");
	create_prototype_evil_bomber_bat(			"sprites\\bat3.spr");
	create_prototype_liekki(				"sprites\\flame.spr");
	create_prototype_liekki2(			"sprites\\flame2.spr");
*/
    create_prototype_liekki3("sprites\\flame3.spr");    // uusi - update 3!
/*	create_prototype_possu(				"sprites\\pig.spr");
	create_prototype_possu2(				"sprites\\pig2.spr");
	create_prototype_lentava_possu(		"sprites\\fly_pig.spr");
	create_prototype_bomb(				"sprites\\bomb.spr");	// p�ivitetty - update 2!
	create_prototype_bird(				"sprites\\bird1.spr");
	create_prototype_bird2(				"sprites\\bird2.spr");
	create_prototype_bird3(				"sprites\\bird3.spr");
	create_prototype_bird4(				"sprites\\bird4.spr");	// p�ivitetty - update 2
	create_prototype_wolf(				"sprites\\wolf1.spr");
	create_prototype_evil_wolf(				"sprites\\wolf2.spr");
	create_prototype_smart_wolf(				"sprites\\wolf3.spr");
	create_prototype_giant_mouse(			"sprites\\gmouse1.spr");
	create_prototype_giant_mouse2(		"sprites\\gmouse2.spr"); // p�ivitetty - update 1!
	create_prototype_giant_mouse3(		"sprites\\gmouse3.spr");
	create_prototype_fish1(				"sprites\\fish1.spr");
	create_prototype_fish2(				"sprites\\fish2.spr");
	create_prototype_fish3(				"sprites\\fish3.spr");
	create_prototype_bee(			"sprites\\bee.spr");
	create_prototype_bee2(		"sprites\\bee2.spr");
	create_prototype_bee3(		"sprites\\bee3.spr");	// uusi - update 1!
	create_prototype_beehive(		"sprites\\beehive.spr");
	create_prototype_beehive2(		"sprites\\beehive2.spr");
	create_prototype_tankki1(			"sprites\\tank.spr");
	create_prototype_tankki2(			"sprites\\tank2.spr");
	create_prototype_tankki3(			"sprites\\tank3.spr");
	create_prototype_tankki4(			"sprites\\tank4.spr");
	create_prototype_aave1(				"sprites\\ghost1.spr");
	create_prototype_pahakana(			"sprites\\evilone.spr");
	create_prototype_pahakana2(			"sprites\\evilone2.spr"); // uusi - update 2!
	create_prototype_giant_bee(	"sprites\\gbee.spr");	// uusi - update 2!
	create_prototype_robokana(			"sprites\\robohen.spr"); // uusi - update 2!
	create_prototype_robokanan_paa(		"sprites\\robohead.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto1("sprites\\roboss1.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto2("sprites\\roboss2.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto3("sprites\\roboss3.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto4("sprites\\roboss4.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto5("sprites\\roboss5.spr"); // uusi - update 2!
*/
    create_prototype_luut1("sprites\\bones1.spr");        // uusi - update 3!!
    create_prototype_luut2("sprites\\bones2.spr");        // uusi - update 3!!
    create_prototype_piikkipaa1("sprites\\spkhead1.spr");    // uusi - update 3!!
    create_prototype_piikkipaa2("sprites\\spkhead2.spr");    // uusi - update 3!!
    create_prototype_piikkipaa3("sprites\\spkhead3.spr");    // uusi - update 3!!
    create_prototype_piikkipaa4("sprites\\spkhead4.spr");    // uusi - update 3!!
    create_prototype_piikkipaa5("sprites\\spkhead5.spr");    // uusi - update 3!!
    create_prototype_piikkipaa6("sprites\\spkhead6.spr");    // uusi - update 3!!
    create_prototype_piikkipaa7("sprites\\spkhead7.spr");    // uusi - update 3!!
    create_prototype_piikkipaa8("sprites\\spkhead8.spr");    // uusi - update 3!!

    create_prototype_hamahakki1("sprites\\spider1.spr");    // uusi - update 3!!
    //create_prototype_hamahakki2(			"sprites\\spider2.spr");	// uusi - update 3!!
    //create_prototype_hamahakki3(			"sprites\\spider3.spr");	// uusi - update 3!!

    create_prototype_turtle_walking("sprites\\turtle1.spr");    // uusi - update 3!!
    create_prototype_turtle_shell("sprites\\turtle2.spr");    // uusi - update 3!!
    create_prototype_turtle_waking("sprites\\turtle3.spr");    // uusi - update 3!!

    create_prototype_red_turtle("sprites\\turtle4.spr");    // uusi - update 3!!

    create_prototype_hissi1("sprites\\platfrm1.spr");    // uusi - update 3!!
    create_prototype_hissi2("sprites\\platfrm2.spr");    // uusi - update 3!!
    create_prototype_hissi3("sprites\\platfrm3.spr");    // uusi - update 3!!

    create_prototype_nuoli_oikealle("sprites\\arrow_r.spr");    // uusi - update 3!!
    create_prototype_nuoli_ylos("sprites\\arrow_u.spr");    // uusi - update 3!!
    create_prototype_nuoli_vasemmalle("sprites\\arrow_l.spr");    // uusi - update 3!!
    create_prototype_nuoli_alas("sprites\\arrow_d.spr");    // uusi - update 3!!
    create_prototype_nuoli_oikealle2("sprites\\arrow2_r.spr");    // uusi - update 3!!
    create_prototype_nuoli_ylos2("sprites\\arrow2_u.spr");    // uusi - update 3!!
    create_prototype_nuoli_vasemmalle2("sprites\\arrow2_l.spr");    // uusi - update 3!!
    create_prototype_nuoli_alas2("sprites\\arrow2_d.spr");    // uusi - update 3!!
    //create_prototype_aave2(				"sprites\\ghost1.spr");

    // BONUKSET
    create_prototype_pineapple1("sprites\\pineapp1.spr");    // uusi - update 3!!
    create_prototype_candy("sprites\\candy1.spr", "candy 1", COLOR_BLUE, 10);// uusi - update 3!!
    create_prototype_candy("sprites\\candy2.spr", "candy 2", COLOR_RED, 20);// uusi - update 3!!
    create_prototype_candy("sprites\\candy3.spr", "candy 3", COLOR_GREEN, 30);// uusi - update 3!!
    create_prototype_candy("sprites\\candy4.spr", "candy 4", COLOR_ORANGE, 40);// uusi - update 3!!
    create_prototype_candy("sprites\\candy5.spr", "candy 5", COLOR_PURPLE, 50);// uusi - update 3!!
    create_prototype_candy("sprites\\candy6.spr", "candy 6", COLOR_TURQUOISE, 60);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel1.spr", "jewel 1", COLOR_BLUE, 80);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel2.spr", "jewel 2", COLOR_RED, 90);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel3.spr", "jewel 3", COLOR_GREEN, 100);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel4.spr", "jewel 4", COLOR_ORANGE, 110);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel5.spr", "jewel 5", COLOR_PURPLE, 120);// uusi - update 3!!
    create_prototype_jewel("sprites\\jewel6.spr", "jewel 6", COLOR_TURQUOISE, 130);// uusi - update 3!!
/*
    create_prototype_apple(				"sprites\\apple.spr");
	create_prototype_falling_apple(				"sprites\\apple2.spr");
	create_prototype_changing_apple(				"sprites\\apple3.spr");
	create_prototype_rotten_apple(				"sprites\\apple4.spr");
	create_prototype_apple5(				"sprites\\apple5.spr");
	create_prototype_big_apple(			"sprites\\big_appl.spr");
	create_prototype_big_apple2(			"sprites\\big_app2.spr");
	create_prototype_nippu_omenoita(		"sprites\\apples.spr");
	create_prototype_carrot(			"sprites\\carrot.spr");
	create_prototype_cheese(				"sprites\\cheese.spr");
	create_prototype_pepper(			"sprites\\pepper.spr");
	create_prototype_feather(				"sprites\\feather.spr");
	create_prototype_feather2(			"sprites\\feather2.spr");
	create_prototype_orange(			"sprites\\orange.spr");
	create_prototype_grapefruit(			"sprites\\grape.spr");
	create_prototype_grape(		"sprites\\wgrape.spr"); // uusi - update 2!
	create_prototype_rypaleterttu(		"sprites\\wgrapes.spr"); // uusi - update 2!
*/
    create_prototype_ghost_powder1("sprites\\gpowder1.spr"); // uusi - update 3!!
    create_prototype_ghost_powder2("sprites\\gpowder2.spr"); // uusi - update 3!!
/*
	create_prototype_key1(				"sprites\\key.spr");
	create_prototype_key2(				"sprites\\key2.spr");
	create_prototype_key3(				"sprites\\key3.spr");
	create_prototype_jumping_key(				"sprites\\key4.spr");	// p�ivitetty - update 2

	create_prototype_clock20s(				"sprites\\clock1.spr");
	create_prototype_clock10s(				"sprites\\clock2.spr");
	create_prototype_clock30s(				"sprites\\clock3.spr");

	// "AMMUKSET"
	create_prototype_egg(				"sprites\\egg.spr");//
	create_prototype_fire_egg(			"sprites\\egg_fire.spr");
	create_prototype_smelly_egg(			"sprites\\egg_stnk.spr");
	create_prototype_ice_egg(			"sprites\\egg_ice.spr");	// uusi! - update 2
	create_prototype_cockadoodledoo(				"sprites\\doodle.spr");
	create_prototype_fire_doodle_flame(			"sprites\\doodle_f.spr");
	create_prototype_fire_doodle(		"sprites\\doodlef2.spr");
	create_prototype_ice_doodle(			"sprites\\doodle_i.spr");
	create_prototype_sonic_doodle(		"sprites\\doodle_s.spr");
	create_prototype_ice_cube(			"sprites\\icecube.spr");
	create_prototype_bad_breath(	"sprites\\bad_brth.spr");
	create_prototype_tennis_ball(		"sprites\\tennisb.spr");
	create_prototype_ammo1(				"sprites\\ammo1.spr");
	create_prototype_ammo2(				"sprites\\ammo2.spr");

	// LAATIKOT
	create_prototype_laatikko_megafooni( "sprites\\box_mph.spr");
	create_prototype_laatikko_juoma_muna("sprites\\box_egg.spr");
	create_prototype_laatikko_juoma_tulimuna("sprites\\box_fegg.spr");
	create_prototype_laatikko_appelsiini("sprites\\box_o.spr");
	create_prototype_laatikko_hoyhen(	"sprites\\box_f.spr");
	create_prototype_laatikko_kana(		"sprites\\box_r.spr");
	create_prototype_laatikko_pikkukana(	"sprites\\box_s.spr");
	create_prototype_laatikko_pikkukanoja("sprites\\box_3s.spr");
	create_prototype_laatikko_siili(		"sprites\\box_hedg.spr");
	create_prototype_laatikko_perhoset(	"sprites\\box_bfly.spr");	// uusi! - update 2

	// LAHJAT
	create_prototype_lahja_hoyhen(		"sprites\\gift_fth.spr");
	create_prototype_lahja_doodle_tuli(	"sprites\\gift_cac.spr");
	create_prototype_lahja_doodle_lumi(	"sprites\\gift_cai.spr");
	create_prototype_lahja(				"sprites\\gift_flw.spr", "bg_flwr1.spr", "gift flower"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_chi.spr", "smallhen.spr", "gift chick"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_hed.spr", "hedgehog.spr", "gift hedgehog"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_stn.spr", "stncube.spr", "stone cube");  // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_swi.spr", "switch1.spr", "gift switch");  // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bf1.spr", "bttrfly2.spr", "gift butterfly"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bf2.spr", "bttrfly3.spr", "gift butterfly 2"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_key.spr", "key.spr", "gift key");      // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_ke2.spr", "key2.spr", "gift blue key");     // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bee.spr", "bee3.spr", "gift bee");     // uusi - update 1!

	// TAIKAJUOMAT
	create_prototype_taikajuoma_kana(	"sprites\\potion_h.spr");
	create_prototype_taikajuoma_pikkukana("sprites\\potion_s.spr");
	create_prototype_taikajuoma_possu(	"sprites\\potion_p.spr");
	create_prototype_taikajuoma_lintu(	"sprites\\potion_b.spr");
	create_prototype_frog_potion("sprites\\potion_f.spr"); // uusi! - update 2
	create_prototype_egg_potion(	        "sprites\\ptn_egg.spr");
	create_prototype_fire_egg_potion(     "sprites\\ptn_fegg.spr");
	create_prototype_paasiaismuna(		"sprites\\eegg_egg.spr","easter egg","egg.spr",10,COLOR_NORMAL, DESTRUCTION_SMOKE_GRAY);
	create_prototype_paasiaismuna(		"sprites\\eegg_fir.spr","fire easter egg","egg_fire.spr",20,COLOR_RED, DESTRUCTION_SMOKE_RED);
	create_prototype_paasiaismuna(		"sprites\\eegg_stn.spr","smelly easter egg","egg_stnk.spr",30,COLOR_GREEN, DESTRUCTION_SMOKE_GREEN);
	create_prototype_easter_egg(		"sprites\\eegg_ice.spr","ice egg","egg_ice.spr",30,COLOR_BLUE, DESTRUCTION_SMOKE_BLUE);  // uusi! - update 2

	// DOODLET-JUOMAT JA MEGAFOONI
	create_prototype_loud_speaker(		    "sprites\\megaphon.spr");
	create_prototype_doodle_chili(		"sprites\\can_chil.spr");
	create_prototype_doodle_jaatee(		"sprites\\can_icet.spr");
	create_prototype_doodle_valkosipuli(	"sprites\\can_garl.spr");
	create_prototype_doodle_sonic(		"sprites\\can_snic.spr");

	// TELEPORTIT
	create_prototype_teleportti1(		"sprites\\tlport1.spr");
	create_prototype_teleportti2(		"sprites\\tlport2.spr");
	create_prototype_teleportti3(		"sprites\\tlport3.spr");

	// KYTKIMET
	create_prototype_kallokytkin1(		"sprites\\switch1.spr");
	create_prototype_kallokytkin2(		"sprites\\switch2.spr");

	// TAUSTASPRITET
	create_prototype_bg_flower(		"sprites\\bg_flwr1.spr");	// p�ivitetty - update 1!
	create_prototype_bg_flower2(		"sprites\\bg_flwr2.spr");
	create_prototype_bg_tree(			"sprites\\bg_tree.spr");
	create_prototype_bg_tree2(		"sprites\\bg_tree2.spr");
	create_prototype_bg_bush(		"sprites\\bg_bush.spr");
	create_prototype_bg_water_bush(	"sprites\\bg_wbush.spr");
	create_prototype_bg_cloud(		"sprites\\bg_cloud.spr");
	create_prototype_bg_moon(		    "sprites\\bg_moon.spr");
	create_prototype_bg_chain(  "sprites\\bg_chan1.spr");
	create_prototype_bg_chain2(  "sprites\\bg_chan2.spr");
	create_prototype_worm(				"sprites\\worm.spr");
	create_prototype_butterfly(			"sprites\\bttrfly.spr");	// p�ivitetty - update 1!
	create_prototype_butterfly2(			"sprites\\bttrfly2.spr");	// p�ivitetty - update 1!
	create_prototype_butterfly3(			"sprites\\bttrfly3.spr");	// p�ivitetty - update 1!
	create_prototype_fly(			"sprites\\fly.spr");
	create_prototype_bg_pile_of_stones(	"sprites\\bg_spile.spr");	// uusi - update 2!
	create_prototype_bg_stump(		"sprites\\bg_stump.spr");	// uusi - update 2!
	create_prototype_bg_metal_hen_ad(		"sprites\\bg_ad.spr");		// uusi - update 2!

	// SEIN�T
	create_prototype_stone_cube(			"sprites\\stncube.spr");
	create_prototype_stone_cube_item(			"sprites\\stncube2.spr");
	create_prototype_stone_cube_bonus(			"sprites\\stncube3.spr");

	create_prototype_ansa("sprites\\trap1_ff.spr", "falling spike floor",		AI_FRICTION_AFFECTS, true, false, true, true, 1, 1, COLOR_NORMAL);
	create_prototype_ansa("sprites\\trap2_cf.spr", "falling spike ceiling",		AI_FRICTION_AFFECTS, false, true, true, true, 2, 1, COLOR_NORMAL);
	create_prototype_ansa("sprites\\trap3_wf.spr", "falling spike wall left",	AI_FRICTION_AFFECTS, true, true, false, true, 3, 1, COLOR_NORMAL);
	create_prototype_ansa("sprites\\trap4_wf.spr", "falling spike wall right",	AI_FRICTION_AFFECTS, true, true, true, false, 4, 1, COLOR_NORMAL);
	create_prototype_ansa("sprites\\trap5_f2.spr", "switch 2 move up",			AI_MOVES_UP_IF_SWITCH2_PRESSED, true, false, true, true, 1, 0, COLOR_BLUE);
	create_prototype_ansa("sprites\\trap6_c2.spr", "switch 2 move down",			AI_MOVES_DOWN_IF_SWITCH2_PRESSED, false, true, true, true, 2, 0, COLOR_BLUE);
	create_prototype_ansa("sprites\\trap7_w3.spr", "switch 3 move left",			AI_MOVES_LEFT_IF_SWITCH3_PRESSED, true, true, false, true, 3, 0, COLOR_GREEN);
	create_prototype_trap("sprites\\trap8_w3.spr", "switch 3 move right",		AI_MOVES_RIGHT_IF_SWITCH3_PRESSED, true, true, true, false,   4, 0, COLOR_GREEN);

	create_prototype_big_wall(		    "sprites\\wall1big.spr");
	create_prototype_big_wall_elevator_vert("sprites\\wall2big.spr");
 */
    create_prototype_big_trap_falling("sprites\\bigtrap1.spr");    //uusi! - update 3!
    create_prototype_big_trap_rising("sprites\\bigtrap2.spr");    //uusi! - update 3!
/*
	// INFO-KYLTIT
	create_prototype_info( "sprites\\info1.spr" ,"doodle attack info" ,	AI_INFO1);
	create_prototype_info( "sprites\\info2.spr" ,"egg attack info"    ,	AI_INFO2);
	create_prototype_info( "sprites\\info3.spr" ,"exit sign info"    ,	AI_INFO3);
	create_prototype_info( "sprites\\info4.spr" ,"keys info"    ,		AI_INFO4);
	create_prototype_info( "sprites\\info5.spr" ,"switch info"    ,		AI_INFO5);
	create_prototype_info( "sprites\\info6.spr" ,"green switch info"    ,AI_INFO6);
	create_prototype_info( "sprites\\info7.spr" ,"blue switch info",		AI_INFO7);
	create_prototype_info( "sprites\\info8.spr" ,"orange switch info",	AI_INFO8);
	create_prototype_info( "sprites\\info9.spr" ,"gifts info"    ,		AI_INFO9);
	create_prototype_info( "sprites\\info10.spr" ,"energy items info",	AI_INFO10);
	create_prototype_info( "sprites\\info11.spr" ,"teleports info"    ,	AI_INFO11);
	create_prototype_info( "sprites\\info12.spr" ,"score info"    ,		AI_INFO12);
	create_prototype_info( "sprites\\info13.spr" ,"hidden bonuses info", AI_INFO13);
	create_prototype_info( "sprites\\info14.spr" ,"key-lock info"   ,	AI_INFO14);
	create_prototype_info( "sprites\\info15.spr" ,"traps info"    ,		AI_INFO15);
	create_prototype_info( "sprites\\info16.spr" ,"time info"    ,		AI_INFO16);
	create_prototype_info( "sprites\\info17.spr" ,"enemies info"    ,		AI_INFO17);
	create_prototype_info( "sprites\\info18.spr" ,"wind info"    ,		AI_INFO18);
	create_prototype_info( "sprites\\info19.spr" ,"rescue info"    ,		AI_INFO19);
*/

/*
    // UPDATE 2
	create_prototype_rooster(				"sprites\\rooster.spr");	// p�ivitetty - update 1!
	create_prototype_naked_rooster(		"sprites\\r_naked.spr");	// p�ivitetty - update 1!
	create_prototype_evil_frog(			"sprites\\frog4.spr");		//uusi! - update 2!
	create_prototype_player_frog(			"sprites\\frog5.spr");		//uusi! - update 2!
	create_prototype_bunny(			"sprites\\jumper.spr");		//��net p�ivitetty - update 2!
	create_prototype_bunny2(			"sprites\\jumper2.spr");	//��net p�ivitetty - update 2!
	create_prototype_boxing_bunny(				"sprites\\rabbit.spr");		//��net p�ivitetty - update 2!
	create_prototype_jumping_bunny(				"sprites\\rabbit2.spr");	//��net p�ivitetty - update 2!
	create_prototype_bomb(				"sprites\\bomb.spr");	// p�ivitetty - update 2!
	create_prototype_giant_mouse2(		"sprites\\gmouse2.spr"); // p�ivitetty - update 1!
	create_prototype_bee3(		"sprites\\bee3.spr");	// uusi - update 1!
	create_prototype_tankki1(			"sprites\\tank.spr");	//��net p�ivitetty - update 2!
	create_prototype_tankki2(			"sprites\\tank2.spr");	//��net p�ivitetty - update 2!
	create_prototype_tankki3(			"sprites\\tank3.spr");	//��net p�ivitetty - update 2!
	create_prototype_tankki4(			"sprites\\tank4.spr");	//��net p�ivitetty - update 2!
	create_prototype_pahakana2(			"sprites\\evilone2.spr"); // uusi - update 2!
	create_prototype_giant_bee(	"sprites\\gbee.spr");	// uusi - update 2!
	create_prototype_robokana(			"sprites\\robohen.spr"); // uusi - update 2!
	create_prototype_robokanan_paa(		"sprites\\robohead.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto1("sprites\\roboss1.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto2("sprites\\roboss2.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto3("sprites\\roboss3.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto4("sprites\\roboss4.spr"); // uusi - update 2!
	create_prototype_jattirobokana_muoto5("sprites\\roboss5.spr"); // uusi - update 2!
	create_prototype_bird4(				"sprites\\bird4.spr");	// p�ivitetty - update 2!

	// BONUKSET
	create_prototype_grape(		"sprites\\wgrape.spr"); // uusi - update 2!
	create_prototype_rypaleterttu(		"sprites\\wgrapes.spr"); // uusi - update 2!
	create_prototype_jumping_key(				"sprites\\key4.spr");	// p�ivitetty - update 2!

	// "AMMUKSET"
	create_prototype_ice_egg(			"sprites\\egg_ice.spr");	// uusi! - update 2

	// LAATIKOT
	create_prototype_laatikko_perhoset(	"sprites\\box_bfly.spr");	// uusi! - update 2

	// LAHJAT
	create_prototype_lahja(				"sprites\\gift_flw.spr", "bg_flwr1.spr", "gift flower"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_chi.spr", "smallhen.spr", "gift chick"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_hed.spr", "hedgehog.spr", "gift hedgehog"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_stn.spr", "stncube.spr", "stone cube");  // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_swi.spr", "switch1.spr", "gift switch");  // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bf1.spr", "bttrfly2.spr", "gift butterfly"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bf2.spr", "bttrfly3.spr", "gift butterfly 2"); // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_key.spr", "key.spr", "gift key");      // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_ke2.spr", "key2.spr", "gift blue key");     // uusi - update 1!
	create_prototype_lahja(				"sprites\\gift_bee.spr", "bee3.spr", "gift bee");     // uusi - update 1!

	// TAIKAJUOMAT
	create_prototype_frog_potion("sprites\\potion_f.spr"); // uusi! - update 2
	create_prototype_easter_egg(		"sprites\\eegg_ice.spr","ice egg","egg_ice.spr",30,COLOR_BLUE, DESTRUCTION_SMOKE_BLUE);  // uusi! - update 2

	// DOODLET-JUOMAT JA MEGAFOONI
	// TELEPORTIT
	// KYTKIMET
	// TAUSTASPRITET
	create_prototype_bg_flower(		"sprites\\bg_flwr1.spr");	// p�ivitetty - update 1!
	create_prototype_butterfly(			"sprites\\bttrfly.spr");	// p�ivitetty - update 1!
	create_prototype_butterfly2(			"sprites\\bttrfly2.spr");	// p�ivitetty - update 1!
	create_prototype_butterfly3(			"sprites\\bttrfly3.spr");	// p�ivitetty - update 1!
	create_prototype_bg_pile_of_stones(	"sprites\\bg_spile.spr");	// uusi - update 2!
	create_prototype_bg_stump(		"sprites\\bg_stump.spr");	// uusi - update 2!
	create_prototype_bg_metal_hen_ad(		"sprites\\bg_ad.spr");		// uusi - update 2!
*/
/*
*info01:			use doodle to knock out enemies.
*info02:			use eggs to knock out enemies.
*info03:			find the exit sign.
*info04:			collecting all keys opens all locks.
*info05:			you can activate switches by jumping on them.
*info06:			green switches activate green blocks.
*info07:			blue switches activate blue blocks.
*info08:			orange switches turn off fire.
*info09:			collect gifts to obtain useful items.
*info10:			some items will restore energy.
*info11:			don't get confused with teleports.
*info12:			some items can give you special attacks.
*info13:			look for hidden bonuses!
*info14:			you can stomp on enemies, but you will lose energy.
*info15:			look out for traps!
*info16:			hurry up! time is running out!
*info17:			be aware that some enemies resist certain attacks.
*info18:			a hen is mightier than the sword.
*info19:			why did the chicken cross the road?
*/
    return 0;
}