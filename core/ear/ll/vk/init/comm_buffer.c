#include "comm_buffer.h"
#include "../../../../cutil.h"

#include "comm_pool.h"
#include "sync.h"
#include "device_log.h"

VkCommandBuffer _ear_vk_comm_buffers[EAR_VK_MAX_FRAMES_IN_FLIGHT];

void
_ear_vk_create_command_buffers(
    void
    ) {
    VkCommandBufferAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,

        .commandPool = _ear_vk_comm_pool,

        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,

        .commandBufferCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkAllocateCommandBuffers(_ear_vk_device, &allocinfo, _ear_vk_comm_buffers) == VK_SUCCESS, "failed to create command buffers!");
}

void
_ear_vk_start_command_buffer(
    VkCommandBuffer buffer
    ) {
    vkResetCommandBuffer(buffer, 0);

    VkCommandBufferBeginInfo begininfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,

        .flags = 0,

        .pInheritanceInfo = NULL,
        };

    eat_assert(vkBeginCommandBuffer(buffer, &begininfo) == VK_SUCCESS, "failed to start command buffer!");
}
void
_ear_vk_end_command_buffer(
    VkCommandBuffer buffer
    ) {
    eat_assert(vkEndCommandBuffer(buffer) == VK_SUCCESS, "failed to end command buffer!");
}

void
_ear_vk_submit_command_buffer(
    VkCommandBuffer* buffer,
    uint32_t frame
    ) {
    VkSemaphore waitsemaphores[] = { _ear_vk_image_available_sems[frame] };
    VkPipelineStageFlags waitstages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSemaphore signalsemaphores[] = { _ear_vk_render_finish_sems[frame] };

    VkSubmitInfo submitinfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,

        .waitSemaphoreCount = sizeof(waitsemaphores) / sizeof(waitsemaphores[0]),
        .pWaitSemaphores    = waitsemaphores,
        .pWaitDstStageMask  = waitstages,

        .commandBufferCount = 1,
        .pCommandBuffers    = buffer,

        .signalSemaphoreCount = sizeof(signalsemaphores) / sizeof(signalsemaphores[0]),
        .pSignalSemaphores    = signalsemaphores,
        };

    eat_assert(vkQueueSubmit(_ear_vk_graphics_queue, 1, &submitinfo, _ear_vk_inflight_fences[frame]) == VK_SUCCESS,
        "failed to submit command buffer!");
}
