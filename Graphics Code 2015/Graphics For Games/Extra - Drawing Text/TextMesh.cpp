#include "TextMesh.h"

/*
Our really basic TextMesh is going to make one big line of quads, each
containing a character of text, generated from the passed in string, and
the font struct.

Each character of text will be kept in a quad 1 unit big one each axis,
and each quad is right next to its neighbours, making it easy to work out
how large a textmesh will be when scaled etc
*/
TextMesh::TextMesh(const std::string &text, const Font &font)
    : font(font)
{
  // Set our Mesh to have the font's texture, containing all of the
  // individual characters we need.
  SetTexture(font.texture);

  // Each quad will be 4 points, drawn using a triangle strip
  // just like the GenerateQuad function from early on!
  m_type = GL_TRIANGLE_STRIP;

  // Each character has 4 vertices...
  m_numVertices = text.length() * 4;

  m_vertices = new Vector3[m_numVertices];
  m_textureCoords = new Vector2[m_numVertices];

  // Now to work out how much of the texture each character
  // of the font takes up. Remember, texture coordinates
  // are 'normalised', so a bitmap font texture of 16 by 16
  // will have characters that are 1 / 16.0 in size etc...
  float texelWidth = 1.0f / font.xCount;
  float texelHeight = 1.0f / font.yCount;

  /*
  Now to generate the vertex attributes for each character
  of text. Vertex position is 'easy' - it's just i to i+1
  on the width, and 0 to -1 on the height. It's -1 so that
  if we draw orthographic text at a screen position of
  (0,0) (top LEFT of the screen intuitively), the text
  'hangs down' from the top left, and is visible.
  */
  for (unsigned int i = 0; i < text.length(); ++i)
  {
    unsigned int c = (unsigned int)text[i];

    // We can use the modulus operator to wrap around the
    // position of the character, using the cell counts
    // of the font! so if we have a cell count of 16
    // by 16, character 0 will be at the top left,
    // character 15 will be at the far right, and 16
    // will be on the left, down a row, and so on.

    float x = (float)(c % font.xCount);
    float y = (float)((c / font.xCount) % font.yCount);

    m_vertices[(i * 4)] = Vector3((float)i, 0, 0);
    m_vertices[(i * 4) + 1] = Vector3((float)i, -1, 0);
    m_vertices[(i * 4) + 2] = Vector3((float)i + 1, 0, 0);
    m_vertices[(i * 4) + 3] = Vector3((float)i + 1, -1, 0);

    // Now we can simply use our worked out font character sizes
    // to generate the correct texture coordinates for each glyph...
    m_textureCoords[(i * 4)] = Vector2(x * texelWidth, (y)*texelHeight);
    m_textureCoords[(i * 4) + 1] = Vector2(x * texelWidth, (y + 1) * texelHeight);
    m_textureCoords[(i * 4) + 2] = Vector2((x + 1) * texelWidth, (y)*texelHeight);
    m_textureCoords[(i * 4) + 3] = Vector2((x + 1) * texelWidth, (y + 1) * texelHeight);
  }
  // Lastly, we buffer the data, just like a 'normal' mesh!
  BufferAllData();
}

/*
Multiple textMeshes could be keeping hold of the same texture (borrowed as a
pointer from its Font) so we must NULL the texture name before we call the
parent class destructor, which deletes the texture! (We could overload
Draw instead to dereference texture every frame, but this way is slightly
more elegant)
*/
TextMesh::~TextMesh(void)
{
  m_texture = 0;
}
