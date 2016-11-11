/** @file */

#pragma once

#include <GL/glew.h>

#include "SceneNode.h"
#include "Texture.h"

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

  Texture *texture;
  std::string uniformName;
  GLuint textureBuffer;
};

class TextureNode : public SceneNode
{
public:
  typedef std::vector<TextureMapping> TextureMappingList;

public:
  TextureNode(const std::string &name, const TextureMappingList &mappings);
  virtual ~TextureNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

private:
  TextureMappingList m_mappings;
};
}