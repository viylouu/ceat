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

    vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);
    memcpy(tex->data, pixels, imgsize);
    vkUnmapMemory(_ear_vk_device, tex->stagmem);

    _ear_vk_make_img(
        width, height,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &tex->img, &tex->imgmem
        );

    _ear_vk_trans_img(
        tex->img, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
    _ear_vk_copy_buf_img(
        tex->stagbuf, tex->img,
        width, height
        );

    _ear_vk_trans_img(
        tex->img, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

    _ear_vk_make_imgview(
        tex->img, VK_FORMAT_R8G8B8A8_SRGB,
        &tex->imgview
        );

    return tex;
}
void
ear_vk_delete_texture(
    ear_vk_texture* tex
    ) {
    vkDestroyImageView(_ear_vk_device, tex->imgview, NULL);

    vkDestroyImage(_ear_vk_device, tex->img, NULL);
    vkFreeMemory(_ear_vk_device, tex->imgmem, NULL);

    vkDestroyBuffer(_ear_vk_device, tex->stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, tex->stagmem, NULL);

    free(tex);
}
