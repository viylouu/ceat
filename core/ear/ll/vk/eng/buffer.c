#include "buffer.h"
#include "../../../../cutil.h"

#include <string.h>

#include "../util/buffer.h"

#include "../init/device_log.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"

ear_vk_buffer*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    ear_vk_buffer* buf = malloc(sizeof(ear_vk_buffer));
    buf->type = desc.type;

    VkBuffer stagbuf;
    VkDeviceMemory stagmem;
    _ear_vk_make_buf(
        size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagbuf, &stagmem
        );

    vkMapMemory(_ear_vk_device, stagmem, 0, size, 0, &buf->data);
    memcpy(buf->data, data, size);
    vkUnmapMemory(_ear_vk_device, stagmem);

    _ear_vk_make_buf(
        size, _ear_vk_convert_buf_type(desc.type), 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &buf->buffer, &buf->memory
        );

    _ear_vk_copy_buf(stagbuf, buf->buffer, size);

    vkDestroyBuffer(_ear_vk_device, stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, stagmem, NULL);

    return buf;
}
void
ear_vk_delete_buffer(
    ear_vk_buffer* buf
    ) {
    vkDestroyBuffer(_ear_vk_device, buf->buffer, NULL);
    vkFreeMemory(_ear_vk_device, buf->memory, NULL);

    free(buf);
}

void
ear_vk_bind_buffer(
    ear_vk_buffer* buf,
    uint32_t slot
    ) {
    switch (buf->type) {
    case EAR_BUF_VERTEX:
        vkCmdBindVertexBuffers(
            _ear_vk_comm_buffers[_ear_vk_cur_frame], 
            slot, 
            1, &buf->buffer, 
            &(VkDeviceSize){0}
            );
        break;
    case EAR_BUF_INDEX:
        vkCmdBindIndexBuffer(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            buf->buffer,
            0,
            VK_INDEX_TYPE_UINT32
            );
        break;
    }
}
