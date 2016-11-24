/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
class CameraNode;

/**
 * @class CameraSelectorNode
 * @brief Node used to select the camera that is used to draw the children of this node.
 */
class CameraSelectorNode : public ShaderDataNode
{
public:
  CameraSelectorNode(const std::string &name);
  virtual ~CameraSelectorNode();

  void SetCamera(const std::string &cameraName);

  /**
   * @brief Sets the camera to be activated by this node.
   * @param camera Camera
   */
  inline void SetCamera(CameraNode *camera)
  {
    m_camera = camera;
  }

  /**
   * @brief Gets the camera to be acivated by this node.
   * @return Camera
   */
  inline CameraNode *Camera()
  {
    return m_camera;
  }

  virtual void SetActive(bool active);

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s);

protected:
  CameraNode *m_camera;         //!< The camera that will be activated by this node.
  CameraNode *m_previousCamera; //!< Perviously active camera
};
}
