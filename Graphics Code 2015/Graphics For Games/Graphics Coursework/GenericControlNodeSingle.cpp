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

/**
 * @copydoc SceneNode::PreRender
 */
void GenericControlNodeSingle::PreRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    m_onEntry(state);
}

/**
 * @copydoc SceneNode::PostRender
 */
void GenericControlNodeSingle::PostRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    m_onExit(state);
}
}
