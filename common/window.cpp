#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>

#include "window.hpp"

Window::Window(int width, int height, const char* title) {
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

  glfwMakeContextCurrent(glfwWindow);

  glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
  glfwSetKeyCallback(glfwWindow, keyCallback);

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

void Window::keyCallback(GLFWwindow* window, int key,
                         [[maybe_unused]] int scancode, int action,
                         [[maybe_unused]] int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::framebufferSizeCallback([[maybe_unused]] GLFWwindow* window,
                                     int width, int height) {
  glViewport(0, 0, width, height);
}

void Window::errorCallback(int error, const char* description) {
  fprintf(stderr, "Error %d: %s\n", error, description);
}
