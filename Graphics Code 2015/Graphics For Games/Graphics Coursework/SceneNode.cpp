#include "SceneNode.h"

#include <algorithm>

namespace GraphicsCoursework
{
SceneNode::SceneNode(const std::string &name)
    : m_name(name)
    , m_parent(nullptr)
    , m_localTransform(Matrix4())
    , m_worldTransform(Matrix4())
{
}

SceneNode::~SceneNode()
{
  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    delete *it;

  m_children.clear();
}

void SceneNode::AddChild(SceneNode *child)
{
  child->m_renderer = m_renderer;
  child->m_parent = this;
  m_children.push_back(child);
}

bool SceneNode::RemoveChild(SceneNode *child)
{
  bool retVal = false;

  auto it = std::find(m_children.begin(), m_children.end(), child);
  if (it != m_children.end())
  {
    child->m_renderer = nullptr;
    child->m_parent = nullptr;
    child->m_worldTransform.ToIdentity();
    m_children.erase(it);

    retVal = true;
  }

  return retVal;
}

bool SceneNode::RemoveChild(const std::string & name)
{
  bool retVal = false;

  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode * n) {return n->Name() == name; });
  if (it != m_children.end())
  {
    (*it)->m_renderer = nullptr;
    (*it)->m_parent = nullptr;
    (*it)->m_worldTransform.ToIdentity();
    m_children.erase(it);

    retVal = true;
  }

  return retVal;
}

SceneNode * SceneNode::Child(const std::string & name)
{
  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode * n) {return n->Name() == name; });
  return (it == m_children.end() ? nullptr : *it);
}

void SceneNode::UpdateTransformations()
{
  if (m_parent == nullptr)
    m_worldTransform = m_localTransform;
  else
    m_worldTransform = m_parent->m_worldTransform * m_localTransform;

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->UpdateTransformations();
}

void SceneNode::Render(RenderState & state)
{
  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Render(state);
}
}