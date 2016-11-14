/** @file */

#pragma once

#include <../../NCLGL/mesh.h>
#include <vector>

namespace GraphicsCoursework
{
class Font;

class TextMesh : public Mesh
{
public:
  TextMesh(size_t maxLenght, Font *font);
  ~TextMesh(void);

  void SetText(const std::string &text);

protected:
  Font *m_font;
  float m_texelWidth;
  float m_texelHeight;
};
}