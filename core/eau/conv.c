#include "conv.h"
//#include "../cutil.h"

eau_rect
eau_rect_topleftify(
    eau_rect rect
    ) {
    float offx; float offy;

    switch (rect.align) {
    case EAU_ALIGN_TOP_LEFT:  offx = 0;  offy = 0;  break;
    case EAU_ALIGN_TOP:       offx = .5; offy = 0;  break;
    case EAU_ALIGN_TOP_RIGHT: offx = 1;  offy = 0;  break;
    case EAU_ALIGN_MID_LEFT:  offx = 0;  offy = .5; break;
    case EAU_ALIGN_MID:       offx = .5; offy = .5; break;
    case EAU_ALIGN_MID_RIGHT: offx = 1;  offy = .5; break;
    case EAU_ALIGN_BOT_LEFT:  offx = 0;  offy = 1;  break;
    case EAU_ALIGN_BOT:       offx = .5; offy = 1;  break;
    case EAU_ALIGN_BOT_RIGHT: offx = 1;  offy = 1;  break;
    }

    rect.x -= rect.w * offx;
    rect.y -= rect.h * offy;
    rect.align = EAU_ALIGN_TOP_LEFT;

    return rect;
}

eau_rect
eau_rect_alignify(
    eau_rect rect,
    eau_align align
    ) {
    rect = eau_rect_topleftify(rect);

    float offx; float offy;

    switch (align) {
    case EAU_ALIGN_TOP_LEFT:  offx = 0;  offy = 0;  break;
    case EAU_ALIGN_TOP:       offx = .5; offy = 0;  break;
    case EAU_ALIGN_TOP_RIGHT: offx = 1;  offy = 0;  break;
    case EAU_ALIGN_MID_LEFT:  offx = 0;  offy = .5; break;
    case EAU_ALIGN_MID:       offx = .5; offy = .5; break;
    case EAU_ALIGN_MID_RIGHT: offx = 1;  offy = .5; break;
    case EAU_ALIGN_BOT_LEFT:  offx = 0;  offy = 1;  break;
    case EAU_ALIGN_BOT:       offx = .5; offy = 1;  break;
    case EAU_ALIGN_BOT_RIGHT: offx = 1;  offy = 1;  break;
    }

    rect.x += rect.w * offx;
    rect.y += rect.h * offy;
    rect.align = align;

    return rect;
}

eau_rect
eau_rect_scale_to_fit(
    eau_rect rect,
    eau_rect fit
    ) {
    eau_align origalign = rect.align;

    rect = eau_rect_topleftify(rect);
    fit = eau_rect_topleftify(fit);

    float rectaspect = rect.w / rect.h;
    float fitaspect = fit.w / fit.h;

    float width = fit.w;
    float height = fit.h;
    if (fitaspect > rectaspect) width = height * rectaspect;
    else height = width / rectaspect;

    rect.x = (fit.w - width) * .5;
    rect.y = (fit.h - height) * .5;
    rect.w = width;
    rect.h = height;
    rect.x += fit.x;
    rect.y += fit.y;

    rect = eau_rect_alignify(rect, origalign);
    return rect;
}

eau_rect
eau_rect_space_convert(
    eau_rect rect,
    eau_rect orig,
    eau_rect new
    ) {
    eau_align origalign = rect.align;

    rect = eau_rect_topleftify(rect);
    orig = eau_rect_topleftify(orig);
    new = eau_rect_topleftify(new);

    float scalex = new.w / orig.w;
    float scaley = new.h / orig.h;

    rect.x = new.x + (rect.x - orig.x) * scalex;
    rect.y = new.y + (rect.y - orig.y) * scaley;
    rect.w *= scalex;
    rect.h *= scaley;

    rect = eau_rect_alignify(rect, origalign);
    return rect;
}

void
eau_point_space_convert(
    float x, float y,
    eau_rect orig,
    eau_rect new,
    float* outx, float* outy
    ) {
    eau_rect rect = (eau_rect){ x,y,0,0, EAU_ALIGN_TOP_LEFT };
    rect = eau_rect_space_convert(rect, orig, new);
    if (outx) *outx = rect.x;
    if (outy) *outy = rect.y;
}
