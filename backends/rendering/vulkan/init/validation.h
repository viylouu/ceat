#pragma once

#include <stdint.h>
#include <stdbool.h>

static bool _ear_vk_validation = true;

static const char* _ear_vk_validations[] = {
    "VK_LAYER_KHRONOS_validation",
};
static const uint32_t _ear_vk_validation_amt = 1;

bool
_ear_vk_validation_support(
    void
    );
