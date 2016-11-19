/** @file */

#include "Font.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new font texture.
 *
 * @param x Number of glyphs in X axis
 * @param y Number of glyphs in Y axis
 */
Font::Font(size_t x, size_t y)
    : Texture()
    , m_xDim(x)
    , m_yDim(y)
{
}

Font::~Font()
{
}
}
