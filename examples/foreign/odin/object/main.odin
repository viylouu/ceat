#+feature using-stmt
package main

import "core:fmt"

import "../../../../bindings/odin/eat"
import "../../../../bindings/odin/eau"
import "../../../../bindings/odin/ear"

MyObject :: struct{
    frame: i32
}

object_init :: proc(obj: ^eau.Object(MyObject)) {
    using obj.user.data

    frame = 0

    fmt.printf("hello world :D\n")
}

object_tick :: proc(obj: ^eau.Object(MyObject)) {
    using obj.user.data
    fmt.printf("%d\n", frame)
}

object_draw :: proc(obj: ^eau.Object(MyObject)) {
    using obj.user.data
    ear.rect(f32(frame % (256-64)) * 2, f32((frame / 256) % 4) * 64 * 2, 128,128, [3]f32{ f32(frame % 256)/256., f32((frame/256)%4) / 4., f32(frame % (256-64)) / (256.-64) })
}

object_stop :: proc(obj: ^eau.Object(MyObject)) {
    fmt.printf("goodbye world :(\n")
}

main :: proc() {
    eat.init(
        "window",
        1600, 900,
        { vsync = false }
        )
    defer eat.stop()

    obj := eau.create_object(MyObject{})
    defer obj->delete()

    eau.init_objects()
    defer eau.stop_objects()

    for eat.frame() {
        eau.try_tick_objects()
        eau.draw_objects()

        fmt.printf("%.3f FPS\n", 1./eat.delta64)
    }
}
