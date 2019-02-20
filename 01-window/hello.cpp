#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *window,
                  int key,
                  [[maybe_unused]] int scancode,
                  int action,
                  [[maybe_unused]] int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  auto window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0, 0.6, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
