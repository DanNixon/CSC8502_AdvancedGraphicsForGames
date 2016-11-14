/** @file */

#include "Font.h"

namespace GraphicsCoursework
{
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