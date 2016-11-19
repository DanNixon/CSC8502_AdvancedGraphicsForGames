/** @file */

#include "RenderableNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
RenderableNode::RenderableNode(const std::string &name, bool transparent)
    : SceneNode(name)
    , m_transparent(transparent)
    , m_repeatedDraw(1)
    , m_cameraDistance(0.0f)
    , m_boundingSphereRadius(1.0f)
    , m_specularPower(10.0f)
    , m_specularIntensity(0.33f)
{
}

RenderableNode::~RenderableNode()
{
}

void RenderableNode::RenderSingle(RenderState &state)
{
  std::vector<SceneNode *> stack;
  m_parent->GetStack(stack);

  for (auto it = stack.begin(); it != stack.end(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->PreRender(state);
  }

  if (state.cameraViewFrustum.ContainsSceneNode(this))
  {
    for (size_t i = 0; i < m_repeatedDraw; i++)
      Draw(state);
  }

  for (auto it = stack.rbegin(); it != stack.rend(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->PostRender(state);
  }
}

void RenderableNode::Draw(RenderState &state)
{
  GLuint program = state.shader->Program();

  // Set model matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float *)&m_worldTransform);

  // Set lighting parameters
  glUniform1f(glGetUniformLocation(program, "specularPower"), m_specularPower);
  glUniform1f(glGetUniformLocation(program, "specularIntensity"), m_specularIntensity);
}

void RenderableNode::PreRender(RenderState &state)
{
  if (!m_transparent)
  {
    if (state.cameraViewFrustum.ContainsSceneNode(this))
    {
      for (size_t i = 0; i < m_repeatedDraw; i++)
        Draw(state);
    }
  }
  else
  {
    m_cameraDistance = DistanceFrom((SceneNode *)state.camera);
    state.transparentNodes.push_back(this);
  }
}
}