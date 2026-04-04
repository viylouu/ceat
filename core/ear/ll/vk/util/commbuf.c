#include "commbuf.h"
//#include "../../../../cutil.h"

#include "../init/device_log.h"
#include "../init/comm_pool.h"

VkCommandBuffer
_ear_vk_begin_stcomms(
    void
    ) {
    VkCommandBufferAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,

        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,

        .commandPool        = _ear_vk_comm_pool,
        .commandBufferCount = 1,
        };

    VkCommandBuffer commbuf;
    vkAllocateCommandBuffers(_ear_vk_device, &allocinfo, &commbuf);

    VkCommandBufferBeginInfo begininfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,

        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,

        .pInheritanceInfo = NULL,
        };
    vkBeginCommandBuffer(commbuf, &begininfo);

    return commbuf;
}
void
_ear_vk_end_stcomms(
    VkCommandBuffer commbuf
    ) {
    vkEndCommandBuffer(commbuf);

    VkSubmitInfo submitinfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,

        .waitSemaphoreCount = 0,
        .pWaitSemaphores    = NULL,

        .pWaitDstStageMask = NULL,

        .commandBufferCount = 1,
        .pCommandBuffers    = &commbuf,

        .signalSemaphoreCount = 0,
        .pSignalSemaphores    = NULL,
        };

    vkQueueSubmit(_ear_vk_graphics_queue, 1, &submitinfo, NULL);
    vkQueueWaitIdle(_ear_vk_graphics_queue);

    vkFreeCommandBuffers(_ear_vk_device, _ear_vk_comm_pool, 1, &commbuf);
}
