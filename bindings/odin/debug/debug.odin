package debug

import "../eaw"

when ODIN_OS == .Windows {
    foreign import ceat {
        "../../../build/ceat.lib",
    }
} else when ODIN_OS == .Linux {
    foreign import ceat {
        "../../../build/libceat.a",
    }
} else {
    // unsupported os!
}

ConsoleDesc :: struct{
    enabled: bool,
    key: eaw.Key,
    command_solver: proc "c" (command: cstring)
}

DebugDesc :: struct{
    enabled: bool,
    key: eaw.Key,
}

LLObj :: struct{
    data: rawptr,
    name: cstring,
    debug_window: proc "c" (obj: rawptr, x,y,w,h: f32),

    next: ^LLObj,
    prev: ^LLObj,
}

@(default_calling_convention="c")
foreign ceat {
    @(link_name="eat_debug_add_obj") add_obj :: proc(data: rawptr, name: cstring, debug_window: proc "c" (obj: rawptr, x,y,w,h: f32)) -> ^LLObj ---
    @(link_name="eat_debug_remove_obj") remove_obj :: proc(obj: ^LLObj) ---
}
