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
class TransparentRenderingNode;

/**
 * @brief Stores data used when rendering.
 *
 * Passed to each node in the scene graph during traversal.
 */
struct RenderState
{
  RenderState()
      : numPointLights(0)
      , numSpotLights(0)
      , camera(nullptr)
      , shader(nullptr)
      , processPass(false)
      , transparentSync(nullptr)
  {
  }

  size_t numPointLights; //!< Number of point lights in the scene
  size_t numSpotLights;  //!< Number of spot lights in the scene

  CameraNode *camera;        //!< Active camera (may be nullptr during rendering)
  Frustum cameraViewFrustum; //!< Cached camera view frustum

  ShaderProgram *shader;                             //!< Current shader (should never be null after ShaderSyncNode)
  std::vector<ShaderDataNode *> shaderDataNodeStack; //!< Stack of ShaderDataNode to be activated by ShaderSyncNode

  bool processPass;

  std::vector<RenderableNode *> transparentNodes; //!< List of transparent nodes
  TransparentRenderingNode
      *transparentSync; //!< Lowest TransparentRenderingNode (the one that will next process transparentNodes)
};
}
