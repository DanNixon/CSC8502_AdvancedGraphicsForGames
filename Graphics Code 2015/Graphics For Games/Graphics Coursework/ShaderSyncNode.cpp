/** @file */

#include "ShaderSyncNode.h"

#include "CameraNode.h"
#include "ShaderDataNode.h"
#include "ShaderProgram.h"

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
  GLuint program = 0;
  if (state.shader != nullptr)
    program = state.shader->Program();

  glUseProgram(program);

  for (auto it = state.shaderDataNodeStack.begin(); it != state.shaderDataNodeStack.end(); ++it)
  {
    if ((*it)->IsActive())
      (*it)->ShaderBind(state.shader);
  }

  glUniform1i(glGetUniformLocation(program, "numPointLights"), state.numPointLights);
  glUniform1i(glGetUniformLocation(program, "numSpotLights"), state.numSpotLights);

  if (state.camera != nullptr)
  {
    Matrix4 proj;
    glGetUniformfv(program, glGetUniformLocation(state.shader->Program(), "projMatrix"), (float *)&proj);
    state.cameraViewFrustum = Frustum(proj * state.camera->ViewMatrix());
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