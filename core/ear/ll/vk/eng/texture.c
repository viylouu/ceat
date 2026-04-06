#include "texture.h"
#include "../../../../cutil.h"

#include <string.h>

#include "../util/buffer.h"
#include "../init/device_log.h"
#include "../util/texture.h"

ear_vk_texture*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t pixels[],
    uint32_t width, uint32_t height
    ) {
    ear_vk_texture* tex = malloc(sizeof(ear_vk_texture));

    VkDeviceSize imgsize = width * height * 4;

    _ear_vk_make_buf(
        imgsize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &tex->stagbuf, &tex->stagmem
        );

    if (pixels == NULL) {
        tex->selfalloc = true;

        uint32_t perpix;
        switch (desc.type) {
        case EAR_TEX_COLOR: perpix = sizeof(uint8_t)  * 4; break;
        case EAR_TEX_DEPTH: perpix = sizeof(uint8_t)  * 4; break;
        case EAR_TEX_HDR:   perpix = sizeof(uint16_t) * 4; break;
        case EAR_TEX_HDR32: perpix = sizeof(uint32_t) * 4; break;
        default: eat_unreachable();
        }

        tex->alloc = malloc(width * height * perpix);
        pixels = tex->alloc;
    } else tex->selfalloc = false;

    vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);

    uint32_t* px = (uint32_t*)pixels;
    if (desc.type == EAR_TEX_COLOR) for (uint32_t i = 0; i < width * height; ++i) { 
        uint32_t p = px[i];
        px[i] = (
            (p & 0xFF00FF00) |
            (p & 0x00FF0000) >> 16 |
            (p & 0x000000FF) << 16
            );
    }

    memcpy(tex->data, pixels, imgsize);
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
    ear_vk_texture* tex
    ) {
    vkDestroySampler(_ear_vk_device, tex->samp, NULL);
    vkDestroyImageView(_ear_vk_device, tex->imgview, NULL);

    vkDestroyImage(_ear_vk_device, tex->img, NULL);
    vkFreeMemory(_ear_vk_device, tex->imgmem, NULL);

    vkDestroyBuffer(_ear_vk_device, tex->stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, tex->stagmem, NULL);

    if (tex->selfalloc) free(tex->alloc);
    free(tex);
}
