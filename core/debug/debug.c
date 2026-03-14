#include "debug.h"
#include "../cutil.h"

#include "../../eat.h"

eat_debug_theme debug_theme;

eat_debug_desc debug;

bool eat_debug_toggled = false;

eat_debug_ll_obj* eat_debug_ll_first = NULL;
eat_debug_ll_obj* eat_debug_ll_last = NULL;

int32_t selected = -1;

void
eat_debug_init(
    void
    ) {
    static const char tex_data[] = {
    #embed "verdana.ttf"
    };

    debug_theme = (eat_debug_theme){
        .font = ear_load_truetype_font(tex_data, sizeof(tex_data), NULL),

        .bg_col      = { .15, .15, .15, 1 },
        .text_col    = { 1,   1,   1,   1 },
        .but_col     = { .25, .25, .25, 1 },
        .sel_but_col = { .35, .35, .35, 1 },
    };
}

void
eat_debug_stop(
    void
    ) {
    ear_delete_font(debug_theme.font);
}

void
eat_debug_try_do(
    void
    ) {
    if (eaw_is_key_pressed(debug.key)) {
        eat_debug_toggled = !eat_debug_toggled;
        selected = -1;
    } if (!eat_debug_toggled) return;

    ear_rect(0,0, eaw_window_width, 16, debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);
    ear_text(debug_theme.font, "yummy debug", 0,0, 14, debug_theme.text_col, EAU_ALIGN_TOP_LEFT);

    float x = 4; 
    float y = eaw_window_height - 256;

    ear_rect(0, y, eaw_window_width, 256, debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);
    ear_mask(0, y, eaw_window_width, 256);

    y += 4;

    uint32_t prev_sel = selected;
    eat_debug_ll_obj* sel_obj;

    uint32_t i = 0;
    for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s %d", it->type_name, i);

        float w;
        ear_text_size(debug_theme.font, buf, 14, &w,NULL);

        bool highlight = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y, w+8,16 });
        bool sel = i == selected || highlight;

        ear_rect(x,y, w+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2, w+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

        if (highlight && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) selected = i;
        if (selected == i) sel_obj = it;

        x += w + 12;
        if (x + 4 + w + 12 > eaw_window_width) {
            y += 20;
            x = 4;
        }

        ++i;
    }

    x = 4;
    y = eaw_window_height - 256 + 4;

    i = 0;
    for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s %d", it->type_name, i);

        float w;
        ear_text_size(debug_theme.font, buf, 14, &w,NULL);
        ear_text(debug_theme.font, buf, x+4, y, 14, debug_theme.text_col, EAU_ALIGN_TOP_LEFT);

        x += w + 12;
        if (x + 4 > eaw_window_width) {
            y += 20;
            x = 4;
        }

        ++i;
    }

    if (selected == -1) {
        ear_tex(_eat_screen_color, 0,16, eaw_window_width, eaw_window_height - 256 - 16, 0,0,eat_width,-eat_height, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);
        return;
    }

    ear_rect(0,16, eaw_window_width/2.f, eaw_window_height - 256 - 16, debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);
    ear_mask(4,20, eaw_window_width/2.f - 8, eaw_window_height - 256 - 16 - 8);

    sel_obj->debug_window(sel_obj->data, 4,20, eaw_window_width/2.f - 8, eaw_window_height - 256 - 16 - 8, debug_theme, &selected);

    if (prev_sel == selected && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) selected = -1;

    ear_tex(_eat_screen_color, eaw_window_width / 2.f, 16, eaw_window_width / 2.f, eaw_window_height - 256 - 16, 0,0,eat_width,-eat_height, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);
}


eat_debug_ll_obj*
eat_debug_add_obj(
    void* data,
    char* name,
    void (*debug_window)(void* obj, float x, float y, float w, float h, eat_debug_theme theme, int32_t* sel)
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

void
eat_debug_get_screen_size(
    int32_t* width, 
    int32_t* height
    ) {
    if (!eat_debug_toggled) {
        if (width) *width = eaw_window_width;
        if (height) *height = eaw_window_height;
        return;
    }

    if (selected == -1) {
        if (width) *width = eaw_window_width;
        if (height) *height = eaw_window_height - 16 - 256;
    } else {
        if (width) *width = eaw_window_width / 2;
        if (height) *height = eaw_window_height - 16 - 256;
    }
}
