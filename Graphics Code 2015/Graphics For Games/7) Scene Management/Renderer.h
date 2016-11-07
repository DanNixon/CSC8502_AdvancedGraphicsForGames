#pragma once

#include "../nclgl/Camera.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  void BuildNodeLists(SceneNode *from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes();
  void DrawNode(SceneNode *n);

  SceneNode *root;
  Camera *camera;
  Mesh *quad;

  Frustum frameFrustum;

  vector<SceneNode *> transparentNodeList;
  vector<SceneNode *> nodeList;
};
