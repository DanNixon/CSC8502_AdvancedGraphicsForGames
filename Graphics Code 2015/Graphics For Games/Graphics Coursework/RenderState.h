/** @file */

#pragma once

#include <../nclgl/Matrix4.h>
#include <vector>

namespace GraphicsCoursework
{
class CameraNode;
class ShaderProgram;
class MeshNode;
class TextureNode;
class Light;

struct RenderState
{
  RenderState()
      : shader(nullptr)
      , camera(nullptr)
  {
    viewMatrix.ToIdentity();
    projectionMatrix.ToIdentity();
  }

  CameraNode * camera;
  Matrix4 viewMatrix;
  Matrix4 projectionMatrix;

  ShaderProgram *shader;

  std::vector<TextureNode *> textures;
  std::vector<Light *> lights;

  std::vector<MeshNode *> transparentNodes;
};
}
