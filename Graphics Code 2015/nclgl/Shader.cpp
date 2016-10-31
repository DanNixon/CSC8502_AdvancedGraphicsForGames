#include "Shader.h"

Shader::Shader(std::string vertex, std::string fragment, std::string geometry)
{
  program = glCreateProgram();
  objects[SHADER_VERTEX] = GenerateShader(vertex, GL_VERTEX_SHADER);
  objects[SHADER_FRAGMENT] = GenerateShader(fragment, GL_FRAGMENT_SHADER);
  objects[SHADER_GEOMETRY] = 0;

  if (!geometry.empty())
  {
    objects[SHADER_GEOMETRY] = GenerateShader(geometry, GL_GEOMETRY_SHADER);
    glAttachShader(program, objects[SHADER_GEOMETRY]);
  }

  glAttachShader(program, objects[SHADER_VERTEX]);
  glAttachShader(program, objects[SHADER_FRAGMENT]);

  SetDefaultAttributes();
}

Shader::~Shader()
{
  for (int i = 0; i < 3; ++i)
  {
    glDetachShader(program, objects[i]);
    glDeleteShader(objects[i]);
  }
  glDeleteProgram(program);
}

bool Shader::LinkProgram()
{
  if (loadFailed)
    return false;

  glLinkProgram(program);
  GLint code;
  glGetProgramiv(program, GL_LINK_STATUS, &code);
  return code == GL_TRUE;
}

void Shader::SetDefaultAttributes()
{
  glBindAttribLocation(program, VERTEX_BUFFER, "position");
  glBindAttribLocation(program, COLOUR_BUFFER, "colour");
}

bool Shader::LoadShaderFile(std::string from, std::string &into)
{
  std::ifstream file;
  std::string temp;

  std::cerr << "Loading shader text from " << from << "\n\n";
  file.open(from.c_str());
  if (!file.is_open())
  {
    std::cerr << "File does not exist!\n";
    return false;
  }

  while (!file.eof())
  {
    getline(file, temp);
    into += temp + "\n";
  }

  file.close();
  std::cerr << into << "\n\n";
  std::cerr << "Loaded shader text!\n\n";
  return true;
}

GLuint Shader::GenerateShader(std::string from, GLenum type)
{
  std::cerr << "Compiling Shader... \n";

  std::string load;
  if (!LoadShaderFile(from, load))
  {
    std::cerr << "Compiling failed!\n";
    loadFailed = true;
    return 0;
  }

  GLuint shader = glCreateShader(type);

  const char *chars = load.c_str();
  glShaderSource(shader, 1, &chars, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE)
  {
    std::cerr << "Compiling failed!\n";
    char error[512];
    glGetInfoLogARB(shader, sizeof(error), NULL, error);
    std::cerr << error;
    loadFailed = true;
    return 0;
  }

  std::cerr << "Compiling success!\n";
  loadFailed = false;
  return shader;
}
