/** @file */

#pragma once

#include <GL/glew.h>

#include "ShaderDataNode.h"
#include "ITexture.h"

namespace GraphicsCoursework
{
struct TextureMapping
{
  void Map(GLuint program)
  {
    texture->BindToShader(program, uniformName, textureBuffer);
  }

  void UnMap(GLuint program)
  {
    texture->UnBindFromShader(program, uniformName, textureBuffer);
  }

  ITexture *texture;
  std::string uniformName;
  GLuint textureBuffer;
};

class TextureNode : public ShaderDataNode
{
public:
  typedef std::vector<TextureMapping> TextureMappingList;

public:
  TextureNode(const std::string &name, const TextureMappingList &mappings);
  virtual ~TextureNode();

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

private:
  TextureMappingList m_mappings;
};
}