#include "file.h"
#include "../cutil.h"

char*
eau_load_file(
    const char* path,
    uint32_t* out_size
    ) {
    FILE* f = fopen(path, "rb");
    eat_assert(f, "cannot find file at path \"%s\"!", path);

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = malloc(size + 1);
    eat_assert(buffer, "failed to allocate buffer!");

    fread(buffer, 1, size, f);
    fclose(f);

    buffer[size] = 0;
    if (out_size) *out_size = size;

    return buffer;
}
