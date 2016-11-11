/** @file */

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

void CameraSelectorNode::PreRender(RenderState &state)
{
  state.camera = m_camera;
  state.viewMatrix = m_camera->ViewMatrix();
}

void CameraSelectorNode::PostRender(RenderState &state)
{
  state.camera = nullptr;
  state.viewMatrix.ToIdentity();
}
}
