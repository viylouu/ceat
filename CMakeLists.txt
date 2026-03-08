cmake_minimum_required( VERSION 3.23 )
project(ceat)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)

option(BUILD_LINUX "do a linux build instead of windows" ON)

add_library(ceat STATIC)
target_sources(ceat
        PRIVATE
            core/eat.c

            core/eaw/eaw.c
            core/eaw/window.c
            core/eaw/input.c

            core/ear/ear.c
            core/ear/gl.c
            core/ear/pipeline.c
            core/ear/buffer.c
            core/ear/texture.c
            core/ear/framebuffer.c
            core/ear/texarray.c
            core/ear/text.c
            core/ear/data.c
            core/ear/user.c

            core/eau/file.c
            core/eau/mat4.c
            core/eau/coll.c
            core/eau/arena.c
            core/eau/clock.c
            core/eau/object.c
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

add_executable(ex_cpp)
target_sources(ex_cpp
    PRIVATE
        examples/foreign/cpp/main.cpp
)
target_link_libraries(ex_cpp
    PRIVATE
        ceat
        ceat_cpp
)
