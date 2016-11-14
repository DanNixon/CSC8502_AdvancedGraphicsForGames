/** @file */

#pragma once

#include "Texture.h"

namespace GraphicsCoursework
{
class Font : public Texture
{
public:
  Font(size_t x, size_t y);
  virtual ~Font();

  inline size_t XDim() const
  {
    return m_xDim;
  }

  inline size_t YDim() const
  {
    return m_yDim;
  }

protected:
  size_t m_xDim;
  size_t m_yDim;
};
}