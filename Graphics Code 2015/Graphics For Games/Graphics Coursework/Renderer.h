/** @file */

#pragma once

#include "../../nclgl/OGLRenderer.h"

#include "RenderState.h"

namespace GraphicsCoursework
{
class SceneNode;

/**
 * @class Renderer
 * @brief Responsible for updating and rendering a scene graph.
 */
class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer();

  /**
   * @brief Gets a pointer to the root node of the scene graph.
   * @return Root node
   */
  inline SceneNode *Root()
  {
    return m_sceneGraphRoot;
  }

  virtual void AddPersistentDataNode(ShaderDataNode *node);
  virtual bool RemovePersistentDataNode(ShaderDataNode *node);

  virtual void RenderScene();

  friend std::ostream &operator<<(std::ostream &s, const Renderer &r);

protected:
  RenderState m_state;         //!< Persistent render state
  SceneNode *m_sceneGraphRoot; //!< Root node of the scene graph
};
}
