/** @file */

#include "Renderer.h"

#include "PointLight.h"
#include "RenderableNode.h"
#include "SpotLight.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new renderer.
 * @param parent Parent window to operate within
 */
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;
}

/**
 * @brief Adds a ShaderDataNode to the persistent render state that will reside on the bottom of the stack.
 * @param node Persistent node
 */
void Renderer::AddPersistentDataNode(ShaderDataNode *node)
{
  m_state.shaderDataNodeStack.push_back(node);

  PointLight *pointLight = dynamic_cast<PointLight *>(node);
  if (pointLight != nullptr)
  {
    pointLight->SetIndex(m_state.numPointLights++);
    m_state.lights.push_back(pointLight);
  }

  SpotLight *spotLight = dynamic_cast<SpotLight *>(node);
  if (spotLight != nullptr)
  {
    spotLight->SetIndex(m_state.numSpotLights++);
    m_state.lights.push_back(spotLight);
  }
}

/**
 * @brief Removes a persistent ShaderDataNode from the stack.
 * @param node Node to remove
 * @return True if removed successfully
 */
bool Renderer::RemovePersistentDataNode(ShaderDataNode *node)
{
  auto it = std::find(m_state.shaderDataNodeStack.begin(), m_state.shaderDataNodeStack.end(), node);
  bool found = (it != m_state.shaderDataNodeStack.end());
  if (found)
    m_state.shaderDataNodeStack.erase(it);

  return found;
}

/**
 * @brief Renders the scene and swaps the front/back buffers.
 */
void Renderer::RenderScene()
{
  // Do shadow passes
  for (auto it = m_state.lights.begin(); it != m_state.lights.end(); ++it)
    (*it)->DoShadowRender(m_state);

  // Do main pass
  m_sceneGraphRoot->Render(m_state);

  SwapBuffers();

  // Reset state
  m_state.transparentNodes.clear();
  m_state.shadowMaps.clear();
}

/**
 * @brief Outputs a Renderer to a stream.
 * @param s Reference to output stream
 * @param r Reference to Renderer to output
 * @return Reference to output stream
 */
std::ostream &operator<<(std::ostream &s, const Renderer &r)
{
  s << "Renderer scene graph:\n";
  r.m_sceneGraphRoot->PrettyPrint(s);
  return s;
}
}
