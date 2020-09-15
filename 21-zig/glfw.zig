extern fn glfwInit() c_int;
extern fn glfwCreateWindow(width: c_int, height: c_int, title: [*c]const u8, monitor: ?*GLFWmonitor, share: ?*GLFWwindow) ?*GLFWwindow;

pub const Init = glfwInit;
pub const CreateWindow = glfwCreateWindow;

pub const TRUE = 1;
pub const FALSE = 0;
