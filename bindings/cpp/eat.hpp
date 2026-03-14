#pragma once

#include "../../eat.h"

#include <string>
#include <vector>
#include <array>
#include <math.h>

namespace eau{
    typedef struct Rect Rect;
};

template <typename T>
struct vec2{
    T x; T y;

    vec2(T x, T y);
    vec2(T n);
    vec2();

    vec2 operator+(vec2 b);
    vec2 operator-();
    vec2 operator-(vec2 b);
    vec2 operator*(vec2 b);
    vec2 operator/(vec2 b);
    vec2 operator*(T b);
    vec2 operator/(T b);
    vec2& operator+=(vec2 b);
    vec2& operator-=(vec2 b);
    vec2& operator*=(vec2 b);
    vec2& operator/=(vec2 b);
    vec2& operator*=(T b);
    vec2& operator/=(T b);
    vec2& operator=(vec2 b);
    vec2& operator=(T b);

    vec2
    space_convert(
        eau::Rect original_space,
        eau::Rect new_space
        );
    vec2
    aspect_space_convert(
        eau::Rect original_space,
        eau::Rect new_space
        );

    vec2 norm();
    T dist(vec2 b);
    T mag();
};

template <typename T>
struct vec3{
    T x; T y; T z;

    vec3(T x, T y, T z);
    vec3(T n);
    vec3();

    vec3 operator+(vec3 b);
    vec3 operator-();
    vec3 operator-(vec3 b);
    vec3 operator*(vec3 b);
    vec3 operator/(vec3 b);
    vec3 operator*(T b);
    vec3 operator/(T b);
    vec3& operator+=(vec3 b);
    vec3& operator-=(vec3 b);
    vec3& operator*=(vec3 b);
    vec3& operator/=(vec3 b);
    vec3& operator*=(T b);
    vec3& operator/=(T b);
    vec3& operator=(vec3 b);
    vec3& operator=(T b);

    vec3 norm();
    T dist(vec3 b);
    T mag();
};

struct Mat4{
    float mat[16];

    Mat4();

    Mat4 
    operator*(const Mat4& b);

    static Mat4 
    ortho(
        float left, float right,
        float bottom, float top,
        float near, float far
        );

    Mat4
    translate(
        vec3<float> pos
        );

    Mat4
    scale(
        vec3<float> size
        );

    Mat4
    rotate_x(
        float ang
        );
    Mat4
    rotate_y(
        float ang
        );
    Mat4
    rotate_z(
        float ang
        );
};

namespace eaw{
    enum class KeyState{
        Released,
        Pressed,
        Held,
        Inactive,
    };

    enum class Key{
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
    };
    enum class Mouse{
        Left, Right, Middle,
        Last,
    };

    enum class MouseMode{
        Normal,
        Hidden,
        Locked,
    };

    extern vec2<float> mouse;
    extern vec2<double> mouse64;
    extern vec2<float> mouse_delta;
    extern vec2<double> mouse_delta64;
    extern vec2<float> mouse_scroll;
    extern vec2<double> mouse_scroll64;

    extern std::string text_input;
    extern std::string tick_text_input;

    bool
    is_key(
        Key key
        );
    bool
    is_key_pressed(
        Key key
        );
    bool
    is_key_released(
        Key key
        );

    bool
    is_mouse(
        Mouse mouse
        );
    bool
    is_mouse_pressed(
        Mouse mouse
        );
    bool
    is_mouse_released(
        Mouse mouse
        );

    bool
    tick_is_key(
        Key key
        );
    bool
    tick_is_key_pressed(
        Key key
        );
    bool
    tick_is_key_released(
        Key key
        );

    bool
    tick_is_mouse(
        Mouse mouse
        );
    bool
    tick_is_mouse_pressed(
        Mouse mouse
        );
    bool
    tick_is_mouse_released(
        Mouse mouse
        );

    void
    set_mouse_mode(
        MouseMode mode
        );
};
namespace eat{
    extern int32_t width;
    extern int32_t height;

    extern float time;
    extern float delta;
    extern double time64;
    extern double delta64;

    struct console_desc{
        bool enabled = false;
        eaw::Key key = eaw::Key::Enter;
        void (*command_solver)(std::string command) = nullptr;
    };

    struct debug_desc{
        bool enabled = false;
        eaw::Key key = eaw::Key::F8;
    };

    struct init_opts{
        bool vsync = true;
        console_desc console;
        debug_desc debug;
    };

    void
    init(
        std::string title,
        int32_t width, int32_t height,
        init_opts opts
        );

    void
    stop(
        void
        );

    bool
    frame(
        void
        );
};
namespace eau{
    typedef struct Arena Arena;
    struct Destructor{
        eau_destructor* destructor;
        Arena* arena;
    };

    struct Arena{
        eau_arena* arena;

        Arena();
        ~Arena();

        void
        clear(
            void
            );

        void
        add(
            Arena* arena,
            Destructor* user_dest,
            void* data,
            void (*delete_fn)(void* data)
            );
    };

    enum class Align{
        TopLeft, Top, TopRight,
        MidLeft, Mid, MidRight,
        BotLeft, Bot, BotRight,
    };

    struct Rect{
        vec2<float> pos;
        vec2<float> size;
        Align align = Align::TopLeft;

        Rect
        topleftify(
            void
            );
        Rect
        alignify(
            Align align
            );

        Rect
        scale_to_fit(
            Rect fit
            );

        Rect
        space_convert(
            Rect original_space,
            Rect new_space
            );
        Rect
        aspect_space_convert(
            Rect original_space,
            Rect new_space
            );
    };

    struct CollisionInfo{
        vec3<float> norm;
        float depth;
    };

    bool
    aabb2d(
        vec2<float> min1,
        vec2<float> max1,
        vec2<float> min2,
        vec2<float> max2
        );

    bool
    point_aabb2d(
        vec2<float> point,
        vec2<float>min, vec2<float>max
        );

    bool
    rect_rect(
        Rect a,
        Rect b
        );

    bool
    point_rect(
        vec2<float> point,
        Rect rect
        );

    bool
    aabb3d(
        vec3<float> min1,
        vec3<float> max1,
        vec3<float> min2,
        vec3<float> max2
        );

    bool
    gjk3d(
        std::vector<float[3]> hull1,
        std::vector<float[3]> hull2,
        vec3<float> (*out_simplex)[4]
        );

    CollisionInfo
    epa3d(
        vec3<float> simplex[4],
        std::vector<float[3]> hull1,
        std::vector<float[3]> hull2
        );

    char*
    load_file(
        std::string path,
        size_t* out_size
        );

    struct Clock{
        eau_clock* clock;
        Arena* arena;

        Clock(
            Arena* arena = nullptr
            );
        ~Clock();

        void
        reset(
            void
            );

        void
        set_speed(
            float speed
            );

        void
        set_time(
            float time
            );
        void
        set_time(
            double time
            );

        void
        start(
            void
            );

        void
        stop(
            void
            );
    };

    void
    update_clocks(
        void
        );


    template <typename T>
    void _object_init(eau_object* _obj);

    template <typename T>
    void _object_stop(eau_object* _obj);

    template <typename T>
    void _object_draw(eau_object* _obj);

    template <typename T>
    void _object_tick(eau_object* _obj);

    template <typename T>
    struct Object{
        eau_object* object;
        Arena* arena;

        float delta;

        struct position_tag {};
        struct rotation_tag {};

protected:
        Object(
            int layer = 0,
            Arena* arena = nullptr
            ) {
            eau_object_desc desc = eau_object_desc{
                .pos3d = { nullptr }, .pos3d64 = { nullptr },
                .rot3d = { nullptr }, .rot3d64 = { nullptr },
                .pos2d = { nullptr }, .pos2d64 = { nullptr },
                .rot2d =   nullptr,   .rot2d64 =   nullptr,

                .init = &_object_init<T>,
                .tick = &_object_tick<T>,
                .draw = &_object_draw<T>,
                .stop = &_object_stop<T>,

                .render_layer = layer,
                };

            T* obj = (T*)this;
            if constexpr (requires { obj->get(position_tag{}); }) {
                auto& pos = obj->get(position_tag{}); {
                    using type = std::remove_cvref_t<decltype(pos)>;

                    if constexpr (std::is_same_v<type, vec3<float>>) {
                        desc.pos3d[0] = &pos.x;
                        desc.pos3d[1] = &pos.y;
                        desc.pos3d[2] = &pos.z;
                    } else if constexpr (std::is_same_v<type, vec3<double>>) {
                        desc.pos3d64[0] = &pos.x;
                        desc.pos3d64[1] = &pos.y;
                        desc.pos3d64[2] = &pos.z;
                    } else if constexpr (std::is_same_v<type, vec2<float>>) {
                        desc.pos2d[0] = &pos.x;
                        desc.pos2d[1] = &pos.y;
                    } else if constexpr (std::is_same_v<type, vec2<double>>) {
                        desc.pos2d64[0] = &pos.x;
                        desc.pos2d64[1] = &pos.y;
                    }
                }
            }
            if constexpr (requires { obj->get(rotation_tag{}); }) {
                auto& rot = obj->get(rotation_tag{}); {
                    using type = std::remove_cvref_t<decltype(rot)>;

                    if constexpr (std::is_same_v<type, vec3<float>>) {
                        desc.rot3d[0] = &rot.x;
                        desc.rot3d[1] = &rot.y;
                        desc.rot3d[2] = &rot.z;
                    } else if constexpr (std::is_same_v<type, vec3<double>>) {
                        desc.rot3d64[0] = &rot.x;
                        desc.rot3d64[1] = &rot.y;
                        desc.rot3d64[2] = &rot.z;
                    } else if constexpr (std::is_same_v<type, float>) desc.rot2d = &rot;
                    else if constexpr (std::is_same_v<type, double>) desc.rot2d64 = &rot;
                }
            }

            object = eau_create_object(
                desc, 
                this, 
                arena == nullptr? nullptr : arena->arena
                );
            this->arena = arena;
            this->delta = object->delta;
        }
        ~Object() {
            if (!arena) eau_delete_object(object);
        }

public:
        void
        init(
            void
            ) {
            eau_init_object(object);
        }

        void
        stop(
            void
            ) {
            eau_stop_object(object);
        }

        void
        draw(
            void
            ) {
            eau_draw_object(object);
        }

        void
        try_tick(
            void
            ) {
            eau_try_tick_object(object);
        }

private:
        virtual void
        on_init(
            void
            ) {}

        virtual void
        on_stop(
            void
            ) {}

        virtual void
        on_draw(
            void
            ) {}

        virtual void
        on_tick(
            void
            ) {}

        friend void _object_init<T>(eau_object*);
        friend void _object_tick<T>(eau_object*);
        friend void _object_draw<T>(eau_object*);
        friend void _object_stop<T>(eau_object*);
    };

    void
    set_object_tickrate(
        float delta
        );

    void
    init_objects(
        void
        );

    void
    stop_objects(
        void
        );

    void
    draw_objects(
        void
        );

    void
    try_tick_objects(
        void
        );

    template <typename T>
    void _object_init(eau_object* obj) {
        auto type_obj = (Object<T>*)obj->data;
        type_obj->on_init();
    }

    template <typename T>
    void _object_stop(eau_object* obj) {
        auto type_obj = (Object<T>*)obj->data;
        type_obj->on_stop();
    }

    template <typename T>
    void _object_draw(eau_object* obj) {
        auto type_obj = (Object<T>*)obj->data;
        type_obj->on_draw();
    }

    template <typename T>
    void _object_tick(eau_object* obj) {
        auto type_obj = (Object<T>*)obj->data;
        type_obj->on_tick();
    }
};
namespace eaa{
    typedef struct Mixer Mixer;

    struct MixerDesc{
        float volume = 1;
        float pitch = 1;

        Mixer* parent = nullptr;
    };

    struct Mixer{
        eaa_mixer* mixer;
        eau::Arena* arena;

        Mixer(
            MixerDesc desc,
            eau::Arena* arena = nullptr
            );
        ~Mixer();

        void
        set_volume(
            float volume
            );

        void
        set_pitch(
            float pitch
            );
    };

    struct SoundDesc{
        float volume = 1;
        float pitch = 1;
        bool loop = false;

        Mixer* mixer = nullptr;
    };

    struct Sound{
        eaa_sound* sound;
        eau::Arena* arena;

        Sound(
            SoundDesc desc,
            const char* data, size_t data_size,
            eau::Arena* arena = nullptr
            );
        ~Sound();

        void
        play(
            void
            );

        void
        stop(
            void
            );

        void
        set_volume(
            float volume
            );
        
        void
        set_pitch(
            float pitch
            );

        void
        set_loop(
            bool loop
            );
    };
};
namespace ear{
    enum class TextureFilter{
        Nearest,
        Linear,
    };

    enum class TextureType{
        Color,
        Depth,
        Hdr,
        Hdr32,
    };

    enum class TextureWrap{
        Repeat,
        Clamp,
        Color,
    };

    struct TextureDesc{
        TextureFilter filter;
        TextureType type;
        TextureWrap wrap;
            float wrap_color[4];
    };

    struct Texture{
        ear_texture* texture;
        eau::Arena* arena;

        Texture(
            TextureDesc desc,
            uint8_t pixels[],
            uint32_t width, uint32_t height,
            eau::Arena* arena = nullptr
            );
        Texture(
            TextureDesc desc,
            const char* data, size_t data_size,
            eau::Arena* arena = nullptr
            );
        ~Texture();

        void
        bind(
            uint32_t slot
            );

        void
        resize(
            uint32_t width,
            uint32_t height
            );

        std::array<float,4>
        get_color(
            uint32_t x, uint32_t y
            );

        void
        set_color(
            uint32_t x, uint32_t y,
            float col[4]
            );

        void
        update(
            void
            );
    };

    enum class BufferType{
        Uniform,
        Storage,
        Vertex,
    };

    enum class BufferUsage{
        Dynamic,
        Static,
    };

    struct BufferDesc{
        BufferType type;
        BufferUsage usage;
        uint32_t stride;
    };

    struct Buffer{
        ear_buffer* buffer;
        eau::Arena* arena;

        Buffer(
            BufferDesc desc,
            void* data,
            uint32_t size,
            eau::Arena* arena = nullptr
            );
        ~Buffer();

        void
        bind(
            uint32_t slot
            );

        void
        update(
            void
            );
    };

    struct FramebufferDesc{
        std::vector<Texture*> out_colors;
        Texture* out_depth;
        uint32_t width; uint32_t height;
    };

    struct Framebuffer{
        ear_framebuffer* framebuffer;
        eau::Arena* arena;

        Framebuffer(
            FramebufferDesc desc,
            eau::Arena* arena = nullptr
            );
        ~Framebuffer();

        void
        bind(
            void
            );

        void
        resize(
            uint32_t width,
            uint32_t height
            );

        void
        set_as_default(
            void
            );
    };

    void
    unbind_framebuffer(
        void
        );

    void
    unset_default_framebuffer(
        void
        );


    enum class PrimitiveType{
        Float,
        Int,
    };

    enum class CullMode{
        None,
        Front,
        Back,
    };

    enum class FrontFace{
        CW,
        CCW,
    };

    enum class BlendFactor{
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
    };

    enum class BlendOp{
        Add,
        Subtract,
        RevSubtract,
        Min,
        Max,
    };

    enum class FillMode{
        Fill,
        Line,
    };

    struct ShaderDesc{
        char* source;
    };

    struct VertexAttribDesc{
        uint32_t location;
        PrimitiveType type;
        uint32_t components;
        bool norm;
        uint32_t offset;
        uint32_t slot;
    };

    struct BlendState{
        BlendFactor src_color; BlendFactor dst_color;
        BlendOp color_op;
        BlendFactor src_alpha; BlendFactor dst_alpha;
        BlendOp alpha_op;

        bool used = true;
    };

    struct PipelineDesc{
        ShaderDesc vertex;
        ShaderDesc fragment;

        std::vector<VertexAttribDesc> vertex_attribs;

        bool depth = false;

        CullMode cull_mode = CullMode::None;
        FrontFace front_face;

        BlendState blend_state = BlendState{ .used = false };

        FillMode fill_mode = FillMode::Fill;
    };

    struct Pipeline{
        ear_pipeline* pipeline;
        eau::Arena* arena;

        Pipeline(
            PipelineDesc desc,
            eau::Arena* arena = nullptr
            );
        ~Pipeline();

        void
        bind(
            void
            );
    };

    struct TexarrayDesc{
        TextureFilter filter;
        TextureType type;
        TextureWrap wrap;
        float wrap_color[4];

        uint32_t width; uint32_t height;
        uint32_t layers;
    };

    struct Texarray{
        ear_texarray* texarray;
        eau::Arena* arena;

        Texarray(
            TexarrayDesc desc,
            eau::Arena* arena = nullptr
            );
        ~Texarray();

        void
        bind(
            uint32_t slot
            );

        void
        add(
            Texture* tex,
            uint32_t layer
            );

        void
        update(
            void
            );

        void
        update_layer(
            uint32_t layer
            );
    };

    enum class CameraType{
        Cam2d,
        Cam3d,
    };

    struct Camera2dDesc{
        vec2<float> pos = vec2<float>{ 0,0 };
        vec2<float> scale = vec2<float>{ 1,1 };
        float rotation = 0;
    };
    struct Camera3dDesc{
        // unimplemented
    };

    struct CameraDesc{
        Framebuffer* fb = nullptr;

        CameraType type;
        union{
            Camera2dDesc desc_2d;
            Camera3dDesc desc_3d;
        };
    };

    struct Camera{
        ear_camera* camera;
        eau::Arena* arena;

        Camera(
            CameraDesc desc,
            eau::Arena* arena = nullptr
            );
        ~Camera();

        void
        bind(
            bool ui_mode = false
            );

        void
        set_position_2d(
            vec2<float> pos
            );
        void
        set_position_2d(
            float x, float y
            );

        void
        set_scale_2d(
            vec2<float> scale
            );
        void
        set_scale_2d(
            float x, float y
            );

        void
        set_rotation_2d(
            float angle
            );
    };

    enum class FontType{
        BitmapMono,
        Truetype,
    };

    struct Font{
        ear_font* font;
        eau::Arena* arena;

        Font(
            FontType type,
            const char* data, size_t data_size,
            eau::Arena* arena = nullptr
            );
        ~Font();
    };

    void
    unbind_camera(
        void
        );

    void text(Font* font, std::string text, float x, float y, float scale, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, float scale, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, float x, float y, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, float x, float y, float scale, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, float scale, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, float x, float y, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, float x, float y, float scale, float col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, float scale, float col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, float x, float y, float col, eau::Align align = eau::Align::TopLeft);
    void text(Font* font, std::string text, vec2<float> pos, float col, eau::Align align = eau::Align::TopLeft);

    void
    clear(
        std::array<float,3> col
        );

    enum class DrawMode{
        Triangles,
        Lines,
    };

    void
    draw(
        int32_t vertices,
        int32_t instances = 1,
        DrawMode draw_mode = DrawMode::Triangles
        );

    void
    mask(
        float x, float y, float w, float h
        );
    void
    mask(
        vec2<float> pos, vec2<float> size
        );

    void
    flush(
        void
        );

    void rect(float x, float y, float w, float h, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void rect(vec2<float> pos, vec2<float> size, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void rect(float x, float y, float w, float h, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void rect(vec2<float> pos, vec2<float> size, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void rect(float x, float y, float w, float h, float col, eau::Align align = eau::Align::TopLeft);
    void rect(vec2<float> pos, vec2<float> size, float col, eau::Align align = eau::Align::TopLeft);

    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> samp, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float w, float h, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> samp, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,3> samp, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float w, float h, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, std::array<float,3> samp, std::array<float,4> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, std::array<float,3> col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float w, float h, float sx, float sy, float sw, float sh, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, std::array<float,4> samp, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float w, float h, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, vec2<float> size, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float sx, float sy, float sw, float sh, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, std::array<float,4> samp, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, float x, float y, float col, eau::Align align = eau::Align::TopLeft);
    void tex(Texture* tex, vec2<float> pos, float col, eau::Align align = eau::Align::TopLeft);

    void
    translate(
        float x, float y
        );
    void
    translate(
        vec2<float> pos
        );
    void
    scale(
        float x, float y
        );
    void
    scale(
        vec2<float> scale
        );
    void
    rotate(
        float ang
        );
    void
    rev_translate(
        float x, float y
        );
    void
    rev_translate(
        vec2<float> pos
        );
    void
    rev_scale(
        float x, float y
        );
    void
    rev_scale(
        vec2<float> scale
        );
    void
    rev_rotate(
        float ang
        );
    void
    reset_transform(
        void
        );
    void
    save_transform(
        Mat4* out
        );
    void
    load_transform(
        Mat4& mat
        );
};

template <typename T>
vec2<T>::vec2(T x, T y) {
    this->x = x;
    this->y = y;
}

template <typename T>
vec2<T>::vec2(T n) {
    this->x = n;
    this->y = n;
}

template <typename T>
vec2<T>::vec2() {
    *this = vec2(0);
}

template <typename T>
vec2<T> vec2<T>::operator+(vec2 b) { return vec2(x + b.x, y + b.y); }
template <typename T>
vec2<T> vec2<T>::operator-() { return vec2(-x, -y); }
template <typename T>
vec2<T> vec2<T>::operator-(vec2 b) { return *this + -b; }
template <typename T>
vec2<T> vec2<T>::operator*(vec2 b) { return vec2(x * b.x, y * b.y); }
template <typename T>
vec2<T> vec2<T>::operator/(vec2 b) { return vec2(x / b.x, y / b.y); }
template <typename T>
vec2<T> vec2<T>::operator*(T b) { return vec2(x * b, y * b); }
template <typename T>
vec2<T> vec2<T>::operator/(T b) { return vec2(x / b, y / b); }
template <typename T>
vec2<T>& vec2<T>::operator+=(vec2 b) { return *this = vec2(x + b.x, y + b.y); }
template <typename T>
vec2<T>& vec2<T>::operator-=(vec2 b) { return *this = *this + -b; }
template <typename T>
vec2<T>& vec2<T>::operator*=(vec2 b) { return *this = vec2(x * b.x, y * b.y); }
template <typename T>
vec2<T>& vec2<T>::operator/=(vec2 b) { return *this = vec2(x / b.x, y / b.y); }
template <typename T>
vec2<T>& vec2<T>::operator*=(T b) { return *this = vec2(x * b, y * b); }
template <typename T>
vec2<T>& vec2<T>::operator/=(T b) { return *this = vec2(x / b, y / b); }
template <typename T>
vec2<T>& vec2<T>::operator=(vec2 b) { this->x = b.x; this->y = b.y; return *this; }
template <typename T>
vec2<T>& vec2<T>::operator=(T b) { return *this = vec2(b); }

template <typename T>
vec2<T>
vec2<T>::space_convert(eau::Rect original_space, eau::Rect new_space) {
    float nx; float ny;
    eau_point_space_convert(
        x,y, 
        eau_rect{
            original_space.pos.x,
            original_space.pos.y,
            original_space.size.x,
            original_space.size.y,
            (eau_align)original_space.align,
            }, 
        eau_rect{
            new_space.pos.x,
            new_space.pos.y,
            new_space.size.x,
            new_space.size.y,
            (eau_align)new_space.align,
            }, 
        &nx, &ny);
    return { nx,ny };
}

template <typename T>
vec2<T>
vec2<T>::aspect_space_convert(eau::Rect original_space, eau::Rect new_space) {
    float nx; float ny;
    eau_point_aspect_space_convert(
        x,y, 
        eau_rect{
            original_space.pos.x,
            original_space.pos.y,
            original_space.size.x,
            original_space.size.y,
            (eau_align)original_space.align,
            }, 
        eau_rect{
            new_space.pos.x,
            new_space.pos.y,
            new_space.size.x,
            new_space.size.y,
            (eau_align)new_space.align,
            }, 
        &nx, &ny);
    return { nx,ny };
}

template <typename T>
vec2<T> vec2<T>::norm() {
    return *this / mag();
}

template <typename T>
T vec2<T>::dist(vec2 b) {
    return (b - *this).mag();
}

template <typename T>
T vec2<T>::mag() {
    return sqrt(x*x + y*y);
}

template <typename T>
vec3<T>::vec3(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

template <typename T>
vec3<T>::vec3(T n) {
    *this = vec3(n,n,n);
}

template <typename T>
vec3<T>::vec3() {
    *this = vec3(0);
}

template <typename T>
vec3<T> vec3<T>::operator+(vec3 b) { return vec3(x + b.x, y + b.y, z + b.z); }
template <typename T>
vec3<T> vec3<T>::operator-() { return vec3(-x, -y, -z); }
template <typename T>
vec3<T> vec3<T>::operator-(vec3 b) { return *this + -b; }
template <typename T>
vec3<T> vec3<T>::operator*(vec3 b) { return vec3(x * b.x, y * b.y, z * b.z); }
template <typename T>
vec3<T> vec3<T>::operator/(vec3 b) { return vec3(x / b.x, y / b.y, z / b.z); }
template <typename T>
vec3<T> vec3<T>::operator*(T b) { return vec3(x * b, y * b, z * b); }
template <typename T>
vec3<T> vec3<T>::operator/(T b) { return vec3(x / b, y / b, z / b); }
template <typename T>
vec3<T>& vec3<T>::operator+=(vec3 b) { return *this = vec3(x + b.x, y + b.y, z + b.z); }
template <typename T>
vec3<T>& vec3<T>::operator-=(vec3 b) { return *this = *this + -b; }
template <typename T>
vec3<T>& vec3<T>::operator*=(vec3 b) { return *this = vec3(x * b.x, y * b.y, z * b.z); }
template <typename T>
vec3<T>& vec3<T>::operator/=(vec3 b) { return *this = vec3(x / b.x, y / b.y, z / b.z); }
template <typename T>
vec3<T>& vec3<T>::operator*=(T b) { return *this = vec3(x * b, y * b, z * b); }
template <typename T>
vec3<T>& vec3<T>::operator/=(T b) { return *this = vec3(x / b, y / b, z / b); }
template <typename T>
vec3<T>& vec3<T>::operator=(vec3 b) { this->x = b.x; this->y = b.y; this->z = b.z; return *this; }
template <typename T>
vec3<T>& vec3<T>::operator=(T b) { return *this = vec3(b); }

template <typename T>
vec3<T> vec3<T>::norm() {
    return *this / mag();
}

template <typename T>
T vec3<T>::dist(vec3 b) {
    return (b - *this).mag();
}

template <typename T>
T vec3<T>::mag() {
    return sqrt(x*x + y*y + z*z);
}
