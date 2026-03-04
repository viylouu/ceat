package eau

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
