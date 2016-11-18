/** @file */

#include "SceneNode.h"

#include <algorithm>
#include <string>

namespace GraphicsCoursework
{
SceneNode::SceneNode(const std::string &name, Renderer *renderer)
    : m_name(name)
    , m_renderer(renderer)
    , m_parent(nullptr)
    , m_active(true)
    , m_owner(true)
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

void SceneNode::SetActive(bool active, bool recursive)
{
  m_active = active;

  if (recursive)
  {
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
      (*it)->SetActive(active, true);
  }
}

void SceneNode::GetStack(std::vector<SceneNode *> &stack)
{
  SceneNode *n = this;
  while (n != nullptr)
  {
    stack.push_back(n);
    n = n->m_parent;
  }
}

SceneNode *SceneNode::AddChild(SceneNode *child)
{
  if (this == child)
    return nullptr;

  child->m_renderer = m_renderer;
  child->m_parent = this;
  m_children.push_back(child);

  return child;
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

bool SceneNode::RemoveChild(const std::string &name)
{
  bool retVal = false;

  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode *n) { return n->Name() == name; });

  if (it != m_children.end())
    retVal = RemoveChild(*it);

  return retVal;
}

SceneNode *SceneNode::Child(const std::string &name)
{
  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode *n) { return n->Name() == name; });
  return (it == m_children.end() ? nullptr : *it);
}

SceneNode *SceneNode::FindFirstChildByName(const std::string &name)
{
  SceneNode *retVal = nullptr;

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
  {
    if ((*it)->Name() == name)
    {
      retVal = *it;
      break;
    }

    retVal = (*it)->FindFirstChildByName(name);
    if (retVal != nullptr)
      break;
  }

  return retVal;
}

void SceneNode::Update(float msec)
{
  if (m_parent == nullptr)
    m_worldTransform = m_localTransform * m_localRotation;
  else
    m_worldTransform = m_parent->m_worldTransform * m_localTransform * m_localRotation;

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Update(msec);
}

void SceneNode::Render(RenderState &state)
{
  if (m_active)
    PreRender(state);

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Render(state);

  if (m_active)
    PostRender(state);
}

void SceneNode::PrettyPrint(std::ostream &s, size_t level) const
{
  s << " " << std::string(level, ' ') << " - " << (*this) << "\n";

  for (auto it = m_children.cbegin(); it != m_children.cend(); ++it)
    (*it)->PrettyPrint(s, level + 1);
}

std::ostream &operator<<(std::ostream &s, const SceneNode &n)
{
  s << n.m_name;
  return s;
}
}
