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
#include "RGBABufferedTexture.h"
#include "StencilBufferedTexture.h"
#include "FramebufferNode.h"

using namespace GraphicsCoursework;

int main()
{
  Window w("Planet", 800, 600, false);
  if (!w.HasInitialised())
    return 1;

  w.LockMouseToWindow(true);
  w.ShowOSPointer(false);

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  Vector2 winDims = w.GetScreenSize();

  FramebufferNode * screenBuffer = new FramebufferNode("screenBuffer", false);
  r.Root()->AddChild(screenBuffer);

  WindowsSystemMonitor sysMon;

  // SYSTEM MONITOR STUFF
  ShaderProgram *sysMonShader;
  TextNode *loadingNode;

  {
    Font *sysMonFont = new Font(16, 16);
    sysMonFont->LoadFromFile(TEXTUREDIR "tahoma.tga", SOIL_FLAG_COMPRESS_TO_DXT);

    sysMonShader = new ShaderProgram({ new VertexShader(SHADERDIR "coursework/BasicTextureVertex.glsl"),
      new FragmentShader(SHADERDIR "coursework/BasicTextureFragment.glsl") });

    r.Root()->AddChild(new CameraNode("sysMonCamera"));

    CameraSelectorNode *sysMonCameraSelect = new CameraSelectorNode("sysMonCameraSelect");
    screenBuffer->AddChild(sysMonCameraSelect);
    sysMonCameraSelect->SetCamera("sysMonCamera");

    auto dims = r.ParentWindow().GetScreenSize();
    r.Root()
      ->FindFirstChildByName("sysMonCameraSelect")
      ->AddChild(
        new MatrixNode("sysMonProj", "projMatrix", Matrix4::Orthographic(-1.0f, 1.0f, dims.x, 0.0f, dims.y, 0.0f)));

    r.Root()->FindFirstChildByName("sysMonProj")->AddChild(new ShaderNode("sysMonShader", sysMonShader));

    r.Root()
      ->FindFirstChildByName("sysMonShader")
      ->AddChild(new TextureNode("sysMonFontTex", { { sysMonFont, "diffuseTex", 1 } }));

    r.Root()->FindFirstChildByName("sysMonFontTex")->AddChild(new ShaderSyncNode("sysMonShaderSync"));

    TextNode *sysMonNode = new PerformanceMonitorNode("sysMonNode", sysMonFont, &sysMon);
    r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(sysMonNode);
    sysMonNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 1.0f, 0.0f)));

    loadingNode = new TextNode("loadingNode", sysMonFont, 10);
    r.Root()->FindFirstChildByName("sysMonShaderSync")->AddChild(loadingNode);
    loadingNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 2.0f, 0.0f)));
    loadingNode->SetText("Loading...");

    r.Root()->Update(w.GetTimer()->GetTimedMS());
    r.RenderScene();
  }
  // END SYSTEM MONITOR STUFF

  ITexture *tex1 = new Texture();
  tex1->LoadFromFile(TEXTUREDIR "brick.tga");

  ITexture *tex2 = new Texture();
  tex2->LoadFromFile(TEXTUREDIR "Barren Reds.jpg");

  ITexture *tex3 = new Texture();
  tex3->LoadFromFile(TEXTUREDIR "stainedglass.tga");

  ShaderProgram *shader1 = new ShaderProgram(
      {new VertexShader(SHADERDIR "PerPixelVertex.glsl"), new FragmentShader(SHADERDIR "PerPixelFragment.glsl")});

  ShaderProgram *shader2 = new ShaderProgram(
      {new VertexShader(SHADERDIR "TexVertex.glsl"), new FragmentShader(SHADERDIR "TexTranspFrag.glsl")});

  PositionableCamera *cam1 = new PositionableCamera("cam1");
  r.Root()->AddChild(cam1);
  cam1->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 1.0f, -8.0f)));
  cam1->LinearSpeed() = 0.01f;

  CameraSelectorNode *cs1 = new CameraSelectorNode("cs1");
  r.Root()->AddChild(cs1);
  cs1->SetCamera("cam1");

  auto globalTexMatrixIdentity = cs1->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));
  auto proj1 = globalTexMatrixIdentity->AddChild(
      new MatrixNode("proj1", "projMatrix", Matrix4::Perspective(1.0f, 10000.0f, winDims.x / winDims.y, 45.0f)));

  FramebufferNode * sceneBuffer = new FramebufferNode("sceneBuffer");
  proj1->AddChild(sceneBuffer);

  ITexture *cubeMapTex = new CubeMapTexture();
  cubeMapTex->LoadFromFiles({
      TEXTUREDIR "rusted_west.jpg", TEXTUREDIR "rusted_east.jpg", TEXTUREDIR "rusted_up.jpg",
      TEXTUREDIR "rusted_down.jpg", TEXTUREDIR "rusted_south.jpg", TEXTUREDIR "rusted_north.jpg",
  });
  cubeMapTex->SetRepeating(true);

  // SKYBOX
  {
    auto skyboxGLcontrol = new GenericControlNode("skyboxGLcontrol");
    sceneBuffer->AddChild(skyboxGLcontrol);
    skyboxGLcontrol->OnBind() = [](ShaderProgram *) { glDepthMask(GL_FALSE); };
    skyboxGLcontrol->OnUnBind() = [](ShaderProgram *) { glDepthMask(GL_TRUE); };

    auto skyboxShader = skyboxGLcontrol->AddChild(
        new ShaderNode("skyboxShader", new ShaderProgram({new VertexShader(SHADERDIR "SkyboxVertex.glsl"),
                                                          new FragmentShader(SHADERDIR "SkyboxFragment.glsl")})));
    auto skyboxTexture = skyboxShader->AddChild(new TextureNode("skyboxTexture", {{cubeMapTex, "skyboxTexture", 1}}));
    auto skyboxShaderSync = skyboxTexture->AddChild(new ShaderSyncNode("skyboxShaderSync"));
    auto skyboxQuadMesh = skyboxShaderSync->AddChild(new MeshNode("skyboxQuadMesh", Mesh::GenerateQuad()));
  }
  // END SKYBOX

  // LIGHTS
  Light *sun;
  Light *moon;
  {
    Vector4 sunColour(1.0f, 1.0f, 0.85f, 1.0f);
    Vector4 moonColour(0.5f, 0.6f, 1.0f, 1.0f);

    auto lightRenderShader = sceneBuffer->AddChild(
        new ShaderNode("lightRenderShader",
                       new ShaderProgram({new VertexShader(SHADERDIR "coursework/PerPixelVertex.glsl"),
                                          new FragmentShader(SHADERDIR "coursework/PlanetLightSOurceFragment.glsl")})));
    auto lightShaderSync = lightRenderShader->AddChild(new ShaderSyncNode("lightShaderSync"));

    sun = new Light("sun");
    lightShaderSync->AddChild(sun);
    r.AddPersistentDataNode(sun);
    sun->Radius() = 100.0f;
    sun->AmbientIntensity() = 0.2f;
    sun->Colour() = sunColour;
    sun->SetLocalTransformation(Matrix4::Translation(Vector3(50.0f, 50.0f, -50.0f)));

    MeshNode *sunMesh = new MeshNode("sunMesh", Mesh::GenerateSphere());
    sun->AddChild(sunMesh);
    sunMesh->GetMesh()->SetUniformColour(sunColour);

    moon = new Light("moon");
    lightShaderSync->AddChild(moon);
    r.AddPersistentDataNode(moon);
    moon->Radius() = 80.0f;
    moon->AmbientIntensity() = 0.05f;
    moon->Colour() = moonColour;
    moon->SetLocalTransformation(Matrix4::Translation(Vector3(-50.0f, 50.0f, -50.0f)));

    MeshNode *moonMesh = new MeshNode("moonMesh", Mesh::GenerateSphere());
    moon->AddChild(moonMesh);
    moonMesh->GetMesh()->SetUniformColour(moonColour);
  }
  // END LIGHTS

  sceneBuffer->AddChild(new ShaderNode("shader1", shader1));

  r.Root()->FindFirstChildByName("shader1")->AddChild(new TextureNode("texm1", {{tex1, "diffuseTex", 1}}));

  r.Root()->FindFirstChildByName("texm1")->AddChild(new ShaderSyncNode("ss1"));

  MeshNode *s0 = new MeshNode("tri1", Mesh::GenerateSphere());
  r.Root()->FindFirstChildByName("ss1")->AddChild(s0);
  s0->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 0.0f, -20.0f)));
  s0->SpecularIntensity() = 0.5f;
  s0->SpecularPower() = 100.0f;

  // HEIGHTMAP
  {
    auto texm2 = sceneBuffer->AddChild(new TextureNode("texm2", {{tex2, "diffuseTex", 1}}));
    auto shader2 = texm2->AddChild(new ShaderNode("shader2", shader1));
    auto ss2 = shader2->AddChild(new ShaderSyncNode("ss2"));

    PerlinNoise noise;
    FractalBrownianMotion fbm(noise);
    fbm.NumOctaves() = 3;
    fbm.UniformAmplitude() = 5.0f;
    fbm.Offset() = -2.5f;
    fbm.Frequency() = 15.0f;
    fbm.ZValue() = 0.8f;

    // HeightMapMesh *hmm = new HeightMapMesh(1000.0f, 1000.0f, 1000, 1000);
    HeightMapMesh *hmm = new HeightMapMesh(1000.0f, 1000.0f, 100, 100);
    hmm->SetHeightmapFromFBM(&fbm);

    MeshNode *hm = new MeshNode("hm", hmm);
    ss2->AddChild(hm);
    hm->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -2.5f, 0.0f)));
  }
  // END HEIGHTMAP

  // WATER
  {
    ITexture *waterTex = new Texture();
    waterTex->LoadFromFile(TEXTUREDIR "water_surface.jpg");
    waterTex->SetRepeating(true);

    auto waterShader = sceneBuffer->AddChild(
        new ShaderNode("waterShader", new ShaderProgram({new VertexShader(SHADERDIR "PerPixelVertex.glsl"),
                                                         new FragmentShader(SHADERDIR "ReflectFragment.glsl")})));

    auto waterTexture = waterShader->AddChild(
        new TextureNode("waterTexture", {{waterTex, "waterTexture", 1}, {cubeMapTex, "cubeTex", 2}}));
    auto waterTexMatrix = waterTexture->AddChild(
        new MatrixNode("waterTexMatrix", "textureMatrix", Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
                                                              Matrix4::Rotation(90.0f, Vector3(0.0f, 0.0f, 1.0f))));

    auto waterShaderSync = waterTexMatrix->AddChild(new ShaderSyncNode("waterShaderSync"));

    RenderableNode *waterQuad =
        (RenderableNode *)waterShaderSync->AddChild(new MeshNode("waterQuad", Mesh::GenerateQuad()));
    waterQuad->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -2.5f, 0.0f)) * Matrix4::Scale(1000.0f));
    waterQuad->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
    waterQuad->SpecularPower() = 2.0f;
  }
  // END WATER

  // FBO TEXTURES
  ITexture * bufferDepthTex = new StencilBufferedTexture(winDims.x, winDims.y);
  ITexture * bufferColourTex1 = new RGBABufferedTexture(winDims.x, winDims.y);
  ITexture * bufferColourTex2 = new RGBABufferedTexture(winDims.x, winDims.y);

  {
    glBindFramebuffer(GL_FRAMEBUFFER, sceneBuffer->Buffer());

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex->GetTextureID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex->GetTextureID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex1->GetTextureID(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      return 2;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  // POST PROCESSING
  {
    FramebufferNode * processBuffer = new FramebufferNode("processBuffer");
    r.Root()->AddChild(processBuffer);

    GenericControlNode * depthDisable = new GenericControlNode("depthDisable");
    processBuffer->AddChild(depthDisable);
    depthDisable->OnBind() = [](ShaderProgram *) {
      glDisable(GL_DEPTH_TEST);
    };
    depthDisable->OnUnBind() = [](ShaderProgram *) {
      glEnable(GL_DEPTH_TEST);
    };

    auto shader = depthDisable->AddChild(new ShaderNode("shader",
      new ShaderProgram({new VertexShader(SHADERDIR "TexturedVertex.glsl"), new FragmentShader(SHADERDIR "ProcessFragment.glsl")})));

    auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

    auto proj = globalTexMatrixIdentity->AddChild(new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

    GenericControlNode * control1 = new GenericControlNode("control1");
    view->AddChild(control1);
    control1->OnBind() = [bufferColourTex2](ShaderProgram *s)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex2->GetTextureID(), 0);
    };
    auto texture1 = control1->AddChild(new TextureNode("texture1", { {bufferColourTex1, "diffuseTex", 1} }));
    auto sync1 = texture1->AddChild(new ShaderSyncNode("sync1"));
    RenderableNode *quad1 = new MeshNode("quad1", Mesh::GenerateQuad());
    sync1->AddChild(quad1);
  }

  // POST PROCESSING PRESENTATION
  {
    auto shader = screenBuffer->AddChild(new ShaderNode("shader",
      new ShaderProgram({ new VertexShader(SHADERDIR "TexturedVertex.glsl"), new FragmentShader(SHADERDIR "ProcessFragment.glsl") })));

    auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

    auto proj = globalTexMatrixIdentity->AddChild(new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

    auto texture = view->AddChild(new TextureNode("texture", { { bufferColourTex1, "diffuseTex", 1 } }));
    auto sync = texture->AddChild(new ShaderSyncNode("sync"));
    auto quad = sync->AddChild(new MeshNode("quad", Mesh::GenerateQuad()));
  }

  // Output scene tree and mark loading as completed
  std::cout << r;
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