/** @file */

#pragma once

#include <../../NCLGL/mesh.h>
#include <vector>

namespace GraphicsCoursework
{
class Font;

/**
 * @class TextMesh
 * @brief Mesh containing a quad showing text.
 */
class TextMesh : public Mesh
{
public:
  TextMesh(size_t maxLength, Font *font);
  ~TextMesh();

  void SetText(const std::string &text);

protected:
  Font *m_font;        //!< Font texture
  size_t m_maxLength;  //!< Maximum length of the string displayed
  float m_glyphWidth;  //!< Width of a glyph
  float m_glyphHeight; //!< Height of a glyph
};
}
