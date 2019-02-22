#include <fstream>
#include <iostream>
#include <sstream>

#include "Shader.hpp"

Shader::Shader(GLenum type, const std::string& filename) {
  std::ifstream f(filename);
  std::stringstream buffer;
  buffer << f.rdbuf();
  auto source = buffer.str();

  id = glCreateShader(type);
  const char* sources[1] = {source.c_str()};
  glShaderSource(id, 1, sources, NULL);
  glCompileShader(id);

  // check for errors
  int success;
  char infoLog[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    std::cerr << "error compiling shader\n" << infoLog << '\n';
  }
}

Shader::~Shader() {
  std::cout << "deleting shader " << id << '\n';
  glDeleteShader(id);
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> list) {
  id = glCreateProgram();
  for (const auto& shader : list) {
    glAttachShader(id, shader.id);
  }

  std::cout << "linking shader program " << id << '\n';
  glLinkProgram(id);

  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    std::cerr << "error linking program\n" << infoLog << '\n';
  }
}
