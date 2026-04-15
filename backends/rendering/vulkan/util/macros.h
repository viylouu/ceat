#pragma once

#define get_vk(type, var) \
    ((ear_vk_##type*)var->vk)

// temp tests

#define TEMP_test_pass_cmd() \
    eat_warn("inpass: %d, incmd: %d, frame: %d, img: %d\n", _ear_vk_in_pass, _ear_vk_commbuf_rec, _ear_vk_cur_frame, _ear_vk_cur_img_index)
