#include "swapchain.h"
#include "../../../../cutil.h"

#include "../../../../eaw/window.h"

#include "../vk.h"
#include "framebuffer.h"
#include "image_views.h"
#include "render_pass.h"

#include "../init/sync.h"
#include "../init/queue_fam.h"
#include "../init/device_phys.h"
#include "../init/device_log.h"
#include "../init/surface.h"

uint32_t _ear_vk_cur_frame = EAR_VK_MAX_FRAMES_IN_FLIGHT;
uint32_t _ear_vk_cur_img_index = 0;
bool _ear_vk_first_frame = true;

VkSwapchainKHR _ear_vk_swapchain;
VkImage* _ear_vk_swapchain_imgs;
    uint32_t _ear_vk_swapchain_img_amt;
VkFormat _ear_vk_swapchain_img_fmt;
VkExtent2D _ear_vk_swapchain_extent;

//VkSemaphore* _ear_vk_img_rend_sems;
//VkFence* _ear_vk_images_inflight;

_ear_vk_swapchain_support_details
_ear_vk_query_swapchain_support(
    VkPhysicalDevice device
    ) {
    _ear_vk_swapchain_support_details details = {
        .capabilities = {0},
        
        .formats = NULL,
        .format_amt = 0,

        .present_modes = NULL,
        .present_mode_amt = 0,
        };

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _ear_vk_surface, &details.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _ear_vk_surface, &details.format_amt, NULL);
    details.formats = malloc(sizeof(VkSurfaceFormatKHR) * details.format_amt);
    if (details.format_amt != 0) vkGetPhysicalDeviceSurfaceFormatsKHR(device, _ear_vk_surface, &details.format_amt, details.formats);

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _ear_vk_surface, &details.present_mode_amt, NULL);
    details.present_modes = malloc(sizeof(VkPresentModeKHR) * details.present_mode_amt);
    if (details.present_mode_amt != 0) vkGetPhysicalDeviceSurfacePresentModesKHR(device, _ear_vk_surface, &details.present_mode_amt, details.present_modes);

    return details;
}

VkSurfaceFormatKHR
_ear_vk_choose_swap_surface_format(
    VkSurfaceFormatKHR* availablefmts, 
    uint32_t fmtamt
    ) {
    for (uint32_t i = 0; i < fmtamt; ++i) {
        VkSurfaceFormatKHR aft = availablefmts[i];
        if (aft.format == VK_FORMAT_B8G8R8A8_SRGB && aft.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return aft;
    }

    return availablefmts[0];
}

VkPresentModeKHR
_ear_vk_choose_swap_present_mode(
    VkPresentModeKHR* availablepmds,
    uint32_t pmdamt
    ) {
    for (uint32_t i = 0; i < pmdamt; ++i) {
        VkPresentModeKHR apmd = availablepmds[i];
        if (apmd == VK_PRESENT_MODE_MAILBOX_KHR)
            return apmd;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
_ear_vk_choose_swap_extent(
    VkSurfaceCapabilitiesKHR caps
    ) {
    VkExtent2D actextent = caps.currentExtent;

    if (caps.currentExtent.width == UINT32_MAX) {
        actextent = (VkExtent2D){
            _eaw_window_width,
            _eaw_window_height,
            };
    }

    if (actextent.width < caps.minImageExtent.width)
        actextent.width = caps.minImageExtent.width;
    if (actextent.height < caps.minImageExtent.height)
        actextent.height = caps.minImageExtent.height;
    if (actextent.width > caps.maxImageExtent.width)
        actextent.width = caps.maxImageExtent.width;
    if (actextent.height > caps.maxImageExtent.height)
        actextent.height = caps.maxImageExtent.height;

    return actextent;
}

void
_ear_vk_create_swapchain(
    void
    ) {
    _ear_vk_swapchain_support_details details = _ear_vk_query_swapchain_support(_ear_vk_physical_device);

    VkSurfaceFormatKHR surface_fmt = _ear_vk_choose_swap_surface_format(details.formats, details.format_amt);
    VkPresentModeKHR present_mode = _ear_vk_choose_swap_present_mode(details.present_modes, details.present_mode_amt);
    VkExtent2D extent = _ear_vk_choose_swap_extent(details.capabilities);

    uint32_t images = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && images > details.capabilities.maxImageCount)
        images = details.capabilities.maxImageCount;

    _ear_vk_queue_family_inds inds = _ear_vk_find_queue_families(_ear_vk_physical_device);
    uint32_t indices[] = { inds.graphics_family, inds.present_family };
    bool issameboth = indices[0] == indices[1];

    VkSwapchainCreateInfoKHR createinfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,

        .flags = 0,

        .surface = _ear_vk_surface,

        .minImageCount = images,
        .imageFormat = surface_fmt.format,
        .imageColorSpace = surface_fmt.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

        .imageSharingMode = issameboth? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = issameboth? 0 : 2,
        .pQueueFamilyIndices = issameboth? NULL : indices,

        .preTransform = details.capabilities.currentTransform,

        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

        .presentMode = present_mode,
        .clipped = true,

        .oldSwapchain = NULL,
        };

    eat_assert(vkCreateSwapchainKHR(_ear_vk_device, &createinfo, NULL, &_ear_vk_swapchain) == VK_SUCCESS, 
        "failed to create swapchain!");

    vkGetSwapchainImagesKHR(_ear_vk_device, _ear_vk_swapchain, &_ear_vk_swapchain_img_amt, NULL);
    _ear_vk_swapchain_imgs = malloc(sizeof(VkImage) * _ear_vk_swapchain_img_amt);
    vkGetSwapchainImagesKHR(_ear_vk_device, _ear_vk_swapchain, &_ear_vk_swapchain_img_amt, _ear_vk_swapchain_imgs);

    _ear_vk_swapchain_img_fmt = surface_fmt.format;
    _ear_vk_swapchain_extent = extent;

    //_ear_vk_images_inflight = malloc(sizeof(VkFence) * _ear_vk_swapchain_img_amt);
    //for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i)
    //    _ear_vk_images_inflight[i] = NULL;
}
void
_ear_vk_delete_swapchain(
    void
    ) {
    free(_ear_vk_swapchain_imgs);
    //free(_ear_vk_images_inflight);
    vkDestroySwapchainKHR(_ear_vk_device, _ear_vk_swapchain, NULL);
}

int32_t
_ear_vk_acquire_swapchain_image(
    uint32_t frame
    ) {
    uint32_t index;
    VkResult res = vkAcquireNextImageKHR(_ear_vk_device, _ear_vk_swapchain, UINT64_MAX, _ear_vk_image_available_sems[frame], NULL, &index);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || ear_framebuffer_resize) { 
        ear_framebuffer_resize = false; 
        _ear_vk_recreate_swapchain(); 
        return -1; 
    } else eat_assert(res == VK_SUCCESS, 
        "failed to acquire swapchain image! (%d)", res);

    return index;
}

void
_ear_vk_present_swapchain(
    uint32_t index
    ) {
    VkSemaphore signalsemaphores[] = { _ear_vk_render_finish_sems[index] };

    VkSwapchainKHR swapchains[] = { _ear_vk_swapchain };

    VkPresentInfoKHR presentinfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = NULL,

        .waitSemaphoreCount = sizeof(signalsemaphores) / sizeof(signalsemaphores[0]),
        .pWaitSemaphores    = signalsemaphores,

        .swapchainCount = sizeof(swapchains) / sizeof(swapchains[0]),
        .pSwapchains    = swapchains,

        .pImageIndices = &index,

        .pResults = NULL,
        };

    vkQueuePresentKHR(_ear_vk_present_queue, &presentinfo);
}

void
_ear_vk_cleanup_swapchain(
    void
    ) {
    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i) {
        vkDestroyFramebuffer(_ear_vk_device, _ear_vk_swapchain_fbufs[i], NULL);
        vkDestroyImageView(_ear_vk_device, _ear_vk_swapchain_img_views[i], NULL);
    }

    vkDestroySwapchainKHR(_ear_vk_device, _ear_vk_swapchain, NULL);
}

void
_ear_vk_create_full_swapchain(
    void
    ) {
    _ear_vk_create_swapchain();
    _ear_vk_create_sync_objects();
    _ear_vk_create_image_views();
    _ear_vk_create_render_pass();
    _ear_vk_create_framebuffers();
}
void
_ear_vk_delete_full_swapchain(
    void
    ) {
    _ear_vk_delete_framebuffers();
    _ear_vk_delete_render_pass();
    _ear_vk_delete_image_views();
    _ear_vk_delete_sync_objects();
    _ear_vk_delete_swapchain();
}
void
_ear_vk_recreate_swapchain(
    void
    ) {
    _ear_vk_device_wait_idle();

    _ear_vk_cleanup_swapchain();

    _ear_vk_create_swapchain();
    _ear_vk_create_image_views();
    _ear_vk_create_framebuffers();
}
