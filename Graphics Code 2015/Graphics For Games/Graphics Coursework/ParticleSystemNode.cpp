#include "ParticleSystemNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new particle system node.
 * @param name Node name
 * @param particleSystem Particle system to contain
 */
ParticleSystemNode::ParticleSystemNode(const std::string &name, ParticleSystem *particleSystem)
    : MeshNode(name, particleSystem, true)
{
  SetBoundingSphereRadius(-1.0f);
}

ParticleSystemNode::~ParticleSystemNode()
{
}

/**
 * @copydoc MeshNode::Update
 */
void ParticleSystemNode::Update(float msec)
{
  RenderableNode::Update(msec);

  if (m_active)
    static_cast<ParticleSystem *>(m_mesh)->Update(msec);
}
}
