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
    , m_inViewLastFrame(false)
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

  // Only update if it was in view last frame
  // A little hacky but does save much CPU time in updating non-rendered particles
  if (m_active && m_inViewLastFrame)
    static_cast<ParticleSystem *>(m_mesh)->Update(msec);

  m_inViewLastFrame = false;
}

void ParticleSystemNode::Draw(RenderState &state)
{
  MeshNode::Draw(state);
  m_inViewLastFrame =
      m_inViewLastFrame || (state.cameraViewFrustum.ContainsSceneNode(this) || m_boundingSphereRadius < 0.0f);
}
}
