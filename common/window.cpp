#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>

#include "window.hpp"

static void errorCallback(int error, const char* description);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                        int mods);
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Window::Window(int width, int height, const char* title) {
  this->width = width;
  this->height = height;

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) throw std::exception();

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!glfwWindow) {
    glfwTerminate();
    throw std::exception();
  }

  // link glfw
  glfwSetWindowUserPointer(glfwWindow, this);

  glfwMakeContextCurrent(glfwWindow);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
  glfwSetKeyCallback(glfwWindow, keyCallback);
  glfwSetCursorPosCallback(glfwWindow, cursorPosCallback);

  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
    glfwTerminate();
    throw std::exception();
  }
}

Window::~Window() { glfwTerminate(); }

bool Window::shouldClose() { return glfwWindowShouldClose(glfwWindow); }

void Window::swapBuffers() { glfwSwapBuffers(glfwWindow); }

void Window::pollEvents() { glfwPollEvents(); }

static void errorCallback(int error, const char* description) {
  fprintf(stderr, "Error %d: %s\n", error, description);
}

static void keyCallback(GLFWwindow* window, int key,
                        [[maybe_unused]] int scancode, int action,
                        [[maybe_unused]] int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  Window* win = (Window*)glfwGetWindowUserPointer(window);
  win->cursorPos = {(float)xpos, (float)ypos};
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  Window* win = (Window*)glfwGetWindowUserPointer(window);
  win->width = width;
  win->height = height;
  glViewport(0, 0, width, height);
}
