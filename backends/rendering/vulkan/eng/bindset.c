#include "bindset.h"
//#include "cutil.h"

#include <stdlib.h>

#include "../util/bindset.h"
#include "../init/device_log.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "pipeline.h"
//#include "texture.h"

/*
static void
_ear_vk_refresh_texture_bindings_for_frame(
    ear_vk_bindset* set,
    uint32_t frame
    ) {
    for (uint32_t b = 0; b < set->desc.binding_amt; ++b) {
        if (set->desc.bindings[b].type != EAR_BIND_TEXTURE2D) continue;

        ear_texture*    tex  = set->desc.bindings[b].object;
        ear_vk_texture* vtex = tex->vk;

        VkDescriptorImageInfo imageinfo = {
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .imageView   = vtex->imgview,
            .sampler     = vtex->samp,
            };

        VkWriteDescriptorSet descwrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = NULL,

            .dstSet          = set->sets[frame],
            .dstBinding      = set->desc.bindings[b].binding,
            .dstArrayElement = 0,

            .descriptorType  = _ear_vk_convert_desc_type(set->desc.bindings[b].type),
            .descriptorCount = 1,

            .pImageInfo = &imageinfo,
            };

        vkUpdateDescriptorSets(_ear_vk_device, 1, &descwrite, 0, NULL);
    }
}
*/

void*
ear_vk_create_bindset(
    ear_bindset_desc desc
    ) {
    ear_vk_bindset* set = malloc(sizeof(ear_vk_bindset));
    set->desc = desc;

    _ear_vk_make_bindset_pool(set, desc);
    _ear_vk_make_bindset_lay(set, desc);
    _ear_vk_make_bindset_sets(set);

    _ear_vk_update_bindset_sets(set, desc);

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
ear_vk_update_bindset(
    void* _set,
    ear_bindset_desc desc
    ) {
    ear_vk_bindset* set = _set;
    _ear_vk_update_bindset_sets(set, desc);
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
