#include "../../../../bindings/cpp/eat.hpp"
#include <iostream>

int main(void) {
    eat::init(
        "window",
        1600, 900,
        { .vsync = false }
        );

    while (eat::frame()) {
        ear::clear({ .2,.4,.3 });

        ear::rect(0,0, 64,64, { 1,0,0,1 });

        std::cout << 1./eat::delta64 << " FPS" << std::endl;
    }

    eat::exit();

    return 0;
}
