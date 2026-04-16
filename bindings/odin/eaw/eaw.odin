package eaw

when ODIN_OS == .Windows {
    foreign import ceat {
        "../../../zig-out/lib/ceat.lib",
    }
} else when ODIN_OS == .Linux {
    foreign import ceat {
        "../../../zig-out/lib/libceat.a",
    }
} else {
    // unsupported os!
}

KeyState :: enum i32 {
    Released,
    Pressed,
    Held,
    Inactive,
}

Key :: enum i32 {
    Escape,
    Caps,
    Space, Tab,
    Backspace, Delete,
    Home, End,
    PageUp, PageDown,
    Insert,
    Enter,

    LShift, RShift,
    LCtrl, RCtrl,
    LAlt, RAlt,
    LSuper, RSuper,

    Up, Left, Right, Down,

    A, B, C, D, E,
    F, G, H, I, J,
    K, L, M, N, O,
    P, Q, R, S, T,
    U, V, W, X, Y, Z,
    
    K1, K2, K3, K4, K5,
    K6, K7, K8, K9, K0,

    F1, F2, F3, F4,
    F5, F6, F7, F8,
    F9, F10, F11, F12,

    Last,
}

Mouse :: enum i32 {
    Left, Right, Middle,
    Last,
}

MouseMode :: enum i32 {
    Normal,
    Hidden,
    Locked,
}

@(default_calling_convention="c")
foreign ceat {
    // dont use unless you want the real time
    @(link_name="eaw_time")  time: f32
    @(link_name="eaw_delta") delta: f32


    @(link_name="eaw_mouse_x")   _mouse_x: f32
    @(link_name="eaw_mouse_y")   _mouse_y: f32
    @(link_name="eaw_mouse_x64") _mouse_x64: f64
    @(link_name="eaw_mouse_y64") _mouse_y64: f64
    @(link_name="eaw_mouse_delta_x")   _mouse_delta_x: f32
    @(link_name="eaw_mouse_delta_y")   _mouse_delta_y: f32
    @(link_name="eaw_mouse_delta_x64") _mouse_delta_x64: f64
    @(link_name="eaw_mouse_delta_y64") _mouse_delta_y64: f64
    @(link_name="eaw_mouse_scroll_x")   _mouse_scroll_x: f32
    @(link_name="eaw_mouse_scroll_y")   _mouse_scroll_y: f32
    @(link_name="eaw_mouse_scroll_x64") _mouse_scroll_x64: f64
    @(link_name="eaw_mouse_scroll_y64") _mouse_scroll_y64: f64

    @(link_name="eaw_text_input") text_input: cstring
    @(link_name="eaw_text_input_chars") text_input_chars: u32
    @(link_name="eaw_tick_text_input") tick_text_input: cstring
    @(link_name="eaw_tick_text_input_chars") tick_text_input_chars: u32

    @(link_name="eaw_is_key")          is_key :: proc(key: Key) -> bool ---
    @(link_name="eaw_is_key_pressed")  is_key_pressed :: proc(key: Key) -> bool ---
    @(link_name="eaw_is_key_released") is_key_released :: proc(key: Key) -> bool ---

    @(link_name="eaw_is_mouse")          is_mouse :: proc(mouse: Mouse) -> bool ---
    @(link_name="eaw_is_mouse_pressed")  is_mouse_pressed :: proc(mouse: Mouse) -> bool ---
    @(link_name="eaw_is_mouse_released") is_mouse_released :: proc(mouse: Mouse) -> bool ---

    @(link_name="eaw_tick_is_key")          tick_is_key :: proc(key: Key) -> bool ---
    @(link_name="eaw_tick_is_key_pressed")  tick_is_key_pressed :: proc(key: Key) -> bool ---
    @(link_name="eaw_tick_is_key_released") tick_is_key_released :: proc(key: Key) -> bool ---

    @(link_name="eaw_tick_is_mouse")          tick_is_mouse :: proc(mouse: Mouse) -> bool ---
    @(link_name="eaw_tick_is_mouse_pressed")  tick_is_mouse_pressed :: proc(mouse: Mouse) -> bool ---
    @(link_name="eaw_tick_is_mouse_released") tick_is_mouse_released :: proc(mouse: Mouse) -> bool ---

    @(link_name="eaw_set_mouse_mode") set_mouse_mode :: proc(mode: MouseMode) ---
}

mouse:   [2]f32
mouse64: [2]f64
mouse_delta:   [2]f32
mouse_delta64: [2]f64
mouse_scroll:   [2]f32
mouse_scroll64: [2]f64

//text_input: string
