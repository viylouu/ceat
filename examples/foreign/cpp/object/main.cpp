#include "../../../../bindings/cpp/eat.hpp"
#include <iostream>

class MyObject : public eau::Object<MyObject> {
    int frame;

public:
    MyObject() : eau::Object<MyObject>(nullptr) {}
    ~MyObject() {}

private:
    void on_init() {
        printf("hello world :D\n");

        frame = 0;
    }

    void on_tick() {
        printf("%d\n", this->frame++);
    }

    void on_draw() {
        ear::rect(this->frame % (256-64) * 2, ((this->frame / 256) % 4) * 64 * 2, 128,128, (std::array<float,3>){ (this->frame % 256)/256.f, ((this->frame/256)%4) / 4.f, (this->frame % (256-64)) / (256.f-64) });
    }

    void on_stop() {
        printf("goodbye world :(\n");
    }
};

int main(void) {
    eat::init(
        "object",
        512, 512,
        { .vsync = true }
        );

    MyObject obj = MyObject();

    eau::init_objects();

    while (eat::frame()) {
        eau::try_tick_objects();
        eau::draw_objects();

        //std::cout << 1./eat::delta64 << " FPS" << std::endl;
    }

    eau::stop_objects();

    eat::stop();

    return 0;
}
