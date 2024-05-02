#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include "sprites.h"
#include "Engine/draw.h"
#include "Engine/sound.h"

#include "old_headers.h"

/* -------- PK2Sprite Prototypes ------------------------------------------------------------------ */

PK2Sprite_Prototype::PK2Sprite_Prototype() {
    strcpy(version, LATEST_VERSION_PK2SPRITE);
    strcpy(file, "");
    strcpy(image_file, "");
    strcpy(name, "");
    strcpy(this->transformation_sprite, "");
    strcpy(this->bonus_sprite, "");
    strcpy(this->projectile1_sprite, "");
    strcpy(this->projectile2_sprite, "");
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], "");
        sounds[sound] = -1;
    }
    sound_frequency = 22050;
    projectile1 = -1;
    projectile2 = -1;
    animation_count = 0;
    key = false;
    bonus = -1;
    bonus_count = 1;
    health = 0;
    obstacle = false;
    blocks_up = true;
    blocks_down = true;
    blocks_right = true;
    blocks_left = true;
    frame_count = 0;
    frame_rate = 0;
    attack1_duration = 60;
    attack2_duration = 60;
    index = 0;
    image_x = 0;
    image_y = 0;
    frame_width = 0;
    frame_height = 0;
    height = 0;
    reload_time = 0;
    width = 0;
    max_jump = 0;
    max_speed = 3;
    transformation = -1;
    weight = 0;
    parallax_coefficient = 0;
    points = 0;
    random_freq = true;
    protection = DAMAGE_NONE;
    shakes = false;
    collides_with_tiles = true;
    destruction = DESTRUCTION_ANIMATION;
    type = TYPE_NOTHING;
    damage = 0;
    damage_type = DAMAGE_HIT;
    color = COLOR_NORMAL;
    enemy = false;

    transparency = false;
    glows = false;
    fire_rate = 0;
    can_float_down = false;
    is_boss = false;
    always_drop_bonus = false;
    can_swim = false;

    for (int i = 0; i < SPRITE_MAX_AI; i++) {
        AI[i] = AI_NONE;
    }
    for (int i = 0; i < SPRITE_MAX_FRAMES; i++)
        frames[i] = 0;
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = 0;
        animations[i].loop = false;
        animations[i].frame_count = 0;
    }
}

PK2Sprite_Prototype::~PK2Sprite_Prototype() {
    for (int i = 0; i < SPRITE_MAX_FRAMES; i++) {
        if (frames[i] > 0)
            PisteDraw_Buffer_Destroy(this->frames[i]);
    }
}

void PK2Sprite_Prototype::Copy(const PK2Sprite_Prototype &prototype) {
    strcpy(version, prototype.version);
    strcpy(file, prototype.file);
    strcpy(image_file, prototype.image_file);
    strcpy(name, prototype.name);
    strcpy(this->transformation_sprite, prototype.transformation_sprite);
    strcpy(this->bonus_sprite, prototype.bonus_sprite);
    strcpy(this->projectile1_sprite, prototype.projectile1_sprite);
    strcpy(this->projectile2_sprite, prototype.projectile2_sprite);

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], prototype.sound_files[sound]);
        sounds[sound] = prototype.sounds[sound];
    }

    sound_frequency = prototype.sound_frequency;
    projectile1 = prototype.projectile1;
    projectile2 = prototype.projectile2;
    animation_count = prototype.animation_count;
    key = prototype.key;
    bonus = prototype.bonus;
    bonus_count = prototype.bonus_count;
    health = prototype.health;
    obstacle = prototype.obstacle;
    blocks_up = prototype.blocks_up;
    blocks_down = prototype.blocks_down;
    blocks_right = prototype.blocks_right;
    blocks_left = prototype.blocks_left;
    frame_count = prototype.frame_count;
    frame_rate = prototype.frame_rate;
    attack1_duration = prototype.attack1_duration;
    attack2_duration = prototype.attack2_duration;
    index = prototype.index;
    image_x = prototype.image_x;
    image_y = prototype.image_y;
    frame_width = prototype.frame_width;
    frame_height = prototype.frame_height;
    height = prototype.height;
    reload_time = prototype.reload_time;
    width = prototype.width;
    max_jump = prototype.max_jump;
    max_speed = prototype.max_speed;
    transformation = prototype.transformation;
    weight = prototype.weight;
    parallax_coefficient = prototype.parallax_coefficient;
    points = prototype.points;
    random_freq = prototype.random_freq;
    protection = prototype.protection;
    shakes = prototype.shakes;
    collides_with_tiles = prototype.collides_with_tiles;
    destruction = prototype.destruction;
    type = prototype.type;
    damage = prototype.damage;
    damage_type = prototype.damage_type;
    color = prototype.color;
    enemy = prototype.enemy;

    transparency = prototype.transparency;
    glows = prototype.glows;
    fire_rate = prototype.fire_rate;
    can_float_down = prototype.can_float_down;
    is_boss = prototype.is_boss;
    always_drop_bonus = prototype.always_drop_bonus;
    can_swim = prototype.can_swim;

    for (int i = 0; i < SPRITE_MAX_AI; i++) {
        AI[i] = prototype.AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_FRAMES; i++)
        frames[i] = prototype.frames[i];
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = prototype.animations[i].sequence[j];
        animations[i].loop = prototype.animations[i].loop;
        animations[i].frame_count = prototype.animations[i].frame_count;
    }
}

void PK2Sprite_Prototype::New() {
    strcpy(version, LATEST_VERSION_PK2SPRITE);
    strcpy(file, "");
    strcpy(image_file, "");
    strcpy(name, "");
    strcpy(transformation_sprite, "");
    strcpy(bonus_sprite, "");
    strcpy(projectile1_sprite, "");
    strcpy(projectile2_sprite, "");

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], "");
        sounds[sound] = -1;
    }
    sound_frequency = 22050;
    projectile1 = -1;
    projectile2 = -1;
    animation_count = 0;
    key = false;
    bonus = -1;
    bonus_count = 1;
    health = 0;
    obstacle = false;
    blocks_up = true;
    blocks_down = true;
    blocks_right = true;
    blocks_left = true;
    frame_count = 0;
    frame_rate = 0;
    attack1_duration = 60;
    attack2_duration = 60;
    index = 0;
    image_x = 0;
    image_y = 0;
    frame_width = 0;
    frame_height = 0;
    height = 0;
    reload_time = 0;
    width = 0;
    max_jump = 0;
    max_speed = 3;
    transformation = -1;
    weight = 0;
    parallax_coefficient = 0;
    points = 0;
    random_freq = true;
    protection = DAMAGE_NONE;
    shakes = false;
    collides_with_tiles = true;
    destruction = DESTRUCTION_ANIMATION;
    type = TYPE_NOTHING;
    damage = 0;
    damage_type = DAMAGE_HIT;
    color = COLOR_NORMAL;
    enemy = false;

    transparency = false;
    glows = false;
    fire_rate = 0;
    can_float_down = false;
    is_boss = false;
    always_drop_bonus = false;
    can_swim = false;

    for (int i = 0; i < SPRITE_MAX_AI; i++) {
        AI[i] = AI_NONE;
    }
    for (int i = 0; i < SPRITE_MAX_FRAMES; i++) {
        if (frames[i] != 0)
            PisteDraw_Buffer_Destroy(this->frames[i]);

        if (this->mirror_frames[i] != 0)
            PisteDraw_Buffer_Destroy(this->mirror_frames[i]);
    }
    for (int i = 0; i < SPRITE_MAX_FRAMES; i++) {
        frames[i] = 0;
        mirror_frames[i] = 0;
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = 0;
        animations[i].loop = false;
        animations[i].frame_count = 0;
    }
}

int PK2Sprite_Prototype::New_Animation(int index, unsigned char *sequence, bool loop) {
    unsigned char frame_i = 0;
    if (index < SPRITE_MAX_ANIMATIONS) {
        animations[index].frame_count = 0;
        while (frame_i < ANIMATION_MAX_SEQUENCES && sequence[frame_i] != 0) {
            animations[index].sequence[frame_i] = sequence[frame_i];
            animations[index].frame_count++;
            frame_i++;
        }
        animations[index].loop = loop;
    }
    return 0;
}

PK2Sprite_Prototype10 PK2Sprite_Prototype::Get_Prototype10() {
    PK2Sprite_Prototype10 proto;
    strcpy(proto.image_file, image_file);
    strcpy(proto.name, name);
    strcpy(proto.change_sprite, transformation_sprite);
    strcpy(proto.bonus_sprite, bonus_sprite);
    strcpy(proto.projectile1_sprite, projectile1_sprite);
    strcpy(proto.projectile2_sprite, projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], sound_files[sound]);
        proto.sounds[sound] = sounds[sound];
    }
    proto.animation_count = animation_count;
    proto.key = key;
    proto.energy = health;
    proto.obstacle = obstacle;
    proto.frame_count = frame_count;
    proto.frame_rate = frame_rate;
    proto.image_x = image_x;
    proto.image_y = image_y;
    proto.frame_width = frame_width;
    proto.frame_height = frame_height;
    proto.height = height;
    proto.reload_time = reload_time;
    proto.width = width;
    proto.max_jump = max_jump;
    proto.max_speed = (unsigned char) max_speed;
    proto.weight = weight;
    proto.points = points;
    proto.protection = protection;
    proto.destruction = destruction;
    proto.type = type;
    proto.damage = damage;
    proto.color = color;
    proto.enemy = enemy;
    for (int i = 0; i < 5; i++) {
        proto.AI[i] = AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            proto.animations[i].sequence[j] = animations[i].sequence[j];
        proto.animations[i].loop = animations[i].loop;
        proto.animations[i].frame_count = animations[i].frame_count;
    }
    return proto;
}

PK2Sprite_Prototype11 PK2Sprite_Prototype::Get_Prototype11() {
    PK2Sprite_Prototype11 proto;
    strcpy(proto.image_file, image_file);
    strcpy(proto.name, name);
    strcpy(proto.transform_sprite, transformation_sprite);
    strcpy(proto.bonus_sprite, bonus_sprite);
    strcpy(proto.ammo1_sprite, projectile1_sprite);
    strcpy(proto.ammo2_sprite, projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], sound_files[sound]);
        proto.sounds[sound] = sounds[sound];
    }
    proto.animation_count = animation_count;
    proto.key = key;
    proto.bonus_count = bonus_count;
    proto.health = health;
    proto.obstacle = obstacle;
    proto.frame_count = frame_count;
    proto.frame_rate = frame_rate;
    proto.attack1_time = attack1_duration;
    proto.attack2_time = attack2_duration;
    proto.image_x = image_x;
    proto.image_y = image_y;
    proto.frame_width = frame_width;
    proto.frame_height = frame_height;
    proto.height = height;
    proto.reload_time = reload_time;
    proto.width = width;
    proto.max_jump = max_jump;
    proto.max_speed = (unsigned char) max_speed;
    proto.weight = weight;
    proto.background_ratio = parallax_coefficient;
    proto.points = points;
    proto.protection = protection;
    proto.shakes = shakes;
    proto.destruction = destruction;
    proto.type = type;
    proto.damage = damage;
    proto.damage_type = damage_type;
    proto.color = color;
    proto.enemy = enemy;

    for (int i = 0; i < 5; i++) {
        proto.AI[i] = AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            proto.animations[i].sequence[j] = animations[i].sequence[j];
        proto.animations[i].loop = animations[i].loop;
        proto.animations[i].frame_count = animations[i].frame_count;
    }
    return proto;
}

PK2Sprite_Prototype12 PK2Sprite_Prototype::Get_Prototype12() {
    PK2Sprite_Prototype12 proto;
    strcpy(proto.image_file, image_file);
    strcpy(proto.name, name);
    strcpy(proto.transformation_sprite, transformation_sprite);
    strcpy(proto.bonus_sprite, bonus_sprite);
    strcpy(proto.projectile1_sprite, projectile1_sprite);
    strcpy(proto.projectile2_sprite, projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], sound_files[sound]);
        proto.sounds[sound] = sounds[sound];
    }
    proto.sound_frequency = sound_frequency;
    proto.animations_count = animation_count;
    proto.key = key;
    proto.bonus_count = bonus_count;
    proto.health = health;
    proto.obstacle = obstacle;
    proto.obstacle_up = blocks_up;
    proto.obstacle_down = blocks_down;
    proto.obstacle_right = blocks_right;
    proto.obstacle_left = blocks_left;
    proto.frames = frame_count;
    proto.frame_rate = frame_rate;
    proto.attack1_duration = attack1_duration;
    proto.attack2_duration = attack2_duration;
    proto.image_x = image_x;
    proto.image_y = image_y;
    proto.frame_width = frame_width;
    proto.frame_height = frame_height;
    proto.height = height;
    proto.recharge_time = reload_time;
    proto.width = width;
    proto.max_jump = max_jump;
    proto.max_speed = (unsigned char) max_speed;
    proto.weight = weight;
    proto.background_ratio = parallax_coefficient;
    proto.points = points;
    proto.random_frequency = random_freq;
    proto.protection = protection;
    proto.trembling = shakes;
    proto.tile_collision = collides_with_tiles;
    proto.destruction = destruction;
    proto.type = type;
    proto.damage = damage;
    proto.damage_type = damage_type;
    proto.color = color;
    proto.enemy = enemy;
    for (int i = 0; i < 5; i++) {
        proto.AI[i] = AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            proto.animations[i].sequence[j] = animations[i].sequence[j];
        proto.animations[i].loop = animations[i].loop;
        proto.animations[i].frame_count = animations[i].frame_count;
    }
    return proto;
}

PK2Sprite_Prototype13 PK2Sprite_Prototype::Get_Prototype13() {
    PK2Sprite_Prototype13 proto;
    strcpy(proto.image_file, image_file);
    strcpy(proto.name, name);
    strcpy(proto.change_sprite, transformation_sprite);
    strcpy(proto.bonus_sprite, bonus_sprite);
    strcpy(proto.projectile1_sprite, projectile1_sprite);
    strcpy(proto.projectile2_sprite, projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(proto.sound_files[sound], sound_files[sound]);
        proto.sounds[sound] = sounds[sound];
    }
    proto.sound_frequency = sound_frequency;
    proto.number_of_animations = animation_count;
    proto.key = key;
    proto.number_of_bonuses = bonus_count;
    proto.health = health;
    proto.obstacle = obstacle;
    proto.obstacle_up = blocks_up;
    proto.obstacle_down = blocks_down;
    proto.obstacle_right = blocks_right;
    proto.obstacle_left = blocks_left;
    proto.frames = frame_count;
    proto.frame_rate = frame_rate;
    proto.attack1_duration = attack1_duration;
    proto.attack2_duration = attack2_duration;
    proto.image_x = image_x;
    proto.iamge_y = image_y;
    proto.frame_width = frame_width;
    proto.frame_height = frame_height;
    proto.height = height;
    proto.reload_time = reload_time;
    proto.width = width;
    proto.max_jump = max_jump;
    proto.max_speed = max_speed;
    proto.weight = weight;
    proto.backgroundX_multiplier = parallax_coefficient;
    proto.points = points;
    proto.random_frequency = random_freq;
    proto.protection = protection;
    proto.shakes = shakes;
    proto.tile_collision = collides_with_tiles;
    proto.destruction = destruction;
    proto.type = type;
    proto.damage = damage;
    proto.damage_type = damage_type;
    proto.color = color;
    proto.enemy = enemy;

    proto.transparency = transparency;
    proto.glows = glows;
    proto.cooldown = fire_rate;
    proto.glide_ability = can_float_down;
    proto.boss = is_boss;
    proto.always_drops_bonus = always_drop_bonus;
    proto.can_swim = can_swim;
    for (int i = 0; i < 10; i++) {
        proto.AI[i] = AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            proto.animations[i].sequence[j] = animations[i].sequence[j];
        proto.animations[i].loop = animations[i].loop;
        proto.animations[i].frame_count = animations[i].frame_count;
    }
    return proto;
}

void PK2Sprite_Prototype::Set_Prototype10(PK2Sprite_Prototype10 &prototype) {
    strcpy(image_file, prototype.image_file);
    strcpy(name, prototype.name);
    strcpy(transformation_sprite, prototype.change_sprite);
    strcpy(bonus_sprite, prototype.bonus_sprite);
    strcpy(projectile1_sprite, prototype.projectile1_sprite);
    strcpy(projectile2_sprite, prototype.projectile2_sprite);

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], prototype.sound_files[sound]);
        sounds[sound] = prototype.sounds[sound];
    }
    animation_count = prototype.animation_count;
    key = prototype.key;
    health = prototype.energy;
    obstacle = prototype.obstacle;
    frame_count = prototype.frame_count;
    frame_rate = prototype.frame_rate;
    image_x = prototype.image_x;
    image_y = prototype.image_y;
    frame_width = prototype.frame_width;
    frame_height = prototype.frame_height;
    height = prototype.height;
    reload_time = prototype.reload_time;
    width = prototype.width;
    max_jump = prototype.max_jump;
    max_speed = prototype.max_speed;
    weight = prototype.weight;
    points = prototype.points;
    protection = prototype.protection;
    destruction = prototype.destruction;
    type = prototype.type;
    damage = prototype.damage;
    color = prototype.color;
    enemy = prototype.enemy;

    for (int i = 0; i < 5; i++) {
        AI[i] = prototype.AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = prototype.animations[i].sequence[j];
        animations[i].loop = prototype.animations[i].loop;
        animations[i].frame_count = prototype.animations[i].frame_count;
    }
}

void PK2Sprite_Prototype::Set_Prototype11(PK2Sprite_Prototype11 &prototype) {
    strcpy(image_file, prototype.image_file);
    strcpy(name, prototype.name);
    strcpy(transformation_sprite, prototype.transform_sprite);
    strcpy(bonus_sprite, prototype.bonus_sprite);
    strcpy(projectile1_sprite, prototype.ammo1_sprite);
    strcpy(projectile2_sprite, prototype.ammo2_sprite);

    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], prototype.sound_files[sound]);
        sounds[sound] = prototype.sounds[sound];
    }
    animation_count = prototype.animation_count;
    key = prototype.key;
    bonus_count = prototype.bonus_count;
    health = prototype.health;
    obstacle = prototype.obstacle;
    frame_count = prototype.frame_count;
    frame_rate = prototype.frame_rate;
    attack1_duration = prototype.attack1_time;
    attack2_duration = prototype.attack2_time;
    image_x = prototype.image_x;
    image_y = prototype.image_y;
    frame_width = prototype.frame_width;
    frame_height = prototype.frame_height;
    height = prototype.height;
    reload_time = prototype.reload_time;
    width = prototype.width;
    max_jump = prototype.max_jump;
    max_speed = prototype.max_speed;
    weight = prototype.weight;
    parallax_coefficient = prototype.background_ratio;
    points = prototype.points;
    protection = prototype.protection;
    shakes = prototype.shakes;
    destruction = prototype.destruction;
    type = prototype.type;
    damage = prototype.damage;
    damage_type = prototype.damage_type;
    color = prototype.color;
    enemy = prototype.enemy;
    for (int i = 0; i < 5; i++) {
        AI[i] = prototype.AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = prototype.animations[i].sequence[j];
        animations[i].loop = prototype.animations[i].loop;
        animations[i].frame_count = prototype.animations[i].frame_count;
    }
}

void PK2Sprite_Prototype::Set_Prototype12(PK2Sprite_Prototype12 &prototype) {
    strcpy(image_file, prototype.image_file);
    strcpy(name, prototype.name);
    strcpy(transformation_sprite, prototype.transformation_sprite);
    strcpy(bonus_sprite, prototype.bonus_sprite);
    strcpy(projectile1_sprite, prototype.projectile1_sprite);
    strcpy(projectile2_sprite, prototype.projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], prototype.sound_files[sound]);
        sounds[sound] = prototype.sounds[sound];
    }
    sound_frequency = prototype.sound_frequency;
    animation_count = prototype.animations_count;
    key = prototype.key;
    bonus_count = prototype.bonus_count;
    health = prototype.health;
    obstacle = prototype.obstacle;
    blocks_up = prototype.obstacle_up;
    blocks_down = prototype.obstacle_down;
    blocks_right = prototype.obstacle_right;
    blocks_left = prototype.obstacle_left;
    frame_count = prototype.frames;
    frame_rate = prototype.frame_rate;
    attack1_duration = prototype.attack1_duration;
    attack2_duration = prototype.attack2_duration;
    image_x = prototype.image_x;
    image_y = prototype.image_y;
    frame_width = prototype.frame_width;
    frame_height = prototype.frame_height;
    height = prototype.height;
    reload_time = prototype.recharge_time;
    width = prototype.width;
    max_jump = prototype.max_jump;
    max_speed = prototype.max_speed;
    weight = prototype.weight;
    parallax_coefficient = prototype.background_ratio;
    points = prototype.points;
    random_freq = prototype.random_frequency;
    protection = prototype.protection;
    shakes = prototype.trembling;
    collides_with_tiles = prototype.tile_collision;
    destruction = prototype.destruction;
    type = prototype.type;
    damage = prototype.damage;
    damage_type = prototype.damage_type;
    color = prototype.color;
    enemy = prototype.enemy;
    for (int i = 0; i < 5; i++) {
        AI[i] = prototype.AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = prototype.animations[i].sequence[j];
        animations[i].loop = prototype.animations[i].loop;
        animations[i].frame_count = prototype.animations[i].frame_count;
    }
}

void PK2Sprite_Prototype::Set_Prototype13(PK2Sprite_Prototype13 &prototype) {
    strcpy(image_file, prototype.image_file);
    strcpy(name, prototype.name);
    strcpy(transformation_sprite, prototype.change_sprite);
    strcpy(bonus_sprite, prototype.bonus_sprite);
    strcpy(projectile1_sprite, prototype.projectile1_sprite);
    strcpy(projectile2_sprite, prototype.projectile2_sprite);
    for (int sound = 0; sound < MAX_SOUNDS; sound++) {
        strcpy(sound_files[sound], prototype.sound_files[sound]);
        sounds[sound] = prototype.sounds[sound];
    }
    sound_frequency = prototype.sound_frequency;
    animation_count = prototype.number_of_animations;
    key = prototype.key;
    bonus_count = prototype.number_of_bonuses;
    health = prototype.health;
    obstacle = prototype.obstacle;
    blocks_up = prototype.obstacle_up;
    blocks_down = prototype.obstacle_down;
    blocks_right = prototype.obstacle_right;
    blocks_left = prototype.obstacle_left;
    frame_count = prototype.frames;
    frame_rate = prototype.frame_rate;
    attack1_duration = prototype.attack1_duration;
    attack2_duration = prototype.attack2_duration;
    image_x = prototype.image_x;
    image_y = prototype.iamge_y;
    frame_width = prototype.frame_width;
    frame_height = prototype.frame_height;
    height = prototype.height;
    reload_time = prototype.reload_time;
    width = prototype.width;
    max_jump = prototype.max_jump;
    max_speed = prototype.max_speed;
    weight = prototype.weight;
    parallax_coefficient = prototype.backgroundX_multiplier;
    points = prototype.points;
    random_freq = prototype.random_frequency;
    protection = prototype.protection;
    shakes = prototype.shakes;
    collides_with_tiles = prototype.tile_collision;
    destruction = prototype.destruction;
    type = prototype.type;
    damage = prototype.damage;
    damage_type = prototype.damage_type;
    color = prototype.color;
    enemy = prototype.enemy;

    transparency = prototype.transparency;
    glows = prototype.glows;
    fire_rate = prototype.cooldown;
    can_float_down = prototype.glide_ability;
    is_boss = prototype.boss;
    always_drop_bonus = prototype.always_drops_bonus;
    can_swim = prototype.can_swim;
    for (int i = 0; i < 10; i++) {
        AI[i] = prototype.AI[i];
    }
    for (int i = 0; i < SPRITE_MAX_ANIMATIONS; i++) {
        for (int j = 0; j < ANIMATION_MAX_SEQUENCES; j++)
            animations[i].sequence[j] = prototype.animations[i].sequence[j];
        animations[i].loop = prototype.animations[i].loop;
        animations[i].frame_count = prototype.animations[i].frame_count;
    }
}

int PK2Sprite_Prototype::Load(char *path, char *file_name) {
    this->New();
    char image[_MAX_PATH];
    strcpy(image, "");
    strcpy(image, path);
    // Let's load sprite file
    if (strcmp(path, "") != 0)
        strcat(path, file_name);
    else {
        //strcpy(path,"\\");
        strcpy(path, file_name);
    }
    ifstream *file = new ifstream(path, ios::binary); // | ios::nocreate);
    char version[4];
    if (file->fail()) {
        delete (file);
        return 1;
    }
    file->read((char *) version, 4);
    if (strcmp(version, "1.0") == 0) {
        this->New();
        PK2Sprite_Prototype10 proto;
        file->read((char *) &proto, sizeof(proto));
        this->Set_Prototype10(proto);
        strcpy(this->version, version);
        strcpy(this->file, file_name);
    }
    if (strcmp(version, "1.1") == 0) {
        this->New();
        PK2Sprite_Prototype11 proto;
        file->read((char *) &proto, sizeof(proto));
        this->Set_Prototype11(proto);
        strcpy(this->version, version);
        strcpy(this->file, file_name);
    }
    if (strcmp(version, "1.2") == 0) {
        this->New();
        PK2Sprite_Prototype12 proto;
        file->read((char *) &proto, sizeof(proto));
        this->Set_Prototype12(proto);
        strcpy(this->version, version);
        strcpy(this->file, file_name);
    }
    if (strcmp(version, "1.3") == 0) {
        this->New();
        PK2Sprite_Prototype13 proto;
        file->read((char *) &proto, sizeof(proto));
        this->Set_Prototype13(proto);
        strcpy(this->version, version);
        strcpy(this->file, file_name);
    }
    //file->read ((char *)this, sizeof (*this));
    if (file->fail()) {
        delete (file);
        return 1;
    }
    delete (file);
    // Let's load the image with the sprite graphics
    strcat(image, image_file);
    int buffer = PisteDraw_Buffer_Create(640, 480, false, 255);
    if (PisteDraw_Image_Load(buffer, image, false) == PD_ERROR)
        return 1;
    if (this->color != COLOR_NORMAL) {
        unsigned char *buffer = NULL;
        unsigned long width;
        unsigned char color;
        PisteDraw_Draw_Begin(buffer, *&buffer, width);
        for (int x = 0; x < 640; x++)
            for (int y = 0; y < 480; y++)
                if ((color = buffer[x + y * width]) != 255) {
                    color %= 32;
                    color += this->color;
                    buffer[x + y * width] = color;
                }
        PisteDraw_Draw_End(buffer);
    }
    int frame_i = 0, frame_x = image_x, frame_y = image_y;
    for (frame_i = 0; frame_i < frame_count; frame_i++) {
        frames[frame_i] = PisteDraw_Buffer_Create(frame_width, frame_height, true, 255);
        mirror_frames[frame_i] = PisteDraw_Buffer_Create(frame_width, frame_height, true, 255);
        PisteDraw_Buffer_Clear(frames[frame_i], 255);
        PisteDraw_Buffer_Clear(mirror_frames[frame_i], 255);
        if (frame_x + frame_width > 640) {
            frame_y += this->frame_height + 3;
            frame_x = image_x;
        }
        PisteDraw_Buffer_Flip_Fast(buffer, frames[frame_i], 0, 0, frame_x, frame_y, frame_x + frame_width, frame_y + frame_height);
        PisteDraw_Buffer_Flip(frames[frame_i], mirror_frames[frame_i], 0, 0, true, false);
        frame_x += this->frame_width + 3;
    }
    PisteDraw_Buffer_Destroy(buffer);
    return 0;
}

void PK2Sprite_Prototype::Save(char *file_name) {
    strcpy(this->file, file_name);
    PK2Sprite_Prototype13 proto = Get_Prototype13();
    //strcpy(proto.version, LATEST_VERSION_PK2SPRITE);
    ofstream *file = new ofstream(file_name, ios::binary);
    file->write(LATEST_VERSION_PK2SPRITE, 4);
    file->write((char *) &proto, sizeof(proto));
    delete (file);
}

/*
int PK2Sprite_Prototype::Load(char *filename) {
	FILE *file;
	if ((file = fopen(filename, "r")) == NULL) {
		return 1;
	}
	char version[4] = "\0";
	fread(version, sizeof(this->version), 1, file);
	fclose(file);
	// check the version number if needed
	if ((file = fopen(filename, "r")) == NULL) {
		return(1);
	}
	fread(this, sizeof(PK2Sprite_Prototype), 1, file);
	fclose(file);
	int buffer = PisteDraw_Buffer_Uusi(640,480,false,255);
	PisteDraw_Image_Load(buffer,image_file,false);
	int frame_i = 0,
		frame_x = image_x;
	for (frame_i=0; frame_i<frame_count; frame_i++) {
		frames[frame_i] = PisteDraw_Buffer_Uusi(frame_width,frame_height,true,255);
		mirror_frames[frame_i] = PisteDraw_Buffer_Create(frame_width,frame_height,true,255);
		PisteDraw_Buffer_Clear(frames[frame_i],255);
		PisteDraw_Buffer_Clear(mirror_frames[frame_i],255);
		PisteDraw_Buffer_Flip_Fast(buffer,frames[frame_i],0,0,frame_x,image_y,frame_x + frame_width,image_y  + frame_height);
		PisteDraw_Buffer_Flip(frames[frame_i], mirror_frames[frame_i], 0, 0, true, false);
		frame_x += 35;
	}
	PisteDraw_Buffer_Destroy(buffer);
	return 0;
}

int PK2Sprite_Prototype::Save(char *filename) {
	FILE *file;
	if ((file = fopen(filename, "w")) == NULL) {
		return(1);
	}
	fwrite(this, sizeof(PK2Sprite_Prototype), 1, file);
	fclose(file);
	return 0;
}
*/

int PK2Sprite_Prototype::Draw(int x, int y, int frame) {
    PisteDraw_Buffer_Flip_Fast(frames[frame], PD_BACKBUFFER, x, y);
    return 0;
}

bool PK2Sprite_Prototype::Has_AI(int ai) {
    for (int i = 0; i < SPRITE_MAX_AI; i++) {
        if (AI[i] == ai)
            return true;
    }
    return false;
}

/* -------- PK2Sprite  ------------------------------------------------------------------ */

PK2Sprite::PK2Sprite() {
    this->type = NULL;
    this->player = 0;
    this->is_hidden = true;
    this->x = 0;
    this->y = 0;
    this->start_x = 0;
    this->start_y = 0;
    this->movement_x = 0;
    this->movement_y = 0;
    this->jump_timer = 0;
    this->is_crouching = false;
    this->health = 0;
    this->initial_weight = 0;
    this->weight = 0;
    this->total_weight = 0;
    this->flip_horizontal = false;
    this->flip_vertical = false;
    this->animation_index = ANIMATION_IDLE;
    this->can_move_down = true;
    this->can_move_up = true;
    this->can_move_right = true;
    this->can_move_left = true;
    this->edge_right = false;
    this->edge_left = false;
    this->frame_time = 0;
    this->sequence_index = 0;
    this->hit_counter = 0;
    this->charge_time = 0;
    this->attack_timer_1 = 0;
    this->attack_timer_2 = 0;
    this->in_water = false;
    this->hiding = false;
    this->damage_received = 0;
    this->is_enemy = false;
    this->projectile_1 = -1;
    this->projectile_2 = -1;
    this->last_seen_player_x = -1;
    this->last_seen_player_y = -1;
    this->timer = 0;
    this->transformation_timer = 0;
}

PK2Sprite::PK2Sprite(PK2Sprite_Prototype *type, int player, bool is_hidden, double x, double y) {
    if (type) {
        this->type = type;
        this->player = player;
        this->is_hidden = is_hidden;
        this->x = x;
        this->y = y;
        this->start_x = x;
        this->start_y = y;
        this->movement_x = 0;
        this->movement_y = 0;
        this->jump_timer = 0;
        this->health = type->health;
        this->initial_weight = type->weight;
        this->weight = this->initial_weight;
        this->total_weight = 0;
        this->is_crouching = false;
        this->flip_horizontal = false;
        this->flip_vertical = false;
        this->animation_index = ANIMATION_IDLE;
        this->can_move_down = true;
        this->can_move_up = true;
        this->can_move_right = true;
        this->can_move_left = true;
        this->edge_right = false;
        this->edge_left = false;
        this->frame_time = 0;
        this->sequence_index = 0;
        this->hit_counter = 0;
        this->charge_time = 0;
        this->attack_timer_1 = 0;
        this->attack_timer_2 = 0;
        this->in_water = false;
        this->hiding = false;
        this->damage_received = 0;
        this->is_enemy = type->enemy;
        this->projectile_1 = type->projectile1;
        this->projectile_2 = type->projectile2;
        this->last_seen_player_x = -1;
        this->last_seen_player_y = -1;
        this->timer = 0;
        this->transformation_timer = 0;
    }
}

PK2Sprite::~PK2Sprite() {}

bool PK2Sprite::Has_AI(int ai) {
    for (int i = 0; i < SPRITE_MAX_AI; i++) {
        if (type->AI[i] == ai)
            return true;
    }
    return false;
}

int PK2Sprite::Change_Animation(int anim_index, bool reset) {
    if (anim_index != animation_index) {
        if (reset)
            sequence_index = 0;
        animation_index = anim_index;
    }
    return 0;
}

int PK2Sprite::Animate() {
    int frame = 0;
    switch (type->AI[0]) {
        case AI_CHICKEN:
            Animation_Chicken();
            break;
        case AI_SMALL_CHICKEN:
            Animation_Chicken();
            break;
        case AI_BONUS:
            Animation_Bonus();
            break;
        case AI_EGG:
            Animation_Egg();
            break;
        case AI_PROJECTILE:
            Animation_Projectile();
            break;
        case AI_JUMPING:
            Animation_Chicken();
            break;
        case AI_BASIC:
            Animation_Default();
            break;
        case AI_TELEPORT:
            Animation_Default();
            break;
        default:
            break;
    }

    PK2SpriteAnimation &animation = type->animations[animation_index];
    if (sequence_index >= animation.frame_count)
        sequence_index = 0;
    frame = animation.sequence[sequence_index] - 1;
    // Calculate how much of the current frame is still being shown
    if (frame_time < type->frame_rate)
        frame_time++;
        // If duration has run out, switch to the next frame from the current animation
    else {
        frame_time = 0;
        // Are there any more frame_count in the animation?
        if (sequence_index < animation.frame_count - 1)
            sequence_index++;
            // If not and the animation is set to loop, start the animation from the beginning.
        else {
            if (animation.loop)
                sequence_index = 0;
        }
    }
    if (frame > type->frame_count)
        frame = type->frame_count;
    return frame;
}

int PK2Sprite::Draw(int camera_x, int camera_y) {
    // Create auxiliary variables
    int l = (int) type->frame_width / 2,//width
    h = (int) type->frame_height / 2, x = (int) this->x - (camera_x), y = (int) this->y - (camera_y), frame = 0;
    frame = Animate();
    if (type->shakes) {
        x += rand() % 2 - rand() % 2;
        y += rand() % 2 - rand() % 2;
    }
    if (flip_horizontal) {
        if (!flip_vertical)
            PisteDraw_Buffer_Flip_Fast(type->mirror_frames[frame], PD_BACKBUFFER, x - l - 1, y - h);
        else
            PisteDraw_Buffer_Flip(type->mirror_frames[frame], PD_BACKBUFFER, x - l - 1, y - h, false, true);
    } else {
        if (!flip_vertical)
            PisteDraw_Buffer_Flip_Fast(type->frames[frame], PD_BACKBUFFER, x - l - 1, y - h);
        else
            PisteDraw_Buffer_Flip(type->frames[frame], PD_BACKBUFFER, x - l - 1, y - h, false, true);
    }
    return 0;
}

int PK2Sprite::AI_Default() {
    if (x < 10) {
        x = 10;
        can_move_left = false;
    }
    if (x > 8192) {
        x = 8192;
        can_move_right = false;
    }
    if (y > 9920) {
        y = 9920;
    }
    if (y < -32) {
        y = -32;
    }
    if (movement_x < 0)
        flip_horizontal = true;
    if (movement_x > 0)
        flip_horizontal = false;
    timer++;
    if (timer > 31320) // divisible by 360
        timer = 0;
    return 0;
}

int PK2Sprite::AI_Turns_From_Horizontal_Obstacle() {
    if (health > 0) {
        if (!can_move_right) {
            movement_x = this->type->max_speed / -3.5;
            return 1;
        }
        if (!can_move_left) {
            movement_x = this->type->max_speed / 3.5;
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Turns_From_Vertical_Obstacle() {
    if (health > 0) {
        if (!can_move_down) {
            movement_y = this->type->max_speed / -3.5;
            return 1;
        }

        if (!can_move_up) {
            movement_y = this->type->max_speed / 3.5;
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Climber() {
    if (health > 0) {
        if (!can_move_down && can_move_left) {
            movement_y = 0;
            movement_x = this->type->max_speed / -3.5;
            //return 1;
        }
        if (!can_move_up && can_move_right) {
            movement_y = 0;
            movement_x = this->type->max_speed / 3.5;
            //movement_y = this->type->max_speed / 3.5;
            //return 1;
        }
        if (!can_move_right && can_move_down) {
            movement_x = 0;
            movement_y = this->type->max_speed / 3.5;
            //return 1;
        }
        if (!can_move_left && can_move_up) {
            movement_x = 0;
            movement_y = this->type->max_speed / -3.5;
            //return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Climber2() {
    if (health > 0) {
        if (can_move_left && can_move_right && can_move_up && can_move_down) {
            if (movement_x < 0) {
                movement_y = this->type->max_speed / 3.5;
                //movement_x = 0;
            } else if (movement_x > 0) {
                movement_y = this->type->max_speed / -3.5;
                //movement_x = 0;
            } else if (movement_y < 0) {
                movement_x = this->type->max_speed / -3.5;
                //movement_y = 0;
            } else if (movement_y > 0) {
                movement_x = this->type->max_speed / 3.5;
                //movement_y = 0;
            }
/*
			if (movement_x != 0)
				movement_y = 0;
*/
            if (movement_y != 0)
                movement_x = 0;
        }
    }
    return 0;
}

int PK2Sprite::AI_Avoids_Pit() {
    double max = type->max_speed / 3.5;
    if (health > 0) {
        if (this->edge_right && this->movement_x > -max) {
            this->movement_x -= 0.13;
        }
        if (this->edge_left && this->movement_x < max) {
            this->movement_x += 0.13;
        }
/*
		if (this->edge_right && this->movement_x > 0)
		{
			this->movement_x = this->movement_x * -1;
			flip_horizontal = true;
		}
		if (this->edge_left && this->movement_x < 0)
		{
			this->movement_x = this->movement_x * -1;
			flip_horizontal = false;
		}
*/
    }
    return 0;
}

int PK2Sprite::AI_Random_Jump() {
    if (health > 0) {
        if (rand() % 150 == 10 && movement_y == 0 && jump_timer == 0 && can_move_up) {
            jump_timer = 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Frog1() {
    if (health > 0) {
        if (timer % 100 == 0 && jump_timer == 0 && can_move_up) {
            jump_timer = 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Frog2() {
    if (health > 0) {
        if (timer % 100 == 0 && can_move_up) {
            jump_timer = 1;
        }
        if (jump_timer > 0) {
            if (!flip_horizontal)
                movement_x = this->type->max_speed / 3.5;
            else
                movement_x = this->type->max_speed / -3.5;
        }
    }
    return 0;
}

int PK2Sprite::AI_Random_Horizontal_Direction_Change() {
    if (health > 0) {
        if (rand() % 150 == 1) {
            int max = (int) type->max_speed / 4;
            while (movement_x == 0 && max > 0)
                movement_x = rand() % max + 1 - rand() % max + 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Random_Turn() {
    if (health > 0) {
        if (timer % 400 == 1 && movement_x == 0) {
            flip_horizontal = !flip_horizontal;
        }
    }
    return 0;
}

int PK2Sprite::AI_Turns_If_Hit() {
    if (hit_counter == DAMAGE_TIME/* damage_received*/ > 0 && health > 0) {
        if (movement_x != 0)
            movement_x = -movement_x;
        flip_horizontal = !flip_horizontal;
    }
    return 0;
}

int PK2Sprite::AI_Random_Vertical_Horizontal_Move() {
    if (health > 0) {
        if (rand() % 150 == 1 || timer == 1)
            if ((int) movement_x == 0 || (int) movement_y == 0) {
                int max = (int) type->max_speed;
                if (max != 0) {
                    while (movement_x == 0)
                        movement_x = rand() % (max + 1) - rand() % (max + 1);
                    while (movement_y == 0)
                        movement_y = rand() % (max + 1) - rand() % (max + 1);
                    //movement_x /= 3.0;
                    //movement_y /= 3.0;
                }
            }
    }
    return 0;
}

int PK2Sprite::AI_Follows_Player(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        double max = type->max_speed / 3.5;
        if (movement_x > -max && x > player.x) {
            movement_x -= 0.1;
        }
        if (movement_x < max && x < player.x) {
            movement_x += 0.1;
        }
        last_seen_player_x = (int) (player.x + player.movement_x);
        last_seen_player_y = (int) (player.y + player.movement_y);
        if (type->max_speed == 0) {
            if (player.x < x)
                flip_horizontal = true;
            else
                flip_horizontal = false;
        }
    }
    return 0;
}

int PK2Sprite::AI_Flees_From_Player_If_Seen(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        if ((player.x < x && flip_horizontal && !player.flip_horizontal) || (player.x > x && !flip_horizontal && player.flip_horizontal))
            if ((player.x - x < 300 && player.x - x > -300) &&
                (player.y - y < type->height && player.y - y > -type->height)) {
                double max = type->max_speed / 2.5;
                if (x > player.x) {
                    movement_x = max;
                    flip_horizontal = false;
                }
                if (x < player.x) {
                    movement_x = max * -1;
                    flip_horizontal = true;
                }
            }
    }
    return 0;
}

int PK2Sprite::AI_Follows_Player_If_Seen(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        double max = type->max_speed / 3.5;
        if (last_seen_player_x != -1) {
            if (movement_x > -max && x > last_seen_player_x)
                movement_x -= 0.1;
            if (movement_x < max && x < last_seen_player_x)
                movement_x += 0.1;
        }
        if ((player.x < x && flip_horizontal) || (player.x > x && !flip_horizontal))
            if ((player.x - x < 300 && player.x - x > -300) &&
                (player.y - y < type->height && player.y - y > -type->height)) {
                last_seen_player_x = (int) (player.x + player.movement_x);
                last_seen_player_y = (int) (player.y + player.movement_y);
            } else {
                last_seen_player_x = -1;
                last_seen_player_y = -1;
            }
    }
    return 0;
}

// TODO: =>
int PK2Sprite::AI_Chases_Player(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        double max = type->max_speed / 3.5;
        if (last_seen_player_x != -1) {
            if (movement_x > -max && x > last_seen_player_x)
                movement_x -= 0.1;
            if (movement_x < max && x < last_seen_player_x)
                movement_x += 0.1;
            if (x - last_seen_player_x > -8 && x - last_seen_player_x < 8) // onko sprite saavuttanut pelaajan viime sijainnin
                last_seen_player_x = -1;
        }
        if (last_seen_player_y != -1 && weight == 0) {
            if (movement_y > -max && y > last_seen_player_y)
                movement_y -= 0.1;
            if (movement_y < max && y < last_seen_player_y)
                movement_y += 0.1;
            if (y - last_seen_player_y > -8 && y - last_seen_player_y < 8) // onko sprite saavuttanut pelaajan viime sijainnin
                last_seen_player_y = -1;
        }
        if (((player.x < x && !flip_horizontal) || (player.x > x && flip_horizontal)) && player.attack_timer_2 != player.type->reload_time)
            return 0;
        if ((player.x - x < 300 && player.x - x > -300) &&
            (player.y - y < type->height && player.y - y > -type->height)) {
            last_seen_player_x = (int) (player.x + player.movement_x);
            last_seen_player_y = (int) (player.y + player.movement_y);
        }
    }
    return 0;
}

int PK2Sprite::AI_Follows_Player_Vertical_Horizontal(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        double max = type->max_speed / 3.5;
        if (movement_x > -max && x > player.x) {
            movement_x -= 0.1;
        }
        if (movement_x < max && x < player.x) {
            movement_x += 0.1;
        }
        if (movement_y > -max && y > player.y) {
            movement_y -= 0.4;
        }
        if (movement_y < max && y < player.y) {
            movement_y += 0.4;
        }
        last_seen_player_x = (int) (player.x + player.movement_x);
        last_seen_player_y = (int) (player.y + player.movement_y);
        if (type->max_speed == 0) {
            if (player.x < x)
                flip_horizontal = true;
            else
                flip_horizontal = false;
        }
    }
    return 0;
}

int PK2Sprite::AI_Follows_Player_If_Seen_Vertical_Horizontal(PK2Sprite &player) {
    if (health > 0 && player.health > 0) {
        double max = type->max_speed / 3.5;
        if (last_seen_player_x != -1) {
            if (movement_x > -max && x > last_seen_player_x)
                movement_x -= 0.1;
            if (movement_x < max && x < last_seen_player_x)
                movement_x += 0.1;
            if (movement_y > -max && y > last_seen_player_y)
                movement_y -= 0.4;
            if (movement_y < max && y < last_seen_player_y)
                movement_y += 0.4;
        }
        if ((player.x < x && flip_horizontal) || (player.x > x && !flip_horizontal))
            if ((player.x - x < 300 && player.x - x > -300) && (player.y - y < 80 && player.y - y > -80)) {
                last_seen_player_x = (int) (player.x + player.movement_x);
                last_seen_player_y = (int) (player.y + player.movement_y);
            } else {
                last_seen_player_x = -1;
                last_seen_player_y = -1;
            }
    }
    return 0;
}

int PK2Sprite::AI_Change_If_Energy_Less_Than_2(PK2Sprite_Prototype &change) {
    if (health < 2 && change.index != type->index) {
        type = &change;
        initial_weight = type->weight;
        //projectile_1 = type->projectile_1;
        //projectile_2 = type->projectile_2;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Change_If_Energy_More_Than_1(PK2Sprite_Prototype &change) {
    if (health > 1 && change.index != type->index) {
        type = &change;
        initial_weight = type->weight;
        //projectile_1 = type->projectile_1;
        //projectile_2 = type->projectile_2;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Change_Timer(PK2Sprite_Prototype &change) {
    if (health > 0 && change.index != type->index) {
        if (transformation_timer/*charge_time*/ == 0)
            transformation_timer/*charge_time*/ = type->reload_time;
        if (transformation_timer/*charge_time*/ == 1) {
            type = &change;
            initial_weight = type->weight;
            projectile_1 = type->projectile1;
            projectile_2 = type->projectile2;
            animation_index = -1;
            Change_Animation(ANIMATION_IDLE, true);
        }
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Change_If_Hit(PK2Sprite_Prototype &change) {
    if (health > 0 && change.index != type->index) {
        if (damage_received > 0) {
            type = &change;
            initial_weight = type->weight;
            projectile_1 = type->projectile1;
            projectile_2 = type->projectile2;
            animation_index = -1;
            Change_Animation(ANIMATION_IDLE, true);
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Dies_If_Parent_Dies(PK2Sprite *sprites) {
    if (parent_sprite > -1) {
        if (sprites[parent_sprite].health < 1 && health > 0) {
            damage_received = health;
            this->damage_type_received = DAMAGE_ALL;
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Attack_1_If_Hit() {
    if (damage_received > 0 && health > 0) {
        this->attack_timer_1 = this->type->attack1_duration;
        this->charge_time = 0;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Attack_2_If_Hit() {
    if (damage_received > 0 && health > 0) {
        this->attack_timer_2 = this->type->attack2_duration;
        this->charge_time = 0;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Attack_1_Nonstop() {
    if (this->charge_time == 0 && health > 0) {
        this->attack_timer_1 = this->type->attack1_duration;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Attack_2_Nonstop() {
    if (this->charge_time == 0 && health > 0) {
        this->attack_timer_2 = this->type->attack2_duration;
        return 1;
    }
    return 0;
}

int PK2Sprite::AI_Attack_1_If_Player_In_Front(PK2Sprite &player) {
    if (health > 0 && hit_counter == 0 && player.health > 0) {
        if ((player.x - x < 200 && player.x - x > -200) &&
            (player.y - y < type->height && player.y - y > -type->height)) {
            if ((player.x < x && flip_horizontal) || (player.x > x && !flip_horizontal)) {
                this->attack_timer_1 = this->type->attack1_duration;
                return 1;
            }
        }
    }
    return 0;
}

int PK2Sprite::AI_Attack_2_If_Player_In_Front(PK2Sprite &player) {
    if (health > 0 && hit_counter == 0 && player.health > 0) {
        if ((player.x - x < 200 && player.x - x > -200) &&
            (player.y - y < type->height && player.y - y > -type->height)) {
            if ((player.x < x && flip_horizontal) || (player.x > x && !flip_horizontal)) {
                this->attack_timer_2 = this->type->attack2_duration;
                return 1;
            }
        }
    }
    return 0;
}

int PK2Sprite::AI_Attack_1_If_Player_Below(PK2Sprite &player) {
    if (health > 0 && hit_counter == 0 && player.health > 0) {
        if ((player.x - x < type->width && player.x - x > -type->width) &&
            (player.y > y && player.y - y < 350)) {
            this->attack_timer_1 = this->type->attack2_duration;
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Jump_If_Player_Above(PK2Sprite &player) {
    if (health > 0 && jump_timer == 0 && player.health > 0) {
        if ((player.x - x < type->width && player.x - x > -type->width) && (player.y < y && y - player.y < 350)) {
            jump_timer = 1;
            return 1;
        }
    }
    return 0;
}

int PK2Sprite::AI_NonStop() {
    if (health > 0) {
        double max = type->max_speed / 3.5;
        if (flip_horizontal) {
            if (movement_x > -max)
                movement_x -= 0.1;
        } else {
            if (movement_x < max)
                movement_x += 0.1;
        }
    }
    return 0;
}

int PK2Sprite::AI_Friction_Affects() {
    if (health > 0) {
        if (!can_move_down)
            movement_x /= 1.07;
        else
            movement_x /= 1.02;
    }
    return 0;
}

int PK2Sprite::AI_Hides() {
    if (health > 0 && hiding) {
        movement_x /= 1.02;
        is_crouching = true;
    }
    return 0;
}

int PK2Sprite::AI_Returns_To_Start_X() {
    if (health < 1 || last_seen_player_x != -1)
        return 0;
    double max = type->max_speed / 3.5;
    if (x < start_x - 16 && movement_x < max)
        movement_x += 0.05;
    if (x > start_x + 16 && movement_x > -max)
        movement_x -= 0.05;
    return 0;
}

int PK2Sprite::AI_Returns_To_Start_Y() {
    if (health > 0 && last_seen_player_x == -1) {
        double max = type->max_speed / 3.5;
        if (y < start_y - 16 && movement_y < max)
            movement_y += 0.04;
        if (y > start_y + 16 && movement_y > -max)
            movement_y -= 0.04;
    }
    return 0;
}

int PK2Sprite::AI_Falls_From_Story(int story) {
    if (health > 0 && story > 0) {
        initial_weight = 0.5;
    }
    return 0;
}

int PK2Sprite::AI_Takes_Damage_From_Water() {
    if (health > 0)
        if (this->in_water)
            damage_received++;
    return 0;
}

int PK2Sprite::AI_Kill_All() {
    if (health > 0)
        this->is_enemy = !this->is_enemy;
    return 0;
}

int PK2Sprite::AI_Jumper() {
    if (x < 10) {
        x = 10;
    }
    if (y > 9920) {
        y = 9920;
    }
    if (health > 0) {
        if (!can_move_down && movement_y == 0 && jump_timer == 0) {
            jump_timer = 1;
        }
    }
    if (movement_x < 0)
        flip_horizontal = true;
    if (movement_x > 0)
        flip_horizontal = false;
    return 0;
}

int PK2Sprite::AI_Moves_X(double movement) {
    if (health > 0)
        this->x = this->start_x + movement;
    return 0;
}

int PK2Sprite::AI_Moves_Y(double movement) {
    if (health > 0) {
        this->y = this->start_y + movement;
    }
    return 0;
}

int PK2Sprite::AI_Falls_If_Switch_Pressed(int switch_id) {
    if (switch_id > 0) {
        initial_weight = 1.5;
    }
    return 0;
}

int PK2Sprite::AI_Moves_If_Switch_Pressed(int switch_id, int ak, int bk) {
    if (switch_id > 0) {
        if (movement_x == 0 && ak != 0) {
            movement_x = type->max_speed / 3.5 * ak; // ak = -1 / 1
        }
        if (movement_y == 0 && bk != 0)
            movement_y = type->max_speed / 3.5 * bk; // bk = -1 / 1
    }
    flip_horizontal = false;
    return 0;
}

bool PK2Sprite::AI_Info(PK2Sprite &player) {
    if ((player.x - x < 10 && player.x - x > -10) &&
        (player.y - y < type->height && player.y - y > -type->height)) {
        return true;
    }
    return false;
}

int PK2Sprite::AI_Chicken() {
    if (x < 10) {
        x = 10;
    }
    if (y > 9920) {
        y = 9920;
    }
    if (health > 0) {
        if (rand() % 50 == 10 && movement_x != 0)
            movement_x /= 1.1;
        if (rand() % 150 == 10 && movement_y == 0 && jump_timer == 0 && can_move_up) {
            jump_timer = 1;
            while (movement_x == 0)
                movement_x = rand() % 2 - rand() % 2;
        }
        if (rand() % 20 == 1 && movement_y == 0 && jump_timer == 0 && !can_move_right && !flip_horizontal) {
            jump_timer = 1;
            while (movement_x == 0)
                movement_x = rand() % 2;
        }
        if (rand() % 20 == 1 && movement_y == 0 && jump_timer == 0 && !can_move_left && flip_horizontal) {
            jump_timer = 1;
            while (movement_x == 0)
                movement_x = rand() % 2 * -1;
        }
        if (rand() % 200 == 10)
            movement_x = rand() % 2 - rand() % 2;
        if (jump_timer == type->max_jump && movement_x == 0) {
            while (movement_x == 0)
                movement_x = rand() % 2 - rand() % 2;
        }
/*      // edge check
		if (rand()%100 == 2)
			movement_x = rand()%2-rand()%2;

		if (edge_left && movement_x < 0)
			movement_x = 0;

		if (edge_right && movement_x > 0)
			movement_x = 0;
*/
        if (movement_x < 0)
            flip_horizontal = true;
        if (movement_x > 0)
            flip_horizontal = false;
    }
    return 0;
}

int PK2Sprite::AI_Bonus() {
    if (x < 10) {
        x = 10;
    }
    if (y > 9920) {
        y = 9920;
    }
    return 0;
}

int PK2Sprite::AI_Egg() {
    if (x < 10) {
        x = 10;
    }
    if (y > 9920) {
        y = 9920;
    }
    if (!can_move_down)
        health = 0;
    //movement_x /= 1.01;
    if (health == 0 && charge_time == 0)
        charge_time = type->reload_time;
    if (this->charge_time == 1)
        this->is_hidden = true;
    return 0;
}

int PK2Sprite::AI_Projectile() {
    if (x < 10) {
        x = 10;
    }
    if (y > 9920) {
        y = 9920;
    }
    if (movement_x < 0)
        flip_horizontal = true;
    if (movement_x > 0)
        flip_horizontal = false;
    if (this->charge_time == 0)
        charge_time = this->type->reload_time;
    if (this->charge_time == 1) {
        this->damage_received = this->type->health;
        this->damage_type_received = DAMAGE_ALL;
    }
    if (health < 1)
        is_hidden = true;
    return 0;
}

int PK2Sprite::AI_Bomb() {
    if (this->charge_time == 0)
        charge_time = this->type->reload_time;
    if (this->charge_time == 1) {
        this->damage_received = this->health;
        this->damage_type_received = DAMAGE_ALL;
    }
    return 0;
}

int PK2Sprite::AI_Teleport(int index, PK2Sprite *sprites, int max, PK2Sprite &player) {
    int transfer = 0;
    if (health > 0 && charge_time == 0 && attack_timer_1 == 0) {
        if (player.x <= x + type->width / 2 && player.x >= x - type->width / 2 &&
            player.y <= y + type->height / 2 && player.y >= y - type->height / 2) {
            int gates[1200];
            int gate_index = 0;
            int number_of_gates = 0;
            // initialize the gates array, where the indices of all teleport sprites are collected
            for (int i = 0; i < max; i++)
                gates[i] = -1;
            // find SAME TYPE teleporters
            for (int i = 0; i < max; i++)
                if (sprites[i].type != NULL)
                    if (sprites[i].type->type == TYPE_TELEPORTER && type->index == sprites[i].type->index) {
                        gates[gate_index] = i;
                        gate_index++;
                        number_of_gates++;
                    }
            // if no same type is found...
            if (number_of_gates == 0) {
                // ...find ALL teleporters
                gate_index = 0;
                number_of_gates = 0;
                for (int i = 0; i < max; i++)
                    if (sprites[i].type != NULL)
                        if (sprites[i].type->type == TYPE_TELEPORTER) {
                            gates[gate_index] = i;
                            gate_index++;
                            number_of_gates++;
                        }
            }
            // if still no teleporter is found (excluding the teleporter itself), exit.
            if (number_of_gates <= 1)
                return 0;
            if (number_of_gates > 1119) //599
                number_of_gates = 1119; //599
            // select target gate randomly
            int id = gates[rand() % number_of_gates];
            while (id == index || id == -1)
                id = gates[rand() % number_of_gates];
            if (sprites[id].type != NULL) {
                player.x = sprites[id].x;
                player.y = sprites[id].y;
                //charge_time = type->reload_time;
                //attack_timer_1 = type->attack1_duration;
                //sprites[id].charge_time = sprites[id].type->reload_time;
                sprites[id].attack_timer_1 = sprites[id].type->attack1_duration;
                charge_time = 0;
                sprites[id].charge_time = 0;
                transfer = 1;
            }
        }
    }
    return transfer;
}

/*
int PK2Sprite::AI_Teleport(int index, PK2Sprite *sprites, int max, PK2Sprite &player) {
	int transfer = 0;
	if (health > 0 && charge_time == 0 && attack_timer_1 == 0) {
		unsigned char probability = 100; // probability to be selected as the target teleporter
		if (player.x <= x + type->width /2 && player.x >= x - type->width /2 && player.y <= y + type->height/2 && player.y >= y - type->height/2 ) {
			for (int i=0;i<max;i++) {
				if (sprites[i].type != NULL) {
					if (sprites[i].type->type == TYPE_TELEPORTER) {
						if (type->index == sprites[i].type->index && index != i) {
							if (rand()%100 < probability) {
								player.x = sprites[i].x;
								player.y = sprites[i].y;
								//charge_time = type->reload_time;
								attack_timer_1 = type->attack1_duration;
								//sprites[i].charge_time = sprites[i].type->reload_time;
								sprites[i].attack_timer_1 = sprites[i].type->attack1_duration;
								charge_time = 0;
								sprites[i].charge_time = 0;
								transfer = 1;
							}
							probability /= 2;
						}
					}
				}
			}
		}
	}
	return transfer;
}
*/
int PK2Sprite::Animation_Default() {
    int new_animation = -1;
    bool initialize = false;
    if (health < 1 && !can_move_down) {
        new_animation = ANIMATION_DEATH;
        initialize = true;
    } else {
        if (movement_x > -0.2 && movement_x < 0.2 && movement_y == 0 && jump_timer <= 0) {
            new_animation = ANIMATION_IDLE;
            initialize = true;
        }
        if ((movement_x < -0.2 || movement_x > 0.2) && jump_timer <= 0) {
            new_animation = ANIMATION_WALK;
            initialize = false;
        }
        if (movement_y < 0) {//-0.3
            new_animation = ANIMATION_JUMP_UP;
            initialize = false;
        }
        if ((jump_timer > type->max_jump || movement_y > 1.5) && can_move_down) {
            new_animation = ANIMATION_JUMP_DOWN;
            initialize = false;
        }
        if (is_crouching) {
            new_animation = ANIMATION_CROUCH;
            initialize = true;
        }
        if (attack_timer_1 > 0) {
            new_animation = ANIMATION_ATTACK1;
            initialize = true;
        }
        if (attack_timer_2 > 0) {
            new_animation = ANIMATION_ATTACK2;
            initialize = true;
        }
        if (hit_counter > 0) {
            new_animation = ANIMATION_DAMAGE;
            initialize = false;
        }
    }
    if (new_animation != -1)
        Change_Animation(new_animation, initialize);
    return 0;
}

int PK2Sprite::Animation_Chicken() {
    int new_animation = -1;
    bool initialize = false;
    if (health < 1 && !can_move_down) {
        new_animation = ANIMATION_DEATH;
        initialize = true;
    } else {
        if (movement_x > -0.2 && movement_x < 0.2 && movement_y == 0 && jump_timer <= 0) {
            new_animation = ANIMATION_IDLE;
            initialize = true;
        }
        if ((movement_x < -0.2 || movement_x > 0.2) && jump_timer <= 0) {
            new_animation = ANIMATION_WALK;
            initialize = false;
        }
        if (movement_y < 0) {//-0.3
            new_animation = ANIMATION_JUMP_UP;
            initialize = false;
        }
        if ((jump_timer > 90 + 10/*type->max_jump || movement_y > 1.5*/) && can_move_down) {
            new_animation = ANIMATION_JUMP_DOWN;
            initialize = false;
        }
        if (attack_timer_1 > 0) {
            new_animation = ANIMATION_ATTACK1;
            initialize = true;
        }
        if (attack_timer_2 > 0) {
            new_animation = ANIMATION_ATTACK2;
            initialize = true;
        }
        if (is_crouching) {
            new_animation = ANIMATION_CROUCH;
            initialize = true;
        }
        if (hit_counter > 0) {
            new_animation = ANIMATION_DAMAGE;
            initialize = false;
        }
    }
    if (new_animation != -1)
        Change_Animation(new_animation, initialize);
    return 0;
}

int PK2Sprite::Animation_Bonus() {
    Change_Animation(ANIMATION_IDLE, true);
    return 0;
}

int PK2Sprite::Animation_Projectile() {
    Change_Animation(ANIMATION_IDLE, true);
    return 0;
}

int PK2Sprite::Animation_Egg() {
    int new_animation = 0;
    bool initialize = false;
    new_animation = ANIMATION_IDLE;
    initialize = true;
    if (health < type->health) {
        new_animation = ANIMATION_DEATH;
        initialize = true;
    }
    Change_Animation(new_animation, initialize);
    return 0;
}
