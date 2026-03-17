#pragma once

#include <stdint.h>
#include <stdbool.h>

#define EAU_MAX_ENTITY_AMT 2048
#define EAU_MAX_COMPONENT_AMT 256

typedef uint32_t eau_entity;
typedef uint32_t eau_component;

typedef struct eau_component_arr{
    void* data;
    uint32_t stride;
} eau_component_arr;

extern eau_component_arr eau_components[EAU_MAX_COMPONENT_AMT];
extern uint32_t eau_component_amt;


uint32_t eau_register_component(uint32_t size);
