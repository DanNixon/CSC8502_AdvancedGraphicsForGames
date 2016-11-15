/** @file */

#pragma once

#include <../nclgl/Matrix4.h>
#include <GL/glew.h>
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

  ShaderProgram *shader;
  GLuint numLights;
  std::vector<ShaderDataNode *> shaderDataNodeStack;

  std::vector<RenderableNode *> transparentNodes;
};
}
