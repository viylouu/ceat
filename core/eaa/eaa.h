#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "sound.h"
#include "mixer.h"

void
eaa_init(
    void
    );

void 
eaa_stop(
    void
    );

void
eaa_frame(
    void
    );
