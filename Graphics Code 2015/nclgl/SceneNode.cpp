#include "SceneNode.h"

SceneNode::SceneNode(Mesh *mesh, Vector4 colour)
    : m_mesh(mesh)
    , m_colour(colour)
    , m_parent(nullptr)
    , m_modelScale(Vector3(1.0f, 1.0f, 1.0f))
    , m_boundingRadius(1.0f)
    , m_distanceFromCamera(0.0f)
{
}

SceneNode::~SceneNode(void)
{
  for (unsigned int i = 0; i < m_children.size(); ++i)
    delete m_children[i];
}

void SceneNode::AddChild(SceneNode *s)
{
  m_children.push_back(s);
  s->m_parent = this;
}

void SceneNode::RemoveChild(SceneNode *s)
{
  auto it = std::find(m_children.begin(), m_children.end(), s);
  if (it != m_children.end())
  {
    delete *it;
    m_children.erase(it);
  }
}

void SceneNode::Draw(const OGLRenderer &r)
{
  if (m_mesh)
    m_mesh->Draw();
}

void SceneNode::Update(float msec)
{
  if (m_parent)
    m_worldTransform = m_parent->m_worldTransform * m_transform;
  else
    m_worldTransform = m_transform;

  for (vector<SceneNode *>::iterator i = m_children.begin(); i != m_children.end(); ++i)
    (*i)->Update(msec);
}
