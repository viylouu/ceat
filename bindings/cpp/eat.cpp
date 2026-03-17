#include "eat.hpp"

namespace eat{
    int32_t width;
    int32_t height;

    float time;
    float delta;
    double time64;
    double delta64;

    void (*user_command_solver)(std::string);

    void 
    _command_solver(
        char* command
        ) {
        user_command_solver(std::string(command));
    }

    void 
    init(
        std::string title,
        int32_t width, int32_t height,
        init_opts opts
        ) {
        user_command_solver = opts.console.command_solver;

        eat_init(
            (char*)title.c_str(),
            width, height,
            eat_init_opts{
                .vsync = opts.vsync,
                .console = eat_console_desc{
                    .enabled = opts.console.enabled,
                    .key = (eaw_key)opts.console.key,
                    .command_solver = _command_solver,
                    },
                .debug = eat_debug_desc{
                    .enabled = opts.debug.enabled,
                    .key = (eaw_key)opts.debug.key,
                    },
                }
            );
    }

    void
    stop(
        void
        ) {
        eat_stop();
    }

    bool
    frame(
        void
        ) {
        bool res = eat_frame();

        width = eat_width;
        height = eat_height;
        time = eat_time;
        delta = eat_delta;
        time64 = eat_time64;
        delta64 = eat_delta64;

        eaw::mouse = { eaw_mouse_x, eaw_mouse_y };
        eaw::mouse64 = { eaw_mouse_x64, eaw_mouse_y64 };
        eaw::mouse_delta = { eaw_mouse_delta_x, eaw_mouse_delta_y };
        eaw::mouse_delta64 = { eaw_mouse_delta_x64, eaw_mouse_delta_y64 };
        eaw::mouse_scroll = { eaw_mouse_scroll_x, eaw_mouse_scroll_y };
        eaw::mouse_scroll64 = { eaw_mouse_scroll_x64, eaw_mouse_scroll_y64 };

        eaw::text_input.assign(eaw_text_input, eaw_text_input_chars);
        eaw::tick_text_input.assign(eaw_tick_text_input, eaw_tick_text_input_chars);

        return res;
    }
};
namespace eaw{
    vec2<float> mouse;
    vec2<double> mouse64;
    vec2<float> mouse_delta;
    vec2<double> mouse_delta64;
    vec2<float> mouse_scroll;
    vec2<double> mouse_scroll64;

    std::string text_input;
    std::string tick_text_input;

    bool
    is_key(
        Key key
        ) {
        return eaw_is_key((eaw_key)key);
    }
    bool
    is_key_pressed(
        Key key
        ) {
        return eaw_is_key_pressed((eaw_key)key);
    }
    bool
    is_key_released(
        Key key
        ) {
        return eaw_is_key_released((eaw_key)key);
    }

    bool
    is_mouse(
        Mouse mouse
        ) {
        return eaw_is_mouse((eaw_mouse)mouse);
    }
    bool
    is_mouse_pressed(
        Mouse mouse
        ) {
        return eaw_is_mouse_pressed((eaw_mouse)mouse);
    }
    bool
    is_mouse_released(
        Mouse mouse
        ) {
        return eaw_is_mouse_released((eaw_mouse)mouse);
    }

    bool
    tick_is_key(
        Key key
        ) {
        return eaw_tick_is_key((eaw_key)key);
    }
    bool
    tick_is_key_pressed(
        Key key
        ) {
        return eaw_tick_is_key_pressed((eaw_key)key);
    }
    bool
    tick_is_key_released(
        Key key
        ) {
        return eaw_tick_is_key_released((eaw_key)key);
    }

    bool
    tick_is_mouse(
        Mouse mouse
        ) {
        return eaw_tick_is_mouse((eaw_mouse)mouse);
    }
    bool
    tick_is_mouse_pressed(
        Mouse mouse
        ) {
        return eaw_tick_is_mouse_pressed((eaw_mouse)mouse);
    }
    bool
    tick_is_mouse_released(
        Mouse mouse
        ) {
        return eaw_tick_is_mouse_released((eaw_mouse)mouse);
    }

    void
    set_mouse_mode(
        MouseMode mode
        ) {
        eaw_set_mouse_mode((eaw_mouse_mode)mode);
    }
};
namespace eau{
    Arena::Arena() {
        arena = eau_create_arena();
    }
    Arena::~Arena() {
        eau_delete_arena(arena);
    }

    void
    Arena::clear() {
        eau_clear_arena(arena);
    }

    void
    Arena::add(
        Arena* arena,
        Destructor* user_dest,
        void* data,
        void (*delete_fn)(void* data)
        ) {
        eau_add_to_arena(
                arena->arena,
                &user_dest->destructor,
                data,
                delete_fn
                );
    }

    Rect
    Rect::topleftify(
        void
        ) {
        eau_rect rect = eau_rect_topleftify(eau_rect{
            pos.x, pos.y,
            size.x, size.y,
            (eau_align)align,
            });

        return Rect{
            .pos = { rect.x, rect.y },
            .size = { rect.w, rect.h },
            };
    }
    Rect
    Rect::alignify(
        Align align
        ) {
        eau_rect rect = eau_rect_alignify(eau_rect{
            pos.x, pos.y,
            size.x, size.y,
            (eau_align)this->align,
            }, (eau_align)align);

        return Rect{
            .pos = { rect.x, rect.y },
            .size = { rect.w, rect.h },
            .align = align,
            };
    }

    Rect
    Rect::scale_to_fit(
        Rect fit
        ) {
        eau_rect rect = eau_rect_scale_to_fit(
            eau_rect{
                pos.x, pos.y,
                size.x, size.y,
                (eau_align)align,
                }, 
            eau_rect{
                fit.pos.x, fit.pos.y,
                fit.size.x, fit.size.y,
                (eau_align)fit.align
                }
            );

        return Rect{
            .pos = { rect.x, rect.y },
            .size = { rect.w, rect.h },
            .align = (eau::Align)rect.align,
            };
    }

    Rect
    Rect::space_convert(
        Rect orig,
        Rect new_
        ) {
        eau_rect rect = eau_rect_space_convert(
            eau_rect{
                pos.x, pos.y,
                size.x, size.y,
                (eau_align)align,
                }, 
            eau_rect{
                orig.pos.x, orig.pos.y,
                orig.size.x, orig.size.y,
                (eau_align)orig.align
                },
            eau_rect{
                new_.pos.x, new_.pos.y,
                new_.size.x, new_.size.y,
                (eau_align)new_.align
                }
            );

        return Rect{
            pos = { rect.x, rect.y },
            size = { rect.w, rect.h },
            align = (eau::Align)rect.align,
            };
    }


    bool
    aabb2d(
        vec2<float> min1,
        vec2<float> max1,
        vec2<float> min2,
        vec2<float> max2
        ) {
        return eau_aabb2d(
            min1.x,min1.y, 
            max1.x,max1.y, 
            min2.x,min2.y, 
            max2.x,max2.y
            );
    }

    bool
    point_aabb2d(
        vec2<float> point,
        vec2<float>min, vec2<float>max
        ) {
        return eau_point_aabb2d(
            point.x,point.y, 
            min.x,min.y, 
            max.x,max.y
            );
    }

    bool
    rect_rect(
        Rect a,
        Rect b
        ) {
        return eau_rect_rect(
            eau_rect{ 
                a.pos.x,  a.pos.y, 
                a.size.x, a.size.y, 
                (eau_align)a.align
                },
            eau_rect{
                a.pos.x,  a.pos.y,
                a.size.x, a.size.y,
                (eau_align)a.align
                }
            );
    }

    bool
    point_rect(
        vec2<float> point,
        Rect rect
        ) {
        return eau_point_rect(
            point.x, point.y,
            eau_rect{
                rect.pos.x,  rect.pos.y,
                rect.size.x, rect.size.y,
                (eau_align)rect.align,
                }
            );
    }

    bool
    aabb3d(
        vec3<float> min1,
        vec3<float> max1,
        vec3<float> min2,
        vec3<float> max2
        ) {
        return eau_aabb3d(
            min1.x, min1.y, min1.z,
            max1.x, max1.y, max1.z,
            min2.x, min2.y, min2.z,
            max2.x, max2.y, max2.z
            );
    }

    bool
    gjk3d(
        std::vector<float[3]> hull1,
        std::vector<float[3]> hull2,
        vec3<float> (*out_simplex)[4]
        ) {
        float simpl[4][3];
        bool res = eau_gjk3d(
            hull1.data(), hull1.size(),
            hull2.data(), hull2.size(),
            &simpl
            );

        if (out_simplex) {
            (*out_simplex)[0] = { simpl[0][0], simpl[0][1], simpl[0][2] };
            (*out_simplex)[1] = { simpl[1][0], simpl[1][1], simpl[1][2] };
            (*out_simplex)[2] = { simpl[2][0], simpl[2][1], simpl[2][2] };
            (*out_simplex)[3] = { simpl[3][0], simpl[3][1], simpl[3][2] };
        }

        return res;
    }

    CollisionInfo
    epa3d(
        vec3<float> simplex[4],
        std::vector<float[3]> hull1,
        std::vector<float[3]> hull2
        ) {
        float simpl[4][3] = {
            { simplex[0].x, simplex[0].y, simplex[0].z, },
            { simplex[1].x, simplex[1].y, simplex[1].z, },
            { simplex[2].x, simplex[2].y, simplex[2].z, },
            { simplex[3].x, simplex[3].y, simplex[3].z, },
            };
        eau_collision_info info = eau_epa3d(
            simpl,
            hull1.data(), hull1.size(),
            hull2.data(), hull2.size()
            );
        return CollisionInfo{
            .norm = vec3<float>(info.normx, info.normy, info.normz),
            .depth = info.depth,
            };
    }

    char*
    load_file(
        std::string path,
        size_t* out_size
        ) {
        return eau_load_file(
            path.c_str(),
            out_size
            );
    }

    Clock::Clock(
        eau::Arena* arena
        ) {
        clock = eau_create_clock(arena == nullptr? nullptr : arena->arena);
        this->arena = arena;
    }
    Clock::~Clock() {
        if (!arena) eau_delete_clock(clock);
    }

    void
    Clock::reset(
        void
        ) {
        eau_reset_clock(clock);
    }

    void
    Clock::set_speed(
        float speed
        ) {
        eau_set_clock_speed(clock, speed);
    }

    void
    Clock::set_time(
        float time
        ) {
        eau_set_clock_time(clock, (double)time);
    }
    void
    Clock::set_time(
        double time
        ) {
        eau_set_clock_time(clock, time);
    }

    void
    Clock::start(
        void
        ) {
        eau_start_clock(clock);
    }

    void
    Clock::stop(
        void
        ) {
        eau_stop_clock(clock);
    }

    void
    update_clocks(
        void
        ) {
        eau_update_clocks();
    }

    void
    set_object_tickrate(
        float delta
        ) {
        eau_set_object_tickrate(delta);
    }

    void
    init_objects(
        void
        ) {
        eau_init_objects();
    }

    void
    stop_objects(
        void
        ) {
        eau_stop_objects();
    }

    void
    draw_objects(
        void
        ) {
        eau_draw_objects();
    }

    void
    try_tick_objects(
        void
        ) {
        eau_try_tick_objects();
    }
};
namespace eaa{
    Mixer::Mixer(
        MixerDesc desc,
        eau::Arena* arena
        ) {
        mixer = eaa_create_mixer(
            eaa_mixer_desc{
                .volume = desc.volume,
                .pitch = desc.pitch,
                .parent = desc.parent == nullptr? nullptr : desc.parent->mixer,
                },
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Mixer::~Mixer() {
        if (!arena) eaa_delete_mixer(mixer);
    }

    void
    Mixer::set_volume(
        float volume
        ) {
        eaa_set_mixer_volume(mixer, volume);
    }

    void
    Mixer::set_pitch(
        float pitch
        ) {
        eaa_set_mixer_pitch(mixer, pitch);
    }

    Sound::Sound(
        SoundDesc desc,
        const char* data, size_t data_size,
        eau::Arena* arena
        ) {
        sound = eaa_load_sound(
            eaa_sound_desc{
                .volume = desc.volume,
                .pitch = desc.pitch,
                .loop = desc.loop,
                .mixer = desc.mixer == nullptr? nullptr : desc.mixer->mixer,
                },
            (const uint8_t*)data, (uint32_t)data_size,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Sound::~Sound() {
        if (!arena) eaa_delete_sound(sound);
    }

    void
    Sound::play(
        void
        ) {
        eaa_play_sound(sound);
    }

    void
    Sound::stop(
        void
        ) {
        eaa_stop_sound(sound);
    }

    void
    Sound::set_volume(
        float volume
        ) {
        eaa_set_sound_volume(sound, volume);
    }
    
    void
    Sound::set_pitch(
        float pitch
        ) {
        eaa_set_sound_pitch(sound, pitch);
    }

    void
    Sound::set_loop(
        bool loop
        ) {
        eaa_set_sound_loop(sound, loop);
    }
};
namespace ear{
    Texture::Texture(
        TextureDesc desc,
        uint8_t pixels[],
        uint32_t width, uint32_t height,
        eau::Arena* arena
        ) {
        auto _desc = ear_texture_desc{
            .filter = (ear_texture_filter)desc.filter,
                .type = (ear_texture_type)desc.type,
                .wrap = (ear_texture_wrap)desc.wrap,
            };
        std::copy(desc.wrap_color, desc.wrap_color + 4, _desc.wrap_color);

        texture = ear_create_texture(
            _desc,
            pixels,
            width, height,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Texture::Texture(
        TextureDesc desc,
        const char* data, size_t data_size,
        eau::Arena* arena
        ) {
        auto _desc = ear_texture_desc{
            .filter = (ear_texture_filter)desc.filter,
                .type = (ear_texture_type)desc.type,
                .wrap = (ear_texture_wrap)desc.wrap,
            };
        std::copy(desc.wrap_color, desc.wrap_color + 4, _desc.wrap_color);

        texture = ear_load_texture(
            _desc,
            data, data_size,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Texture::~Texture() {
        if (!arena) ear_delete_texture(texture);
    }

    void
    Texture::bind(
        uint32_t slot
        ) {
        ear_bind_texture(texture, slot);
    }

    void
    Texture::resize(
        uint32_t width,
        uint32_t height
        ) {
        ear_resize_texture(texture, width, height);
    }

    std::array<float,4>
    Texture::get_color(
        uint32_t x, uint32_t y
        ) {
        float col[4];
        ear_get_texture_color(texture, x,y, &col);
        return std::array<float,4>{ col[0], col[1], col[2], col[3] };
    }

    void
    Texture::set_color(
        uint32_t x, uint32_t y,
        float col[4]
        ) {
        ear_set_texture_color(texture, x, y, col);
    }

    void
    Texture::update(
        void
        ) {
        ear_update_texture(texture);
    }

    Buffer::Buffer(
        BufferDesc desc,
        void* data,
        uint32_t size,
        eau::Arena* arena
        ) {
        buffer = ear_create_buffer(
            ear_buffer_desc{
                .type = (ear_buffer_type)desc.type,
                .usage = (ear_buffer_usage)desc.usage,
                .stride = desc.stride,
                },
            data,
            size,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Buffer::~Buffer() {
        if (!arena) ear_delete_buffer(buffer);
    }
    
    void
    Buffer::bind(
        uint32_t slot
        ) {
        ear_bind_buffer(buffer, slot);
    }

    void
    Buffer::update(
        void
        ) {
        ear_update_buffer(buffer);
    }

    Framebuffer::Framebuffer(
        FramebufferDesc desc,
        eau::Arena* arena
        ) {
        std::vector<ear_texture*> out_colors;
        for (auto col : desc.out_colors) out_colors.push_back(col->texture);

        framebuffer = ear_create_framebuffer(
            ear_framebuffer_desc{
                .out_colors = out_colors.data(),
                .out_color_amt = (uint32_t)out_colors.size(),
                .out_depth = desc.out_depth == nullptr? nullptr : desc.out_depth->texture,
                .width = desc.width, .height = desc.height,
                },
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Framebuffer::~Framebuffer() {
        if (!arena) ear_delete_framebuffer(framebuffer);
    }

    void
    Framebuffer::bind(
        void
        ) {
        ear_bind_framebuffer(framebuffer);
    }

    void
    Framebuffer::resize(
        uint32_t width,
        uint32_t height
        ) {
        ear_resize_framebuffer(framebuffer, width, height);
    }

    void
    Framebuffer::set_as_default(
        void
        ) {
        ear_set_default_framebuffer(framebuffer);
    }

    void
    unbind_framebuffer(
        void
        ) {
        ear_bind_framebuffer(nullptr);
    }

    void
    unset_default_framebuffer(
        void
        ) {
        ear_set_default_framebuffer(nullptr);
    }

    Pipeline::Pipeline(
        PipelineDesc desc,
        eau::Arena* arena
        ) {
        std::vector<ear_vertex_attrib_desc> vertex_attribs;
        for (auto attrib : desc.vertex_attribs) vertex_attribs.push_back(ear_vertex_attrib_desc{
                .location = attrib.location,
                .type = (ear_primitive_type)attrib.type,
                .components = attrib.components,
                .norm = attrib.norm,
                .offset = attrib.offset,
                .slot = attrib.slot,
                });

        pipeline = ear_create_pipeline(
            ear_pipeline_desc{
                .vertex = ear_shader_desc{ .source = desc.vertex.source },
                .fragment = ear_shader_desc{ .source = desc.fragment.source },

                .vertex_attribs = vertex_attribs.data(),
                .vertex_attrib_amt = (uint32_t)vertex_attribs.size(),

                .depth = desc.depth,

                .cull_mode = (ear_cull_mode)desc.cull_mode,
                .front_face = (ear_front_face)desc.front_face,

                .blend_state = { 
                    .has_state = desc.blend_state.used, 
                    .state = ear_blend_state{
                        .src_color = (ear_blend_factor)desc.blend_state.src_color,
                        .dst_color = (ear_blend_factor)desc.blend_state.dst_color,
                        .color_op = (ear_blend_op)desc.blend_state.color_op,
                        .src_alpha = (ear_blend_factor)desc.blend_state.src_alpha,
                        .dst_alpha = (ear_blend_factor)desc.blend_state.dst_alpha,
                        .alpha_op = (ear_blend_op)desc.blend_state.alpha_op,
                        },
                    },

                .fill_mode = (ear_fill_mode)desc.fill_mode,
                },
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Pipeline::~Pipeline() {
        if (!arena) ear_delete_pipeline(pipeline);
    }

    void
    Pipeline::bind(
        void
        ) {
        ear_bind_pipeline(pipeline);
    }

    Texarray::Texarray(
        TexarrayDesc desc,
        eau::Arena* arena
        ) {
        auto _desc = ear_texarray_desc{
            .filter = (ear_texture_filter)desc.filter,
            .type = (ear_texture_type)desc.type,
            .wrap = (ear_texture_wrap)desc.wrap,
            .width = desc.width, .height = desc.height,
            .layers = desc.layers,
            };
        std::copy(desc.wrap_color, desc.wrap_color + 4, _desc.wrap_color);

        texarray = ear_create_texarray(
            _desc,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Texarray::~Texarray() {
        if (!arena) ear_delete_texarray(texarray);
    }

    void
    Texarray::bind(
        uint32_t slot
        ) {
        ear_bind_texarray(texarray, slot);
    }

    void
    Texarray::add(
        Texture* tex,
        uint32_t layer
        ) {
        ear_add_to_texarray(texarray, tex == nullptr? nullptr : tex->texture, layer);
    }
    
    void
    Texarray::update(
        void
        ) {
        ear_update_texarray(texarray);
    }

    void
    Texarray::update_layer(
        uint32_t layer
        ) {
        ear_update_texarray_layer(texarray, layer);
    }

    Camera::Camera(
        CameraDesc desc,
        eau::Arena* arena
        ) {
        ear_camera_desc _desc = (ear_camera_desc){
            .fb = desc.fb == nullptr? nullptr : desc.fb->framebuffer,
            .type = (ear_camera_type)desc.type,
            };

        switch (desc.type) {
        case CameraType::Cam2d:
            _desc.desc_2d = (ear_camera_2d_desc){
                .x = desc.desc_2d.pos.x,
                .y = desc.desc_2d.pos.y,
                .scalex = desc.desc_2d.scale.x,
                .scaley = desc.desc_2d.scale.y,
                .rotation = desc.desc_2d.rotation,
                };

            break;
        case CameraType::Cam3d:
            _desc.desc_3d = (ear_camera_3d_desc){
                };

            break;
        }

        camera = ear_create_camera(
            _desc,
            arena == nullptr? nullptr : arena->arena
            );
        this->arena = arena;
    }
    Camera::~Camera() {
        if (!arena) ear_delete_camera(camera);
    }

    void
    Camera::bind(
        bool ui_mode
        ) {
        ear_bind_camera(camera, ui_mode);
    }

    void
    Camera::set_position_2d(
        vec2<float> pos
        ) {
        ear_set_camera_position_2d(camera, pos.x,pos.y);
    }
    void
    Camera::set_position_2d(
        float x, float y
        ) {
        ear_set_camera_position_2d(camera, x,y);
    }

    void
    Camera::set_scale_2d(
        vec2<float> scale
        ) {
        ear_set_camera_scale_2d(camera, scale.x,scale.y);
    }
    void
    Camera::set_scale_2d(
        float x, float y
        ) {
        ear_set_camera_scale_2d(camera, x,y);
    }

    void
    Camera::set_rotation_2d(
        float angle
        ) {
        ear_set_camera_rotation_2d(camera, angle);
    }

    void
    unbind_camera(
        void
        ) {
        ear_bind_camera(nullptr, false);
    }

    Font::Font(
        FontType type,
        const char* data, size_t data_size,
        eau::Arena* arena
        ) {
        switch (type) {
        case FontType::BitmapMono:
            font = ear_load_bitmap_mono_font(data, data_size, arena == nullptr? nullptr : arena->arena);
            break;
        case FontType::Truetype:
            font = ear_load_truetype_font(data, data_size, arena == nullptr? nullptr : arena->arena);
            break;
        }
    }
    Font::~Font() {
        if (!arena) ear_delete_font(font);
    }

    void text(Font* font, std::string text, float x, float y, float scale, std::array<float,4> col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), x,y, scale, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void text(Font* font, std::string text, vec2<float> pos, float scale, std::array<float,4> col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), pos.x,pos.y, scale, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void text(Font* font, std::string text, float x, float y, float scale, std::array<float,3> col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), x,y, scale, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void text(Font* font, std::string text, vec2<float> pos, float scale, std::array<float,3> col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), pos.x,pos.y, scale, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void text(Font* font, std::string text, float x, float y, float scale, float col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), x,y, scale, (float[4]){ col, col, col, 1 }, (eau_align)align); }
    void text(Font* font, std::string text, vec2<float> pos, float scale, float col, eau::Align align) {
        ear_text(font->font, (char*)text.c_str(), pos.x,pos.y, scale, (float[4]){ col, col, col, 1 }, (eau_align)align); }

    void
    clear(
        std::array<float,3> col
        ) {
        ear_clear((float[3]){ col[0], col[1], col[2] });
    }

    void
    draw(
        int32_t vertices,
        int32_t instances,
        DrawMode draw_mode
        ) {
        ear_draw(vertices, instances, (ear_draw_mode)draw_mode);
    }

    void
    mask(
        float x, float y, float w, float h
        ) {
        ear_mask(x,y,w,h);
    }
    void
    mask(
        vec2<float> pos, vec2<float> size
        ) {
        ear_mask(pos.x,pos.y, size.x,size.y);
    }

    void
    flush(
        void
        ) {
        ear_flush();
    }

    void rect(float x, float y, float w, float h, std::array<float,4> col, eau::Align align) {
        ear_rect(x,y, w,h, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void rect(vec2<float> pos, vec2<float> size, std::array<float,4> col, eau::Align align) {
        ear_rect(pos.x,pos.y, size.x,size.y, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void rect(float x, float y, float w, float h, std::array<float,3> col, eau::Align align) {
        ear_rect(x,y, w,h, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void rect(vec2<float> pos, vec2<float> size, std::array<float,3> col, eau::Align align) {
        ear_rect(pos.x,pos.y, size.x,size.y, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void rect(float x, float y, float w, float h, float col, eau::Align align) {
        ear_rect(x,y, w,h, (float[4]){ col, col, col, col }, (eau_align)align); }
    void rect(vec2<float> pos, vec2<float> size, float col, eau::Align align) {
        ear_rect(pos.x,pos.y, size.x,size.y, (float[4]){ col, col, col, col }, (eau_align)align); }

    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, sx,sy,sw,sh, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> samp, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, samp[0],samp[1],samp[2],samp[3], (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float w, float h, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, x,y, sw,sh, sx,sy,sw,sh, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> samp, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, samp[2],samp[3], samp[0],samp[1],samp[2],samp[3], (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, x,y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, sx,sy,sw,sh, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> samp, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, samp[0],samp[1],samp[2],samp[3], (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float w, float h, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, x,y, sw,sh, sx,sy,sw,sh, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> samp, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, samp[2],samp[3], samp[0],samp[1],samp[2],samp[3], (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, x,y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], 1 }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, std::array<float,3> col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col[0], col[1], col[2], col[3] }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, float col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, sx,sy,sw,sh, (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> samp, float col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, samp[0],samp[1],samp[2],samp[3], (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float w, float h, float col, eau::Align align) {
        ear_tex(tex->texture, x,y, w,h, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, float col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, size.x,size.y, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, float col, eau::Align align) {
        ear_tex(tex->texture, x,y, sw,sh, sx,sy,sw,sh, (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> samp, float col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, samp[2],samp[3], samp[0],samp[1],samp[2],samp[3], (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, float x, float y, float col, eau::Align align) {
        ear_tex(tex->texture, x,y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col, col, col, col }, (eau_align)align); }
    void tex(Texture* tex, vec2<float> pos, float col, eau::Align align) {
        ear_tex(tex->texture, pos.x,pos.y, tex->texture->width,tex->texture->height, 0,0,tex->texture->width,tex->texture->height, (float[4]){ col, col, col, col }, (eau_align)align); }

    void
    translate(
        float x, float y
        ) {
        ear_translate(x,y);
    }
    void
    translate(
        vec2<float> pos
        ) {
        ear_translate(pos.x,pos.y);
    }
    void
    scale(
        float x, float y
        ) {
        ear_scale(x,y);
    }
    void
    scale(
        vec2<float> scale
        ) {
        ear_scale(scale.x,scale.y);
    }
    void
    rotate(
        float ang
        ) {
        ear_rotate(ang);
    }
    void
    rev_translate(
        float x, float y
        ) {
        ear_rev_translate(x,y);
    }
    void
    rev_translate(
        vec2<float> pos
        ) {
        ear_rev_translate(pos.x,pos.y);
    }
    void
    rev_scale(
        float x, float y
        ) {
        ear_rev_scale(x,y);
    }
    void
    rev_scale(
        vec2<float> scale
        ) {
        ear_rev_scale(scale.x,scale.y);
    }
    void
    rev_rotate(
        float ang
        ) {
        ear_rev_rotate(ang);
    }
    void
    reset_transform(
        void
        ) {
        ear_reset_transform();
    }
    Mat4
    save_transform(
        void
        ) {
        Mat4 mat;
        ear_save_transform(&mat.mat);
        return mat;
    }
    void
    load_transform(
        Mat4& mat
        ) {
        ear_load_transform(mat.mat);
    }
};


Mat4::Mat4() {
    eau_mat4_identity(&this->mat);
}

Mat4 
Mat4::operator*(const Mat4& b) {
    Mat4 res;
    eau_mat4_mult(&res.mat, res.mat, b.mat);
    return res;
}

Mat4 
Mat4::ortho(
    float left, float right,
    float bottom, float top,
    float near, float far
    ) {
    Mat4 mat;
    eau_mat4_ortho(
        &mat.mat,
        left, right,
        bottom, top,
        near, far
        );
    return mat;
}

Mat4
Mat4::translate(
    vec3<float> pos
    ) {
    Mat4 res;
    eau_mat4_translate(&res.mat, pos.x,pos.y,pos.z);
    return res;
}

Mat4
Mat4::scale(
    vec3<float> size
    ) {
    Mat4 res;
    eau_mat4_scale(&res.mat, size.x,size.y,size.z);
    return res;
}

Mat4
Mat4::rotate_x(
    float ang
    ) {
    Mat4 res;
    eau_mat4_rotate_x(&res.mat, ang);
    return res;
}
Mat4
Mat4::rotate_y(
    float ang
    ) {
    Mat4 res;
    eau_mat4_rotate_y(&res.mat, ang);
    return res;
}
Mat4
Mat4::rotate_z(
    float ang
    ) {
    Mat4 res;
    eau_mat4_rotate_z(&res.mat, ang);
    return res;
}
