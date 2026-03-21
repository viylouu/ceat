#include "vk.h"
#include "../../../cutil.h"

#include "comm_pool.h"
#include "device_log.h"
#include "device_phys.h"
#include "surface.h"
#include "validation.h"
#include "instance.h"

void 
ear_vk_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    eat_assert(!_ear_vk_validation || _ear_vk_validation_support(), "validation layers unavailable!");

    _ear_vk_create_instance();

    _ear_vk_create_surface();

    _ear_vk_pick_physical_device();
    _ear_vk_create_logical_device();

    _ear_vk_create_command_pool();

    eat_warn("vulkan init complete!");
}
void
ear_vk_exit(
    void
    ) {
    _ear_vk_delete_command_pool();

    _ear_vk_delete_logical_device();

    _ear_vk_delete_surface();

    _ear_vk_delete_instance();
}
