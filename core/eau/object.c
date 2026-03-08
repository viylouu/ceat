#include "object.h"
#include "../cutil.h"

#include "../eaw/eaw.h"

eau_object_ll* eau_object_ll_first = NULL;
eau_object_ll* eau_object_ll_last = NULL;

float tickrate = 1.f / 240;


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

        .use_global_tickrate = true,
        .delta = tickrate,
        .last_time = eaw_time,
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

// tickrate:
// - measured in delta time
// - if its 0, objects update each frame instead of on a fixed framerate

void
eau_set_object_tickrates(
    float delta
    ) {
    tickrate = delta;

    if (eau_object_ll_first == NULL) return;

    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next) {
        if (!item->obj->use_global_tickrate) continue;
        item->obj->delta = delta;
    }
}

void
eau_set_object_tickrate(
    eau_object* obj,
    float delta
    ) {
    obj->use_global_tickrate = false;
    obj->delta = delta;
}

void
eau_reset_object_tickrate(
    eau_object* obj
    ) {
    obj->use_global_tickrate = true;
    obj->delta = tickrate;
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
    if (eaw_time - obj->last_time < obj->delta) return;
    obj->last_time = eaw_time;
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

void
eau_draw_objects(
    void
    ) {
    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_draw_object(item->obj);
}

void
eau_try_tick_objects(
    void
    ) {
    for (eau_object_ll* item = eau_object_ll_first; item != NULL; item = item->next)
        eau_try_tick_object(item->obj);
}
