package eau

import "base:runtime"

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

mat4 :: [16]f32


Rect :: struct{
    min: [2]f32,
    max: [2]f32,
}

CollisionInfo :: struct{
    norm: [3]f32,
    depth: f32,
}


_arena :: struct{
    dests: [^]^Destructor,
    dest_amt: u32,
}

Destructor :: struct{
    arena: ^_arena,
    data: rawptr,
    delete: proc "c" (rawptr),
    user_dest: ^^Destructor,
}

Arena :: struct{
    using arena: ^_arena,

    delete: proc(arena: ^Arena),
    add: proc(arena: ^Arena, user_dest: ^^Destructor, data: rawptr, delete: proc "c" (rawptr)),
    clear: proc(arena: ^Arena),
}

_clock_ll :: struct{
    clock: ^_clock,
    next, prev: ^_clock_ll,
};

_clock :: struct{
    ll: _clock_ll,

    time: f32,
    delta: f32,
    time64: f64,
    delta64: f64,

    speed: f32,

    dest: ^Destructor,

    paused: bool,
}

Clock :: struct{
    using clock: ^_clock,

    delete: proc(clock: ^Clock),
    reset: proc(clock: ^Clock),
    set_speed: proc(clock: ^Clock, speed: f32),
    set_time: proc(clock: ^Clock, time: f32),
    set_time64: proc(clock: ^Clock, time: f64), // this just uses 64 bit for the time setting, it does not only set time64
    start: proc(clock: ^Clock),
    stop: proc(clock: ^Clock),
}

_object_ll :: struct{
    obj: ^_object,
    next, prev: ^_object_ll,
}

_object :: struct{
    desc: _object_desc,

    ll: _object_ll,

    data: rawptr,

    use_global_tickrate: bool,
    delta: f32,
    last_time: f64,

    dest: ^Destructor,
}

_object_desc :: struct{
    pos3d: [3]^f32, pos3d64: [3]^f64,
    rot3d: [3]^f32, rot3d64: [3]^f64,

    pos2d: [2]^f32, pos2d64: [2]^f64,
    rot2d: ^f32,    rot2d64: ^f64,

    init: proc "c" (obj: ^_object),
    tick: proc "c" (obj: ^_object),
    draw: proc "c" (obj: ^_object),
    stop: proc "c" (obj: ^_object),
}

Object :: struct($T: typeid) {
    using object: ^_object,

    user: struct{
        data: ^T,

        pos3d: ^[3]f32, rot3d: ^[3]f32,
        pos2d: ^[2]f32, rot2d: ^f32,
        pos3d64: ^[3]f64, rot3d64: ^[3]f64,
        pos2d64: ^[2]f64, rot2d64: ^f64,

        init: ^ObjectProc,
        tick: ^ObjectProc,
        draw: ^ObjectProc,
        stop: ^ObjectProc,
    },

    delete: proc(object: ^Object(T)),
    set_tickrate: proc(object: ^Object(T), delta: f32),
    reset_tickrate: proc(object: ^Object(T)),
    init: proc(object: ^Object(T)),
    stop: proc(object: ^Object(T)),
    draw: proc(object: ^Object(T)),
    try_tick: proc(object: ^Object(T)),
}

ObjectProc :: struct{
    fn: proc(rawptr, rawptr),
    ctx: rawptr,
}

wrap_object_proc :: proc($p: proc(^Object)) -> ObjectProc {
    
}

//ObjectDesc :: struct{
//    
//}

@(default_calling_convention="c")
foreign ceat {
    @(link_name="eaw_mat4_identity") mat4_identity :: proc(mat: ^mat4) ---
    @(link_name="eaw_mat4_copy") mat4_copy :: proc(from: mat4, to: ^mat4) ---
    @(link_name="eaw_mat4_mult") mat4_mult :: proc(out: ^mat4, a, b: mat4) ---
    @(link_name="eaw_mat4_ortho") mat4_ortho :: proc(mat: ^mat4, left, right, bottom, top, near, far: f32) ---
    @(link_name="eaw_mat4_translate") mat4_translate :: proc(mat: ^mat4, x,y,z: f32) ---
    @(link_name="eaw_mat4_scale") mat4_scale :: proc(mat: ^mat4, x,y,z: f32) ---
    @(link_name="eaw_mat4_rotate_x") mat4_rotate_x :: proc(mat: ^mat4, ang: f32) ---
    @(link_name="eaw_mat4_rotate_y") mat4_rotate_y :: proc(mat: ^mat4, ang: f32) ---
    @(link_name="eaw_mat4_rotate_z") mat4_rotate_z :: proc(mat: ^mat4, ang: f32) ---


    @(link_name="eau_aabb2d") aabb2d :: proc(a, b: Rect) -> bool ---
    @(link_name="eau_point_aabb2d") _point_aabb2d :: proc(px,py: f32, r: Rect) -> bool ---
    @(link_name="eau_aabb3d") _aabb3d :: proc(min1x,min1y,min1z, max1x,max1y,max1z, min2x,min2y,min2z, max2x,max2y,max2z: f32) -> bool ---
    @(link_name="eau_gjk3d") _gjk3d :: proc(hull1: [^][3]f32, hull1_verts: u32, hull2: [^][3]f32, hull2_verts: u32, out_simplex: ^[4][3]f32) -> bool ---
    @(link_name="eau_epa3d") _epa3d :: proc(simplex: [4][3]f32, hull1: [^][3]f32, hull1_verts: u32, hull2: [^][3]f32, hull2_verts: u32) -> CollisionInfo ---


    @(link_name="eau_create_arena") _create_arena :: proc() -> ^_arena ---
    @(link_name="eau_delete_arena") _delete_arena :: proc(arena: ^_arena) ---
    @(link_name="eau_add_to_arena") _add_to_arena :: proc(arena: ^_arena, user_dest: ^^Destructor, data: rawptr, delete: proc "c" (rawptr)) ---
    @(link_name="eau_clear_arena") _clear_arena :: proc(arena: ^_arena) ---


    @(link_name="eau_create_clock") _create_clock :: proc(arena: ^_arena) -> ^_clock ---
    @(link_name="eau_delete_clock") _delete_clock :: proc(clock: ^_clock) ---
    @(link_name="eau_reset_clock") _reset_clock :: proc(clock: ^_clock) ---
    @(link_name="eau_set_clock_speed") _set_clock_speed :: proc(clock: ^_clock, speed: f32) ---
    @(link_name="eau_set_clock_time") _set_clock_time :: proc(clock: ^_clock, time: f64) ---
    @(link_name="eau_start_clock") _start_clock :: proc(clock: ^_clock) ---
    @(link_name="eau_stop_clock") _stop_clock :: proc(clock: ^_clock) ---
    @(link_name="eau_update_clocks") _update_clocks :: proc() ---


    @(link_name="eau_create_object") _create_object :: proc(desc: _object_desc, data: rawptr, arena: ^_arena) -> ^_object ---
    @(link_name="eau_delete_object") _delete_object :: proc(object: ^_object) ---
    @(link_name="eau_set_object_tickrates") _set_object_tickrates :: proc(delta: f32) ---
    @(link_name="eau_set_object_tickrate") _set_object_tickrate :: proc(object: ^_object, delta: f32) ---
    @(link_name="eau_reset_object_tickrate") _reset_object_tickrate :: proc(object: ^_object) ---
    @(link_name="eau_init_object") _init_object :: proc(object: ^_object) ---
    @(link_name="eau_stop_object") _stop_object :: proc(object: ^_object) ---
    @(link_name="eau_draw_object") _draw_object :: proc(object: ^_object) ---
    @(link_name="eau_try_tick_object") _try_tick_object :: proc(object: ^_object) ---
    @(link_name="eau_init_objects") init_objects :: proc() ---
    @(link_name="eau_stop_objects") stop_objects :: proc() ---
    @(link_name="eau_draw_objects") draw_objects :: proc() ---
    @(link_name="eau_try_tick_objects") try_tick_objects :: proc() ---
}

point_aabb2d :: proc(point: [2]f32, rect: Rect) -> bool {
    return _point_aabb2d(point.x, point.y, rect)
}

aabb3d :: proc(min1, max1, min2, max2: [3]f32) -> bool {
    return _aabb3d(
        min1.x, min1.y, min1.z,
        max1.x, max1.y, max1.z,
        min2.x, min2.y, min2.z,
        max2.x, max2.y, max2.z,
        )
}

gjk3d :: proc(hull1, hull2: [][3]f32, out_simplex: ^[4][3]f32 = nil) -> bool {
    return _gjk3d(raw_data(hull1), u32(len(hull1)), raw_data(hull2), u32(len(hull2)), out_simplex)
}

epa3d :: proc(simplex: [4][3]f32, hull1, hull2: [][3]f32) -> CollisionInfo {
    return _epa3d(simplex, raw_data(hull1), u32(len(hull1)), raw_data(hull2), u32(len(hull2)))
}


create_arena :: proc() -> ^Arena {
    return new_clone(Arena{
        arena = _create_arena(),

        delete = delete_arena,
        add = add_to_arena,
        clear = clear_arena,
        })
}

delete_arena :: proc(arena: ^Arena) {
    _delete_arena(arena.arena)
    free(arena)
}

add_to_arena :: proc(arena: ^Arena, user_dest: ^^Destructor, data: rawptr, delete: proc "c" (data: rawptr)) {
    _add_to_arena(arena.arena, user_dest, data, delete)
}

clear_arena :: proc(arena: ^Arena) {
    _clear_arena(arena.arena)
}


create_clock :: proc(arena: ^Arena = nil) -> ^Clock {
    return new_clone(Clock{
        clock = _create_clock(arena),

        delete = delete_clock,
        reset = reset_clock,
        set_speed = set_clock_speed,
        set_time = set_clock_time32,
        set_time64 = set_clock_time64,
        start = start_clock,
        stop = stop_clock,
        })
}

delete_clock :: proc(clock: ^Clock) {
    _delete_clock(clock.clock)
    free(clock)
}

reset_clock :: proc(clock: ^Clock) {
    _reset_clock(clock.clock)
}

set_clock_speed :: proc(clock: ^Clock, speed: f32) {
    _set_clock_speed(clock.clock, speed)
}

set_clock_time :: proc{
    set_clock_time32,
    set_clock_time64,
}

set_clock_time32 :: proc(clock: ^Clock, time: f32) { _set_clock_time(clock.clock, f64(time)) }
set_clock_time64 :: proc(clock: ^Clock, time: f64) { _set_clock_time(clock.clock, time) }

start_clock :: proc(clock: ^Clock) {
    _start_clock(clock.clock);
}

stop_clock :: proc(clock: ^Clock) {
    _stop_clock(clock.clock)
}

update_clocks :: proc() {
    _update_clocks()
}


_obj_init :: proc "c" (cobj: ^_object) {
    context = runtime.default_context()
    init := (^Object(any))(cobj.data).user.init
    init.fn(cobj.data, init.ctx)
}

_obj_stop :: proc "c" (cobj: ^_object) {
    context = runtime.default_context()
    stop := (^Object(any))(cobj.data).user.stop
    stop.fn(cobj.data, stop.ctx)
}

_obj_draw :: proc "c" (cobj: ^_object) {
    context = runtime.default_context()
    draw := (^Object(any))(cobj.data).user.draw
    draw.fn(cobj.data, draw.ctx)
}

_obj_tick :: proc "c" (cobj: ^_object) {
    context = runtime.default_context()
    tick := (^Object(any))(cobj.data).user.tick
    tick.fn(cobj.data, tick.ctx)
}

create_object :: proc(data: $T, arena: ^Arena = nil) -> ^Object(T) {
    obj := new_clone(Object{
        data = new_clone(data),

        delete = delete_object,
        set_tickrate = set_object_tickrate,
        reset_tickrate = reset_object_tickrate,
        init = init_object,
        stop = stop_object,
        draw = draw_object,
        try_tick = try_tick_object,
        })

    info := runtime.type_info_base(type_info_of(T))

    #partial switch str in info.variant {
    case:
    case runtime.Type_Info_Struct:
        for i in 0..<str.field_count do switch str.tags[i] {
        case "init", "draw", "tick", "stop":
            obj_func := str.types[i]
            #partial switch func in obj_func.variant {
            case:
            case runtime.Type_Info_Named:
                assert(func.name == "ObjectProc")

                ptr := (^ObjectProc)(uintptr(obj.data) + str.offsets[i])

                switch str.tags[i] {
                case "init": obj.user.init = ptr
                case "stop": obj.user.stop = ptr
                case "tick": obj.user.tick = ptr
                case "draw": obj.user.draw = ptr
                }
            }
        case "position":
            pos_arr := str.types[i]
            #partial switch pos in pos_arr.variant {
            case:
            case runtime.Type_Info_Array:
                ptr := (^rawptr)(uintptr(obj.data) + str.offsets[i])

                switch pos.count {
                case:
                case 2:
                    if pos.elem_size == size_of(f32) do obj.user.pos2d = (^[2]f32)(ptr)
                    else if pos.elem_size == size_of(f64) do obj.user.pos2d64 = (^[2]f64)(ptr)
                case 3:
                    if pos.elem_size == size_of(f32) do obj.user.pos3d = (^[3]f32)(ptr)
                    else if pos.elem_size == size_of(f64) do obj.user.pos3d64 = (^[3]f64)(ptr)
                }
            }
        case "rotation":
            rot_arr := str.types[i]
            #partial switch rot in rot_arr.variant {
            case:
            case runtime.Type_Info_Array:
                ptr := (^rawptr)(uintptr(obj.data) + str.offsets[i])

                switch pos.count {
                case:
                case 3:
                    if rot.elem_size == size_of(f32) do obj.user.rot3d = (^[3]f32)(ptr)
                    else if rot.elem_size == size_of(f64) do obj.user.rot3d64 = (^[3]f64)(ptr)
                }
            case runtime.Type_Info_Float:
                ptr := (^rawptr)(uintptr(obj.data) + str.offsets[i])

                if rot_arr.size == size_of(f32) do obj.rot2d = (^f32)(ptr)
                else if rot_arr.size == size_of(f64) do obj.rot2d64 = (^f64)(ptr)
            }
        }
    }

    obj.object = _create_object(_object_desc{
            .pos3d = obj.user.pos3d == nil? { nil } : { &obj.user.pos3d.x, &obj.user.pos3d.y, &obj.user.pos3d.z },
            .pos3d64 = obj.user.pos3d64 == nil? { nil } : { &obj.user.pos3d64.x, &obj.user.pos3d64.y, &obj.user.pos3d64.z },
            .rot3d = obj.user.rot3d == nil? { nil } : { &obj.user.rot3d.x, &obj.user.rot3d.y, &obj.user.rot3d.z },
            .rot3d64 = obj.user.rot3d64 == nil? { nil } : { &obj.user.rot3d64.x, &obj.user.rot3d64.y, &obj.user.rot3d64.z },
            .pos2d = obj.user.pos2d == nil? { nil } : { &obj.user.pos2d.x, &obj.user.pos2d.y },
            .pos2d64 = obj.user.pos2d64 == nil? { nil } : { &obj.user.pos2d64.x, &obj.user.pos2d64.y },
            .rot2d = obj.user.rot2d == nil? nil : &obj.user.rot2d,
            .rot2d64 = obj.user.rot2d64 == nil? nil : &obj.user.rot2d64,

            .init = obj.user.init == nil? nil : _obj_init,
            .stop = obj.user.stop == nil? nil : _obj_stop,
            .draw = obj.user.draw == nil? nil : _obj_draw,
            .tick = obj.user.tick == nil? nil : _obj_tick,
            },
        obj,
        arena == nil? nil : arena.arena
        )

    return obj
}

delete_object :: proc(object: ^Object($T)) {
    _delete_object(object.object)
    free(object)
}

set_tickrates :: proc(delta: f32) {
    _set_object_tickrates(delta)
}

set_tickrate :: proc(object: ^Object($T), delta: f32) {
    _set_object_tickrate(object.object, delta)
}

reset_tickrate :: proc(object: ^Object($T)) {
    _reset_object_tickrate(object.object)
}

init_object :: proc(object: ^Object($T)) {
    _init_object(object.object)
}

stop_object :: proc(object: ^Object($T)) {
    _stop_object(object.object)
}

draw_object :: proc(object: ^Object($T)) {
    _draw_object(object.object)
}

try_tick_object :: proc(object: ^Object($T)) {
    _try_tick_object(object.object)
}


// this is taken from eat-engine since im too lazy to make it in c firt

// hex is 0xAARRGGBB because then i can do 0xRRGGBB and have it work when i do .rgb

as_rgba :: proc{
    rgb_as_rgba,

    xrgba_as_rgba,
    xrgb_as_rgba,

    hex_as_rgba,
}

rgb_as_rgba :: proc(rgb: [3]f32) -> [4]f32 { return { rgb.r, rgb.g, rgb.b, 1 } }
xrgba_as_rgba :: proc(xrgba: [4]u8) -> [4]f32 { return { f32(xrgba.r) / 255, f32(xrgba.g) / 255, f32(xrgba.b) / 255, f32(xrgba.a) / 255 } }
xrgb_as_rgba :: proc(xrgb: [3]u8) -> [4]f32 { return xrgba_as_rgba(xrgb_as_xrgba(xrgb)) }
hex_as_rgba :: proc(hex: u32) -> [4]f32 { return xrgba_as_rgba(hex_as_xrgba(hex)) }

as_xrgba :: proc{
    rgba_as_xrgba,
    rgb_as_xrgba,

    xrgb_as_xrgba,

    hex_as_xrgba,
}

rgba_as_xrgba :: proc(rgba: [4]f32) -> [4]u8 { return { u8(rgba.r * 255), u8(rgba.g * 255), u8(rgba.b * 255), u8(rgba.a * 255) } }
rgb_as_xrgba :: proc(rgb: [3]f32) -> [4]u8 { return rgba_as_xrgba({ rgb.r, rgb.g, rgb.b, 1 }) }
xrgb_as_xrgba :: proc(xrgb: [3]u8) -> [4]u8 { return { xrgb.r, xrgb.g, xrgb.b, 255 } }
hex_as_xrgba :: proc(hex: u32) -> [4]u8 { return { u8(hex >> 16), u8(hex >> 08), u8(hex >> 00), u8(hex >> 24) } }

// can convert rgba to rgb by doing .rgb (no matter the format)
