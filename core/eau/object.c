#include "object.h"
#include "../cutil.h"

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

eau_object*
eau_create_object(
    eau_object_desc desc,
    void* data,
    eau_arena* arena
    ) {
    eau_object* obj = malloc(sizeof(eau_object));
    *obj = (eau_object){
        desc = desc,

        .data = data,

        .ll = (eau_object_ll){
            .obj = obj,
            .prev = eau_object_ll_last,
            .next = NULL,
            },

        .delta = eau_tickrate,
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
    //
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

    int layer;
} _eau_object_draw_arr_item;

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

        if (arr_size == 0) {
            arr[0] = (_eau_object_draw_arr_item){
                .arr = malloc(sizeof(eau_object*)),
                .arr_size = 1,
                .layer = layer,
                };
            arr[0].arr[0] = item->obj;
            ++arr_size;
        } else {
            bool set = false;
            for (uint32_t i = 0; i < arr_size; ++i) {
                if (arr[i].layer != layer) continue;

                ++arr[i].arr_size;
                arr[i].arr = realloc(arr[i].arr, sizeof(eau_object*) * arr[i].arr_size);
                arr[i].arr[arr[i].arr_size-1] = item->obj;
            }

            if (!set) {
                ++arr_size;
                arr = realloc(arr, sizeof(_eau_object_draw_arr_item) * arr_size);
                arr[arr_size-1] = (_eau_object_draw_arr_item){
                    .arr = malloc(sizeof(eau_object*)),
                    .arr_size = 1,
                    .layer = layer,
                    };
                arr[arr_size-1].arr[0] = item->obj;
            }
        }

        eau_draw_object(item->obj);
    }

    for (uint32_t i = 0; i < arr_size; ++i) free(arr[i].arr);
    free(arr);
}

void
eau_try_tick_objects(
    void
    ) {
    if (!eau_tick_this_frame) return;

    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_try_tick_object(item->obj);
}
