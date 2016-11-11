/** @file */

#pragma once

#include <../nclgl/Matrix4.h>
#include <vector>

namespace GraphicsCoursework
{
class CameraNode;
class ShaderProgram;
class MeshNode;

struct RenderState
{
  RenderState()
      : shader(nullptr)
  {
    viewMatrix.ToIdentity();
    projectionMatrix.ToIdentity();
  }

  Matrix4 viewMatrix;
  Matrix4 projectionMatrix;

  ShaderProgram *shader;

  std::vector<MeshNode *> transparentNodes;
};
}
