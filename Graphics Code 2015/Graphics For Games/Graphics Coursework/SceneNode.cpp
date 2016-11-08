#include "SceneNode.h"

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
  child->m_parent = this;
  m_children.push_back(child);
}

bool SceneNode::RemoveChild(SceneNode *child)
{
  bool retVal = false;

  auto it = std::find(m_children.begin(), m_children.end(), child);
  if (it != m_children.end())
  {
    child->m_parent = nullptr;
    child->m_worldTransform.ToIdentity();
    m_children.erase(it);

    retVal = true;
  }

  return retVal;
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

void SceneNode::Render()
{
  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Render();
}