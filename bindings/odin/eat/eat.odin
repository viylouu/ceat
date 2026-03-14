package eat

import "../debug"
import "../eaw"

when ODIN_OS == .Windows {
    foreign import ceat {
        "../../../build/ceat.lib",
        "system:opengl32",
        "system:glfw3",
    }
} else when ODIN_OS == .Linux {
    foreign import ceat {
        "../../../build/libceat.a",
        "system:GL",
        "system:GLX",
        "system:EGL",
        "system:glfw",
    }
} else {
    // unsupported os!
}



_init_opts :: struct{
    vsync: bool,
    console: debug.ConsoleDesc,
    debug: debug.DebugDesc,
}

@(default_calling_convention="c")
foreign ceat {
    @(link_name="eat_width")  width: i32
    @(link_name="eat_height") height: i32
    @(link_name="eat_time")  time: f32
    @(link_name="eat_delta") delta: f32
    @(link_name="eat_time64")  time64: f64
    @(link_name="eat_delta64") delta64: f64

    @(link_name="eat_init") _init :: proc(title: cstring, width,height: i32, opts: _init_opts) ---
    @(link_name="eat_stop") stop :: proc() ---
    @(link_name="eat_frame") _frame :: proc() -> bool ---
}


InitOpts :: struct{
    vsync: Maybe(bool),
    console: Maybe(debug.ConsoleDesc),
    debug: Maybe(debug.DebugDesc),
}

init :: proc(title: cstring, width,height: i32, opts: InitOpts) {
    _init(
        title,
        width, height,
        _init_opts{
            vsync = opts.vsync.? or_else true,
            console = opts.console.? or_else debug.ConsoleDesc{ enabled = false },
            debug = opts.debug.? or_else debug.DebugDesc{ enabled = false },
            },
        );
}

frame :: proc() -> bool {
    res := _frame()

    eaw.mouse   = { eaw._mouse_x,   eaw._mouse_y }
    eaw.mouse64 = { eaw._mouse_x64, eaw._mouse_y64 }
    eaw.mouse_delta   = { eaw._mouse_delta_x,   eaw._mouse_delta_y }
    eaw.mouse_delta64 = { eaw._mouse_delta_x64, eaw._mouse_delta_y64 }
    eaw.mouse_scroll   = { eaw._mouse_scroll_x,   eaw._mouse_scroll_y }
    eaw.mouse_scroll64 = { eaw._mouse_scroll_x64, eaw._mouse_scroll_y64 }

    //eaw.text_input = 

    return res
}
