/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
class CameraNode;

class CameraSelectorNode : public ShaderDataNode
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

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s);

protected:
  CameraNode *m_camera;
};
}
