#include "mixer.h"
#include "../cutil.h"

#include "engine.h"

bool _eaa_mixer_changed_this_frame = false;

void
_eaa_arena_mixer_delete(
    void* mixer
    ) { 
    eaa_delete_mixer(mixer); 
}

eaa_mixer*
eaa_create_mixer(
    eaa_mixer_desc desc,
    eau_arena* arena
    ) {
    eaa_mixer* mixer = malloc(sizeof(eaa_mixer));
    *mixer = (eaa_mixer){
        .desc = desc,
        };

    if (arena != NULL) eau_add_to_arena(arena, &mixer->dest, mixer, _eaa_arena_mixer_delete);
    return mixer;
}

void
eaa_delete_mixer(
    eaa_mixer* mixer
    ) {
    if (mixer->dest != NULL) mixer->dest->data = NULL;
    free(mixer);
}

void
eaa_set_mixer_volume(
    eaa_mixer* mixer,
    float volume
    ) {
    mixer->desc.volume = volume;
    _eaa_mixer_changed_this_frame = true;
}

void
eaa_set_mixer_pitch(
    eaa_mixer* mixer,
    float pitch
    ) {
    mixer->desc.pitch = pitch;
    _eaa_mixer_changed_this_frame = true;
}
