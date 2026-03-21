#include "vk.h"
#include "../../../cutil.h"

#include "validation.h"
#include "instance.h"

void 
ear_vk_init(
    void
    ) {
    eat_assert(!_ear_vk_validation || _ear_vk_validation_support(), "validation layers unavailable!");

    _ear_vk_create_instance();
}
void
ear_vk_exit(
    void
    ) {
    _ear_vk_delete_instance();
}
