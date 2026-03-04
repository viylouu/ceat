#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

char*
eau_load_file(
    const char* path,
    size_t* out_size
    );
