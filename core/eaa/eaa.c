#include "eaa.h"
#include "../cutil.h"

#define MINIAUDIO_IMPLEMENTATION
#include "../../include/miniaudio.h"

#include "engine.h"

void
eaa_init(
    void
    ) {
    eaa_engine_init();
}

void 
eaa_exit(
    void
    ) {
    eaa_engine_exit();
}

void
eaa_frame(
    void
    ) {
    if (_eaa_mixer_changed_this_frame) {
        if (eaa_sound_ll_first == NULL) return;

        for (eaa_sound_ll* item = eaa_sound_ll_first; item != NULL; item = item->next) {
            float vol = item->sound->desc.volume;
            float pitch = item->sound->desc.pitch;

            for (eaa_mixer* mixer = item->sound->desc.mixer; mixer != NULL; mixer = mixer->desc.parent) {
                vol *= mixer->desc.volume;
                pitch *= mixer->desc.pitch;
            }

            ma_sound_set_volume(&item->sound->sound, vol);
            ma_sound_set_pitch(&item->sound->sound, pitch);
        }
    }

    _eaa_mixer_changed_this_frame = false;
}
