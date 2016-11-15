#pragma once

#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void RenderScene();
  virtual void UpdateScene(float msec);

protected:
  void DrawHeightmap();
  void DrawWater();
  void DrawSkybox();

  Shader *lightShader;
  Shader *reflectShader;
  Shader *skyboxShader;

  HeightMap *heightMap;
  Mesh *quad;

  Light *light;
  Camera *camera;

  GLuint cubeMap;

  float waterRotate;
};
