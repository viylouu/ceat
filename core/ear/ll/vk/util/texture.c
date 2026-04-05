#include "texture.h"
#include "../../../../cutil.h"

#include "../init/device_log.h"
#include "../init/device_phys.h"
#include "buffer.h"
#include "commbuf.h"

void
_ear_vk_make_img(
    uint32_t width, uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags props,
    VkImage* img, VkDeviceMemory* mem
    ) {
    VkImageCreateInfo imginfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .imageType = VK_IMAGE_TYPE_2D,
        
        .extent.width  = width,
        .extent.height = height,
        .extent.depth  = 1,

        .mipLevels = 1,

        .arrayLayers = 1,

        .format = format,
        .tiling = tiling,

        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,

        .usage = usage,

        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,

        .samples = VK_SAMPLE_COUNT_1_BIT,
        };

    eat_assert(vkCreateImage(_ear_vk_device, &imginfo, NULL, img) == VK_SUCCESS,
        "failed to create image!");

    VkMemoryRequirements memreqs;
    vkGetImageMemoryRequirements(_ear_vk_device, *img, &memreqs);

    VkMemoryAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,

        .allocationSize = memreqs.size,
        .memoryTypeIndex = _ear_vk_find_memory_type(memreqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        };

    eat_assert(vkAllocateMemory(_ear_vk_device, &allocinfo, NULL, mem) == VK_SUCCESS,
        "failed to allocate image memory!");

    vkBindImageMemory(_ear_vk_device, *img, *mem, 0);
}
void
_ear_vk_trans_img(
    VkImage img, VkFormat format,
    VkImageLayout oldlay,
    VkImageLayout newlay
    ) {
    VkCommandBuffer commbuf = _ear_vk_begin_stcomms();

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = NULL,

        .oldLayout = oldlay,
        .newLayout = newlay,

        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

        .image = img,

        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1,

        .srcAccessMask = 0,
        .dstAccessMask = 0,
        };

    VkPipelineStageFlags src_stage;
    VkPipelineStageFlags dst_stage;

    if (oldlay == VK_IMAGE_LAYOUT_UNDEFINED && newlay == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldlay == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newlay == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else eat_error("unsupported image layout transition!");

    vkCmdPipelineBarrier(
        commbuf,
        src_stage, dst_stage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier
        );

    _ear_vk_end_stcomms(commbuf);
}
void
_ear_vk_copy_buf_img(
    VkBuffer buf, VkImage img,
    uint32_t width, uint32_t height
    ) {
    VkCommandBuffer commbuf = _ear_vk_begin_stcomms();

    VkBufferImageCopy region = {
        .bufferOffset      = 0,
        .bufferRowLength   = 0,
        .bufferImageHeight = 0,

        .imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .imageSubresource.mipLevel       = 0,
        .imageSubresource.baseArrayLayer = 0,
        .imageSubresource.layerCount     = 1,

        .imageOffset = { 0,0,0 },
        .imageExtent = { width, height, 1 },
        };

    vkCmdCopyBufferToImage(
        commbuf,
        buf, img,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
        );

    _ear_vk_end_stcomms(commbuf);
}
void
_ear_vk_make_imgview(
    VkImage image, VkFormat format,
    VkImageView* view
    ) {
    VkImageViewCreateInfo viewinfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .image = image,

        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,

        .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1,
        };

    eat_assert(vkCreateImageView(_ear_vk_device, &viewinfo, NULL, view) == VK_SUCCESS,
        "failed to create image view!");
}
void
_ear_vk_make_sampler(
    bool anisotropy,
    VkSampler* sampler
    ) {
    VkSamplerCreateInfo sampinfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST,

        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,

        .anisotropyEnable = anisotropy,
        .maxAnisotropy    = anisotropy? _ear_vk_physical_device_props.limits.maxSamplerAnisotropy : 1,

        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,

        .unnormalizedCoordinates = false,

        .compareEnable = false,
        .compareOp     = VK_COMPARE_OP_ALWAYS,

        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0,
        .minLod     = 0,
        .maxLod     = 0,
        };

    eat_assert(vkCreateSampler(_ear_vk_device, &sampinfo, NULL, sampler) == VK_SUCCESS,
        "failed to create texture sampler!");
}
