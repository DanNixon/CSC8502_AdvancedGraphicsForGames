/** @file */

#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/Window.h"

#include "CameraSelectorNode.h"
#include "CubeMapTexture.h"
#include "Font.h"
#include "FractalBrownianMotion.h"
#include "HeightMapMesh.h"
#include "Light.h"
#include "MeshNode.h"
#include "PerformanceMonitorNode.h"
#include "PerlinNoise.h"
#include "PositionableCamera.h"
#include "ProjectionNode.h"
#include "Renderer.h"
#include "ShaderNode.h"
#include "ShaderProgram.h"
#include "ShaderSyncNode.h"
#include "Shaders.h"
#include "Texture.h"
#include "TextureNode.h"
#include "WindowsSystemMonitor.h"

using namespace GraphicsCoursework;

int main()
{
  Window w("Planet", 1440, 720, false);
  if (!w.HasInitialised())
    return 1;

  w.LockMouseToWindow(true);
  w.ShowOSPointer(false);

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  WindowsSystemMonitor sysMon;

  // SYSTEM MONITOR STUFF

  Font *sysMonFont = new Font(16, 16);
  sysMonFont->LoadFromFile(TEXTUREDIR "tahoma.tga", SOIL_FLAG_COMPRESS_TO_DXT);

  ShaderProgram *sysMonShader =
      new ShaderProgram({new VertexShader(SHADERDIR "coursework/BasicTextureVertex.glsl"),
                         new FragmentShader(SHADERDIR "coursework/BasicTextureFragment.glsl")});

  r.Root()->AddChild(new CameraNode("sysMonCamera"));

  CameraSelectorNode *sysMonCameraSelect = new CameraSelectorNode("sysMonCameraSelect");
  r.Root()->AddChild(sysMonCameraSelect);
  sysMonCameraSelect->SetCamera("sysMonCamera");

  auto dims = r.ParentWindow().GetScreenSize();
  r.Root()
      ->FindFirstChildByName("sysMonCameraSelect")
      ->AddChild(new ProjectionNode(
          "sysMonProj", Matrix4::Orthographic(-1.0f, 1.0f, dims.x, 0.0f, dims.y, 0.0f)));

  r.Root()
      ->FindFirstChildByName("sysMonProj")
      ->AddChild(new ShaderNode("sysMonShader", sysMonShader));

  r.Root()
      ->FindFirstChildByName("sysMonShader")
      ->AddChild(new TextureNode("sysMonFontTex", {{sysMonFont, "diffuseTex", 1}}));

  r.Root()->FindFirstChildByName("sysMonFontTex")->AddChild(new ShaderSyncNode("sysMonShaderSync"));

  TextNode *sysMonNode = new PerformanceMonitorNode("sysMonNode", sysMonFont, &sysMon);
  r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(sysMonNode);
  sysMonNode->SetLocalTransformation(Matrix4::Scale(16.0f) *
                                     Matrix4::Translation(Vector3(0.0f, 1.0f, 0.0f)));

  TextNode *loadingNode = new TextNode("loadingNode", sysMonFont, 10);
  r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(loadingNode);
  loadingNode->SetLocalTransformation(Matrix4::Scale(16.0f) *
                                      Matrix4::Translation(Vector3(0.0f, 2.0f, 0.0f)));
  loadingNode->SetText("Loading...");

  r.Root()->Update(w.GetTimer()->GetTimedMS());
  r.RenderScene();

  // END SYSTEM MONITOR STUFF

  ITexture *tex1 = new Texture();
  tex1->LoadFromFile(TEXTUREDIR "brick.tga");

  ITexture *tex2 = new Texture();
  tex2->LoadFromFile(TEXTUREDIR "Barren Reds.jpg");

  ITexture *tex3 = new Texture();
  tex3->LoadFromFile(TEXTUREDIR "stainedglass.tga");

  ITexture *cubeMapTex = new CubeMapTexture();
  cubeMapTex->LoadFromFiles({
      TEXTUREDIR "rusted_west.jpg", TEXTUREDIR "rusted_east.jpg", TEXTUREDIR "rusted_up.jpg",
      TEXTUREDIR "rusted_down.jpg", TEXTUREDIR "rusted_south.jpg", TEXTUREDIR "rusted_north.jpg",
  });

  ShaderProgram *shader1 =
      new ShaderProgram({new VertexShader(SHADERDIR "PerPixelVertex.glsl"),
                         new FragmentShader(SHADERDIR "PerPixelFragment.glsl")});

  ShaderProgram *shader2 = new ShaderProgram({new VertexShader(SHADERDIR "TexVertex.glsl"),
                                              new FragmentShader(SHADERDIR "TexTranspFrag.glsl")});

  PositionableCamera *cam1 = new PositionableCamera("cam1");
  r.Root()->AddChild(cam1);
  cam1->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 1.0f, -8.0f)));
  cam1->LinearSpeed() = 0.01f;

  CameraSelectorNode *cs1 = new CameraSelectorNode("cs1");
  r.Root()->AddChild(cs1);
  cs1->SetCamera("cam1");

  r.Root()->FindFirstChildByName("cs1")->AddChild(
      new ProjectionNode("proj1", Matrix4::Perspective(1.0f, 10000.0f, 800.0f / 600.0f, 45.0f)));

  // LIGHTS
  Light *sun;
  Light *moon;

  {
    // TODO: don't use sysMonShader
    SceneNode * lightRenderShader = r.Root()->FindFirstChildByName("proj1")->AddChild(new ShaderNode("lightRenderShader", sysMonShader));

    sun = new Light("sun");
    lightRenderShader->AddChild(sun);
    r.AddPersistentDataNode(sun);
    sun->Radius() = 100.0f;
    sun->AmbientIntensity() = 0.2f;
    sun->Colour() = Vector4(1.0f, 1.0f, 0.7f, 1.0f);
    sun->SetLocalTransformation(Matrix4::Translation(Vector3(50.0f, 50.0f, -50.0f)));

    MeshNode *sunMesh = new MeshNode("sunMesh", Mesh::GenerateSphere());
    sun->AddChild(new TextureNode("sunTexture", {{ tex3, "diffuseTex", 1 }}))->AddChild(new ShaderSyncNode("sunShaderSync"))->AddChild(sunMesh);

    moon = new Light("moon");
    lightRenderShader->AddChild(moon);
    r.AddPersistentDataNode(moon);
    moon->Radius() = 80.0f;
    moon->AmbientIntensity() = 0.05f;
    moon->Colour() = Vector4(0.5f, 0.6f, 1.0f, 1.0f);
    moon->SetLocalTransformation(Matrix4::Translation(Vector3(-50.0f, 50.0f, -50.0f)));

    MeshNode *moonMesh = new MeshNode("moonMesh", Mesh::GenerateSphere());
    moon->AddChild(new TextureNode("moonTexture", { { tex3, "diffuseTex", 1 } }))->AddChild(new ShaderSyncNode("moonShaderSync"))->AddChild(moonMesh);
  }
  // END LIGHTS

  r.Root()->FindFirstChildByName("proj1")->AddChild(new ShaderNode("shader1", shader1));

  r.Root()->FindFirstChildByName("shader1")->AddChild(
      new TextureNode("texm1", {{tex1, "diffuseTex", 1}}));

  r.Root()->FindFirstChildByName("texm1")->AddChild(new ShaderSyncNode("ss1"));

  MeshNode *s0 = new MeshNode("tri1", Mesh::GenerateSphere());
  r.Root()->FindFirstChildByName("ss1")->AddChild(s0);
  s0->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 0.0f, -20.0f)));
  s0->SpecularIntensity() = 0.5f;
  s0->SpecularPower() = 100.0f;

  s0->AddChild(new TextureNode("texm2", {{tex2, "diffuseTex", 1}}));
  r.Root()->FindFirstChildByName("texm2")->AddChild(new ShaderNode("shader2", shader1));
  r.Root()->FindFirstChildByName("shader2")->AddChild(new ShaderSyncNode("ss2"));

#if 0
  MeshNode *s1 = new MeshNode("s1", Mesh::GenerateQuad(), true);
  r.Root()->FindFirstChildByName("ss2")->AddChild(s1);
  s1->SetLocalTransformation(Matrix4::Translation(Vector3(3.0f, 3.0f, 15.0f)));

  MeshNode *s2 = new MeshNode("s2", Mesh::GenerateQuad(), true);
  r.Root()->FindFirstChildByName("ss2")->AddChild(s2);
  s2->SetLocalTransformation(Matrix4::Translation(Vector3(2.0f, 2.0f, 10.0f)));
  s2->SetLocalRotation(Matrix4::Rotation(30.0f, Vector3(1, 0, 0)));

  MeshNode *s3 = new MeshNode("s3", Mesh::GenerateQuad(), true);
  r.Root()->FindFirstChildByName("ss2")->AddChild(s3);
  s3->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 1.0f, 5.0f)));
  s3->SpecularPower() = 1000.0f;
#endif

  // Heightmap generation
  PerlinNoise noise;
  FractalBrownianMotion fbm(noise);
  fbm.NumOctaves() = 3;
  fbm.UniformAmplitude() = -5.0f;
  fbm.Frequency() = 15.0f;
  fbm.ZValue() = 0.8f;

  HeightMapMesh *hmm = new HeightMapMesh(1000.0f, 1000.0f, 1000, 1000);
  //hmm->SetHeightmapFromFBM(&fbm);

  MeshNode *hm = new MeshNode("hm", hmm);
  r.Root()->FindFirstChildByName("ss2")->AddChild(hm);
  hm->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -2.0f, 0.0f)));

  std::cout << r << '\n';
  loadingNode->SetActive(false);

  GameTimer sysMonTimer;

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
      sun->SetActive(!sun->IsActive());

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
      moon->SetActive(!moon->IsActive());

    if (sysMonTimer.GetTimedMS(false) > 1000.0f)
      sysMon.Update(sysMonTimer.GetTimedMS());

    r.Root()->Update(w.GetTimer()->GetTimedMS());
    r.RenderScene();
  }

  delete tex1;
  delete tex2;
  delete shader1;

  return 0;
}