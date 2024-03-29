/** @file */

#include "RenderableNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new renderable scene node.
 * @param name Node name
 * @param transparent Flag indicating if the mesh and texture combination will result in transparent fragments
 */
RenderableNode::RenderableNode(const std::string &name, bool transparent)
    : SceneNode(name)
    , m_transparent(transparent)
    , m_repeatedDraw(1)
    , m_cameraDistance(0.0f)
    , m_boundingSphereRadius(100.0f)
    , m_specularPower(20.0f)
    , m_specularIntensity(0.5f)
{
  SetProcessMode(PM_BOTH);
}

RenderableNode::~RenderableNode()
{
}

/**
 * @brief Renders this node outside of the scene graph.
 * @param state Reference to render state
 *
 * Used for rendering transparent nodes in a TransparentRenderingNode after Z sorting.
 */
void RenderableNode::RenderSingle(RenderState &state)
{
  std::vector<SceneNode *> stack;
  m_parent->GetStack(stack, (SceneNode *)state.transparentSync);

  for (auto it = stack.rbegin(); it != stack.rend(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->PreRender(state);
  }

  if (state.cameraViewFrustum.ContainsSceneNode(this) || m_boundingSphereRadius < 0.0f)
  {
    for (size_t i = 0; i < m_repeatedDraw; i++)
      Draw(state);
  }

  for (auto it = stack.begin(); it != stack.end(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->PostRender(state);
  }
}

/**
 * @brief Draws this renderable.
 * @param state Reference to render state
 */
void RenderableNode::Draw(RenderState &state)
{
  GLuint program = state.shader->Program();

  // Set model matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float *)&m_worldTransform);

  // Set lighting parameters
  glUniform1f(glGetUniformLocation(program, "specularPower"), m_specularPower);
  glUniform1f(glGetUniformLocation(program, "specularIntensity"), m_specularIntensity);
}

/**
 * @copydoc SceneNode::PreRender
 */
void RenderableNode::PreRender(RenderState &state)
{
  if (!m_transparent)
  {
    if (state.cameraViewFrustum.ContainsSceneNode(this) || m_boundingSphereRadius < 0.0f)
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
