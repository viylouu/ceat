#pragma once

#define get_gl(type, var) \
    ((ear_gl_##type*)(var)->vk)

#define _alignup(size, align) \
    ((size) + (align) - 1) & ~((align) - 1)
