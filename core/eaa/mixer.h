#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../eau/arena.h"
#include "../../include/miniaudio.h"

typedef struct eaa_mixer eaa_mixer;
typedef struct eaa_mixer_desc eaa_mixer_desc;

extern bool _eaa_mixer_changed_this_frame;


eaa_mixer*
eaa_create_mixer(
    eaa_mixer_desc desc,
    eau_arena* arena
    );

void
eaa_delete_mixer(
    eaa_mixer* mixer
    );

void
eaa_set_mixer_volume(
    eaa_mixer* mixer,
    float volume
    );

void
eaa_set_mixer_pitch(
    eaa_mixer* mixer,
    float pitch
    );


struct eaa_mixer_desc{
    float volume;
    float pitch;

    eaa_mixer* parent;
};

struct eaa_mixer{
    eaa_mixer_desc desc;

    eau_destructor* dest;
};
