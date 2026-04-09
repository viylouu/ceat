#include "comm_pool.h"
#include "cutil.h"

#include "device_log.h"
#include "device_phys.h"
#include "queue_fam.h"

VkCommandPool _ear_vk_comm_pool;

void
_ear_vk_create_command_pool(
    void
    ) {
    _ear_vk_queue_family_inds inds = _ear_vk_find_queue_families(_ear_vk_physical_device);

    VkCommandPoolCreateInfo poolinfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,

        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,

        .queueFamilyIndex = inds.graphics_family,
        };

    eat_assert(vkCreateCommandPool(_ear_vk_device, &poolinfo, NULL, &_ear_vk_comm_pool) == VK_SUCCESS, "failed to create command pool!");
}
void
_ear_vk_delete_command_pool(
    void
    ) {
    vkDestroyCommandPool(_ear_vk_device, _ear_vk_comm_pool, NULL);
}
