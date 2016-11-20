/** @file */

#pragma once

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
      : numPointLights(0)
      , numSpotLights(0)
      , camera(nullptr)
      , shader(nullptr)
  {
  }

  size_t numPointLights;
  size_t numSpotLights;

  CameraNode *camera;
  Frustum cameraViewFrustum;

  ShaderProgram *shader;
  std::vector<ShaderDataNode *> shaderDataNodeStack;

  std::vector<RenderableNode *> transparentNodes;
};
}
