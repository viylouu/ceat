#include "buffer.h"
#include "../../../../cutil.h"

#include "../util/buffer.h"

#include "../init/device_log.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../vk.h"
#include "../eng/pipeline.h"

ear_vk_buffer*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    ear_vk_buffer* buf = malloc(sizeof(ear_vk_buffer));
    buf->type = desc.type;
    buf->stride = desc.stride;

    switch (desc.type) {
    case EAR_BUF_VERTEX: case EAR_BUF_INDEX: _ear_vk_make_buf_vi(buf, desc, data, size); break;
    case EAR_BUF_UNIFORM: _ear_vk_make_buf_u(buf, desc, data, size); break;
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
        vkDestroyBuffer(_ear_vk_device, buf->gen.buffer, NULL);
        vkFreeMemory(_ear_vk_device, buf->gen.memory, NULL);
        break;
    case EAR_BUF_UNIFORM:
        for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
            vkDestroyBuffer(_ear_vk_device, buf->ubuf.buffers[i], NULL);
            vkFreeMemory(_ear_vk_device, buf->ubuf.memories[i], NULL);
        }

        vkDestroyDescriptorPool(_ear_vk_device, buf->ubuf.pool, NULL);
        vkDestroyDescriptorSetLayout(_ear_vk_device, buf->ubuf.layout, NULL);
        break;
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
        vkCmdBindDescriptorSets(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            _ear_vk_cur_pipeline->layout,
            0,
            1,
            &buf->ubuf.sets[_ear_vk_cur_frame],
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
    case EAR_BUF_UNIFORM: _ear_vk_update_buf_u(buf, data, size); return;
    case EAR_BUF_VERTEX:
    case EAR_BUF_INDEX:
    case EAR_BUF_STORAGE: 
        eat_error("updating vertex/index/storage buffers is unsupported!");
    }

    eat_unreachable();
}
