/** @file */

#include "Light.h"

namespace GraphicsCoursework
{
Light::Light(const std::string &name)
    : ShaderDataNode(name)
    , m_radius(1.0f)
{
}

Light::~Light()
{
}

void Light::PreRender(RenderState & state)
{
  ShaderDataNode::PreRender(state);
  state.numLights++;
}

void Light::PostRender(RenderState & state)
{
  ShaderDataNode::PostRender(state);
  state.numLights--;
}

void Light::ShaderBind(ShaderProgram * s)
{
  // TODO
}
}