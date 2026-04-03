#include "buffer.h"
#include "../../../../cutil.h"

#include "../util/buffer.h"

//#include "../init/device_log.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "pipeline.h"
//#include "../vk.h"

ear_vk_buffer*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    ear_vk_buffer* buf = malloc(sizeof(ear_vk_buffer));
    buf->type = desc.type;
    buf->stride = desc.stride;
    buf->size = size;

    switch (desc.type) {
    case EAR_BUF_STORAGE_STAGING:
    case EAR_BUF_VERTEX: 
    case EAR_BUF_INDEX: 
        _ear_vk_make_buf_stage(buf, desc, data, size); break;
    case EAR_BUF_UNIFORM:
    case EAR_BUF_STORAGE_PERSISTENT: 
        _ear_vk_make_buf_pers(buf, desc, data, size); break;
    default: eat_unreachable();
    }

    return buf;
}
void
ear_vk_delete_buffer(
    ear_vk_buffer* buf
    ) {
    switch (buf->type) {
    case EAR_BUF_VERTEX:
    case EAR_BUF_INDEX:
    case EAR_BUF_STORAGE_STAGING:
        _ear_vk_del_buf_stage(buf); break;
    case EAR_BUF_STORAGE_PERSISTENT:
    case EAR_BUF_UNIFORM:
        _ear_vk_del_buf_pers(buf); break;
    default: eat_unreachable();
    }

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
            1, &buf->gen.buffer, 
            &(VkDeviceSize){0}
            );
        break;
    case EAR_BUF_INDEX:
        vkCmdBindIndexBuffer(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            buf->gen.buffer,
            0,
            VK_INDEX_TYPE_UINT32
            );
        break;
    case EAR_BUF_UNIFORM:
    case EAR_BUF_STORAGE_PERSISTENT:
    case EAR_BUF_STORAGE_STAGING:
        vkCmdBindDescriptorSets(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            _ear_vk_cur_pipeline->layout,
            0,
            1,
            &buf->cur_set->sets[_ear_vk_cur_frame],
            0,
            NULL
            );
        break;
    default: eat_unreachable();
    }
}

void
ear_vk_update_buffer(
    ear_vk_buffer* buf,
    void* data,
    uint32_t size
    ) {
    switch (buf->type) {
    case EAR_BUF_UNIFORM: 
    case EAR_BUF_STORAGE_PERSISTENT:
        _ear_vk_update_buf_pers(buf, data, size); return;
    case EAR_BUF_VERTEX:
    case EAR_BUF_INDEX:
    case EAR_BUF_STORAGE_STAGING:
        eat_error("updating vertex/index buffers is unsupported!");
    }

    eat_unreachable();
}

void
ear_vk_attach_buffer_bindset(
    ear_vk_buffer* buf,
    ear_vk_bindset* set
    ) {
    buf->cur_set = set;
}
