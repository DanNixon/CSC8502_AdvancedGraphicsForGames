#include "ParticleSystemNode.h"

namespace GraphicsCoursework
{
ParticleSystemNode::ParticleSystemNode(const std::string &name, IParticleSystem *particleSystem)
    : MeshNode(name, particleSystem, true)
{
  SetBoundingSphereRadius(-1.0f);
}

ParticleSystemNode::~ParticleSystemNode()
{
}

void ParticleSystemNode::Update(float msec)
{
  RenderableNode::Update(msec);

  if (m_active)
    static_cast<IParticleSystem *>(m_mesh)->Update(msec);
}
}