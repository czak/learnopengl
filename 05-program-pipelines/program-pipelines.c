#include <stdio.h>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource =
"#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

const char* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

const char* fragmentShaderSource2 =
"#version 450 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);\n"
"}";

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return 1;
  }

  glViewport(0, 0, 800, 600);

  // Set up vertices
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // --- VERTEX ---
  int success;

  unsigned int vertexShader;
  vertexShader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexShaderSource);
  glGetProgramiv(vertexShader, GL_LINK_STATUS, &success);
  if (!success) {
    fprintf(stderr, "Vertex shader failed");
  }

  // --- FRAGMENT ---

  unsigned int fragmentShader;
  fragmentShader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShaderSource);
  glGetProgramiv(fragmentShader, GL_LINK_STATUS, &success);
  if (!success) {
    fprintf(stderr, "Fragment shader failed");
  }

  // --- FRAGMENT 2 ---

  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShaderSource2);
  glGetProgramiv(fragmentShader2, GL_LINK_STATUS, &success);
  if (!success) {
    fprintf(stderr, "Fragment shader 2 failed");
  }

  // --- PIPELINE ---

  unsigned int pipeline;
  glGenProgramPipelines(1, &pipeline);
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexShader);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader);

  unsigned int shaders[2] = {
    fragmentShader,
    fragmentShader2,
  };
  unsigned int currentShader = 0;

  // --- VAO ---

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      currentShader = (currentShader + 1) % 2;

    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindProgramPipeline(pipeline);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, shaders[currentShader]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
