#include "texture.h"
#include "cutil.h"

#include <string.h>

#include "../util/buffer.h"
#include "../init/device_log.h"
#include "../util/texture.h"
#include "../sc/render_pass.h"
#include "../sc/swapchain.h"
#include "framebuffer.h"
#include "../init/comm_buffer.h"
#include "../init/sync.h"

void*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    ) {
    ear_vk_texture* tex = malloc(sizeof(ear_vk_texture));
    tex->type = desc.type;
    tex->width = width; tex->height = height;
    tex->aniso = desc.anisotropy;

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

    vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);
    memcpy(tex->data, *pixels, imgsize);
    //vkUnmapMemory(_ear_vk_device, tex->stagmem);

    _ear_vk_make_img(
        width, height,
        tex->format = _ear_vk_convert_tex_fmt(desc.type),
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
            VK_IMAGE_USAGE_SAMPLED_BIT | 
            (desc.type == EAR_TEX_DEPTH?
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT :
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT),
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &tex->img.img, &tex->imgmem
        );
    tex->img.depth = desc.type == EAR_TEX_DEPTH;
    tex->img.lay = VK_IMAGE_LAYOUT_UNDEFINED;

    bool reset = _ear_vk_in_pass;
    if (reset) _ear_vk_end_render_pass(_ear_vk_cur_img_index);

    _ear_vk_trans_img(
        &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
    _ear_vk_copy_buf_img(
        tex->stagbuf, tex->img.img,
        width, height,
        tex->type == EAR_TEX_DEPTH
        );

    _ear_vk_trans_img(
        &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
        desc.type == EAR_TEX_DEPTH?
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : 
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

    _ear_vk_make_imgview(
        tex->img.img, tex->format,
        tex->type == EAR_TEX_DEPTH,
        &tex->imgview
        );

    _ear_vk_make_sampler(
        desc.anisotropy,
        //VK_FILTER_NEAREST,
        //VK_SAMPLER_ADDRESS_MODE_REPEAT,
        &tex->samp
        );

    if (reset) ear_vk_bind_framebuffer(_ear_vk_last_fb);

    return tex;
}

void
_ear_vk_cleanup_texture(
    ear_vk_texture* tex
    ) {
    bool restart_comm = _ear_vk_commbuf_rec;
    if (restart_comm) {
        _ear_vk_end_render_pass(_ear_vk_cur_frame);
        _ear_vk_end_command_buffer(_ear_vk_comm_buffers[_ear_vk_cur_frame]);
        _ear_vk_submit_command_buffer(&_ear_vk_comm_buffers[_ear_vk_cur_frame], _ear_vk_cur_img_index, _ear_vk_cur_frame);

        _ear_vk_device_wait_idle();
        _ear_vk_wait_for_fences(_ear_vk_cur_frame);
        _ear_vk_reset_fences(_ear_vk_cur_frame);
    }

    vkDestroySampler(_ear_vk_device, tex->samp, NULL);
    vkDestroyImageView(_ear_vk_device, tex->imgview, NULL);

    vkDestroyImage(_ear_vk_device, tex->img.img, NULL);
    vkFreeMemory(_ear_vk_device, tex->imgmem, NULL);

    vkDestroyBuffer(_ear_vk_device, tex->stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, tex->stagmem, NULL);

    if (restart_comm) {
        _ear_vk_start_command_buffer(_ear_vk_comm_buffers[_ear_vk_cur_frame]);
        ear_vk_bind_framebuffer(_ear_vk_last_fb);
    }
}

void
ear_vk_delete_texture(
    void* _tex
    ) {
    ear_vk_texture* tex = _tex;
    _ear_vk_cleanup_texture(tex);
    free(tex);
}

void
ear_vk_update_texture(
    void* _tex,
    const uint8_t* pixels,
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

    if (tex->width != width || tex->height != height) {
        tex->width  = width;
        tex->height = height;

        _ear_vk_cleanup_texture(tex);
        _ear_vk_end_render_pass(_ear_vk_cur_frame);

        _ear_vk_make_buf(
            imgsize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &tex->stagbuf, &tex->stagmem
            );

        vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);
        memcpy(tex->data, pixels, imgsize);
        //vkUnmapMemory(_ear_vk_device, tex->stagmem);

        _ear_vk_make_img(
            width, height,
            tex->format = _ear_vk_convert_tex_fmt(tex->type),
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
                VK_IMAGE_USAGE_SAMPLED_BIT | 
                (tex->type == EAR_TEX_DEPTH?
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT :
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT),
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &tex->img.img, &tex->imgmem
            );

        _ear_vk_trans_img(
            &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            );
        _ear_vk_copy_buf_img_inplace(
            tex->stagbuf, tex->img.img,
            width, height,
            tex->type == EAR_TEX_DEPTH
            );

        _ear_vk_trans_img(
            &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
            tex->type == EAR_TEX_DEPTH?
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );

        _ear_vk_make_imgview(
            tex->img.img, tex->format,
            tex->type == EAR_TEX_DEPTH,
            &tex->imgview
            );

        _ear_vk_make_sampler(
            tex->aniso,
            //VK_FILTER_NEAREST,
            //VK_SAMPLER_ADDRESS_MODE_REPEAT,
            &tex->samp
            );

        ear_vk_bind_framebuffer(_ear_vk_last_fb);
    } else {
        //vkMapMemory(_ear_vk_device, tex->stagmem, 0, imgsize, 0, &tex->data);
        memcpy(tex->data, pixels, imgsize);
        //vkUnmapMemory(_ear_vk_device, tex->stagmem);

        _ear_vk_trans_img(
            &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            );
        _ear_vk_copy_buf_img_inplace(
            tex->stagbuf, tex->img.img,
            width, height,
            tex->type == EAR_TEX_DEPTH
            );
        _ear_vk_trans_img(
            &tex->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
            tex->type == EAR_TEX_DEPTH?
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );
    }
}
