/** @file */

#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/Window.h"

#include "CameraSelectorNode.h"
#include "CubeMapTexture.h"
#include "Font.h"
#include "FractalBrownianMotion.h"
#include "GenericControlNode.h"
#include "HeightMapMesh.h"
#include "Light.h"
#include "MatrixNode.h"
#include "MeshNode.h"
#include "PerformanceMonitorNode.h"
#include "PerlinNoise.h"
#include "PositionableCamera.h"
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
  Window w("Planet", 640, 480, false);
  if (!w.HasInitialised())
    return 1;

  w.LockMouseToWindow(true);
  w.ShowOSPointer(false);

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  WindowsSystemMonitor sysMon;

  ITexture *tex1 = new Texture();
  tex1->LoadFromFile(TEXTUREDIR "brick.tga");

  ITexture *tex2 = new Texture();
  tex2->LoadFromFile(TEXTUREDIR "Barren Reds.jpg");

  ITexture *tex3 = new Texture();
  tex3->LoadFromFile(TEXTUREDIR "stainedglass.tga");

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
      new MatrixNode("proj1", "projMatrix", Matrix4::Perspective(1.0f, 10000.0f, 800.0f / 600.0f, 45.0f)));

  // SKYBOX
  {
    ITexture *cubeMapTex = new CubeMapTexture();
    cubeMapTex->LoadFromFiles({
        TEXTUREDIR "rusted_west.jpg", TEXTUREDIR "rusted_east.jpg", TEXTUREDIR "rusted_up.jpg",
        TEXTUREDIR "rusted_down.jpg", TEXTUREDIR "rusted_south.jpg", TEXTUREDIR "rusted_north.jpg",
    });

    r.SetTextureRepeating(cubeMapTex->GetTextureID(), true);

    auto skyboxGLcontrol = new GenericControlNode("skyboxGLcontrol");
    skyboxGLcontrol->OnBind() = [](ShaderProgram *) { glDepthMask(GL_FALSE); };
    skyboxGLcontrol->OnUnBind() = [](ShaderProgram *) { glDepthMask(GL_TRUE); };
    r.Root()->FindFirstChildByName("proj1")->AddChild(skyboxGLcontrol);

    auto skyboxShader = skyboxGLcontrol->AddChild(new ShaderNode(
        "skyboxShader", new ShaderProgram({new VertexShader(SHADERDIR "SkyboxVertex.glsl"),
                                           new FragmentShader(SHADERDIR "SkyboxFragment.glsl")})));
    auto skyboxTexture = skyboxShader->AddChild(
        new TextureNode("skyboxTexture", {{cubeMapTex, "skyboxTexture", 1}}));
    auto skyboxShaderSync = skyboxTexture->AddChild(new ShaderSyncNode("skyboxShaderSync"));
    auto skyboxQuadMesh =
        skyboxShaderSync->AddChild(new MeshNode("skyboxQuadMesh", Mesh::GenerateQuad()));
  }
  // END SKYBOX

  // SYSTEM MONITOR STUFF
  ShaderProgram *sysMonShader;
  TextNode *loadingNode;
  {
    Font *sysMonFont = new Font(16, 16);
    sysMonFont->LoadFromFile(TEXTUREDIR "tahoma.tga", SOIL_FLAG_COMPRESS_TO_DXT);

    sysMonShader =
        new ShaderProgram({new VertexShader(SHADERDIR "coursework/BasicTextureVertex.glsl"),
                           new FragmentShader(SHADERDIR "coursework/BasicTextureFragment.glsl")});

    r.Root()->AddChild(new CameraNode("sysMonCamera"));

    CameraSelectorNode *sysMonCameraSelect = new CameraSelectorNode("sysMonCameraSelect");
    r.Root()->AddChild(sysMonCameraSelect);
    sysMonCameraSelect->SetCamera("sysMonCamera");

    auto dims = r.ParentWindow().GetScreenSize();
    r.Root()
        ->FindFirstChildByName("sysMonCameraSelect")
        ->AddChild(new MatrixNode("sysMonProj", "projMatrix",
                                  Matrix4::Orthographic(-1.0f, 1.0f, dims.x, 0.0f, dims.y, 0.0f)));

    r.Root()
        ->FindFirstChildByName("sysMonProj")
        ->AddChild(new ShaderNode("sysMonShader", sysMonShader));

    r.Root()
        ->FindFirstChildByName("sysMonShader")
        ->AddChild(new TextureNode("sysMonFontTex", {{sysMonFont, "diffuseTex", 1}}));

    r.Root()
        ->FindFirstChildByName("sysMonFontTex")
        ->AddChild(new ShaderSyncNode("sysMonShaderSync"));

    TextNode *sysMonNode = new PerformanceMonitorNode("sysMonNode", sysMonFont, &sysMon);
    r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(sysMonNode);
    sysMonNode->SetLocalTransformation(Matrix4::Scale(16.0f) *
                                       Matrix4::Translation(Vector3(0.0f, 1.0f, 0.0f)));

    loadingNode = new TextNode("loadingNode", sysMonFont, 10);
    r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(loadingNode);
    loadingNode->SetLocalTransformation(Matrix4::Scale(16.0f) *
                                        Matrix4::Translation(Vector3(0.0f, 2.0f, 0.0f)));
    loadingNode->SetText("Loading...");

    r.Root()->Update(w.GetTimer()->GetTimedMS());
    r.RenderScene();
  }
  // END SYSTEM MONITOR STUFF

  // LIGHTS
  Light *sun;
  Light *moon;
  {
    // TODO: don't use sysMonShader
    SceneNode *lightRenderShader = r.Root()->FindFirstChildByName("proj1")->AddChild(
        new ShaderNode("lightRenderShader", sysMonShader));

    sun = new Light("sun");
    lightRenderShader->AddChild(sun);
    r.AddPersistentDataNode(sun);
    sun->Radius() = 100.0f;
    sun->AmbientIntensity() = 0.2f;
    sun->Colour() = Vector4(1.0f, 1.0f, 0.7f, 1.0f);
    sun->SetLocalTransformation(Matrix4::Translation(Vector3(50.0f, 50.0f, -50.0f)));

    MeshNode *sunMesh = new MeshNode("sunMesh", Mesh::GenerateSphere());
    sun->AddChild(new TextureNode("sunTexture", {{tex3, "diffuseTex", 1}}))
        ->AddChild(new ShaderSyncNode("sunShaderSync"))
        ->AddChild(sunMesh);

    moon = new Light("moon");
    lightRenderShader->AddChild(moon);
    r.AddPersistentDataNode(moon);
    moon->Radius() = 80.0f;
    moon->AmbientIntensity() = 0.05f;
    moon->Colour() = Vector4(0.5f, 0.6f, 1.0f, 1.0f);
    moon->SetLocalTransformation(Matrix4::Translation(Vector3(-50.0f, 50.0f, -50.0f)));

    MeshNode *moonMesh = new MeshNode("moonMesh", Mesh::GenerateSphere());
    moon->AddChild(new TextureNode("moonTexture", {{tex3, "diffuseTex", 1}}))
        ->AddChild(new ShaderSyncNode("moonShaderSync"))
        ->AddChild(moonMesh);
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

  // HEIGHTMAP
  {
    PerlinNoise noise;
    FractalBrownianMotion fbm(noise);
    fbm.NumOctaves() = 3;
    fbm.UniformAmplitude() = -5.0f;
    fbm.Frequency() = 15.0f;
    fbm.ZValue() = 0.8f;

    // HeightMapMesh *hmm = new HeightMapMesh(1000.0f, 1000.0f, 1000, 1000);
    HeightMapMesh *hmm = new HeightMapMesh(1000.0f, 1000.0f, 100, 100);
    hmm->SetHeightmapFromFBM(&fbm);

    MeshNode *hm = new MeshNode("hm", hmm);
    r.Root()->FindFirstChildByName("ss2")->AddChild(hm);
    hm->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -2.0f, 0.0f)));
  }
  // END HEIGHTMAP

  // WATER
  {
    ITexture *waterTex = new Texture();
    waterTex->LoadFromFile(TEXTUREDIR "stainedglass.tga");

    r.SetTextureRepeating(waterTex->GetTextureID(), true);

    auto waterShader = r.Root()->FindFirstChildByName("proj1")->AddChild(new ShaderNode(
        "waterShader", new ShaderProgram({new VertexShader(SHADERDIR "PerPixelVertex.glsl"),
                                          new FragmentShader(SHADERDIR "ReflectFragment.glsl")})));
    auto waterTexture =
        waterShader->AddChild(new TextureNode("waterTexture", {{waterTex, "waterTexture", 1}}));
    auto waterTexMatrix = waterTexture->AddChild(new MatrixNode("waterTexMatrix", "textureMatrix", Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(90.0f, Vector3(0.0f, 0.0f, 1.0f))));
    auto waterShaderSync = waterTexMatrix->AddChild(new ShaderSyncNode("waterShaderSync"));
    auto waterQuad = waterShaderSync->AddChild(new MeshNode("waterQuad", Mesh::GenerateQuad()));
    waterQuad->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
  }
  // END WATER

  std::cout << r << '\n';
  loadingNode->SetActive(false);

  GameTimer sysMonTimer;

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
      sun->SetActive(!sun->IsActive(), true);

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
      moon->SetActive(!moon->IsActive(), true);

    if (sysMonTimer.GetTimedMS(false) > 1000.0f)
      sysMon.Update(sysMonTimer.GetTimedMS());

    r.Root()->Update(w.GetTimer()->GetTimedMS());
    r.RenderScene();
  }

  delete r.Root();

  return 0;
}