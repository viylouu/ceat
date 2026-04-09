#include "buffer.h"
#include "cutil.h"

#include "../util/buffer.h"

#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"

void*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    ) {
    ear_vk_buffer* buf = malloc(sizeof(ear_vk_buffer));
    buf->type = desc.type;
    buf->stride = desc.stride;
    buf->size = size;
    buf->chunk = desc.chunk_size;

    switch (desc.type) {
    case EAR_BUF_VERTEX: 
    case EAR_BUF_INDEX:
    case EAR_BUF_UNIFORM_STAGING:
    case EAR_BUF_STORAGE_STAGING:
        _ear_vk_make_buf_stage(buf, desc, data, size); break;
    case EAR_BUF_UNIFORM_PERSISTENT:
    case EAR_BUF_STORAGE_PERSISTENT: 
        _ear_vk_make_buf_pers(buf, desc, data, size); break;
    default: eat_unreachable();
    }

    return buf;
}
void
ear_vk_delete_buffer(
    void* _buf
    ) {
    ear_vk_buffer* buf = _buf;

    switch (buf->type) {
    case EAR_BUF_VERTEX:
    case EAR_BUF_INDEX:
    case EAR_BUF_STORAGE_STAGING:
    case EAR_BUF_UNIFORM_STAGING:
        _ear_vk_del_buf_stage(buf); break;
    case EAR_BUF_STORAGE_PERSISTENT:
    case EAR_BUF_UNIFORM_PERSISTENT:
        _ear_vk_del_buf_pers(buf); break;
    default: eat_unreachable();
    }

    free(buf);
}

void
ear_vk_bind_buffer(
    void* _buf,
    uint32_t slot,
    uint32_t offset
    ) {
    ear_vk_buffer* buf = _buf;

    switch (buf->type) {
    case EAR_BUF_VERTEX:
        vkCmdBindVertexBuffers(
            _ear_vk_comm_buffers[_ear_vk_cur_frame], 
            slot, 
            1, &buf->gen.buffer, 
            &(VkDeviceSize){offset * buf->chunk * buf->stride}
            );
        break;
    case EAR_BUF_INDEX:
        vkCmdBindIndexBuffer(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            buf->gen.buffer,
            offset * buf->chunk * buf->stride,
            VK_INDEX_TYPE_UINT32
            );
        break;
    case EAR_BUF_UNIFORM_STAGING:
    case EAR_BUF_UNIFORM_PERSISTENT:
    case EAR_BUF_STORAGE_STAGING:
    case EAR_BUF_STORAGE_PERSISTENT:
        eat_warn("binding buffers dont need to be bound, bind the bindset instead!");
        break;
    default: eat_unreachable();
    }
}

void
ear_vk_update_buffer(
    void* _buf,
    const void* data,
    //uint32_t size,
    uint32_t offset
    ) {
    ear_vk_buffer* buf = _buf;

    switch (buf->type) {
    case EAR_BUF_UNIFORM_PERSISTENT: 
    case EAR_BUF_STORAGE_PERSISTENT:
        _ear_vk_update_buf_pers(buf, data, offset); return;
    case EAR_BUF_VERTEX:
    case EAR_BUF_INDEX:
    case EAR_BUF_STORAGE_STAGING:
    case EAR_BUF_UNIFORM_STAGING:
        _ear_vk_update_buf_stage(buf, data, 0, offset); return;
    }

    eat_unreachable();
}
