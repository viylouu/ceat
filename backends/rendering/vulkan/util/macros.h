#pragma once

#define get_vk(type, var) \
    ((ear_vk_##type*)var->vk)
