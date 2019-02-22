#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>

class Window {
 public:
  Window(int width, int height, const char* title);
  ~Window();
  bool shouldClose();
  void swapBuffers();
  void pollEvents();

 private:
  GLFWwindow* glfwWindow;
  static void errorCallback(int error, const char* description);
  static void framebufferSizeCallback(GLFWwindow* window, int width,
                                      int height);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods);
};
