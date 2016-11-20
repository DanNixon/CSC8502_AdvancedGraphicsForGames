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
class PointLight;
class SpotLight;

struct RenderState
{
  RenderState()
      : camera(nullptr)
      , shader(nullptr)
  {
  }

  CameraNode *camera;
  Frustum cameraViewFrustum;

  ShaderProgram *shader;
  std::vector<PointLight *> scenePointLights;
  std::vector<SpotLight *> sceneSpotLights;
  std::vector<ShaderDataNode *> shaderDataNodeStack;

  std::vector<RenderableNode *> transparentNodes;
};
}
