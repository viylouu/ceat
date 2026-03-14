#pragma once

#include "../../eat.h"

extern eat_debug_desc debug;
extern ear_font* debug_font;

void
eat_debug_init(
    void
    );

void
eat_debug_stop(
    void
    );

void
eat_debug_try_do(
    void
    );
