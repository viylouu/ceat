cmake_minimum_required( VERSION 3.23 )
project(ceat)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM MATCHES Linux)
    option(BUILD_LINUX ":3" ON)
elseif(CMAKE_SYSTEM MATCHES Windows)
    option(BUILD_LINUX ":3" OFF)
endif()

if (NOT BUILD_LINUX)
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
    set(GLFW_USE_WAYLAND OFF CACHE INTERNAL "")
    set(GLFW_USE_X11 OFF CACHE INTERNAL "")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -static-libstdc++ -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -pthread")

    add_subdirectory(include/glfw)
else()
    find_package(glfw3 REQUIRED)
endif()

find_program(GLSLC glslc)
if (NOT GLSLC)
    message(FATAL_ERROR "glslc not found! make sure it is in your path or you have it installed!")
endif()

function(compile_shader BASE IN OUT)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${BASE}/${OUT}
        COMMAND ${GLSLC} ${CMAKE_CURRENT_SOURCE_DIR}/${BASE}/${IN} -o ${CMAKE_CURRENT_SOURCE_DIR}/${BASE}/${OUT}
        DEPENDS ${BASE}/${IN}
        COMMENT "in ${BASE}: compiling ${IN} to ${OUT}"
    )
endfunction()

#add_compile_options(-fsanitize=address -g)
#add_link_options(-fsanitize=address)

compile_shader(core/ear/hl/shaders rect.vert rect_v.spv)
compile_shader(core/ear/hl/shaders rect.frag rect_f.spv)

add_custom_target(ceat_shad ALL
    DEPENDS
        core/ear/hl/shaders/rect_v.spv
        core/ear/hl/shaders/rect_f.spv
)

add_library(ceat STATIC)
target_sources(ceat 
    PRIVATE 
        core/eat.c

        core/debug/debug.c
        core/debug/console.c

        core/eaw/eaw.c
        core/eaw/window.c
        core/eaw/input.c
        core/eaw/vk.c

        core/ear/ear.c
        core/ear/ll/vk/vk.c
        core/ear/ll/vk/init/validation.c
        core/ear/ll/vk/init/instance.c
        core/ear/ll/vk/init/surface.c
        core/ear/ll/vk/init/device_phys.c
        core/ear/ll/vk/init/device_log.c
        core/ear/ll/vk/init/queue_fam.c
        core/ear/ll/vk/init/comm_pool.c
        core/ear/ll/vk/init/comm_buffer.c
        core/ear/ll/vk/init/sync.c
        core/ear/ll/vk/sc/swapchain.c
        core/ear/ll/vk/sc/image_views.c
        core/ear/ll/vk/sc/render_pass.c
        core/ear/ll/vk/sc/framebuffer.c
        core/ear/ll/vk/util/buffer.c
        core/ear/ll/vk/eng/pipeline.c
        core/ear/ll/vk/eng/shader_mod.c
        core/ear/ll/vk/eng/screen.c
        core/ear/ll/vk/eng/buffer.c
        core/ear/ll/misc.c
        core/ear/ll/pipeline.c
        #core/ear/ll/texture.c
        core/ear/ll/buffer.c
        #core/ear/ll/framebuffer.c
        #core/ear/ll/texarray.c
        
        #core/ear/hl/text.c
        #core/ear/hl/camera.c
        core/ear/hl/data.c
        core/ear/hl/user.c

        core/eau/file.c
        core/eau/mat4.c
        core/eau/coll.c
        core/eau/arena.c
        core/eau/clock.c
        core/eau/object.c
        core/eau/conv.c
        core/eau/ecs.c
        core/eau/timer.c

        core/eaa/eaa.c
        core/eaa/engine.c
        core/eaa/sound.c
        core/eaa/mixer.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            .
        FILES
            eat.h
)
target_link_libraries(ceat
    PRIVATE
        glfw
)
target_compile_options(ceat
    PRIVATE
        -std=c23
        #-g
        #-O0
)

if(BUILD_LINUX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ceat
        PRIVATE
            #GL
            #GLX
            #EGL
            vulkan
            m
    )
else()
    target_link_libraries(ceat
        PRIVATE
            #opengl32
            vulkan1
            gdi32
            user32
            shell32
            glfw
    )
    target_compile_definitions(ceat 
        PRIVATE 
            _WIN32 
            WINDOWS
    )
endif()

#[[
add_library(ceat_cpp STATIC)
target_sources(ceat_cpp
    PRIVATE
        bindings/cpp/eat.cpp
)
target_link_libraries(ceat_cpp
    PRIVATE
        ceat
)
target_compile_options(ceat_cpp
    PRIVATE
        -std=c++20
)
]]#

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_executable(ex_window)
    target_sources(ex_window
            PRIVATE
                examples/window/main.c
        )
    target_link_libraries(ex_window
        PRIVATE
            ceat
    )

    compile_shader(examples/triangle shad.vert shad_v.spv)
    compile_shader(examples/triangle shad.frag shad_f.spv)

    add_custom_target(ex_triangle_shad ALL
        DEPENDS
            examples/triangle/shad_v.spv
            examples/triangle/shad_f.spv
    )

    add_executable(ex_triangle)
    target_sources(ex_triangle
            PRIVATE
                examples/triangle/main.c
        )
    target_link_libraries(ex_triangle
        PRIVATE
            ceat
    )

    compile_shader(examples/vbuffer shad.vert shad_v.spv)
    compile_shader(examples/vbuffer shad.frag shad_f.spv)

    add_custom_target(ex_vbuffer_shad ALL
        DEPENDS
            examples/vbuffer/shad_v.spv
            examples/vbuffer/shad_f.spv
    )

    add_executable(ex_vbuffer)
    target_sources(ex_vbuffer
            PRIVATE
                examples/vbuffer/main.c
        )
    target_link_libraries(ex_vbuffer
        PRIVATE
            ceat
    )

    compile_shader(examples/ibuffer shad.vert shad_v.spv)
    compile_shader(examples/ibuffer shad.frag shad_f.spv)

    add_custom_target(ex_ibuffer_shad ALL
        DEPENDS
            examples/ibuffer/shad_v.spv
            examples/ibuffer/shad_f.spv
    )

    add_executable(ex_ibuffer)
    target_sources(ex_ibuffer
            PRIVATE
                examples/ibuffer/main.c
        )
    target_link_libraries(ex_ibuffer
        PRIVATE
            ceat
    )

    #[[
    add_executable(ex_texture)
    target_sources(ex_texture
            PRIVATE
                examples/texture/main.c
        )
    target_link_libraries(ex_texture
        PRIVATE
            ceat
    )
    target_compile_options(ex_texture
        PRIVATE
            -std=c23
    )

    add_executable(ex_framebuffer)
    target_sources(ex_framebuffer
            PRIVATE
                examples/framebuffer/main.c
        )
    target_link_libraries(ex_framebuffer
        PRIVATE
            ceat
    )
    target_compile_options(ex_framebuffer
        PRIVATE
            -std=c23
    )

    add_executable(ex_text)
    target_sources(ex_text
            PRIVATE
                examples/text/main.c
        )
    target_link_libraries(ex_text
        PRIVATE
            ceat
    )
    target_compile_options(ex_text
        PRIVATE
            -std=c23
    )

    add_executable(ex_input)
    target_sources(ex_input
        PRIVATE
            examples/input/main.c
    )
    target_link_libraries(ex_input
        PRIVATE
            ceat
    )

    add_executable(ex_object)
    target_sources(ex_object
        PRIVATE
            examples/object/main.c
    )
    target_link_libraries(ex_object
        PRIVATE
            ceat
    )

    add_executable(ex_clock)
    target_sources(ex_clock
        PRIVATE
            examples/clock/main.c
    )
    target_link_libraries(ex_clock
        PRIVATE
            ceat
    )

    add_executable(ex_console)
    target_sources(ex_console
        PRIVATE
            examples/console/main.c
    )
    target_link_libraries(ex_console
        PRIVATE
            ceat
    )

    add_executable(ex_audio)
    target_sources(ex_audio
        PRIVATE
            examples/audio/main.c
    )
    target_link_libraries(ex_audio
        PRIVATE
            ceat
    )

    add_executable(ex_camera)
    target_sources(ex_camera
        PRIVATE
            examples/camera/main.c
    )
    target_link_libraries(ex_camera
        PRIVATE
            ceat
    )

    add_executable(ex_debug)
    target_sources(ex_debug
        PRIVATE
            examples/debug/main.c
    )
    target_link_libraries(ex_debug
        PRIVATE
            ceat
    )

    add_executable(ex_timer)
    target_sources(ex_timer
        PRIVATE
            examples/timer/main.c
    )
    target_link_libraries(ex_timer
        PRIVATE
            ceat
    )

    add_executable(ex_cpp_window)
    target_sources(ex_cpp_window
        PRIVATE
            examples/foreign/cpp/window/main.cpp
    )
    target_link_libraries(ex_cpp_window
        PRIVATE
            ceat
            ceat_cpp
    )

    add_executable(ex_cpp_object)
    target_sources(ex_cpp_object
        PRIVATE
            examples/foreign/cpp/object/main.cpp
    )
    target_link_libraries(ex_cpp_object
        PRIVATE
            ceat
            ceat_cpp
    )
    ]]#
endif()
