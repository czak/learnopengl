#include <cstdio>
#include <fstream>
#include <sstream>

#include <epoxy/gl.h>
#include <SDL2/SDL.h>

using std::printf;
using std::fprintf;

unsigned int loadShader(std::string filename, GLenum type) {
  std::ifstream f(filename);
  std::stringstream buffer;
  buffer << f.rdbuf();

  auto source = buffer.str();
  auto psource = source.c_str();

  unsigned int shader = glCreateShaderProgramv(type, 1, &psource);

  int success;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (!success) {
    if (type == GL_VERTEX_SHADER)
      fprintf(stderr, "Vertex shader failed\n");
    else if (type == GL_FRAGMENT_SHADER)
      fprintf(stderr, "Fragment shader failed\n");
  }

  return shader;
}

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

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GLContext ctx = SDL_GL_CreateContext(window);
  if (ctx == NULL) {
    SDL_Log("Unable to initialize OpenGL context: %s", SDL_GetError());
    exit(1);
  }

  glViewport(0, 0, 800, 600);

  // Set up vertices
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0
  };

  // --- buffer setup ---

  unsigned int VBO;
  glCreateBuffers(1, &VBO);
  glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // --- VAO ---

  unsigned int VAO;
  glCreateVertexArrays(1, &VAO);
  glEnableVertexArrayAttrib(VAO, 0);
  glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(VAO, 0, 0);

  glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 12);

  // --- SHADER PIPELINE ---
  unsigned int vertexShader = loadShader("vert.glsl", GL_VERTEX_SHADER);
  unsigned int fragmentShader = loadShader("frag.glsl", GL_FRAGMENT_SHADER);

  unsigned int pipeline;
  glGenProgramPipelines(1, &pipeline);
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexShader);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader);
  glBindProgramPipeline(pipeline);

  // --- UNIFORM ---

  unsigned int UBO;
  glCreateBuffers(1, &UBO);
  glNamedBufferData(UBO, 4 * sizeof(float), NULL, GL_STATIC_DRAW);
  float* buf = (float*) glMapNamedBuffer(UBO, GL_WRITE_ONLY);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);

  bool running = true;
  while (running) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
          break;
      }
    }

    glClearColor(0.2f, 0.5f, 0.3f, 0.3f);
    glClear(GL_COLOR_BUFFER_BIT);

    buf[1] = 1.0f;
    buf[3] = 1.0f;
  
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
