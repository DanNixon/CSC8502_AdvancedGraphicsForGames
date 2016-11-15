/** @file */

#include "HeightMapMesh.h"

#include "FractalBrownianMotion.h"

namespace GraphicsCoursework
{
HeightMapMesh::HeightMapMesh(float width, float depth, size_t widthSteps, size_t depthSteps)
    : Mesh()
    , m_widthSteps(widthSteps)
    , m_depthSteps(depthSteps)
{
  m_type = GL_TRIANGLE_STRIP;
  // m_type = GL_LINE_STRIP;

  m_numVertices = m_widthSteps * m_depthSteps;
  m_vertices = new Vector3[m_numVertices];
  m_textureCoords = new Vector2[m_numVertices];

  m_numIndices = (m_widthSteps * 2) * (m_depthSteps - 1) + (m_depthSteps - 2);
  m_indices = new GLuint[m_numIndices];

  // Half the plane size
  Vector3 halfSize(width / 2.0f, 0.0f, depth / 2.0f);

  float deltaCol = width / (float)(m_widthSteps - 1);
  float deltaRow = depth / (float)(m_depthSteps - 1);

  // Generate vertices
  size_t idx = 0;
  for (size_t col = 0; col < m_widthSteps; col++)
  {
    for (size_t row = 0; row < m_depthSteps; row++)
    {
      // Positional coordinates
      float x = (float)col * deltaCol;
      float z = (float)row * deltaRow;

      // Texture coordinates
      float u = (float)col / (float)m_widthSteps;
      float v = (float)row / (float)m_depthSteps;

      m_vertices[idx] = Vector3(x, 0.0f, z) - halfSize;
      m_textureCoords[idx] = Vector2(u, v);

      idx++;
    }
  }

  // Generate indices
  int index = 0;
  for (size_t z = 0; z < m_depthSteps - 1; z++)
  {
    int x;

    // Even row (left to right)
    if (z % 2 == 0)
    {
      for (x = 0; x < m_widthSteps; x++)
      {
        m_indices[index++] = (GLuint)(x + (z * m_widthSteps));
        m_indices[index++] = (GLuint)(x + (z * m_widthSteps) + m_widthSteps);
      }

      // Degenerate triangle at end of non-last row
      if (z != m_depthSteps - 2)
        m_indices[index++] = (GLuint)(--x + (z * m_widthSteps));
    }
    // Odd row (right to left)
    else
    {
      for (x = m_widthSteps - 1; x >= 0; x--)
      {
        m_indices[index++] = (GLuint)(x + (z * m_widthSteps));
        m_indices[index++] = (GLuint)(x + (z * m_widthSteps) + m_widthSteps);
      }

      // Degenerate triangle at end of non-last row
      if (z != m_depthSteps - 2)
        m_indices[index++] = (GLuint)(++x + (z * m_widthSteps));
    }
  }

  BufferData();
}

HeightMapMesh::~HeightMapMesh()
{
}

void HeightMapMesh::SetHeightmap(float *height)
{
  // Update y coordinates
  for (size_t i = 0; i < m_numVertices; i++)
    m_vertices[i].y = height[i];

  // Rebuffer vertices
  glBindVertexArray(m_arrayObject);
  glDeleteBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  RegisterBuffer(VERTEX_BUFFER, 3, m_vertices);
  glBindVertexArray(0);
}

void HeightMapMesh::SetHeightmapFromFBM(FractalBrownianMotion *fbm)
{
  float w = 1.0f / (float)m_widthSteps;
  float d = 1.0f / (float)m_depthSteps;

  // Update y coordinates
  for (size_t i = 0; i < m_numVertices; i++)
    m_vertices[i].y = fbm->Fractal((float)(i % m_widthSteps) * d, (float)(i / m_widthSteps) * w);

  // Rebuffer vertices
  glBindVertexArray(m_arrayObject);
  glDeleteBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  RegisterBuffer(VERTEX_BUFFER, 3, m_vertices);
  glBindVertexArray(0);
}
}