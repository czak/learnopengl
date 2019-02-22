#include <fstream>
#include <sstream>

#include "shader.hpp"

static void checkShaderErrors(GLuint id) {
  int success;
  char infoLog[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    fprintf(stderr, "Compile error: %s\n", infoLog);
  }
}

static void checkProgramErrors(GLuint id) {
  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    fprintf(stderr, "Link error: %s\n", infoLog);
  }
}

Shader::Shader(const char* vertexFilename, const char* fragmentFilename) {
  std::ifstream vf(vertexFilename);
  std::ifstream ff(fragmentFilename);
  std::stringstream buffer;
  // read in vertex shader source
  buffer << vf.rdbuf();
  vertexSource = buffer.str();
  // read in fragment shader source
  buffer.str(std::string());
  buffer << ff.rdbuf();
  fragmentSource = buffer.str();

  // compile vertex
  auto vid = glCreateShader(GL_VERTEX_SHADER);
  auto psource = vertexSource.c_str();
  glShaderSource(vid, 1, &psource, NULL);
  glCompileShader(vid);
  checkShaderErrors(vid);

  // compile fragment
  auto fid = glCreateShader(GL_FRAGMENT_SHADER);
  psource = fragmentSource.c_str();
  glShaderSource(fid, 1, &psource, NULL);
  glCompileShader(fid);
  checkShaderErrors(fid);

  // link
  id = glCreateProgram();
  glAttachShader(id, vid);
  glAttachShader(id, fid);
  glLinkProgram(id);
  checkProgramErrors(id);

  // cleanup
  glDeleteShader(vid);
  glDeleteShader(fid);
}
