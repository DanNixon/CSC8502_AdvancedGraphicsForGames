/** @file */

#pragma once

#include <../nclgl/Mesh.h>

namespace GraphicsCoursework
{
  class IParticleSystem : public Mesh
  {
  public:
    IParticleSystem(size_t numParticles, bool colour = true);
    virtual ~IParticleSystem();

    virtual void Update(float msec);

    inline void SetParticlePosition(size_t idx, const Vector3 &position)
    {
      m_vertices[idx] = position;
    }

    inline void SetParticleColour(size_t idx, const Vector4 &colour)
    {
      if (m_colours != nullptr)
        m_colours[idx] = colour;
    }
  };
}