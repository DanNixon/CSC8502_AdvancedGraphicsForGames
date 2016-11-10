#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class CameraNode;

class CameraSelectorNode : public SceneNode
{
public:
  CameraSelectorNode(const std::string &name);
  virtual ~CameraSelectorNode();

  void SetCamera(const std::string &cameraName);

  inline void SetCamera(CameraNode *camera)
  {
    m_camera = camera;
  }

  inline CameraNode *Camera()
  {
    return m_camera;
  }

  virtual void SetActive(bool active);
  virtual void Render(RenderState &state);

protected:
  CameraNode *m_camera;
};
}