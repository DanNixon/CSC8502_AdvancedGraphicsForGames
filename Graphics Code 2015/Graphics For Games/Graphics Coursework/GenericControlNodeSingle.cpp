/** @file */

#include "GenericControlNodeSingle.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
GenericControlNodeSingle::GenericControlNodeSingle(const std::string &name, ControlCallbackFunc onBind,
                                                   ControlCallbackFunc onUnBind)
    : SceneNode(name)
    , m_onEntry(onBind)
    , m_onExit(onUnBind)
{
}

GenericControlNodeSingle::~GenericControlNodeSingle()
{
}

void GenericControlNodeSingle::PreRender(RenderState &state)
{
  m_onEntry(state);
}

void GenericControlNodeSingle::PostRender(RenderState &state)
{
  m_onExit(state);
}
}
