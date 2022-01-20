#include <stdio.h>
#include <SDL2/SDL.h>
#include <GLES2/gl2.h>

char *vertex_shader_source =
"attribute vec4 vPosition;\n"
"void main()\n"
"{\n"
"  gl_Position = vPosition;\n"
"}\n";

char *fragment_shader_source =
"precision mediump float;\n"
"void main()\n"
"{\n"
"  gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

static void checkCompileErrors(GLuint id) {
  int success;
  char infoLog[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    fprintf(stderr, "Compile error: %s\n", infoLog);
  }
}

static void checkLinkErrors(GLuint id) {
  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    fprintf(stderr, "Link error: %s\n", infoLog);
  }
}

unsigned int loadShader(const char *source, GLenum type) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  checkCompileErrors(shader);

  return shader;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

  // some debugging info

  fprintf(stderr, "Vendor graphic card: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "Renderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "Version GL: %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "Version GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  // --- SETUP ---

  glViewport(0, 0, 800, 600);

  // --- SHADERS ---

  unsigned int vertexShader = loadShader(vertex_shader_source, GL_VERTEX_SHADER);
  unsigned int fragmentShader = loadShader(fragment_shader_source, GL_FRAGMENT_SHADER);

  // --- PROGRAM ---

  unsigned int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  checkLinkErrors(program);

  // --- GEOMETRY ---

  // Set up vertices
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
  };

  glUseProgram(program);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(0);

  // --- MAIN LOOP ---

  int running = 1;
  int frames = 0;
  unsigned int start = SDL_GetTicks();
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

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    SDL_GL_SwapWindow(window);

    ++frames;
  }

  float duration = (SDL_GetTicks() - start) / 1000.0f; 
  fprintf(stderr, "%d frames in %f s = %f FPS\n", frames, duration, frames / duration);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
