//Will potentially replace the cmake build ?

const std = @import("std");
const os = std.os;
const exec = std.build.exec;

pub fn build(b: *std.build.Builder) void {
    const cxxArgs = &[_][]const u8{
        "-Wall",
        "-Wextra",
        "-Werror",
    };
    const mode = b.standardReleaseOptions();

    //Building cardGame lib
    const lib = b.addStaticLibrary("cardGame", null);
    lib.addCSourceFile("src/card.c", cxxArgs);
    lib.addCSourceFile("src/stack.c", cxxArgs);
    lib.addCSourceFile("src/c4/c4.c", cxxArgs);
    lib.addCSourceFile("src/r7/r7.c", cxxArgs);
    lib.addCSourceFile("src/qll/qll.c", cxxArgs);
    lib.addCSourceFile("src/game.c", cxxArgs);
    lib.setBuildMode(mode);
    lib.linkLibC();
    lib.addIncludePath("include");
    lib.addIncludePath("src");
    lib.install();

    //Building player

    const exe = b.addExecutable("player", null);
    exe.addCSourceFile("player/player.c", cxxArgs);
    exe.addCSourceFile("player/game-render.c", cxxArgs);
    exe.addCSourceFile("player/graphic-context.c", cxxArgs);
    exe.setBuildMode(mode);
    exe.linkLibC();
    exe.linkSystemLibrary("SDL2");
    exe.linkSystemLibrary("SDL2_image"); // add libsdl
    exe.linkLibrary(lib);
    exe.addIncludePath("include");
    exe.addIncludePath("src");
    exe.addIncludePath("player");
    exe.install();
}
