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
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
  };

  unsigned int indices2[] = {
    0, 1, 2,
    1, 2, 3,
  };

  // --- VAO ---

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // --- buffer setup ---

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  // --- indices ---

  unsigned int EBO[2];
  glGenBuffers(2, EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

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

  // --- PIPELINE ---

  unsigned int pipeline;
  glGenProgramPipelines(1, &pipeline);
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexShader);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader);
  glBindProgramPipeline(pipeline);

  unsigned int frame = 0;
  int running = 1;
  while (running) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      running = 0;

    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    if ((++frame >> 4) % 2 == 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    } else {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
