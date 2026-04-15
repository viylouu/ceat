#include "bindset.h"
//#include "cutil.h"

#include <stdlib.h>

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
    const uint32_t* offsets,
    uint32_t offset_amt
    ) {
    ear_vk_bindset* set = _set;

    vkCmdBindDescriptorSets(
        _ear_vk_comm_buffers[_ear_vk_cur_frame],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _ear_vk_cur_pipeline->layout,
        slot,
        1,
        &set->sets[_ear_vk_cur_frame],
        offset_amt,
        offset_amt == 0? NULL : offsets
        );
}
