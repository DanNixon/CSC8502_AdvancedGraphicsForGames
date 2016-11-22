/** @file */

#include "SceneNode.h"

#include <algorithm>
#include <string>

namespace GraphicsCoursework
{
/**
 * @brief Creates a new scene node.
 * @param name Node name
 * @param renderer Parent renderer (only set when Renderer adds root node)
 */
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

/**
 * @brief Sets the active flag for this node.
 * @param active New active flag value
 * @param recursive If the value should be set for all children of this node
 */
void SceneNode::SetActive(bool active, bool recursive)
{
  m_active = active;

  if (recursive)
  {
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
      (*it)->SetActive(active, true);
  }
}

/**
 * @brief Toggles the value of the active flag for this node.
 * @return New value of the active flag
 */
bool SceneNode::ToggleActive()
{
  SetActive(!m_active);
  return m_active;
}

/**
 * @brief Gets parental hierarchy of this node.
 * @param stack Reference to stack container
 * @param until SceneNode at which to stop graph traversal
 */
void SceneNode::GetStack(std::vector<SceneNode *> &stack, SceneNode *until)
{
  SceneNode *n = this;
  while (n != nullptr && n != until)
  {
    stack.push_back(n);
    n = n->m_parent;
  }
}

/**
 * @brief Adds a child node to this node.
 * @param child Child node
 * @return Child node
 */
SceneNode *SceneNode::AddChild(SceneNode *child)
{
  if (this == child)
    return nullptr;

  child->m_renderer = m_renderer;
  child->m_parent = this;
  m_children.push_back(child);

  return child;
}

/**
 * @brief Removes a child node.
 * @param child Child node
 * @return True for a successful removal
 */
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

/**
 * @brief Removes a child node by name.
 * @param name Node name
 * @return True for a successful removal
 */
bool SceneNode::RemoveChild(const std::string &name)
{
  bool retVal = false;

  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode *n) { return n->Name() == name; });

  if (it != m_children.end())
    retVal = RemoveChild(*it);

  return retVal;
}

/**
 * @brief Gets a child node by name.
 * @param name Node name
 * @return Pointer to child node, nullptr if not found
 */
SceneNode *SceneNode::Child(const std::string &name)
{
  auto it = std::find_if(m_children.begin(), m_children.end(), [name](SceneNode *n) { return n->Name() == name; });
  return (it == m_children.end() ? nullptr : *it);
}

/**
 * @brief Gets the first node in the tree with a given name.
 * @param name Node name
 * @return Pointer to node, nullptr if not found
 */
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

/**
 * @brief Recursively updates the scene.
 * @param msec Time in milliseconds since last call to Update
 */
void SceneNode::Update(float msec)
{
  if (m_parent == nullptr)
    m_worldTransform = m_localTransform * m_localRotation;
  else
    m_worldTransform = m_parent->m_worldTransform * m_localTransform * m_localRotation;

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Update(msec);
}

/**
 * @brief Recursively renders the scene graph.
 * @param state Reference to current render state
 */
void SceneNode::Render(RenderState &state)
{
  if (m_active)
    PreRender(state);

  for (auto it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->Render(state);

  if (m_active)
    PostRender(state);
}

/**
 * @brief Recursively pretty prints the scene graph.
 * @param s Reference to output stream
 * @param level Level in recursion
 */
void SceneNode::PrettyPrint(std::ostream &s, size_t level) const
{
  s << " " << std::string(level, ' ') << " - " << (*this) << "\n";

  for (auto it = m_children.cbegin(); it != m_children.cend(); ++it)
    (*it)->PrettyPrint(s, level + 1);
}

/**
 * @brief Outputs a scene node to a stream.
 * @param s Reference to output stream
 * @param n SceneNode to print
 * @return Reference to output stream
 */
std::ostream &operator<<(std::ostream &s, const SceneNode &n)
{
  s << n.m_name;
  return s;
}
}
