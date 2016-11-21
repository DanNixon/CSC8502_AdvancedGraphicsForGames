/** @file */

#pragma once

#include <GL/glew.h>

#include "ITexture.h"
#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Holds sampler uniform mapping data for a single texture.
 */
struct TextureMapping
{
  /**
   * @brief Performs this mapping on a shader program.
   * @param program Handle to GL shader program
   */
  void Map(GLuint program)
  {
    texture->BindToShader(program, uniformName, textureBuffer);
  }

  /**
   * @brief Performs unmapping on a shader program.
   * @param program Handle to GL shader program
   */
  void UnMap(GLuint program)
  {
    texture->UnBindFromShader(program, uniformName, textureBuffer);
  }

  ITexture *texture;       //!< Texture to map
  std::string uniformName; //!< Uniform name to map to
  GLuint textureBuffer;    //!< GL texture index to use
};

/**
 * @class TextureNode
 * @brief Performs mapping of textures to shader uniform samplers for all child nodes.
 */
class TextureNode : public ShaderDataNode
{
public:
  /**
   * @brief List of texture mappings.
   */
  typedef std::vector<TextureMapping> TextureMappingList;

public:
  TextureNode(const std::string &name, const TextureMappingList &mappings);
  virtual ~TextureNode();

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

private:
  TextureMappingList m_mappings; //!< Texture mappings for this node
};
}
