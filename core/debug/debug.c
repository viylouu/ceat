#include "debug.h"
#include "../cutil.h"

#include "../../eat.h"
#include "core/eaw/window.h"

eat_debug_desc debug;
ear_font* debug_font;

bool eat_debug_toggled = false;

eat_debug_ll_obj* eat_debug_ll_first = NULL;
eat_debug_ll_obj* eat_debug_ll_last = NULL;

void
eat_debug_init(
    void
    ) {
    static const char tex_data[] = {
    #embed "verdana.ttf"
    };

    debug_font = ear_load_truetype_font(tex_data, sizeof(tex_data), NULL);
}

void
eat_debug_stop(
    void
    ) {
    ear_delete_font(debug_font);
}

void
eat_debug_try_do(
    void
    ) {
    if (eaw_is_key_pressed(debug.key)) eat_debug_toggled = !eat_debug_toggled;
    if (!eat_debug_toggled) return;

    float bg_col[4] = { .15,.15,.15, 1 };
    float but_col[4] = { .25,.25,.25, 1 };
    float text_col[4] = { 1,1,1, 1 };

    ear_rect(0,0, eaw_window_width, 16, bg_col, EAU_ALIGN_TOP_LEFT);
    ear_text(debug_font, "yummy debug", 0,0, 14, text_col, EAU_ALIGN_TOP_LEFT);

    float x = 4; 
    float y = eaw_window_height - 256;

    ear_rect(0, y, eaw_window_width, 256, bg_col, EAU_ALIGN_TOP_LEFT);
    ear_mask(0, y, eaw_window_width, 256);

    y += 4;

    uint32_t i = 0;
    for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s %d", it->type_name, i);

        float w;
        ear_text_size(debug_font, buf, 14, &w,NULL);

        ear_rect(x,y, w+8, 16, but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2, w+4, 12, bg_col, EAU_ALIGN_TOP_LEFT);

        ear_text(debug_font, buf, x+4, y, 14, text_col, EAU_ALIGN_TOP_LEFT);

        x += w + 12;
        if (x + 4 > eaw_window_width) {
            y += 20;
            x = 4;
        }

        ++i;
    }

    ear_mask(0,0, eaw_window_width,eaw_window_height);
}


eat_debug_ll_obj*
eat_debug_add_obj(
    void* data,
    char* name,
    void (*debug_window)(void* obj, float x, float y, float w, float h)
    ) {
    eat_debug_ll_obj* obj = malloc(sizeof(eat_debug_ll_obj));
    *obj = (eat_debug_ll_obj){
        .data = data,
        .type_name = name,
        .debug_window = debug_window,

        .prev = eat_debug_ll_last,
        .next = NULL,
        };

    if (eat_debug_ll_last != NULL) eat_debug_ll_last->next = obj;
    eat_debug_ll_last = obj;
    if (eat_debug_ll_first == NULL) eat_debug_ll_first = obj;

    return obj;
}

void
eat_debug_remove_obj(
    eat_debug_ll_obj* obj
    ) {
    if (obj->prev != NULL) obj->prev->next = obj->next;
    else eat_debug_ll_first = obj->next;

    if (obj->next != NULL) obj->next->prev = obj->prev;
    else eat_debug_ll_last = obj->prev;

    free(obj);
}
