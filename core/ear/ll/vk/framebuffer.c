#include "framebuffer.h"
#include "../../../cutil.h"

#include "image_views.h"
#include "swapchain.h"
#include "render_pass.h"
#include "device_log.h"

VkFramebuffer* _ear_vk_swapchain_fbufs;

void
_ear_vk_create_framebuffers(
    void
    ) {
    _ear_vk_swapchain_fbufs = malloc(sizeof(VkFramebuffer) * _ear_vk_swapchain_img_amt);

    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i) {
        VkImageView attachments[] = {
            _ear_vk_swapchain_img_views[i],
            };

        VkFramebufferCreateInfo fbinfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .renderPass = _ear_vk_renderpass,

            .attachmentCount = sizeof(attachments) / sizeof(attachments[0]),
            .pAttachments    = attachments,

            .width  = _ear_vk_swapchain_extent.width,
            .height = _ear_vk_swapchain_extent.height,

            .layers = 1,
            };

        eat_assert(vkCreateFramebuffer(_ear_vk_device, &fbinfo, NULL, &_ear_vk_swapchain_fbufs[i]) == VK_SUCCESS,
            "failed to create framebuffer!");
    }

}
void
_ear_vk_delete_framebuffers(
    void
    ) {
    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i)
        vkDestroyFramebuffer(_ear_vk_device, _ear_vk_swapchain_fbufs[i], NULL);
    free(_ear_vk_swapchain_fbufs);
}
