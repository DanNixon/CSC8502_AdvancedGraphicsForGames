#pragma once

#include "../../nclgl/OGLRenderer.h"

#define NUM_MESHES 2

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);
  virtual void RenderScene();

protected:
  Mesh *meshes[NUM_MESHES];
};
