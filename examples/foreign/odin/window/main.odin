package main

import "core:fmt"

import "../../../../bindings/odin/eat"
import "../../../../bindings/odin/eaw"
import "../../../../bindings/odin/ear"

main :: proc() {
    eat.init(
        "window",
        1600, 900,
        { vsync = false }
        )
    defer eat.exit()

    ear.clear_color(.2,.4,.3, 1)
    for eat.frame() {
        ear.rect(0,0, 64,64, [3]f32{ 1,0,0 })

        fmt.printf("%.3f FPS\n", 1./eat.delta64)
    }
}
