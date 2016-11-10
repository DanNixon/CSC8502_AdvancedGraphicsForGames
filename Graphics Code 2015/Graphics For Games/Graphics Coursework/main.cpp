/** @file */

#pragma comment(lib, "nclgl.lib")

#include <iostream>

#include "../../nclgl/Window.h"

#include "MeshNode.h"
#include "Renderer.h"
#include "ShaderNode.h"
#include "ShaderProgram.h"
#include "Shaders.h"
#include "ProjectionNode.h"
#include "CameraNode.h"
#include "CameraSelectorNode.h"
#include "Texture.h"
#include "TextureNode.h"

using namespace GraphicsCoursework;

int main()
{
  Window w("Planet", 800, 600, false);
  if (!w.HasInitialised())
    return 1;

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  Texture * tex1 = new Texture();
  tex1->LoadFromFile(TEXTUREDIR "brick.tga");

  Texture * tex2 = new Texture();
  tex2->LoadFromFile(TEXTUREDIR "Barren Reds.jpg");

  r.Root()->AddChild(new CameraNode("cam1"));
  r.Root()->FindFirstChildByName("cam1")->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 1.0f, -8.0f)));

  CameraSelectorNode * cs1 = new CameraSelectorNode("cs1");
  r.Root()->AddChild(cs1);
  cs1->SetCamera("cam1");
  cs1->SetActive(true);

  r.Root()->Child("cs1")->AddChild(new ProjectionNode("proj1", Matrix4::Perspective(1.0f, 10000.0f, 800.0f / 600.0f, 110.0f)));

  r.Root()->Child("cs1")->Child("proj1")->AddChild(new ShaderNode(
    "shader1", new ShaderProgram({ new VertexShader(SHADERDIR "TexVertex.glsl"),
      new FragmentShader(SHADERDIR "TexFrag.glsl") })));

  r.Root()->Child("cs1")->Child("proj1")->Child("shader1")->AddChild(new TextureNode("texm1", { {tex1, "diffuseTex", 1}, { tex2, "diffuseTex2", 2 } }));

  MeshNode *tri1 = new MeshNode("tri1", Mesh::GenerateTriangle());
  r.Root()->Child("cs1")->Child("proj1")->Child("shader1")->Child("texm1")->AddChild(tri1);
  tri1->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 0.0f, -10.0f)));

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    r.RenderScene();
  }

  return 0;
}