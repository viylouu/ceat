#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct my_object{
    int frame;
} my_object;

void object_init(eau_object* obj) {
    my_object* this = obj->data;
    printf("hello world :D\n");

    this->frame = 0;
}

void object_tick(eau_object* obj) {
    my_object* this = obj->data;
    printf("%d\n", this->frame++);
}

void object_draw(eau_object* obj) {
    my_object* this = obj->data;
    ear_rect(this->frame % (256-64) * 2, ((this->frame / 256) % 4) * 64 * 2, 128,128, (float[4]){ (this->frame % 256)/256.f,((this->frame/256)%4) / 4.f, (this->frame % (256-64)) / (256.f-64),1 });
}

void object_stop(eau_object* obj) {
    printf("goodbye world :(\n");
}

int main(void) {
    eat_init("object", 512,512, (eat_init_opts){});

    my_object* obj_data = malloc(sizeof(my_object));
    eau_object* obj = eau_create_object((eau_object_desc){
            .init = object_init,
            .stop = object_stop,
            .tick = object_tick,
            .draw = object_draw,
        }, obj_data, NULL);

    eau_init_objects();

    while (eat_frame()) {
        //ear_clear((float[3]){ .2f, .4f, .3f });

        eau_try_tick_objects();
        eau_draw_objects();

        //printf("%.3f FPS\n", 1./eat_delta64);
    }

    eau_stop_objects();

    eau_delete_object(obj);
    free(obj_data);

    eat_stop();

    return 0;
}

