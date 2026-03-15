#include "object.h"
#include "../cutil.h"

#include "../ear/text.h"

eau_object_ll* eau_object_ll_first = NULL;
eau_object_ll* eau_object_ll_last = NULL;

float eau_tickrate = 1.f / 240;
double _eau_last_tick = 0;
bool eau_tick_this_frame;

void
_eau_arena_object_delete(
    void* obj
    ) { 
    eau_delete_object(obj); 
}

void
_eau_debug_object_window(
    void* obj,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eau_object*
eau_create_object(
    eau_object_desc desc,
    void* data,
    eau_arena* arena
    ) {
    eau_object* obj = malloc(sizeof(eau_object));
    *obj = (eau_object){
        .desc = desc,

        .data = data,

        .ll = (eau_object_ll){
            .obj = obj,
            .prev = eau_object_ll_last,
            .next = NULL,
            },

        .delta = eau_tickrate,

        .deb_obj = eat_debug_add_obj(
            obj,
            "object",
            _eau_debug_object_window,
            arena
            ),
        };

    if (eau_object_ll_last != NULL) eau_object_ll_last->next = &obj->ll;
    eau_object_ll_last = &obj->ll;
    if (eau_object_ll_first == NULL) eau_object_ll_first = &obj->ll;

    if (arena != NULL) eau_add_to_arena(arena, &obj->dest, obj, _eau_arena_object_delete);
    return obj;
}

void
eau_delete_object(
    eau_object* obj
    ) {
    eat_debug_remove_obj(obj->deb_obj);

    if (obj->ll.prev != NULL) obj->ll.prev->next = obj->ll.next;
    else eau_object_ll_first = obj->ll.next;

    if (obj->ll.next != NULL) obj->ll.next->prev = obj->ll.prev;
    else eau_object_ll_last = obj->ll.prev;

    if (obj->dest != NULL) obj->dest->data = NULL;
    free(obj);
}

// eau_tickrate:
// - measured in delta time
// - if its 0, objects update each frame instead of on a fixed framerate

void
eau_set_object_tickrate(
    float delta
    ) {
    eau_tickrate = delta;

    if (eau_object_ll_first == NULL) return;

    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next) item->obj->delta = delta;
}

void
eau_init_object(
    eau_object* obj
    ) {
    if (!obj->desc.init) return;
    obj->desc.init(obj);
}

void
eau_stop_object(
    eau_object* obj
    ) {
    if (!obj->desc.stop) return;
    obj->desc.stop(obj);
}

void
eau_draw_object(
    eau_object* obj
    ) {
    if (!obj->desc.draw) return;
    obj->desc.draw(obj);
}

void
eau_try_tick_object(
    eau_object* obj
    ) {
    if (!obj->desc.tick) return;
    if (debug.enabled && eat_debug_toggled) return;

    if (!eau_tick_this_frame) return;
    obj->desc.tick(obj);
}

void
eau_init_objects(
    void
    ) {
    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_init_object(item->obj);
}

void
eau_stop_objects(
    void
    ) {
    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_stop_object(item->obj);
}

typedef struct _eau_object_draw_arr_item{
    eau_object** arr;
    uint32_t arr_size;
    uint32_t arr_cap;

    int layer;
} _eau_object_draw_arr_item;

int _eau_object_draw_arr_cmp(const void* _a, const void* _b) {
    const _eau_object_draw_arr_item* a = _a;
    const _eau_object_draw_arr_item* b = _b;
    return a->layer - b->layer;
}

void
eau_draw_objects(
    void
    ) {
    _eau_object_draw_arr_item* arr;
    arr = malloc(sizeof(_eau_object_draw_arr_item));
    uint32_t arr_size = 0;

    int rendered = 0;

    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next) {
        int layer = item->obj->desc.render_layer;

        bool set = false;
        for (uint32_t i = 0; i < arr_size; ++i) {
            if (arr[i].layer != layer) continue;

            ++arr[i].arr_size;
            if (arr[i].arr_size > arr[i].arr_cap) {
                arr[i].arr_cap *= 2;
                arr[i].arr = realloc(arr[i].arr, sizeof(eau_object*) * arr[i].arr_cap);
            }

            arr[i].arr[arr[i].arr_size-1] = item->obj;

            set = true;
            break;
        }

        if (!set) {
            ++arr_size;
            arr = realloc(arr, sizeof(_eau_object_draw_arr_item) * arr_size);
            arr[arr_size-1] = (_eau_object_draw_arr_item){
                .arr = malloc(sizeof(eau_object*)*4),
                .arr_size = 1,
                .arr_cap = 4,
                .layer = layer,
                };
            arr[arr_size-1].arr[0] = item->obj;
        }
    }

    qsort(arr, arr_size, sizeof(_eau_object_draw_arr_item), _eau_object_draw_arr_cmp);

    for (uint32_t i = 0; i < arr_size; ++i) for (uint32_t j = 0; j < arr[i].arr_size; ++j) eau_draw_object(arr[i].arr[j]);

    for (uint32_t i = 0; i < arr_size; ++i) free(arr[i].arr);
    free(arr);
}

void
eau_try_tick_objects(
    void
    ) {
    if (!eau_tick_this_frame) return;
    if (debug.enabled && eat_debug_toggled) return;

    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_try_tick_object(item->obj);
}


void
_eau_debug_object_window(
    void* _obj,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    eau_object* obj = _obj;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "render layer: %d", obj->desc.render_layer);
    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    size_t l = snprintf(buf, sizeof(buf), "funcs:");
    bool first = true;
    if (obj->desc.init) {
        l += snprintf(buf + l, sizeof(buf) - l, " init");
        first = false;
    } if (obj->desc.stop) {
        l += snprintf(buf + l, sizeof(buf) - l, first? " stop" : ", stop");
        first = false;
    } if (obj->desc.tick) {
        l += snprintf(buf + l, sizeof(buf) - l, first? " tick" : ", tick");
        first = false;
    } if (obj->desc.draw) {
        l += snprintf(buf + l, sizeof(buf) - l, first? " draw" : ", draw");
        first = false;
    } if (first) snprintf(buf, sizeof(buf), "no funcs");

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

#define add_1(dimsize)
#define add_2(dimsize) , *(obj->desc.pos##dimsize[1])
#define add_3(dimsize) , *(obj->desc.pos##dimsize[1]), *(obj->desc.pos##dimsize[2])
#define add_1_s()
#define add_2_s() ", %.3f"
#define add_3_s() ", %.3f, %.3f"
#define pos(name, dimsize, amt) do { \
    if (obj->desc.pos##dimsize[0]) { \
        snprintf(buf, sizeof(buf), "%s: %.3f" add_##amt##_s(), name, *(obj->desc.pos##dimsize[0]) add_##amt(dimsize)); \
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT); \
        offy += off; \
    } \
} while(0)

    pos("pos", 2d, 2);
    pos("pos", 2d64, 2);
    pos("pos", 3d, 3);
    pos("pos", 3d64, 3);
    pos("rot", 2d, 1);
    pos("rot", 2d64, 1);
    pos("rot", 3d, 3);
    pos("rot", 3d64, 3);
}
