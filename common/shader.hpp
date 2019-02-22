#include <GL/glew.h>
#include <string>

class Shader {
 public:
  Shader(const char* vertexFilename, const char* fragmentFilename);
  GLuint id;

  std::string vertexSource;
  std::string fragmentSource;
};

