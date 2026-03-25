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
    function(sex exname)
        add_executable(${exname})
        set_target_properties(${exname}
            PROPERTIES
                OUTPUT_NAME ex_${exname}
        )
        target_sources(${exname}
                PRIVATE
                    examples/${exname}/main.c
            )
        target_link_libraries(${exname}
            PRIVATE
                ceat
        )
        target_compile_options(${exname}
            PRIVATE
                -std=c23
        )
    endfunction()

    sex(window)
    sex(triangle)
    sex(texture)
    sex(framebuffer)
    sex(text)
    sex(input)
    sex(object)
    sex(clock)
    sex(console)
    sex(audio)
    sex(camera)
    sex(debug)
    sex(timer)

    function(cppex exname)
        add_executable(cpp_${exname})
        set_target_properties(cpp_${exname}
            PROPERTIES
                OUTPUT_NAME ex_cpp_${exname}
        )
        target_sources(cpp_${exname}
            PRIVATE
                examples/foreign/cpp/${exname}/main.cpp
        )
        target_link_libraries(cpp_${exname}
            PRIVATE
                ceat
                ceat_cpp
        )
    endfunction()

    cppex(window)
    cppex(object)

    function(odindindone exname)
        set(out ${CMAKE_BINARY_DIR}/ex_odin_${exname})
        set(in ${CMAKE_SOURCE_DIR}/examples/foreign/odin/${exname})
        add_custom_command(
            OUTPUT ${out}
            COMMAND odin build ${in} -out:${out}
            DEPENDS ${in}/main.odin
            #BYPRODUCTS ${out}
            COMMENT "building odin example: ${exname}"
        )

        add_custom_target(odin_${exname} ALL
            DEPENDS ${out}
        )
    endfunction()

    odindindone(window)
    odindindone(object)
endif()
