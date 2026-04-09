#include "sound.h"
//#include "../cutil.h"

#include <stdlib.h>

#include "engine.h"

eaa_sound_ll* eaa_sound_ll_first;
eaa_sound_ll* eaa_sound_ll_last;

void
_eaa_arena_sound_delete(
    void* sound
    ) { 
    eaa_delete_sound(sound); 
}

void
_eaa_debug_sound_window(
    void* sound,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eaa_sound*
eaa_load_sound(
    eaa_sound_desc desc,
    const uint8_t data[],
    uint32_t data_size,
    eau_arena* arena
    ) {
    eaa_sound* sound = malloc(sizeof(eaa_sound));
    *sound = (eaa_sound){
        .ll = (eaa_sound_ll){
            .sound = sound,
            .prev = eaa_sound_ll_last,
            .next = NULL,
            },

        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            sound,
            "sound",
            _eaa_debug_sound_window,
            arena
            ),
        };

    if (eaa_sound_ll_last != NULL) eaa_sound_ll_last->next = &sound->ll;
    eaa_sound_ll_last = &sound->ll;
    if (eaa_sound_ll_first == NULL) eaa_sound_ll_first = &sound->ll;

    ma_decoder_init_memory(
        data,
        data_size,
        NULL,
        &sound->decode
        );

    ma_sound_init_from_data_source(
        &eaa_s.eng,
        &sound->decode,
        0,
        NULL,
        &sound->sound
        );

    float vol = desc.volume;
    float pitch = desc.pitch;

    for (eaa_mixer* mixer = desc.mixer; mixer != NULL; mixer = mixer->desc.parent) {
        vol *= mixer->desc.volume;
        pitch *= mixer->desc.pitch;
    }

    ma_sound_set_volume(&sound->sound, vol);
    ma_sound_set_pitch(&sound->sound, pitch);

    ma_sound_set_looping(&sound->sound, desc.loop);

    if (arena != NULL) eau_add_to_arena(arena, &sound->dest, sound, _eaa_arena_sound_delete);
    return sound;
}

void
eaa_delete_sound(
    eaa_sound* sound
    ) {
    eat_debug_remove_obj(sound->deb_obj);

    if (sound->ll.prev != NULL) sound->ll.prev->next = sound->ll.next;
    else eaa_sound_ll_first = sound->ll.next;

    if (sound->ll.next != NULL) sound->ll.next->prev = sound->ll.prev;
    else eaa_sound_ll_last = sound->ll.prev;

    ma_sound_uninit(&sound->sound);
    ma_decoder_uninit(&sound->decode);

    if (sound->dest != NULL) sound->dest->data = NULL;
    free(sound);
}

void
eaa_play_sound(
    eaa_sound* sound
    ) {
    ma_sound_seek_to_pcm_frame(&sound->sound, 0);
    ma_sound_start(&sound->sound);
}

void
eaa_stop_sound(
    eaa_sound* sound
    ) {
    ma_sound_stop(&sound->sound);
}

void
eaa_set_sound_volume(
    eaa_sound* sound,
    float volume
    ) {
    sound->desc.volume = volume;
    float vol = volume;

    for (eaa_mixer* mixer = sound->desc.mixer; mixer != NULL; mixer = mixer->desc.parent)
        vol *= mixer->desc.volume;

    ma_sound_set_volume(&sound->sound, vol);
}

void
eaa_set_sound_pitch(
    eaa_sound* sound,
    float pitch
    ) {
    sound->desc.pitch = pitch;
    //float _pitch = pitch;

    for (eaa_mixer* mixer = sound->desc.mixer; mixer != NULL; mixer = mixer->desc.parent)
        pitch *= mixer->desc.pitch;

    ma_sound_set_pitch(&sound->sound, pitch);

}

void
eaa_set_sound_loop(
    eaa_sound* sound,
    bool loop
    ) {
    sound->desc.loop = loop;
    ma_sound_set_looping(&sound->sound, loop);
}


void
_eaa_debug_sound_window(
    void* _sound,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    (void)_sound;
    (void)x; (void)y; (void)w; (void)h;
    (void)t; (void)sel;
}
