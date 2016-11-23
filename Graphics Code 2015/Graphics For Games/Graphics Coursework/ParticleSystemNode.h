#pragma once

#include "MeshNode.h"

#include "IParticleSystem.h"

namespace GraphicsCoursework
{
  class ParticleSystemNode : public MeshNode
  {
  public:
    ParticleSystemNode(const std::string &name, IParticleSystem * particleSystem);
    virtual ~ParticleSystemNode();

    inline IParticleSystem * System()
    {
      return static_cast<IParticleSystem *>(m_mesh);
    }

    virtual void Update(float msec);
  };
}