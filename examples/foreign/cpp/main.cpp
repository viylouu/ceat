#include "../../../bindings/cpp/eat.hpp"
#include <iostream>

int main(void) {
    eat::init(
        "hello from c++!",
        800, 600,
        { .vsync = false }
        );

    while (eat::frame()) {
        ear::clear({ .2,.4,.3 });

        ear::rect(0,0, 64,64, { 1,0,0,1 });

        std::cout << 1./eat::delta64 << " FPS" << std::endl;
    }

    eat::stop();

    return 0;
}
