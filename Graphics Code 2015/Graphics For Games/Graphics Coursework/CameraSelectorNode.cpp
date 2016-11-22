/** @file */

#include "CameraSelectorNode.h"

#include "CameraNode.h"
#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new camera selector node.
 * @param name Node name
 */
CameraSelectorNode::CameraSelectorNode(const std::string &name)
    : ShaderDataNode(name)
    , m_camera(nullptr)
{
}

CameraSelectorNode::~CameraSelectorNode()
{
}

/**
 * @brief Sets the camera to be selected given it's node name.
 * @param cameraName Name of camera nose
 */
void CameraSelectorNode::SetCamera(const std::string &cameraName)
{
  if (m_renderer == nullptr)
  {
    m_camera = nullptr;
    return;
  }

  m_camera = dynamic_cast<CameraNode *>(m_renderer->Root()->FindFirstChildByName(cameraName));

  // Update active flag to account for different camera
  SetActive(m_active);
}

/**
 * @copydoc ShaderDataNode::SetActive
 */
void CameraSelectorNode::SetActive(bool active)
{
  m_active = active && m_camera != nullptr;
}

/**
 * @copydoc ShaderDataNode::PreRender
 */
void CameraSelectorNode::PreRender(RenderState &state)
{
  ShaderDataNode::PreRender(state);

  if (ProcessingPassCheck(state))
    state.camera = m_camera;
}

/**
 * @copydoc ShaderDataNode::PostRender
 */
void CameraSelectorNode::PostRender(RenderState &state)
{
  ShaderDataNode::PostRender(state);

  if (ProcessingPassCheck(state))
    state.camera = nullptr;
}

/**
 * @copydoc ShaderDataNode::ShaderBind
 */
void CameraSelectorNode::ShaderBind(ShaderProgram *s)
{
  glUniformMatrix4fv(glGetUniformLocation(s->Program(), "viewMatrix"), 1, false, (float *)&m_camera->ViewMatrix());
  glUniform3fv(glGetUniformLocation(s->Program(), "cameraPos"), 1,
               (float *)&m_camera->GetWorldTransformation().GetPositionVector());
}
}
