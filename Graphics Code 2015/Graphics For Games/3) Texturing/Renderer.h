#pragma once

#include "../nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void RenderScene();

  void UpdateTextureMatrix(float rotation);
  void ToggleRepeating();
  void ToggleFiltering();
  void OffsetBlendFactor(float offset);

protected:
  Mesh *mesh;

  bool filtering;
  bool repeating;
  float blendFactor;
};