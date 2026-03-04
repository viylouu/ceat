package main

import "core:fmt"

import "../../../bindings/odin/eat"
import "../../../bindings/odin/ear"

main :: proc() {
    eat.init(
        "hello from odin!",
        800, 600,
        { vsync = false }
        )
    defer eat.stop()

    for eat.frame() {
        ear.clear([3]f32{ .2,.4,.3 })

        ear.rect(0,0, 64,64, [3]f32{ 1,0,0 })

        fmt.printf("%.3f FPS\n", 1/eat.delta)
    }
}
