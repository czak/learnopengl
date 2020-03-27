#include <GLFW/glfw3.h>
#include <cstdio>

using std::printf;
using std::fprintf;

static void key_callback(GLFWwindow *window,
                  int key,
                  [[maybe_unused]] int scancode,
                  int action,
                  [[maybe_unused]] int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description) {
  fprintf(stderr, "Error %d: %s\n", error, description);
}

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  // glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  switch (glfwGetWindowAttrib(window, GLFW_CLIENT_API)) {
    case GLFW_OPENGL_API:
      printf("API: OpenGL\n"); break;
    case GLFW_OPENGL_ES_API:
      printf("API: OpenGL ES\n"); break;
    case GLFW_NO_API:
      printf("API: none\n"); break;
    default:
      printf("API: unknown\n"); break;
  }

  printf("Version: %d.%d\n",
      glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR),
      glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR));

  switch (glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE)) {
    case GLFW_OPENGL_CORE_PROFILE:
      printf("Profile: core\n"); break;
    case GLFW_OPENGL_COMPAT_PROFILE:
      printf("Profile: compat\n"); break;
    case GLFW_OPENGL_ANY_PROFILE:
      printf("Profile: any\n"); break;
    default:
      printf("Profile: unknown\n"); break;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  printf("GL string: %s\n", glGetString(GL_VERSION));

  // while (!glfwWindowShouldClose(window)) {
  //   glClearColor(1.0, 0.6, 0.0f, 1.0);
  //   glClear(GL_COLOR_BUFFER_BIT);
  //   glfwSwapBuffers(window);
  //   glfwPollEvents();
  // }

  glfwTerminate();
}
