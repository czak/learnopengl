const panic = @import("std").debug.panic;
const glfw = @import("glfw.zig");

pub fn main() !void {
  if (glfw.Init() == glfw.TRUE)
    panic("Unable to init", .{});

  glfw.CreateWindow();
}
