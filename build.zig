const std = @import("std");
const builtin = @import("builtin");
const zcc = @import("compile_commands");

const csource = [_][]const u8{
    "core/eat.c",

    "core/debug/debug.c",
    "core/debug/console.c",

    "core/eaw/eaw.c",
    "core/eaw/window.c",
    "core/eaw/input.c",
    "core/eaw/vk.c",

    "core/ear/ear.c",
    "core/ear/ll/misc.c",
    "core/ear/ll/pipeline.c",
    "core/ear/ll/texture.c",
    "core/ear/ll/buffer.c",
    "core/ear/ll/bindset.c",
    "core/ear/ll/framebuffer.c",
    //"core/ear/ll/texarray.c",
    "core/ear/hl/text.c",
    "core/ear/hl/camera.c",
    "core/ear/hl/data.c",
    "core/ear/hl/user.c",

    "core/eau/mat4.c",
    "core/eau/coll.c",
    "core/eau/arena.c",
    "core/eau/clock.c",
    "core/eau/object.c",
    "core/eau/conv.c",
    "core/eau/ecs.c",
    "core/eau/timer.c",

    "core/eaa/eaa.c",
    "core/eaa/engine.c",
    "core/eaa/sound.c",
    "core/eaa/mixer.c",

    "backends/rendering/impl.c",
};

const csource_vulkan = [_][]const u8{
    "backends/rendering/vulkan/vk.c",
    "backends/rendering/vulkan/init/validation.c",
    "backends/rendering/vulkan/init/instance.c",
    "backends/rendering/vulkan/init/surface.c",
    "backends/rendering/vulkan/init/device_phys.c",
    "backends/rendering/vulkan/init/device_log.c",
    "backends/rendering/vulkan/init/queue_fam.c",
    "backends/rendering/vulkan/init/comm_pool.c",
    "backends/rendering/vulkan/init/comm_buffer.c",
    "backends/rendering/vulkan/init/sync.c",
    "backends/rendering/vulkan/sc/swapchain.c",
    "backends/rendering/vulkan/sc/image_views.c",
    "backends/rendering/vulkan/sc/render_pass.c",
    "backends/rendering/vulkan/util/buffer.c",
    "backends/rendering/vulkan/util/pipeline.c",
    "backends/rendering/vulkan/util/bindset.c",
    "backends/rendering/vulkan/util/shader_mod.c",
    "backends/rendering/vulkan/util/texture.c",
    "backends/rendering/vulkan/util/commbuf.c",
    "backends/rendering/vulkan/util/framebuffer.c",
    "backends/rendering/vulkan/util/img.c",
    "backends/rendering/vulkan/eng/pipeline.c",
    "backends/rendering/vulkan/eng/screen.c",
    "backends/rendering/vulkan/eng/buffer.c",
    "backends/rendering/vulkan/eng/bindset.c",
    "backends/rendering/vulkan/eng/texture.c",
    "backends/rendering/vulkan/eng/framebuffer.c",
};

const cflags = [_][]const u8{
    "-std=c23",
    "-Wall",
    "-Wextra",
    "-Wno-c23-extensions",
    "-Wpedantic",

    "-g", "-O0", //"-fsanitize=address,undefined",
    //"-O3",
};

const cflags_vulkan = [_][]const u8{
    "-std=c99",
    "-Wall",
    "-Wextra",
    "-Wpedantic",

    "-g", "-O0", //"-fsanitize=address,undefined",
    //"-O3",
};

var target: std.Build.ResolvedTarget = undefined;
var optimize: std.builtin.OptimizeMode = undefined;

fn ex_c(b: *std.Build, lib: *std.Build.Step.Compile, libvk: *std.Build.Step.Compile, comptime name: []const u8) void {
    const ex = b.addExecutable(.{
        .name = "ex_" ++ name,
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            }),
        });
    //ex.root_module.linkSystemLibrary("asan", .{});
    //ex.root_module.linkSystemLibrary("ubsan", .{});

    ex.root_module.addCSourceFile(.{ .file = b.path("examples/" ++ name ++ "/main.c"), .flags = &cflags });
    ex.root_module.linkLibrary(lib);
    ex.root_module.linkLibrary(libvk);

    ex.root_module.linkSystemLibrary("glfw", .{});

    if (builtin.os.tag == .linux) {
        ex.root_module.linkSystemLibrary("vulkan", .{});
        ex.root_module.linkSystemLibrary("m", .{});
    } else {
        ex.root_module.linkSystemLibrary("vulkan1", .{});
        ex.root_module.linkSystemLibrary("gdi32", .{});
        ex.root_module.linkSystemLibrary("user32", .{});
        ex.root_module.linkSystemLibrary("shell32", .{});
    }

    b.installArtifact(ex);

}

fn glslc(b: *std.Build, step: *std.Build.Step, 
         comptime base: []const u8, 
         comptime in:   []const u8, 
         comptime out:  []const u8) void {
    const glc = b.addSystemCommand(&[_][]const u8{
        "glslc", base ++ in,
        "-o", base ++ out,
        });
    step.dependOn(&glc.step);
}

pub fn build(b: *std.Build) void {
    target = b.standardTargetOptions(.{});
    optimize = b.standardOptimizeOption(.{});

    var targs = std.ArrayListUnmanaged(*std.Build.Step.Compile){};

    const lib_vk = b.addLibrary(.{
        .linkage = .static,
        .name    = "ceat_vk",
        .root_module = b.createModule(.{
            .target    = target,
            .optimize  = optimize,
            .link_libc = true,
            }),
        });
    lib_vk.root_module.addIncludePath(b.path("core"));
    lib_vk.root_module.addCSourceFiles(.{ .files = &csource_vulkan, .flags = &cflags_vulkan });

    b.installArtifact(lib_vk);
    targs.append(b.allocator, lib_vk) catch @panic("OOM");

    const lib = b.addLibrary(.{
        .linkage = .static,
        .name = "ceat",
        .root_module = b.createModule(.{
            //.root_source_file = b.path("core/root.zig"),
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            }),
        });

    lib.root_module.linkLibrary(lib_vk);
    lib.root_module.addIncludePath(b.path("core"));
    lib.root_module.addIncludePath(b.path("backends"));
    lib.root_module.addCSourceFiles(.{ .files = &csource, .flags = &cflags });

    b.installArtifact(lib);
    targs.append(b.allocator, lib) catch @panic("OOM");

    _ = zcc.createStep(b, "cdb", targs.toOwnedSlice(b.allocator) catch @panic("OOM"));

    const shadstep = b.step("shaders", "compile shaders");
    glslc(b, shadstep, "core/ear/hl/shaders/", "rect.vert", "rect_v.spv");
    glslc(b, shadstep, "core/ear/hl/shaders/", "rect.frag", "rect_f.spv");
    glslc(b, shadstep, "core/ear/hl/shaders/", "tex.vert",  "tex_v.spv");
    glslc(b, shadstep, "core/ear/hl/shaders/", "tex.frag",  "tex_f.spv");

    glslc(b, shadstep, "examples/triangle/", "shad.vert", "shad_v.spv");
    glslc(b, shadstep, "examples/triangle/", "shad.frag", "shad_f.spv");
    glslc(b, shadstep, "examples/vbuffer/",  "shad.vert", "shad_v.spv");
    glslc(b, shadstep, "examples/vbuffer/",  "shad.frag", "shad_f.spv");
    glslc(b, shadstep, "examples/ubuffer/",  "shad.vert", "shad_v.spv");
    glslc(b, shadstep, "examples/ubuffer/",  "shad.frag", "shad_f.spv");
    glslc(b, shadstep, "examples/ibuffer/",  "shad.vert", "shad_v.spv");
    glslc(b, shadstep, "examples/ibuffer/",  "shad.frag", "shad_f.spv");

    ex_c(b,lib,lib_vk, "window");
    ex_c(b,lib,lib_vk, "triangle");
    ex_c(b,lib,lib_vk, "vbuffer");
    ex_c(b,lib,lib_vk, "ubuffer");
    ex_c(b,lib,lib_vk, "ibuffer");
    ex_c(b,lib,lib_vk, "texture");
    ex_c(b,lib,lib_vk, "framebuffer");
    ex_c(b,lib,lib_vk, "text");
    ex_c(b,lib,lib_vk, "input");
    ex_c(b,lib,lib_vk, "object");
    ex_c(b,lib,lib_vk, "clock");
    ex_c(b,lib,lib_vk, "console");
    ex_c(b,lib,lib_vk, "audio");
    ex_c(b,lib,lib_vk, "camera");
    ex_c(b,lib,lib_vk, "debug");
    ex_c(b,lib,lib_vk, "timer");
}
