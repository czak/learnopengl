#include <cstdio>
#include <fstream>
#include <sstream>

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

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
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  glViewport(0, 0, 800, 600);

  // Set up vertices
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0
  };

  // --- VAO ---

  unsigned int VAO;
  glCreateVertexArrays(1, &VAO);
  glEnableVertexArrayAttrib(VAO, 0);
  glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(VAO, 0, 0);

  // --- buffer setup ---

  unsigned int VBO;
  glCreateBuffers(1, &VBO);
  glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
  auto xloc = glGetUniformLocation(fragmentShader, "x");
  auto yloc = glGetUniformLocation(fragmentShader, "y");
  if (xloc == -1 || yloc == -1) {
    fprintf(stderr, "Failed to find uniform: xloc=%d, yloc=%d\n", xloc, yloc);
  }

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    glProgramUniform1f(fragmentShader, xloc, (float)(x / 800));
    glProgramUniform1f(fragmentShader, yloc, (float)(y / 600));
  
    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
