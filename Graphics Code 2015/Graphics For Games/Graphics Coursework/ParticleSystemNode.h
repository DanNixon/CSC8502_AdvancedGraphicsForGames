#pragma once

#include "MeshNode.h"

#include "ParticleSystem.h"

namespace GraphicsCoursework
{
/**
 * @class ParticleSystemNode
 * @brief Renderable node containg a particle system.
 */
class ParticleSystemNode : public MeshNode
{
public:
  ParticleSystemNode(const std::string &name, ParticleSystem *particleSystem);
  virtual ~ParticleSystemNode();

  /**
   * @brief Gets a pointer to the underlying particle system.
   * @return Particle system
   */
  inline ParticleSystem *System()
  {
    return static_cast<ParticleSystem *>(m_mesh);
  }

  virtual void Update(float msec);
  virtual void Draw(RenderState &state);

protected:
  bool m_inViewLastFrame;
};
}
