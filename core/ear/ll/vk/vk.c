#include "vk.h"
#include "../../../cutil.h"

#include "comm_pool.h"
#include "comm_buffer.h"
#include "swapchain.h"
#include "device_log.h"
#include "device_phys.h"
#include "surface.h"
#include "validation.h"
#include "instance.h"
#include "sync.h"

uint32_t cur_frame = 0;
uint32_t cur_img_index;
bool first_frame = true;

void 
ear_vk_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    eat_assert(!_ear_vk_validation || _ear_vk_validation_support(), "validation layers unavailable!");

    _ear_vk_create_instance(title);

    _ear_vk_create_surface();

    _ear_vk_pick_physical_device();
    _ear_vk_create_logical_device();

    _ear_vk_create_command_pool();
    _ear_vk_create_command_buffers();

    _ear_vk_create_sync_objects();

    _ear_vk_create_swapchain();

    eat_warn("vk init!");
}
void
ear_vk_exit(
    void
    ) {
    _ear_vk_delete_swapchain();

    _ear_vk_delete_sync_objects();

    _ear_vk_delete_command_pool();

    _ear_vk_delete_logical_device();

    _ear_vk_delete_surface();

    _ear_vk_delete_instance();
}

void
ear_vk_frame(
    void
    ) {
    if (!first_frame) {
        //_ear_vk_end_render_pass(cur_frame);
        //_ear_vk_end_command_buffer(_ear_vk_comm_buffers[cur_frame]);

        //_ear_vk_submit_command_buffer(&_ear_vk_comm_buffers[cur_frame], cur_frame);
        //_ear_vk_present_swapchain(cur_img_index);
    }

    first_frame = false;

    ++cur_frame;
    if (cur_frame > EAR_VK_MAX_FRAMES_IN_FLIGHT) cur_frame = 0;

    _ear_vk_wait_for_fences(cur_frame);
    cur_img_index = _ear_vk_acquire_swapchain_image(cur_frame);

    //_ear_vk_reset_command_buffer(_ear_vk_comm_buffers[cur_frame]);
    //_ear_vk_start_command_buffer(_ear_vk_comm_buffers[cur_frame], cur_img_index);
    //_ear_vk_start_render_pass(cur_img_index, cur_frame);
}
