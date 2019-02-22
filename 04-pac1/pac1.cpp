#include "../common/window.hpp"

int main() {
  Window win(640, 480, "Hello");

  while (!win.shouldClose()) {
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    win.swapBuffers();
    win.pollEvents();
  }
}
