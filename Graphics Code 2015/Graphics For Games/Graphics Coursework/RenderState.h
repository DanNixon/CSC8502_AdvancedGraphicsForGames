/** @file */

#pragma once

#include <../nclgl/Matrix4.h>
#include <vector>

namespace GraphicsCoursework
{
class ShaderProgram;
class RenderableNode;
class ShaderDataNode;
class CameraNode;

struct RenderState
{
  RenderState()
      : camera(nullptr)
      , shader(nullptr)
  {
  }

  CameraNode *camera;
  Matrix4 viewMatrix;

  ShaderProgram *shader;
  std::vector<ShaderDataNode *> shaderDataNodeStack;

  std::vector<RenderableNode *> transparentNodes;
};
}
