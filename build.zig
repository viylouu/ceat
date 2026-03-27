const std = @import("std");
const builtin = @import("builtin");

const csource = [_][]const u8{
    "core/eat.c",

    "core/debug/debug.c",
    "core/debug/console.c",

    "core/eaw/eaw.c",
    "core/eaw/window.c",
    "core/eaw/input.c",
    "core/eaw/vk.c",

    "core/ear/ear.c",
    "core/ear/ll/vk/vk.c",
    "core/ear/ll/vk/init/validation.c",
    "core/ear/ll/vk/init/instance.c",
    "core/ear/ll/vk/init/surface.c",
    "core/ear/ll/vk/init/device_phys.c",
    "core/ear/ll/vk/init/device_log.c",
    "core/ear/ll/vk/init/queue_fam.c",
    "core/ear/ll/vk/init/comm_pool.c",
    "core/ear/ll/vk/init/comm_buffer.c",
    "core/ear/ll/vk/init/sync.c",
    "core/ear/ll/vk/sc/swapchain.c",
    "core/ear/ll/vk/sc/image_views.c",
    "core/ear/ll/vk/sc/render_pass.c",
    "core/ear/ll/vk/sc/framebuffer.c",
    "core/ear/ll/vk/util/buffer.c",
    "core/ear/ll/vk/util/pipeline.c",
    "core/ear/ll/vk/eng/pipeline.c",
    "core/ear/ll/vk/eng/shader_mod.c",
    "core/ear/ll/vk/eng/screen.c",
    "core/ear/ll/vk/eng/buffer.c",
    "core/ear/ll/misc.c",
    "core/ear/ll/pipeline.c",
    "core/ear/ll/texture.c",
    "core/ear/ll/buffer.c",
    //"core/ear/ll/framebuffer.c",
    //"core/ear/ll/texarray.c",
    //"core/ear/hl/text.c",
    //"core/ear/hl/camera.c",
    "core/ear/hl/data.c",
    "core/ear/hl/user.c",

    "core/eau/file.c",
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
};

const cflags = [_][]const u8{
    "-std=c23",
    //"-g",
    //"-O0",
};

var target: std.Build.ResolvedTarget = undefined;
var optimize: std.builtin.OptimizeMode = undefined;

fn ex_c(b: *std.Build, lib: *std.Build.Step.Compile, comptime name: []const u8) void {
    const ex = b.addExecutable(.{
        .name = "ex_" ++ name,
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
        }),
    });
    ex.root_module.addCSourceFile(.{ .file = b.path("examples/" ++ name ++ "/main.c"), .flags = &cflags });
    ex.root_module.linkLibrary(lib);

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

pub fn build(b: *std.Build) void {
    target = b.standardTargetOptions(.{});
    optimize = b.standardOptimizeOption(.{});

    const lib = b.addLibrary(.{
        .linkage = .static,
        .name = "ceat",
        .root_module = b.createModule(.{
            .root_source_file = b.path("core/root.zig"),
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    lib.root_module.addIncludePath(b.path("core"));
    lib.root_module.addCSourceFiles(.{ .files = &csource, .flags = &cflags });

    b.installArtifact(lib);

    ex_c(b,lib, "window");
    ex_c(b,lib, "triangle");
    ex_c(b,lib, "vbuffer");
    ex_c(b,lib, "ubuffer");
    ex_c(b,lib, "ibuffer");
    //ex_c(b,lib, "texture");
    //ex_c(b,lib, "framebuffer");
    //ex_c(b,lib, "text");
    //ex_c(b,lib, "input");
    //ex_c(b,lib, "object");
    //ex_c(b,lib, "clock");
    //ex_c(b,lib, "console");
    //ex_c(b,lib, "audio");
    //ex_c(b,lib, "camera");
    //ex_c(b,lib, "debug");
    //ex_c(b,lib, "timer");
}
