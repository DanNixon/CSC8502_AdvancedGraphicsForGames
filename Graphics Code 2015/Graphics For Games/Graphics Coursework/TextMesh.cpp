/** @file */

#include "TextMesh.h"

#include "Font.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new text node.
 * @param maxLength Maximum length of the string displayed by this node
 * @param font Font used to render text
 */
TextMesh::TextMesh(size_t maxLength, Font *font)
    : m_font(font)
    , m_maxLength(maxLength)
    , m_glyphWidth(1.0f / font->XDim())
    , m_glyphHeight(1.0f / font->YDim())
{
  m_type = GL_TRIANGLE_STRIP;

  m_numVertices = maxLength * 4;

  m_vertices = new Vector3[m_numVertices];
  m_textureCoords = new Vector2[m_numVertices];

  for (size_t i = 0; i < maxLength; ++i)
  {
    m_vertices[(i * 4)] = Vector3((float)i, 0, 0);
    m_vertices[(i * 4) + 1] = Vector3((float)i, -1, 0);
    m_vertices[(i * 4) + 2] = Vector3((float)i + 1, 0, 0);
    m_vertices[(i * 4) + 3] = Vector3((float)i + 1, -1, 0);

    m_textureCoords[(i * 4)] = Vector2();
    m_textureCoords[(i * 4) + 1] = Vector2();
    m_textureCoords[(i * 4) + 2] = Vector2();
    m_textureCoords[(i * 4) + 3] = Vector2();
  }

  BufferData();
}

TextMesh::~TextMesh()
{
}

/**
 * @brief Sets the text displayed by this mesh.
 * @param text String to display
 */
void TextMesh::SetText(const std::string &text)
{
  /*
  Now to generate the vertex attributes for each character
  of text. Vertex position is 'easy' - it's just i to i+1
  on the width, and 0 to -1 on the height. It's -1 so that
  if we draw orthographic text at a screen position of
  (0,0) (top LEFT of the screen intuitively), the text
  'hangs down' from the top left, and is visible.
  */
  for (unsigned int i = 0; i < m_maxLength; ++i)
  {
    char c = (i < text.length() ? text[i] : ' ');

    // We can use the modulus operator to wrap around the
    // position of the character, using the cell counts
    // of the font! so if we have a cell count of 16
    // by 16, character 0 will be at the top left,
    // character 15 will be at the far right, and 16
    // will be on the left, down a row, and so on.

    float x = (float)(c % m_font->XDim());
    float y = (float)((c / m_font->XDim()) % m_font->YDim());

    // Now we can simply use our worked out font character sizes
    // to generate the correct texture coordinates for each glyph...
    m_textureCoords[(i * 4)] = Vector2(x * m_glyphWidth, y * m_glyphHeight);
    m_textureCoords[(i * 4) + 1] = Vector2(x * m_glyphWidth, (y + 1) * m_glyphHeight);
    m_textureCoords[(i * 4) + 2] = Vector2((x + 1) * m_glyphWidth, y * m_glyphHeight);
    m_textureCoords[(i * 4) + 3] = Vector2((x + 1) * m_glyphWidth, (y + 1) * m_glyphHeight);
  }

  glBindVertexArray(m_arrayObject);
  glDeleteBuffers(1, &m_bufferObjects[TEXTURE_BUFFER]);
  RegisterBuffer(TEXTURE_BUFFER, 2, m_textureCoords);
  glBindVertexArray(0);
}
}
