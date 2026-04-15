#pragma once

#define get_gl(type, var) \
    ((ear_gl_##type*)(var)->vk)
