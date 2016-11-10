#pragma comment(lib, "nclgl.lib")

#include <iostream>

#include "../../nclgl/Window.h"

#include "MeshNode.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Shaders.h"

using namespace GraphicsCoursework;

int main()
{
  Window w("Planet", 800, 600, false);
  if (!w.HasInitialised())
    return 1;

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  Mesh *tri = Mesh::GenerateTriangle();
  tri->SetTexture(
      SOIL_load_OGL_texture(TEXTUREDIR "brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

  ShaderProgram *sp1 = new ShaderProgram({new VertexShader(SHADERDIR "TexturedVertex.glsl"),
                                          new FragmentShader(SHADERDIR "TexturedFragment.glsl")});

  r.Root()->Child("shader1")->AddChild(new MeshNode("tri1", tri, sp1));

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    r.RenderScene();
  }

  return 0;
}