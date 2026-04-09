#include "bindset.h"
#include "cutil.h"

#include "../util/bindset.h"
#include "../init/device_log.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "pipeline.h"

void*
ear_vk_create_bindset(
    ear_bindset_desc desc
    ) {
    ear_vk_bindset* set = malloc(sizeof(ear_vk_bindset));
    set->desc = desc;

    _ear_vk_make_bindset_pool(set, desc);
    _ear_vk_make_bindset_lay(set, desc);
    _ear_vk_make_bindset_sets(set, desc);

    return set;
}
void
ear_vk_delete_bindset(
    void* _set
    ) {
    ear_vk_bindset* set = _set;

    vkDestroyDescriptorSetLayout(_ear_vk_device, set->lay, NULL);
    vkDestroyDescriptorPool(_ear_vk_device, set->pool, NULL);

    free(set);
}

void
ear_vk_bind_bindset(
    void* _set,
    uint32_t slot,
    uint32_t offsets[],
    uint32_t offset_amt
    ) {
    ear_vk_bindset* set = _set;

    if (offset_amt != 0) eat_assert(offset_amt == set->dynamics, "offset amt must be amount of offsettable bindings!");

    uint32_t offamt = offset_amt > 0? offset_amt : set->dynamics;
    uint32_t offs[offamt == 0? 1 : offamt];
    if (offamt > 0) {
        uint32_t bind_i = 0;
        for (uint32_t i = 0; i < offset_amt; ++i) {
            while (bind_i < set->desc.binding_amt &&
                   set->desc.bindings[bind_i].type == EAR_BIND_TEXTURE2D) 
                ++bind_i;
            eat_assert(bind_i < set->desc.binding_amt, "more offsets sent than there are offsettable bindings!");

            ear_buffer* buf = set->desc.bindings[bind_i].object; 
            eat_assert(buf != NULL, "binding buffer cannot be null, especially on binding!");

            offs[i] = offset_amt == 0? 0 : (offsets[i] * buf->desc.chunk_size * buf->desc.stride);

            ++bind_i;
        }
    }

    vkCmdBindDescriptorSets(
        _ear_vk_comm_buffers[_ear_vk_cur_frame],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _ear_vk_cur_pipeline->layout,
        slot,
        1,
        &set->sets[_ear_vk_cur_frame],
        offamt,
        offamt == 0? NULL : offs
        );
}
