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
  m_type = GL_TRIANGLES;

  m_numVertices = m_widthSteps * m_depthSteps;
  m_numIndices = (m_widthSteps - 1) * (m_depthSteps - 1) * 6;
  m_vertices = new Vector3[m_numVertices];
  m_indices = new GLuint[m_numIndices];
  m_textureCoords = new Vector2[m_numVertices];

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
  idx = 0;
  for (size_t x = 0; x < m_widthSteps - 1; ++x)
  {
    for (size_t z = 0; z < m_depthSteps - 1; ++z)
    {
      int a = (x * m_widthSteps) + z;
      int b = ((x + 1) * m_widthSteps) + z;
      int c = ((x + 1) * m_widthSteps) + (z + 1);
      int d = (x * m_widthSteps) + (z + 1);

      m_indices[idx++] = c;
      m_indices[idx++] = b;
      m_indices[idx++] = a;

      m_indices[idx++] = a;
      m_indices[idx++] = d;
      m_indices[idx++] = c;
    }
  }

  GenerateNormals();
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

  // Rebuild normals
  GenerateNormals();

  glBindVertexArray(m_arrayObject);

  // Rebuffer vertices
  glDeleteBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  RegisterBuffer(VERTEX_BUFFER, 3, m_vertices);

  // Rebuffer normals
  glDeleteBuffers(1, &m_bufferObjects[NORMAL_BUFFER]);
  RegisterBuffer(NORMAL_BUFFER, 3, m_normals);

  glBindVertexArray(0);
}

void HeightMapMesh::SetHeightmapFromFBM(FractalBrownianMotion *fbm)
{
  float w = 1.0f / (float)m_widthSteps;
  float d = 1.0f / (float)m_depthSteps;

  // Update y coordinates
  for (size_t i = 0; i < m_numVertices; i++)
    m_vertices[i].y = fbm->Fractal((float)(i % m_widthSteps) * d, (float)(i / m_widthSteps) * w);

  // Rebuild normals
  GenerateNormals();

  glBindVertexArray(m_arrayObject);

  // Rebuffer vertices
  glDeleteBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  RegisterBuffer(VERTEX_BUFFER, 3, m_vertices);

  // Rebuffer normals
  glDeleteBuffers(1, &m_bufferObjects[NORMAL_BUFFER]);
  RegisterBuffer(NORMAL_BUFFER, 3, m_normals);

  glBindVertexArray(0);
}
}