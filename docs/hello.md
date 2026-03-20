## to run hello world build and run this with cmake?
```c
#include "../../eat.h" //engine

#include <stdio.h> //printf

int main(void) {
    // make a window with title "input", 1600x900, default options
    eat_init("input", 1600,900, (eat_init_opts){});
    // main loop - runs once per frame
    while (eat_frame()) {
        printf("hello world?"); //prints to console every frame
    }
    //close window
    eat_stop();
    return 0;
}
```
