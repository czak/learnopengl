#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    exit(1);
  }

  SDL_Window* window = SDL_CreateWindow(
      "sample",
      10,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      SDL_WINDOW_OPENGL);
  if (!window) {
    fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  context = SDL_GL_CreateContext(window);
  if (!context) {
    fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
    return 1;
  }

  int attr;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &attr);
  fprintf(stderr, "attr: %d\n", attr);

  int running = 1;
  int frames = 0;
  while (running) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = 0;
          break;
      }
    }

    glClearColor(0.2f, 0.5f, 0.3f, 0.3f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    ++frames;
  }

  fprintf(stderr, "frames: %d\n", frames);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
