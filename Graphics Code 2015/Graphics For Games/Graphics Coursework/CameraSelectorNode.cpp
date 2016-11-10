#include "CameraSelectorNode.h"

#include "CameraNode.h"
#include "Renderer.h"

namespace GraphicsCoursework
{
CameraSelectorNode::CameraSelectorNode(const std::string &name)
    : SceneNode(name)
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

void CameraSelectorNode::Render(RenderState &state)
{
  if (m_active)
    state.viewMatrix = m_camera->ViewMatrix();

  SceneNode::Render(state);

  if (m_active)
    state.viewMatrix.ToIdentity();
}
}