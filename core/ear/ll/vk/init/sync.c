#include "sync.h"
#include "../../../../cutil.h"

#include "device_log.h"

VkSemaphore _ear_vk_image_available_sems[EAR_VK_MAX_FRAMES_IN_FLIGHT];
VkSemaphore _ear_vk_render_finish_sems[EAR_VK_MAX_FRAMES_IN_FLIGHT];
VkFence _ear_vk_inflight_fences[EAR_VK_MAX_FRAMES_IN_FLIGHT];

void
_ear_vk_create_sync_objects(
    void
    ) {
    VkSemaphoreCreateInfo semaphoreinfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,
        };

    VkFenceCreateInfo fenceinfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,

        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        eat_assert(vkCreateSemaphore(_ear_vk_device, &semaphoreinfo, NULL, &_ear_vk_image_available_sems[i]) == VK_SUCCESS &&
                   vkCreateSemaphore(_ear_vk_device, &semaphoreinfo, NULL, &_ear_vk_render_finish_sems[i])   == VK_SUCCESS &&
                   vkCreateFence    (_ear_vk_device, &fenceinfo,     NULL, &_ear_vk_inflight_fences[i])      == VK_SUCCESS,
            "failed to create sync objects!");
    }
}
void
_ear_vk_delete_sync_objects(
    void
    ) {
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroyFence(_ear_vk_device, _ear_vk_inflight_fences[i], NULL);
        vkDestroySemaphore(_ear_vk_device, _ear_vk_render_finish_sems[i], NULL);
        vkDestroySemaphore(_ear_vk_device, _ear_vk_image_available_sems[i], NULL);
    }
}

void
_ear_vk_wait_for_fences(
    uint32_t frame
    ) {
    vkWaitForFences(_ear_vk_device, 1, &_ear_vk_inflight_fences[frame], VK_TRUE, UINT64_MAX);
    vkResetFences(_ear_vk_device, 1, &_ear_vk_inflight_fences[frame]);
}
