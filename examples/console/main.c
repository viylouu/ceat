#include "../../eat.h"

#include <stdio.h>
#include <string.h>

void solver(char* input) {
    if (!strcmp(input, "hello")) {
        printf("hello :)\n");
    }
}

int main(void) {
    eat_init("console", 1600,900, (eat_init_opts){
        .console = (eat_console_desc){
            .enabled = true,
            .key = EAW_KEY_ENTER,
            .command_solver = solver,
            },
        });

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        ear_rect(0,0, 64,64, (float[4]){ 1,0,0,1 }, EAU_ALIGN_TOP_LEFT);
    }

    eat_exit();

    return 0;
}
