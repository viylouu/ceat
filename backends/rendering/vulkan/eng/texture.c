#include "texture.h"
#include "cutil.h"

#include <string.h>

#include "../util/buffer.h"
#include "../init/device_log.h"
#include "../util/texture.h"

void*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    ) {
    ear_vk_texture* tex = malloc(sizeof(ear_vk_texture));
    tex->type = desc.type;

    uint32_t perpix;
    switch (desc.type) {
    case EAR_TEX_COLOR: perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_DEPTH: perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_HDR:   perpix = sizeof(uint16_t) * 4; break;
    case EAR_TEX_HDR32: perpix = sizeof(uint32_t) * 4; break;
    default: eat_unreachable();
    }
    VkDeviceSize imgsize = width * height * perpix;

    _ear_vk_make_buf(
        imgsize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &tex->stagbuf, &tex->stagmem
        );

    vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);

    if (desc.type == EAR_TEX_COLOR) for (uint32_t i = 0; i < width * height * 4; i += 4) { 
        uint8_t r = (*pixels)[i+0];
        uint8_t g = (*pixels)[i+1];
        uint8_t b = (*pixels)[i+2];
        uint8_t a = (*pixels)[i+3];

        (*pixels)[i+0] = b;
        (*pixels)[i+1] = g;
        (*pixels)[i+2] = r;
        (*pixels)[i+3] = a;
    }

    memcpy(tex->data, *pixels, imgsize);
    vkUnmapMemory(_ear_vk_device, tex->stagmem);

    _ear_vk_make_img(
        width, height,
        tex->format = _ear_vk_convert_tex_fmt(desc.type),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
            VK_IMAGE_USAGE_SAMPLED_BIT | 
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &tex->img, &tex->imgmem
        );

    _ear_vk_trans_img(
        tex->img, desc.type == EAR_TEX_DEPTH,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
    _ear_vk_copy_buf_img(
        tex->stagbuf, tex->img,
        width, height
        );

    _ear_vk_trans_img(
        tex->img, desc.type == EAR_TEX_DEPTH,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

    _ear_vk_make_imgview(
        tex->img, tex->format,
        &tex->imgview
        );

    _ear_vk_make_sampler(
        desc.anisotropy,
        //VK_FILTER_NEAREST,
        //VK_SAMPLER_ADDRESS_MODE_REPEAT,
        &tex->samp
        );

    return tex;
}
void
ear_vk_delete_texture(
    void* _tex
    ) {
    ear_vk_texture* tex = _tex;

    vkDestroySampler(_ear_vk_device, tex->samp, NULL);
    vkDestroyImageView(_ear_vk_device, tex->imgview, NULL);

    vkDestroyImage(_ear_vk_device, tex->img, NULL);
    vkFreeMemory(_ear_vk_device, tex->imgmem, NULL);

    vkDestroyBuffer(_ear_vk_device, tex->stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, tex->stagmem, NULL);

    free(tex);
}

void
ear_vk_update_texture(
    void* _tex,
    uint8_t pixels[],
    uint32_t width, uint32_t height
    ) {
    ear_vk_texture* tex = _tex;

    uint32_t perpix;
    switch (tex->type) {
    case EAR_TEX_COLOR: perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_DEPTH: perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_HDR:   perpix = sizeof(uint16_t) * 4; break;
    case EAR_TEX_HDR32: perpix = sizeof(uint32_t) * 4; break;
    default: eat_unreachable();
    }
    VkDeviceSize imgsize = width * height * perpix;

    vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);
    memcpy(tex->data, pixels, imgsize);
    vkUnmapMemory(_ear_vk_device, tex->stagmem);

    _ear_vk_trans_img(
        tex->img, tex->type == EAR_TEX_DEPTH,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
    _ear_vk_copy_buf_img(
        tex->stagbuf, tex->img,
        width, height
        );
    _ear_vk_trans_img(
        tex->img, tex->type == EAR_TEX_DEPTH,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
}
