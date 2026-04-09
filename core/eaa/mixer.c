#include "mixer.h"
//#include "../cutil.h"

#include <stdlib.h>

//#include "engine.h"

bool _eaa_mixer_changed_this_frame = false;

void
_eaa_arena_mixer_delete(
    void* mixer
    ) { 
    eaa_delete_mixer(mixer); 
}

void
_eaa_debug_mixer_window(
    void* mixer,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eaa_mixer*
eaa_create_mixer(
    eaa_mixer_desc desc,
    eau_arena* arena
    ) {
    eaa_mixer* mixer = malloc(sizeof(eaa_mixer));
    *mixer = (eaa_mixer){
        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            mixer,
            "mixer",
            _eaa_debug_mixer_window,
            arena
            ),
        };

    if (arena != NULL) eau_add_to_arena(arena, &mixer->dest, mixer, _eaa_arena_mixer_delete);
    return mixer;
}

void
eaa_delete_mixer(
    eaa_mixer* mixer
    ) {
    eat_debug_remove_obj(mixer->deb_obj);

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


void
_eaa_debug_mixer_window(
    void* _mixer,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    (void)_mixer;
    (void)x; (void)y; (void)w; (void)h;
    (void)t; (void)sel;
}
