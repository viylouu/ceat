## to run hello world build and run this with cmake?

#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("input", 1600,900, (eat_init_opts){});
    while (eat_frame()) {
        printf("hello world?");
    }
    eat_stop();

    return 0;
}
