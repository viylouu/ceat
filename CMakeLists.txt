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

#add_compile_options(-fsanitize=address -g)
#add_link_options(-fsanitize=address)

add_library(ceat STATIC)
target_sources(ceat 
    PRIVATE 
        core/eat.c

        core/debug/debug.c
        core/debug/console.c

        core/eaw/eaw.c
        core/eaw/window.c
        core/eaw/input.c

        core/ear/ear.c
        core/ear/gl.c
        core/ear/pipeline.c
        core/ear/buffer.c
        core/ear/framebuffer.c
        core/ear/texarray.c
        core/ear/text.c
        core/ear/texture.c
        core/ear/camera.c
        core/ear/misc.c
        core/ear/data.c
        core/ear/user.c

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
            GL
            GLX
            EGL
            m
    )
else()
    target_link_libraries(ceat
        PRIVATE
            opengl32
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

    add_executable(ex_triangle)
    target_sources(ex_triangle
            PRIVATE
                examples/triangle/main.c
        )
    target_link_libraries(ex_triangle
        PRIVATE
            ceat
    )

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
endif()
