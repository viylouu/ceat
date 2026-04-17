const std = @import("std");

const c = @cImport({
    @cInclude("eat.h");
});

const plug_sel_err = error{
    NO_PLUGINS_SUPPORTED,
};

fn choose_best_plug(plugs: []*const c.eat_plugin) plug_sel_err !*const c.eat_plugin {
    var best_score: i32 = -1;
    var best: ?*const c.eat_plugin = null;

    for (plugs) |p| {
        const s = p.score.?();
        if (s < best_score) continue;
        best_score = s;
        best = p;
    }

    if (best) |b| { return b; }
    else return plug_sel_err.NO_PLUGINS_SUPPORTED;
}

pub fn run(
    title: []const u8,
    width: u32, height: u32,
    vsync: bool,
    plugins: []const *const c.eat_plugin
) !void {
    // figure out a way to send this
    _ = title;
    _ = width; _ = height;
    _ = vsync;

    if (plugins.len == 0) { return; }

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const alloc = arena.allocator();

    var generic_plugs = try std.ArrayList(*const c.eat_plugin).initCapacity(alloc, plugins.len);
    var back_window_plugs = try std.ArrayList(*const c.eat_plugin).initCapacity(alloc, plugins.len);
    var back_render_plugs = try std.ArrayList(*const c.eat_plugin).initCapacity(alloc, plugins.len);
    defer generic_plugs.deinit(alloc);
    defer back_render_plugs.deinit(alloc);
    defer back_window_plugs.deinit(alloc);

    for (plugins) |p| switch (p.type) {
        c.EAT_PLUG_BACKEND_WINDOW => try back_window_plugs.append(alloc, p),
        c.EAT_PLUG_BACKEND_RENDER => try back_render_plugs.append(alloc, p),
        c.EAT_PLUG_GENERIC => try generic_plugs.append(alloc, p),
        else => @panic("unknown plugin type!"),
    };

    if (back_window_plugs.items.len == 0) @panic("must have a windowing backend!");
    if (back_render_plugs.items.len == 0) @panic("must have a rendering backend!");

    const plugs_needed = generic_plugs.items.len
        + 1 // window backend
        + 1 // render backend
        ; 
    const plugs: []*const c.eat_plugin = try alloc.alloc(*const c.eat_plugin, plugs_needed);

    plugs[0] = try choose_best_plug(back_window_plugs.items[0..back_window_plugs.items.len]);
    plugs[1] = try choose_best_plug(back_render_plugs.items[0..back_render_plugs.items.len]);

    var i: usize = 0;
    for (plugs[plugs_needed - generic_plugs.items.len..plugs_needed]) |*p| {
        p.* = generic_plugs.items[i];
        i += 1;
    }

    for (plugs) |p| {
        if (p.init == null) continue;

        std.debug.print("{s} init\n", .{ p.name });
        p.init.?();
    }

    i = plugs.len;
    while (i > 0) {
        i -= 1;

        const p = plugs[i];
        if (p.exit == null) continue;

        std.debug.print("{s} exit\n", .{ p.name });
        p.exit.?();
    }
}

export fn eat_run(
    title: [*c]const u8,
    width: u32, height: u32,
    vsync: bool,
    plugs: [*]const *const c.eat_plugin, plug_amt: u32
) void {
    run(
        std.mem.span(title),
        width, height,
        vsync,
        plugs[0..plug_amt]
        ) catch @panic("failed to run program!");
}
