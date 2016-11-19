/** @file */

#pragma once

#include <../nclgl/Matrix4.h>
#include <GL/glew.h>
#include <vector>

#include "Frustum.h"

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
      , numLights(0)
  {
  }

  CameraNode *camera;
  Frustum cameraViewFrustum;

  ShaderProgram *shader;
  GLuint numLights;
  std::vector<ShaderDataNode *> shaderDataNodeStack;

  std::vector<RenderableNode *> transparentNodes;
};
}
