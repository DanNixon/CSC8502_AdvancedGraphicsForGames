/** @file */

#include "IParticleSystem.h"

namespace GraphicsCoursework
{
IParticleSystem::IParticleSystem(size_t numParticles, bool colour)
    : Mesh()
{
  m_type = GL_TRIANGLE_STRIP;
  m_numVertices = numParticles;

  m_vertices = new Vector3[m_numVertices];

  if (colour)
  {
    m_colours = new Vector4[m_numVertices];

    for (size_t i = 0; i < m_numVertices; i++)
      m_colours[i] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  }

  BufferAllData();
}

IParticleSystem::~IParticleSystem()
{
}

void IParticleSystem::Update(float msec)
{
  DeleteBuffer(VERTEX_BUFFER);
  DeleteBuffer(COLOUR_BUFFER);
  BufferData(VERTEX_BUFFER, 3, m_vertices);
  BufferData(COLOUR_BUFFER, 4, m_colours);
}
}