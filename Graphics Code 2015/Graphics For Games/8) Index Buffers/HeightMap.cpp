#include "HeightMap.h"

HeightMap::HeightMap(std::string name)
{
  std::ifstream file(name.c_str(), std::ios::binary);
  if (!file)
    return;

  m_numVertices = RAW_WIDTH * RAW_HEIGHT;
  m_numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;
  m_vertices = new Vector3[m_numVertices];
  m_textureCoords = new Vector2[m_numVertices];
  m_indices = new GLuint[m_numIndices];
  unsigned char *data = new unsigned char[m_numVertices];
  file.read((char *)data, m_numVertices * sizeof(unsigned char));
  file.close();

  for (int x = 0; x < RAW_WIDTH; ++x)
  {
    for (int z = 0; z < RAW_HEIGHT; ++z)
    {
      int offset = (x * RAW_WIDTH) + z;

      m_vertices[offset] = Vector3(x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);

      m_textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
    }
  }

  delete data;
  m_numIndices = 0;

  for (int x = 0; x < RAW_WIDTH - 1; ++x)
  {
    for (int z = 0; z < RAW_HEIGHT - 1; ++z)
    {
      int a = (x * (RAW_WIDTH)) + z;
      int b = ((x + 1) * (RAW_WIDTH)) + z;
      int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
      int d = (x * (RAW_WIDTH)) + (z + 1);

      m_indices[m_numIndices++] = c;
      m_indices[m_numIndices++] = b;
      m_indices[m_numIndices++] = a;

      m_indices[m_numIndices++] = a;
      m_indices[m_numIndices++] = d;
      m_indices[m_numIndices++] = c;
    }
  }

  BufferAllData();
}
