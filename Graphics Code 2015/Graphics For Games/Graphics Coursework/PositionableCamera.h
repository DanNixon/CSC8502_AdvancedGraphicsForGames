/** @file */

#pragma once

#include "CameraNode.h"

#include <../nclgl/Keyboard.h>

namespace GraphicsCoursework
{
class PositionableCamera : public CameraNode
{
public:
  static const KeyboardKeys KEY_MOVE_FORWARDS = KEYBOARD_W;
  static const KeyboardKeys KEY_MOVE_BACKWARS = KEYBOARD_S;
  static const KeyboardKeys KEY_MOVE_LEFT = KEYBOARD_A;
  static const KeyboardKeys KEY_MOVE_RIGHT = KEYBOARD_D;

public:
  PositionableCamera(const std::string &name);
  virtual ~PositionableCamera();

  inline float &LinearSpeed()
  {
    return m_linearSpeed;
  }

  inline float &AngularSpeed()
  {
    return m_angularSpeed;
  }

  virtual void Update(float msec);

protected:
  float m_linearSpeed;
  float m_angularSpeed;
};
}
