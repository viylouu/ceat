#pragma once

#include "../../include/miniaudio.h"

typedef struct eaa_state{
    ma_engine eng;
} eaa_state;
extern eaa_state eaa_s;

void
eaa_engine_init(
    void
    );

void
eaa_engine_stop(
    void
    );
