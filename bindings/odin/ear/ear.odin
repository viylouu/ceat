package ear

import "core:c"

import stbtt "vendor:stb/truetype"

import "../eau"

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

_buffer :: struct{
    id: u32,

    size: u32,
        prev_size: u32,
    data: rawptr,

    desc: BufferDesc,

    dest: ^eau.Destructor,
}

Buffer :: struct{
    using buffer: ^_buffer,

    delete: proc(buffer: ^Buffer),
    bind: proc(buffer: ^Buffer, slot: u32),
    update: proc(buffer: ^Buffer),
}

BufferDesc :: struct{
    type: BufferType,
    usage: BufferUsage,
    stride: u32,
}

BufferType :: enum i32 {
    Uniform,
    Storage,
    Vertex,
}

BufferUsage :: enum i32 {
    Dynamic,
    Static,
}


_framebuffer :: struct{
    id: u32,

    desc: _framebuffer_desc,

    dest: ^eau.Destructor,
}

_framebuffer_desc :: struct{
    out_colors: [^]^_texture,
        out_color_amt: u32,
    out_depth: ^_texture,
    width, height: u32,
}

Framebuffer :: struct{
    using framebuffer: ^_framebuffer,

    delete: proc(framebuffer: ^Framebuffer),
    bind: proc(framebuffer: ^Framebuffer),
    set_as_default: proc(framebuffer: ^Framebuffer),

    _alloc: struct{
        out_colors: []^_texture,
        },
}

FramebufferDesc :: struct{
    out_colors: []^Texture,
    out_depth: ^Texture,
    width, height: u32,
}


_pipeline :: struct{
    id: u32,
    vao: u32,

    desc: _pipeline_desc,

    dest: ^eau.Destructor,
}

_pipeline_desc :: struct{
    vertex: ShaderDesc,
    fragment: ShaderDesc,

    vertex_attribs: [^]VertexAttribDesc,
    vertex_attrib_amt: u32,

    depth: bool,

    cull_mode: CullMode,
    front_face: FrontFace,

    blend_state: struct{ has_state: bool, state: BlendState },

    fill_mode: FillMode,
}

Pipeline :: struct{
    using pipeline: ^_pipeline,

    delete: proc(pipeline: ^Pipeline),
    bind: proc(pipeline: ^Pipeline),
}

PipelineDesc :: struct{
    vertex: ShaderDesc,
    fragment: ShaderDesc,

    vertex_attribs: []VertexAttribDesc,

    depth: bool,

    cull_mode: CullMode,
    front_face: FrontFace,

    blend_state: Maybe(BlendState),

    fill_mode: FillMode,
}

ShaderDesc :: struct{
    source: cstring,
}

VertexAttribDesc :: struct{
    location: u32,
    type: PrimitiveType,
    components: u32,
    norm: bool,
    offset: u32,
    slot: u32,
}

PrimitiveType :: enum i32 {
    Float,
    Int,
}

CullMode :: enum i32 {
    None,
    Front,
    Back
}

FrontFace :: enum i32 {
    CW,
    CCW,
}

BlendState :: struct{
    src_color, dst_color: BlendFactor,
    color_op: BlendOp,
    src_alpha, dst_alpha: BlendFactor,
    alpha_op: BlendOp,
}

BlendFactor :: enum i32 {
    Zero,
    One,
    SrcColor,
    InvSrcColor,
    DstColor,
    InvDstColor,
    SrcAlpha,
    InvSrcAlpha,
    DstAlpha,
    InvDstAlpha,
}

BlendOp :: enum i32 {
    Add,
    Subtract,
    RevSubtract,
    Min,
    Max,
}

FillMode :: enum i32 {
    Fill,
    Line,
}


_texarray :: struct{
    id: u32,

    texs: [^]^_texture,

    desc: TexarrayDesc,

    dest: ^eau.Destructor,
}

TexarrayDesc :: struct{
    filter: TextureFilter,
    type: TextureType,
    wrap: TextureWrap,
        wrap_color: [4]f32,

    width, height: u32,
    layers: u32,
}

TexArray :: struct{
    using texarray: ^_texarray,

    delete: proc(texarray: ^TexArray),
    bind: proc(texarray: ^TexArray, slot: u32),
    add: proc(texarray: ^TexArray, tex: ^Texture, layer: u32),
    update: proc(texarray: ^TexArray),
    update_layer: proc(texarray: ^TexArray, layer: u32),
}


_texture :: struct{
    id: u32,

    pixels: [^]u8,
        stbi_pixels: bool,

    width, height: u32,

    desc: TextureDesc,

    dest: ^eau.Destructor
}

TextureDesc :: struct{
    filter: TextureFilter,
    type: TextureType,
    wrap: TextureWrap,
        wrap_color: [4]f32,
}

Texture :: struct{
    using texture: ^_texture,

    delete: proc(texture: ^Texture),
    bind: proc(texture: ^Texture, slot: u32),
    get_color: proc(texture: ^Texture, x,y: u32) -> [4]f32,
    set_color: proc(texture: ^Texture, x,y: u32, col: [4]f32),
    apply_changes: proc(texture: ^Texture),
}

TextureFilter :: enum i32 {
    Nearest,
    Linear,
}

TextureType :: enum i32 {
    Color,
    Depth,
    Hdr,
    Hdr32,
}

TextureWrap :: enum i32 {
    Repeat,
    Clamp,
    Color,
}


DrawMode :: enum i32 {
    Triangles,
    Lines,
}

_camera_desc_2d :: struct{
    x, y: f32,
    scalex, scaley: f32,
    rot: f32,
}

_camera_desc_3d :: struct{
    // unimplemented
}

_camera_desc :: struct{
    fb: ^_framebuffer,

    desc: union{
        _camera_desc_2d,
        _camera_desc_3d,
    },
}

_camera :: struct{
    mat: ^f32,
    desc: _camera_desc,
    dest: ^eau.Destructor,
}

CameraDesc2d :: struct{
    pos: [2]f32,
    scale: [2]f32,
    rot: f32,
}

CameraDesc3d :: struct{

}

CameraDesc :: struct{
    fb: ^Framebuffer,
    desc: union{
        CameraDesc2d,
        CameraDesc3d,
    },
}

Camera :: struct{
    using camera: ^_camera,

    delete: proc(camera: ^Camera),
    bind: proc(camera: ^Camera, ui_mode: bool),

    set_position_2d: proc(camera: ^Camera, pos: [2]f32),
    set_scale_2d: proc(camera: ^Camera, scale: [2]f32),
    set_rotation_2d: proc(camera: ^Camera, rot: f32),
}

_bitmap_mono_font :: struct{
    atlas: ^_texture,
    charw,charh: f32,
}

_truetype_font :: struct{
    atlases: rawptr,
    atlas_amt: u32,

    info: stbtt.fontinfo,
    ascent, descent, linegap: f32,
}

_font :: struct{
    lineheight: f32,

    types: union{
        _bitmap_mono_font,
        _truetype_font,
    },

    dest: ^eau.Destructor,
}

Font :: struct{
    using font: ^_font,

    delete: proc(font: ^Font),
}

@(default_calling_convention="c")
foreign ceat {
    @(link_name="ear_create_buffer") _create_buffer :: proc(desc: BufferDesc, data: rawptr, size: u32, arena: ^eau._arena) -> ^_buffer ---
    @(link_name="ear_delete_buffer") _delete_buffer :: proc(buffer: ^_buffer) ---
    @(link_name="ear_bind_buffer") _bind_buffer :: proc(buffer: ^_buffer, slot: u32) ---
    @(link_name="ear_update_buffer") _update_buffer :: proc(buffer: ^_buffer) ---


    @(link_name="ear_create_framebuffer") _create_framebuffer :: proc(desc: _framebuffer_desc, arena: ^eau._arena) -> ^_framebuffer ---
    @(link_name="ear_delete_framebuffer") _delete_framebuffer :: proc(framebuffer: ^_framebuffer) ---
    @(link_name="ear_bind_framebuffer") _bind_framebuffer :: proc(framebuffer: ^_framebuffer) ---
    @(link_name="ear_set_default_framebuffer") _set_default_framebuffer :: proc(framebuffer: ^_framebuffer) ---


    @(link_name="ear_create_pipeline") _create_pipeline :: proc(desc: _pipeline_desc, arena: ^eau._arena) -> ^_pipeline ---
    @(link_name="ear_delete_pipeline") _delete_pipeline :: proc(pipeline: ^_pipeline) ---
    @(link_name="ear_bind_pipeline") _bind_pipeline :: proc(pipeline: ^_pipeline) ---


    @(link_name="ear_create_texarray") _create_texarray :: proc(desc: TexarrayDesc, arena: ^eau._arena) -> ^_texarray ---
    @(link_name="ear_delete_texarray") _delete_texarray :: proc(texarray: ^_texarray) ---
    @(link_name="ear_bind_texarray") _bind_texarray :: proc(texarray: ^_texarray, slot: u32) ---
    @(link_name="ear_add_to_texarray") _add_to_texarray :: proc(texarray: ^_texarray, tex: ^_texture, layer: u32) ---
    @(link_name="ear_update_texarray") _update_texarray :: proc(texarray: ^_texarray) ---
    @(link_name="ear_update_texarray_layer") _update_texarray_layer :: proc(texarray: ^_texarray, layer: u32) ---


    @(link_name="ear_create_camera") _create_camera :: proc(desc: _camera_desc, arena: ^eau._arena) -> ^_camera ---
    @(link_name="ear_delete_camera") _delete_camera :: proc(camera: ^_camera) ---
    @(link_name="ear_bind_camera") _bind_camera :: proc(camera: ^_camera, ui_mode: bool) ---
    @(link_name="ear_set_camera_position_2d") _set_camera_position_2d :: proc(camera: ^_camera, x,y: f32) ---
    @(link_name="ear_set_camera_scale_2d") _set_camera_scale_2d :: proc(camera: ^_camera, x,y: f32) ---
    @(link_name="ear_set_camera_rotation_2d") _set_camera_rotation_2d :: proc(camera: ^_camera, angle: f32) ---


    @(link_name="ear_load_bitmap_mono_font") _load_bitmap_mono_font :: proc(data: [^]u8, data_size: c.size_t, arena: ^eau._arena) -> ^_font ---
    @(link_name="ear_load_truetype_font") _load_truetype_font :: proc(data: [^]u8, data_size: c.size_t, arena: ^eau._arena) -> ^_font ---
    @(link_name="ear_delete_font") _delete_font :: proc(font: ^_font) ---
    @(link_name="ear_text") _text :: proc(atlas: ^_texture, text: cstring, x,y: f32, scalex, scaley: f32, col: ^f32, align: eau.Align) ---


    @(link_name="ear_create_texture") _create_texture :: proc(desc: TextureDesc, pixels: [^]u8, width, height: u32, arena: ^eau._arena) -> ^_texture ---
    @(link_name="ear_load_texture") _load_texture :: proc(desc: TextureDesc, data: [^]u8, data_size: c.size_t, arena: ^eau._arena) -> ^_texture ---
    @(link_name="ear_delete_texture") _delete_texture :: proc(texture: ^_texture) ---
    @(link_name="ear_bind_texture") _bind_texture :: proc(texture: ^_texture, slot: u32) ---
    @(link_name="ear_get_texture_color") _get_texture_color :: proc(texture: ^_texture, x,y: u32, out: ^^f32) ---
    @(link_name="ear_set_texture_color") _set_texture_color :: proc(texture: ^_texture, x,y: u32, col: ^f32) ---
    @(link_name="ear_apply_texture_changes") _apply_texture_changes :: proc(texture: ^_texture) ---


    @(link_name="ear_clear") _clear :: proc(col: ^f32) ---
    @(link_name="ear_draw") _draw :: proc(vertices: i32, instances: i32, draw_mode: DrawMode) ---
    @(link_name="ear_flush") flush :: proc() ---
    @(link_name="ear_rect") _rect :: proc(x,y,w,h: f32, col: ^f32, align: eau.Align) ---
    @(link_name="ear_tex") _tex :: proc(tex: ^_texture, x,y,w,h,sx,sy,sw,sh: f32, col: ^f32, align: eau.Align) ---


    @(link_name="ear_translate") translate_xy :: proc(x,y: f32) ---
    @(link_name="ear_scale") scale_xy :: proc(x,y: f32) ---
    @(link_name="ear_rotate") rotate :: proc(ang: f32) ---
    @(link_name="ear_rev_translate") rev_translate_xy :: proc(x,y: f32) ---
    @(link_name="ear_rev_scale") rev_scale_xy :: proc(x,y: f32) ---
    @(link_name="ear_rev_rotate") rev_rotate :: proc(ang: f32) ---
    @(link_name="ear_reset_transform") reset_transform :: proc() ---
    @(link_name="ear_save_transform") _save_transform :: proc(out: ^^f32) ---
    @(link_name="ear_load_transform") _load_transform :: proc(mat: ^f32) ---
}

create_buffer :: proc(desc: BufferDesc, data: rawptr, size: u32, arena: ^eau.Arena = nil) -> ^Buffer {
    return new_clone(Buffer{
        buffer = _create_buffer(
            desc, 
            data, 
            size, 
            arena == nil? nil : arena.arena,
            ),

        delete = delete_buffer,
        bind = bind_buffer,
        update = update_buffer,
        })
}

delete_buffer :: proc(buffer: ^Buffer) {
    _delete_buffer(buffer.buffer)
    free(buffer)
}

bind_buffer :: proc(buffer: ^Buffer, slot: u32) {
    _bind_buffer(buffer.buffer, slot)
}

update_buffer :: proc(buffer: ^Buffer) {
    _update_buffer(buffer.buffer)
}


create_framebuffer :: proc(desc: FramebufferDesc, arena: ^eau.Arena = nil) -> ^Framebuffer {
    out_colors := make([]^_texture, len(desc.out_colors))
    for col,i in desc.out_colors do out_colors[i] = col.texture

    fb := Framebuffer{
        framebuffer = _create_framebuffer(
            _framebuffer_desc{ 
                out_colors = raw_data(out_colors),
                out_color_amt = u32(len(out_colors)),
                out_depth = desc.out_depth != nil? desc.out_depth.texture : nil,
                width = desc.width, height = desc.height,
                },
            arena == nil? nil : arena),

        delete = delete_framebuffer,
        bind = bind_framebuffer,
        set_as_default = set_default_framebuffer,

        _alloc = {
            out_colors = out_colors,
            },
        }

    return new_clone(fb)
}

delete_framebuffer :: proc(fb: ^Framebuffer) {
    _delete_framebuffer(fb.framebuffer)

    delete(fb._alloc.out_colors)
    free(fb)
}

bind_framebuffer :: proc(fb: ^Framebuffer) {
    _bind_framebuffer(fb == nil? nil : fb.framebuffer)
}

set_default_framebuffer :: proc(fb: ^Framebuffer) {
    _set_default_framebuffer(fb == nil? nil : fb.framebuffer)
}


create_pipeline :: proc(desc: PipelineDesc, arena: ^eau.Arena = nil) -> ^Pipeline {
    _,has_blend := desc.blend_state.?

    return new_clone(Pipeline{
        pipeline = _create_pipeline(_pipeline_desc{
            vertex = desc.vertex,
            fragment = desc.fragment,

            vertex_attribs = raw_data(desc.vertex_attribs),
            vertex_attrib_amt = u32(len(desc.vertex_attribs)),

            depth = desc.depth,

            cull_mode = desc.cull_mode,
            front_face = desc.front_face,

            blend_state = { has_blend, desc.blend_state.? or_else {} },

            fill_mode = desc.fill_mode,
            }, arena == nil? nil : arena.arena),

        delete = delete_pipeline,
        bind = bind_pipeline,
        })
}

delete_pipeline :: proc(pln: ^Pipeline) {
    _delete_pipeline(pln.pipeline)

    free(pln)
}

bind_pipeline :: proc(pln: ^Pipeline) {
    _bind_pipeline(pln.pipeline)
}


create_texarray :: proc(desc: TexarrayDesc, arena: ^eau.Arena = nil) -> ^TexArray {
    desc := desc

    return new_clone(TexArray{
            texarray = _create_texarray(
                desc,
                arena == nil? nil : arena
                ),

            delete = delete_texarray,
            bind = bind_texarray,
            add = add_to_texarray,
            update = update_texarray,
            update_layer = update_texarray_layer,
        })
}

delete_texarray :: proc(arr: ^TexArray) {
    _delete_texarray(arr.texarray)

    free(arr)
}

bind_texarray :: proc(arr: ^TexArray, slot: u32) {
    _bind_texarray(arr.texarray, slot)
}

add_to_texarray :: proc(arr: ^TexArray, tex: ^Texture, layer: u32) {
    _add_to_texarray(arr.texarray, tex.texture, layer)
}

update_texarray :: proc(arr: ^TexArray) {
    _update_texarray(arr.texarray)
}

update_texarray_layer :: proc(arr: ^TexArray, layer: u32) {
    _update_texarray_layer(arr.texarray, layer)
}


load_bitmap_mono_font :: proc(data: []u8, arena: ^eau.Arena = nil) -> ^Font {
    return new_clone(Font{
        font = _load_bitmap_mono_font(
            raw_data(data), 
            len(data), 
            arena == nil? nil : arena.arena
            ),

        delete = delete_font,
        })
}

load_truetype_font :: proc(data: []u8, arena: ^eau.Arena = nil) -> ^Font {
    return new_clone(Font{
        font = _load_truetype_font(
            raw_data(data), 
            len(data), 
            arena == nil? nil : arena.arena
            ),

        delete = delete_font,
        })
}

delete_font :: proc(font: ^Font) {
    _delete_font(font.font)

    free(font)
}

text :: proc{
    text_rgba_wh,
    text_rgb_wh,
    text_gray_wh,
    text_rgba_size,
    text_rgb_size,
    text_gray_size,
    text_rgba,
    text_rgb,
    text_gray,

    text_rgba_wh_vec,
    text_rgb_wh_vec,
    text_gray_wh_vec,
    text_rgba_size_vec,
    text_rgb_size_vec,
    text_gray_size_vec,
    text_rgba_vec,
    text_rgb_vec,
    text_gray_vec,
}

text_rgba_wh :: proc(atlas: ^Texture, text: cstring, x, y: f32, scalex, scaley: f32, col: [4]f32, align: eau.Align = .TopLeft) { col := col ;; _text(atlas.texture, text, x,y,scalex, scaley, &col[0], align) }
text_rgb_wh :: proc(atlas: ^Texture, text: cstring, x, y: f32, scalex, scaley: f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgba_wh(atlas, text, x,y, scalex, scaley, eau.as_rgba(col), align) }
text_gray_wh :: proc(atlas: ^Texture, text: cstring, x, y: f32, scalex, scaley: f32, col: f32, align: eau.Align = .TopLeft) { text_rgb_wh(atlas, text, x,y, scalex, scaley, [3]f32 { col, col, col }, align) }
text_rgba_size :: proc(atlas: ^Texture, text: cstring, x, y: f32, size: f32, col: [4]f32, align: eau.Align = .TopLeft) { text_rgba_wh(atlas, text, x,y, size,size, col, align) }
text_rgb_size :: proc(atlas: ^Texture, text: cstring, x, y: f32, size: f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgb_wh(atlas, text, x,y, size,size, col, align) }
text_gray_size :: proc(atlas: ^Texture, text: cstring, x, y: f32, size: f32, col: f32, align: eau.Align = .TopLeft) { text_gray_wh(atlas, text, x,y, size,size, col, align) }
text_rgba :: proc(atlas: ^Texture, text: cstring, x, y: f32, col: [4]f32, align: eau.Align = .TopLeft) { text_rgba_size(atlas, text, x,y, 1, col, align) }
text_rgb :: proc(atlas: ^Texture, text: cstring, x, y: f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgb_size(atlas, text, x,y, 1, col, align) }
text_gray :: proc(atlas: ^Texture, text: cstring, x, y: f32, col: f32, align: eau.Align = .TopLeft) { text_gray_size(atlas, text, x,y, 1, col, align) }

text_rgba_wh_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: [2]f32, col: [4]f32, align: eau.Align = .TopLeft) { text_rgba_wh(atlas, text, pos.x,pos.y, size.x,size.y, col, align) }
text_rgb_wh_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: [2]f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgba_wh_vec(atlas, text, pos, size, eau.as_rgba(col), align) }
text_gray_wh_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: [2]f32, col: f32, align: eau.Align = .TopLeft) { text_rgb_wh_vec(atlas, text, pos, size, [3]f32 { col, col, col }, align) }
text_rgba_size_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: f32, col: [4]f32, align: eau.Align = .TopLeft) { text_rgba_wh_vec(atlas, text, pos, {size,size}, col, align) }
text_rgb_size_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgb_wh_vec(atlas, text, pos, {size,size}, col, align) }
text_gray_size_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, size: f32, col: f32, align: eau.Align = .TopLeft) { text_gray_wh_vec(atlas, text, pos, {size,size}, col, align) }
text_rgba_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, col: [4]f32, align: eau.Align = .TopLeft) { text_rgba_size_vec(atlas, text, pos, 1, col, align) }
text_rgb_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, col: [3]f32, align: eau.Align = .TopLeft) { text_rgb_size_vec(atlas, text, pos, 1, col, align) }
text_gray_vec :: proc(atlas: ^Texture, text: cstring, pos: [2]f32, col: f32, align: eau.Align = .TopLeft) { text_gray_size_vec(atlas, text, pos, 1, col, align) }


create_texture :: proc(desc: TextureDesc, pixels: [^]u8, width,height: u32, arena: ^eau.Arena = nil) -> ^Texture {
    desc := desc

    return new_clone(Texture{
        texture = _create_texture(
            desc,
            pixels,
            width, height,
            arena == nil? nil : arena.arena,
            ),

        delete = delete_texture,
        bind = bind_texture,
        get_color = get_texture_color,
        set_color = set_texture_color,
        apply_changes = apply_texture_changes,
        })
}

load_texture :: proc(desc: TextureDesc, data: []u8, arena: ^eau.Arena = nil) -> ^Texture {
    desc := desc

    return new_clone(Texture{
        texture = _load_texture(
            desc,
            raw_data(data), len(data),
            arena == nil? nil : arena.arena,
            ),

        delete = delete_texture,
        bind = bind_texture,
        get_color = get_texture_color,
        set_color = set_texture_color,
        apply_changes = apply_texture_changes,
        })
}

delete_texture :: proc(tex: ^Texture) {
    _delete_texture(tex.texture)

    free(tex)
}

bind_texture :: proc(tex: ^Texture, slot: u32) {
    _bind_texture(tex.texture, slot)
}

get_texture_color :: proc(tex: ^Texture, #any_int x,y: u32) -> [4]f32 {
    out: [4]f32
    outp := &out[0]
    _get_texture_color(tex.texture, x,y, &outp)
    return out
}

set_texture_color :: proc(tex: ^Texture, #any_int x,y: u32, col: [4]f32) {
    col := col
    _set_texture_color(tex.texture, x,y, &col[0])
}

apply_texture_changes :: proc(tex: ^Texture) {
    _apply_texture_changes(tex.texture)
}


draw :: proc(#any_int vertices: i32, #any_int instances: i32 = 1, draw_mode: DrawMode = .Triangles) {
    _draw(vertices, instances, draw_mode)
}

clear :: proc(col: [3]f32) {
    col := col
    _clear(&col[0])
}

rect :: proc{
    rect_rgba,
    rect_rgb,
    rect_gray,
    rect_rgba_vec,
    rect_rgb_vec,
    rect_gray_vec,
}

rect_rgba :: proc(x, y: f32, w, h: f32, col: [4]f32, align: eau.Align = .TopLeft) { col := col ;; _rect(x,y,w,h,&col[0], align) }
rect_rgb :: proc(x, y: f32, w, h: f32, col: [3] f32, align: eau.Align = .TopLeft) { rect_rgba(x,y, w,h, eau.as_rgba(col), align) }
rect_gray :: proc(x, y: f32, w, h: f32, col: f32, align: eau.Align = .TopLeft) { rect_rgb(x,y, w,h, [3]f32 { col,col,col }, align) }
rect_rgba_vec :: proc(pos: [2]f32, size: [2]f32, col: [4]f32, align: eau.Align = .TopLeft) { rect_rgba(pos.x,pos.y, size.x,size.y, col, align) }
rect_rgb_vec :: proc(pos: [2]f32, size: [2]f32, col: [3]f32, align: eau.Align = .TopLeft) { rect_rgb(pos.x,pos.y, size.x,size.y, col, align) }
rect_gray_vec :: proc(pos: [2]f32, size: [2]f32, col: f32, align: eau.Align = .TopLeft) { rect_gray(pos.x,pos.y, size.x,size.y, col, align) }

tex :: proc{
    tex_rgba_wh_samp,
    tex_rgb_wh_samp,
    tex_gray_wh_samp,
    tex_rgba_samp,
    tex_rgb_samp,
    tex_gray_samp,
    tex_rgba_wh,
    tex_rgb_wh,
    tex_gray_wh,
    tex_rgba,
    tex_rgb,
    tex_gray,

    tex_rgba_wh_samp_vec,
    tex_rgb_wh_samp_vec,
    tex_gray_wh_samp_vec,
    tex_rgba_samp_vec,
    tex_rgb_samp_vec,
    tex_gray_samp_vec,
    tex_rgba_wh_vec,
    tex_rgb_wh_vec,
    tex_gray_wh_vec,
    tex_rgba_vec,
    tex_rgb_vec,
    tex_gray_vec,
}

tex_rgba_wh_samp :: proc(tex: ^Texture, x, y: f32, w, h: f32, sx,sy,sw,sh: f32, col: [4]f32, align: eau.Align = .TopLeft) { col := col ;; _tex(tex.texture, x,y,w,h,sx,sy,sw,sh, &col[0], align) }
tex_rgb_wh_samp :: proc(tex: ^Texture, x, y: f32, w, h: f32, sx,sy,sw,sh: f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp(tex, x,y, w,h, sx,sy,sw,sh, eau.as_rgba(col), align) }
tex_gray_wh_samp :: proc(tex: ^Texture, x, y: f32, w, h: f32, sx,sy,sw,sh: f32, col: f32, align: eau.Align = .TopLeft) { tex_rgb_wh_samp(tex, x,y, w,h, sx,sy,sw,sh, [3]f32{ col,col,col }, align) }
tex_rgba_samp :: proc(tex: ^Texture, x, y: f32, sx,sy,sw,sh: f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp(tex, x,y, sw,sh, sx,sy,sw,sh, col, align) }
tex_rgb_samp :: proc(tex: ^Texture, x, y: f32, sx,sy,sw,sh: f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgb_wh_samp(tex, x,y, sw,sh, sx,sy,sw,sh, col, align) }
tex_gray_samp :: proc(tex: ^Texture, x, y: f32, sx,sy,sw,sh: f32, col: f32, align: eau.Align = .TopLeft) { tex_gray_wh_samp(tex, x,y, sw,sh, sx,sy,sw,sh, col, align) }
tex_rgba_wh :: proc(tex: ^Texture, x, y: f32, w, h: f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp(tex, x,y, w,h, 0,0, f32(tex.width),f32(tex.height), col, align) }
tex_rgb_wh :: proc(tex: ^Texture, x, y: f32, w, h: f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgb_wh_samp(tex, x,y, w,h, 0,0, f32(tex.width),f32(tex.height), col, align) }
tex_gray_wh :: proc(tex: ^Texture, x, y: f32, w, h: f32, col: f32, align: eau.Align = .TopLeft) { tex_gray_wh_samp(tex, x,y, w,h, 0,0, f32(tex.width),f32(tex.height), col, align) }
tex_rgba :: proc(tex: ^Texture, x, y: f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh(tex, x,y, f32(tex.width),f32(tex.height), col, align) }
tex_rgb :: proc(tex: ^Texture, x, y: f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgb_wh(tex, x,y, f32(tex.width),f32(tex.height), col, align) }
tex_gray :: proc(tex: ^Texture, x, y: f32, col: f32, align: eau.Align = .TopLeft) { tex_gray_wh(tex, x,y, f32(tex.width),f32(tex.height), col, align) }

tex_rgba_wh_samp_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, samp: [4]f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp(tex, pos.x,pos.y, size.x,size.y, samp.x,samp.y,samp.z,samp.w, col, align) }
tex_rgb_wh_samp_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, samp: [4]f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp_vec(tex, pos, size, samp, eau.as_rgba(col), align) }
tex_gray_wh_samp_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, samp: [4]f32, col: f32, align: eau.Align = .TopLeft) { tex_rgb_wh_samp_vec(tex, pos, size, samp, [3]f32{ col,col,col }, align) }
tex_rgba_samp_vec :: proc(tex: ^Texture, pos: [2]f32, samp: [4]f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_samp_vec(tex, pos, {samp.z,samp.w}, samp, col, align) }
tex_rgb_samp_vec :: proc(tex: ^Texture, pos: [2]f32, samp: [4]f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgb_wh_samp_vec(tex, pos, {samp.z,samp.w}, samp, col, align) }
tex_gray_samp_vec :: proc(tex: ^Texture, pos: [2]f32, samp: [4]f32, col: f32, align: eau.Align = .TopLeft) { tex_gray_wh_samp_vec(tex, pos, {samp.z,samp.w}, samp, col, align) }
tex_rgba_wh_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh(tex, pos.x,pos.y, size.x,size.y, col, align) }
tex_rgb_wh_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_vec(tex, pos, size, eau.as_rgba(col), align) }
tex_gray_wh_vec :: proc(tex: ^Texture, pos: [2]f32, size: [2]f32, col: f32, align: eau.Align = .TopLeft) { tex_rgb_wh_vec(tex, pos, size, [3]f32 { col,col,col }, align) }
tex_rgba_vec :: proc(tex: ^Texture, pos: [2]f32, col: [4]f32, align: eau.Align = .TopLeft) { tex_rgba_wh_vec(tex, pos, {f32(tex.width),f32(tex.height)}, col, align) }
tex_rgb_vec :: proc(tex: ^Texture, pos: [2]f32, col: [3]f32, align: eau.Align = .TopLeft) { tex_rgb_wh_vec(tex, pos, {f32(tex.width),f32(tex.height)}, col, align) }
tex_gray_vec :: proc(tex: ^Texture, pos: [2]f32, col: f32, align: eau.Align = .TopLeft) { tex_gray_wh_vec(tex, pos, {f32(tex.width),f32(tex.height)}, col, align) }


translate :: proc{
    translate_vec,
    translate_xy,
}

translate_vec :: proc(pos: [2]f32) { translate_xy(pos.x,pos.y) }

scale :: proc{
    scale_vec,
    scale_xy,
}

scale_vec :: proc(scale: [2]f32) { scale_xy(scale.x,scale.y) }

rev_translate :: proc{
    rev_translate_vec,
    rev_translate_xy,
}

rev_translate_vec :: proc(pos: [2]f32) { rev_translate_xy(pos.x,pos.y) }

rev_scale :: proc{
    rev_scale_vec,
    rev_scale_xy,
}

rev_scale_vec :: proc(scale: [2]f32) { rev_scale_xy(scale.x,scale.y) }

save_transform :: proc() -> [16]f32 {
    t: [16]f32
    t2: ^f32 = &t[0]
    _save_transform(&t2)
    return t
}

load_transform :: proc(transf: [16]f32) {
    transf := transf
    _load_transform(&transf[0]);
}

create_camera :: proc(desc: CameraDesc, arena: ^eau.Arena = nil) -> ^Camera {
    _desc := _camera_desc{
        fb = desc.fb == nil? nil : desc.fb.framebuffer
        }

    switch type in desc.desc {
    case CameraDesc2d:
        _desc.desc = _camera_desc_2d{
            x = type.pos.x, y = type.pos.y,
            scalex = type.scale.x, scaley = type.scale.y,
            rot = type.rot,
            }
    case CameraDesc3d:
        _desc.desc = _camera_desc_3d{
            }
    }

    return new_clone(Camera{
        camera = _create_camera(
            _desc,
            arena == nil? nil : arena.arena,
            ),

        delete = delete_camera,
        bind = bind_camera,

        set_position_2d = set_camera_position_2d,
        set_scale_2d = set_camera_scale_2d,
        set_rotation_2d = set_camera_rotation_2d,
        })
}

delete_camera :: proc(camera: ^Camera) {
    _delete_camera(camera.camera)
    free(camera)
}

bind_camera :: proc(camera: ^Camera, ui_mode: bool = false) {
    _bind_camera(camera.camera, ui_mode)
}

set_camera_position_2d :: proc(camera: ^Camera, pos: [2]f32) {
    _set_camera_position_2d(camera.camera, pos.x,pos.y)
}

set_camera_scale_2d :: proc(camera: ^Camera, scale: [2]f32) {
    _set_camera_scale_2d(camera.camera, scale.x,scale.y)
}

set_camera_rotation_2d :: proc(camera: ^Camera, rot: f32) {
    _set_camera_rotation_2d(camera.camera, rot)
}
