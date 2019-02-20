#include <GL/glew.h>
#include <string>
#include <initializer_list>

enum class ShaderType {
  Vertex,
  Fragment,
};

struct Shader {
  Shader(GLenum type, const std::string& filename);
  ~Shader();
  GLuint id;
};

struct ShaderProgram {
  ShaderProgram(std::initializer_list<Shader> l);
  GLuint id;
};
