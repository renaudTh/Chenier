//Will potentially replace the cmake build ?

const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const exe = b.addExecutable("player", null);
    exe.addCSourceFile("main.c", &[_][]const u8{});
    exe.linkLibC();
    exe.linkSystemLibrary("SDL2"); // add libsdl
    exe.install();
}
