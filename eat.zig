const std = @import("std");

const c = @cImport({
    @cInclude("eat.h");
});

pub fn run(plugs: []const *const c.eat_plugin) !void {
    if (plugs.len == 0) { return; }

    for (plugs) |p| {
        if (p.init == null) continue;

        std.debug.print("{s} init\n", .{ p.name });
        p.init.?();
    }

    var i = plugs.len;
    while (i > 0) {
        i -= 1;

        const p = plugs[i];
        if (p.exit == null) continue;

        std.debug.print("{s} exit\n", .{ p.name });
        p.exit.?();
    }
}

export fn eat_run(plugs: [*]const *const c.eat_plugin, plug_amt: u32) void {
    run(plugs[0..plug_amt]) catch @panic("failed to run program!");
}
