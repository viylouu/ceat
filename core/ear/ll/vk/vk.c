#include "vk.h"
#include "../../../cutil.h"

#include "init/comm_pool.h"
#include "init/comm_buffer.h"
#include "init/device_log.h"
#include "init/device_phys.h"
#include "init/surface.h"
#include "init/validation.h"
#include "init/instance.h"
#include "init/sync.h"
#include "sc/render_pass.h"
#include "sc/swapchain.h"
#include "sc/image_views.h"
#include "sc/framebuffer.h"

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
    _ear_vk_create_image_views();
    _ear_vk_create_render_pass();
    _ear_vk_create_framebuffers();

    eat_warn("vk init!");
}
void
ear_vk_exit(
    void
    ) {
    _ear_vk_device_wait_idle();

    _ear_vk_delete_framebuffers();
    _ear_vk_delete_render_pass();
    _ear_vk_delete_image_views();
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
    if (!_ear_vk_first_frame) {
        _ear_vk_end_render_pass(_ear_vk_cur_frame);
        _ear_vk_end_command_buffer(_ear_vk_comm_buffers[_ear_vk_cur_frame]);

        _ear_vk_submit_command_buffer(&_ear_vk_comm_buffers[_ear_vk_cur_frame], _ear_vk_cur_frame);
        _ear_vk_present_swapchain(_ear_vk_cur_img_index, _ear_vk_cur_frame);
    }

    _ear_vk_first_frame = false;

    ++_ear_vk_cur_frame;
    if (_ear_vk_cur_frame >= EAR_VK_MAX_FRAMES_IN_FLIGHT) _ear_vk_cur_frame = 0;

    _ear_vk_wait_for_fences(_ear_vk_cur_frame);
    _ear_vk_cur_img_index = _ear_vk_acquire_swapchain_image(_ear_vk_cur_frame);

    _ear_vk_start_command_buffer(_ear_vk_comm_buffers[_ear_vk_cur_frame], _ear_vk_cur_img_index);
    _ear_vk_start_render_pass(_ear_vk_cur_img_index, _ear_vk_cur_frame);
}
