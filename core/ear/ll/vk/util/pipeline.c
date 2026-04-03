#include "pipeline.h"
#include "../../../../cutil.h"

VkFormat
_ear_vk_convert_prim_fmt(
    ear_primitive_type type,
    uint32_t comps,
    bool norm
    ) {
    switch (type) {
    case EAR_PRIM_FLOAT:
        eat_assert(!norm, "floats cannot be normalized!");

        switch (comps) {
        case 1: return VK_FORMAT_R32_SFLOAT;
        case 2: return VK_FORMAT_R32G32_SFLOAT;
        case 3: return VK_FORMAT_R32G32B32_SFLOAT;
        case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        eat_unreachable();
    case EAR_PRIM_INT:
        eat_assert(!norm, "32 bit integers cannot be normalized!");

        switch (comps) {
        case 1: return VK_FORMAT_R32_SINT;
        case 2: return VK_FORMAT_R32G32_SINT;
        case 3: return VK_FORMAT_R32G32B32_SINT;
        case 4: return VK_FORMAT_R32G32B32A32_SINT;
        }
        eat_unreachable();
    }

    eat_unreachable();
}
VkPrimitiveTopology
_ear_vk_convert_topology(
    ear_topology top
    ) {
    switch (top) {
    case EAR_TOP_TRIS:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case EAR_TOP_LINES: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }

    eat_unreachable();
}
VkPolygonMode
_ear_vk_convert_fill(
    ear_fill_mode fill
    ) {
    switch (fill) {
    case EAR_FILL_FILL: return VK_POLYGON_MODE_FILL;
    case EAR_FILL_LINE: return VK_POLYGON_MODE_LINE;
    }

    eat_unreachable();
}
VkCullModeFlags
_ear_vk_convert_cull(
    ear_cull_mode cull
    ) {
    switch (cull) {
    case EAR_CULL_NONE:  return VK_CULL_MODE_NONE;
    case EAR_CULL_BACK:  return VK_CULL_MODE_BACK_BIT;
    case EAR_CULL_FRONT: return VK_CULL_MODE_FRONT_BIT;
    }

    eat_unreachable();
}
VkFrontFace
_ear_vk_convert_front(
    ear_front_face front
    ) {
    switch (front) {
    case EAR_FRONT_CW:  return VK_FRONT_FACE_CLOCKWISE;
    case EAR_FRONT_CCW: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    eat_unreachable();
}
VkBlendFactor
_ear_vk_convert_blend_fac(
    ear_blend_factor fac
    ) {
    switch (fac) {
    case EAR_FAC_ONE:           return VK_BLEND_FACTOR_ONE;
    case EAR_FAC_ZERO:          return VK_BLEND_FACTOR_ZERO;
    case EAR_FAC_SRC_COLOR:     return VK_BLEND_FACTOR_SRC_COLOR;
    case EAR_FAC_DST_COLOR:     return VK_BLEND_FACTOR_DST_COLOR;
    case EAR_FAC_INV_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case EAR_FAC_INV_DST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case EAR_FAC_SRC_ALPHA:     return VK_BLEND_FACTOR_SRC_ALPHA;
    case EAR_FAC_DST_ALPHA:     return VK_BLEND_FACTOR_DST_ALPHA;
    case EAR_FAC_INV_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case EAR_FAC_INV_DST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    }

    eat_unreachable();
}
VkBlendOp
_ear_vk_convert_blend_op(
    ear_blend_op op
    ) {
    switch (op) {
    case EAR_OP_ADD:          return VK_BLEND_OP_ADD;
    case EAR_OP_SUBTRACT:     return VK_BLEND_OP_SUBTRACT;
    case EAR_OP_REV_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case EAR_OP_MIN:          return VK_BLEND_OP_MIN;
    case EAR_OP_MAX:          return VK_BLEND_OP_MAX;
    }

    eat_unreachable();
}
