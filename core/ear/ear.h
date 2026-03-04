#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "pipeline.h"
#include "buffer.h"
#include "texture.h"
#include "user.h"
#include "framebuffer.h"
#include "text.h"

void
ear_init(
    void
    );

void 
ear_stop(
    void
    );

void
ear_frame(
    void
    );
