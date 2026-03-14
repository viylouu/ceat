#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../eau/arena.h"
#include "../debug/debug.h"
#include "mixer.h"
#include "../../include/miniaudio.h"

typedef struct eaa_sound eaa_sound;
typedef struct eaa_sound_desc eaa_sound_desc;

typedef struct eaa_sound_ll eaa_sound_ll;

extern eaa_sound_ll* eaa_sound_ll_first;
extern eaa_sound_ll* eaa_sound_ll_last;


eaa_sound*
eaa_load_sound(
    eaa_sound_desc desc,
    const uint8_t data[],
    uint32_t data_size,
    eau_arena* arena
    );

void
eaa_delete_sound(
    eaa_sound* sound
    );

void
eaa_play_sound(
    eaa_sound* sound
    );

void
eaa_stop_sound(
    eaa_sound* sound
    );

void
eaa_set_sound_volume(
    eaa_sound* sound,
    float volume
    );

void
eaa_set_sound_pitch(
    eaa_sound* sound,
    float pitch
    );

void
eaa_set_sound_loop(
    eaa_sound* sound,
    bool loop
    );


struct eaa_sound_ll{
    eaa_sound* sound;
    eaa_sound_ll* next;
    eaa_sound_ll* prev;
};

struct eaa_sound_desc{
    float volume;
    float pitch;
    bool loop;

    eaa_mixer* mixer;
};

struct eaa_sound{
    eaa_sound_ll ll;

    ma_sound sound;
    ma_decoder decode;

    eaa_sound_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
