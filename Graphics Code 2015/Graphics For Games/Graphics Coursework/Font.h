/** @file */

#pragma once

#include "Texture.h"

namespace GraphicsCoursework
{
/**
 * @class Font
 * @brief Holds a texture that stores a font.
 */
class Font : public Texture
{
public:
  Font(size_t x, size_t y);
  virtual ~Font();

  /**
   * @brief Gets the number of gylphs in the X axis.
   * @return X dimension
   */
  inline size_t XDim() const
  {
    return m_xDim;
  }

  /**
   * @brief Gets the number of gylphs in the Y axis.
   * @return Y dimension
   */
  inline size_t YDim() const
  {
    return m_yDim;
  }

protected:
  size_t m_xDim; //!< Number of glyphs in x axis
  size_t m_yDim; //!< Number of glyphs in y axis
};
}
