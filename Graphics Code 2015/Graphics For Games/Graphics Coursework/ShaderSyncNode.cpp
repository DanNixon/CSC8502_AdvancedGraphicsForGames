/** @file */

#include "ShaderSyncNode.h"

#include "ShaderDataNode.h"
#include "ShaderProgram.h"
#include "CameraNode.h"

namespace GraphicsCoursework
{
ShaderSyncNode::ShaderSyncNode(const std::string &name)
    : SceneNode(name)
{
}

ShaderSyncNode::~ShaderSyncNode()
{
}

void ShaderSyncNode::PreRender(RenderState &state)
{
  glUseProgram(state.shader->Program());

  for (auto it = state.shaderDataNodeStack.begin(); it != state.shaderDataNodeStack.end(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->ShaderBind(state.shader);
  }

  glUniform1i(glGetUniformLocation(state.shader->Program(), "numPointLights"), state.numPointLights);
  glUniform1i(glGetUniformLocation(state.shader->Program(), "numSpotLights"), state.numSpotLights);

  if (state.camera != nullptr)
  {
    Matrix4 proj;
    glGetUniformfv(state.shader->Program(), glGetUniformLocation(state.shader->Program(), "projMatrix"), (float *)&proj);
    state.cameraViewFrustum = Frustum(proj * state.camera->ViewMatrix());  //proj matrix
  }
}

void ShaderSyncNode::PostRender(RenderState &state)
{
  for (auto it = state.shaderDataNodeStack.rbegin(); it != state.shaderDataNodeStack.rend(); ++it)
    (*it)->ShaderUnBind(state.shader);

  glUniform1i(glGetUniformLocation(state.shader->Program(), "numPointLights"), 0);
  glUniform1i(glGetUniformLocation(state.shader->Program(), "numSpotLights"), 0);

  state.cameraViewFrustum.Reset();
}
}