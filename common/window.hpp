#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>

struct Point {
  float x, y;
};

class Window {
 public:
  Window(int width, int height, const char* title);
  ~Window();
  bool shouldClose();
  void swapBuffers();
  void pollEvents();

  Point cursorPos;
  int width, height;

 private:
  GLFWwindow* glfwWindow;
};
