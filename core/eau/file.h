#pragma once

#include <stdint.h>
#include <stdbool.h>

char*
eau_load_file(
    const char* path,
    uint32_t* out_size
    );
