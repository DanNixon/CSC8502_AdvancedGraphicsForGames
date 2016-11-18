/** @file */

#include "CameraSelectorNode.h"

#include "CameraNode.h"
#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
CameraSelectorNode::CameraSelectorNode(const std::string &name)
    : ShaderDataNode(name)
    , m_camera(nullptr)
{
}

CameraSelectorNode::~CameraSelectorNode()
{
}

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

void CameraSelectorNode::SetActive(bool active)
{
  m_active = active && m_camera != nullptr;
}

void CameraSelectorNode::PreRender(RenderState &state)
{
  ShaderDataNode::PreRender(state);
  state.camera = m_camera;
}

void CameraSelectorNode::PostRender(RenderState &state)
{
  ShaderDataNode::PostRender(state);
  state.camera = nullptr;
}

void CameraSelectorNode::ShaderBind(ShaderProgram *s)
{
  glUniformMatrix4fv(glGetUniformLocation(s->Program(), "viewMatrix"), 1, false, (float *)&m_camera->ViewMatrix());
  glUniform3fv(glGetUniformLocation(s->Program(), "cameraPos"), 1,
               (float *)&m_camera->GetWorldTransformation().GetPositionVector());
}
}
