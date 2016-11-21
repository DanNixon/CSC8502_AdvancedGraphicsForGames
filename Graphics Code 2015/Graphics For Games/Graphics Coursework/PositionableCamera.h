/** @file */

#pragma once

#include "CameraNode.h"

#include <../nclgl/Keyboard.h>

namespace GraphicsCoursework
{
/**
 * @class PositionableCamera
 * @brief Camera that can be positioned with the mouse and WASD.
 */
class PositionableCamera : public CameraNode
{
public:
  static const KeyboardKeys KEY_MOVE_FORWARDS = KEYBOARD_W;
  static const KeyboardKeys KEY_MOVE_BACKWARS = KEYBOARD_S;
  static const KeyboardKeys KEY_MOVE_LEFT = KEYBOARD_A;
  static const KeyboardKeys KEY_MOVE_RIGHT = KEYBOARD_D;
  static const KeyboardKeys KEY_MOVE_UP = KEYBOARD_SHIFT;
  static const KeyboardKeys KEY_MOVE_DOWN = KEYBOARD_SPACE;

public:
  PositionableCamera(const std::string &name);
  virtual ~PositionableCamera();

  /**
   * @brief Gets or sets the speed of the camera for linear movement.
   * @return Linear speed
   */
  inline float &LinearSpeed()
  {
    return m_linearSpeed;
  }

  /**
   * @brief Gets or sets the speed of the camera for angular movement.
   * @return Angular speed
   */
  inline float &AngularSpeed()
  {
    return m_angularSpeed;
  }

  virtual void Update(float msec);

protected:
  float m_linearSpeed;  //!< Speed multiplier for linear motion
  float m_angularSpeed; //!< Speed multiplier for angular motion

  float m_yawAngle;   //!< Yaw angle of the camera in degrees
  float m_pitchAngle; //!< Pitch angle of the camera in degrees
};
}
