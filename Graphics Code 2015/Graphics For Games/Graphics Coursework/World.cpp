/** @file */

#include "World.h"

#include <../nclgl/Window.h>

#include "CameraNode.h"
#include "CameraSelectorNode.h"
#include "CubeMapTexture.h"
#include "DepthStencilTexture.h"
#include "Font.h"
#include "FractalBrownianMotion.h"
#include "FramebufferNode.h"
#include "GenericControlNode.h"
#include "HeightmapTexture.h"
#include "MatrixNode.h"
#include "PerformanceMonitorNode.h"
#include "PerlinNoise.h"
#include "PointLight.h"
#include "PositionableCamera.h"
#include "RGBATexture.h"
#include "Renderer.h"
#include "SceneNode.h"
#include "ShaderNode.h"
#include "ShaderProgram.h"
#include "ShaderSyncNode.h"
#include "Shaders.h"
#include "SpotLight.h"
#include "TextureNode.h"
#include "TransparentRenderingNode.h"
#include "SubTreeNode.h"

#define CW_SHADER_DIR SHADERDIR"coursework/"
#define CW_TEXTURE_DIR TEXTUREDIR"coursework/"
#define CW_MESH_DIR MESHDIR"coursework/"

namespace GraphicsCoursework
{
World::World(Renderer &renderer)
    : m_renderer(renderer)
{
  m_state.screenDims = m_renderer.ParentWindow().GetScreenSize();
  m_state.screenBuffer = new FramebufferNode("screenBuffer", false);
  m_renderer.Root()->AddChild(m_state.screenBuffer);
}

void World::BuildLoadingScreen(SceneNode *root)
{
  Font *sysMonFont = new Font(16, 16);
  sysMonFont->LoadFromFile(CW_TEXTURE_DIR "tahoma.tga", SOIL_FLAG_COMPRESS_TO_DXT);

  root->AddChild(new CameraNode("sysMonCamera"));

  CameraSelectorNode *sysMonCameraSelect = new CameraSelectorNode("sysMonCameraSelect");
  m_state.screenBuffer->AddChild(sysMonCameraSelect);
  sysMonCameraSelect->SetCamera("sysMonCamera");

  auto sysMonProj = sysMonCameraSelect->AddChild(
      new MatrixNode("sysMonProj", "projMatrix",
                     Matrix4::Orthographic(-1.0f, 1.0f, m_state.screenDims.x, 0.0f, m_state.screenDims.y, 0.0f)));

  auto sysMonShader = sysMonProj->AddChild(new ShaderNode(
      "sysMonShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "BasicTextureVertex.glsl"),
                                         new FragmentShader(CW_SHADER_DIR "BasicTextureFragment.glsl")})));

  auto sysMonFontTex = sysMonShader->AddChild(new TextureNode("sysMonFontTex", {{sysMonFont, "diffuseTex", 1}}));

  auto sysMonShaderSync = sysMonFontTex->AddChild(new ShaderSyncNode("sysMonShaderSync"));

  TextNode *sysMonNode = new PerformanceMonitorNode("sysMonNode", sysMonFont, &m_state.sysMonitor);
  sysMonShaderSync->AddChild(sysMonNode);
  sysMonNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 1.0f, 0.0f)));

  m_state.loadingNode = sysMonShaderSync->AddChild(new TextNode("loadingNode", sysMonFont, "Loading..."));
  m_state.loadingNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 2.0f, 0.0f)));
}

void World::Build(SceneNode *root)
{
  // Textures
  ITexture *brickTexture = new Texture();
  //brickTexture->LoadFromFile(CW_TEXTURE_DIR "brick.tga");
  brickTexture->LoadFromFile(TEXTUREDIR "stainedglass.tga");

  // Main camera and view
  PositionableCamera *playerCamera = new PositionableCamera("playerCamera");
  root->AddChild(playerCamera);
  playerCamera->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 20.0f, -8.0f)));
  playerCamera->LinearSpeed() = 0.01f;

  CameraSelectorNode *playerCameraSelect = new CameraSelectorNode("playerCameraSelect");
  root->AddChild(playerCameraSelect);
  playerCameraSelect->SetCamera(playerCamera);

  auto globalTexMatrixIdentity =
      playerCameraSelect->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));
  auto projection = globalTexMatrixIdentity->AddChild(new MatrixNode(
      "projection", "projMatrix", Matrix4::Perspective(1.0f, 10000.0f, m_state.screenDims.x / m_state.screenDims.y, 45.0f)));

  // Scene FBO
  FramebufferNode *sceneBuffer = new FramebufferNode("sceneBuffer");
  projection->AddChild(sceneBuffer);
  auto globalFog = sceneBuffer->AddChild(new GenericControlNode("globalFog", [](ShaderProgram * s) {
    glUniform3f(glGetUniformLocation(s->Program(), "fogBaseColour"), 0.4f, 0.4f, 0.55f);
    glUniform1f(glGetUniformLocation(s->Program(), "fogExp"), 0.0005f);
    glUniform1f(glGetUniformLocation(s->Program(), "fogAtten"), 0.4f);
  }));
  auto globalTransp = globalFog->AddChild(new TransparentRenderingNode("globalTransp"));

  ITexture *bufferDepthTex = new DepthStencilTexture(m_state.screenDims.x, m_state.screenDims.y);
  ITexture *bufferColourTex1 = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);
  ITexture *bufferColourTex2 = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);

  sceneBuffer->BindTexture(GL_DEPTH_ATTACHMENT, bufferDepthTex);
  sceneBuffer->BindTexture(GL_STENCIL_ATTACHMENT, bufferDepthTex);
  sceneBuffer->BindTexture(GL_COLOR_ATTACHMENT0, bufferColourTex1);

  ITexture *cubeMapTex = new CubeMapTexture();
  cubeMapTex->LoadFromFiles({
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_px.jpg",
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_mx.jpg",
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_py.jpg",
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_my.jpg",
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_pz.jpg",
    CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_mz.jpg"
  });
  //cubeMapTex->SetRepeating(true);

  // SKYBOX
  {
    auto skyboxGLcontrol =
      globalTransp->AddChild(new GenericControlNode("skyboxGLcontrol", [](ShaderProgram *) { glDepthMask(GL_FALSE); },
                                                     [](ShaderProgram *) { glDepthMask(GL_TRUE); }));

    auto skyboxShader = skyboxGLcontrol->AddChild(new ShaderNode(
        "skyboxShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "SkyboxVertex.glsl"),
                                           new FragmentShader(CW_SHADER_DIR "SkyboxFragment.glsl")})));
    auto skyboxTexture = skyboxShader->AddChild(new TextureNode("skyboxTexture", {{cubeMapTex, "skyboxTexture", 1}}));
    auto skyboxShaderSync = skyboxTexture->AddChild(new ShaderSyncNode("skyboxShaderSync"));
    RenderableNode * skyboxQuadMesh = (RenderableNode *) skyboxShaderSync->AddChild(new MeshNode("skyboxQuadMesh", Mesh::GenerateQuad()));
    skyboxQuadMesh->SetBoundingSphereRadius(-1.0f);
  }

  // LIGHTS
  {
    Vector4 sunColour(1.0f, 1.0f, 0.75f, 1.0f);
    Vector4 moonColour(0.5f, 0.6f, 1.0f, 1.0f);

    auto lightRenderShader = globalTransp->AddChild(
        new ShaderNode("lightRenderShader",
                       new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                          new FragmentShader(CW_SHADER_DIR "PlanetLightSourceFragment.glsl")})));
    auto lightShaderSync = lightRenderShader->AddChild(new ShaderSyncNode("lightShaderSync"));

    m_state.sun = new PointLight("sun");
    lightShaderSync->AddChild(m_state.sun);
    m_renderer.AddPersistentDataNode(m_state.sun);
    m_state.sun->Reach() = m_state.worldBounds * 1.8f;
    m_state.sun->AmbientIntensity() = 0.3f;
    m_state.sun->Colour() = sunColour;

    MeshNode *sunMesh = new MeshNode("sunMesh", Mesh::GenerateSphere());
    m_state.sun->AddChild(sunMesh);
    sunMesh->GetMesh()->SetUniformColour(sunColour);
    sunMesh->SetBoundingSphereRadius(12.5f);

    m_state.moon = new PointLight("moon");
    lightShaderSync->AddChild(m_state.moon);
    m_renderer.AddPersistentDataNode(m_state.moon);
    m_state.moon->Reach() = m_state.worldBounds * 1.2f;
    m_state.moon->AmbientIntensity() = 0.3f;
    m_state.moon->Colour() = moonColour;

    MeshNode *moonMesh = new MeshNode("moonMesh", Mesh::GenerateSphere());
    m_state.moon->AddChild(moonMesh);
    moonMesh->GetMesh()->SetUniformColour(moonColour);
    moonMesh->SetBoundingSphereRadius(5.0f);
  }

  // PLAYER LIGHTS
  {
    Matrix4 playerLightTransform = Matrix4::Translation(Vector3(0.0f, -0.5f, 0.0f));

    m_state.lantern = new PointLight("playerLantern");
    playerCamera->AddChild(m_state.lantern);
    m_renderer.AddPersistentDataNode(m_state.lantern);
    m_state.lantern->SetActive(false);
    m_state.lantern->SetLocalTransformation(playerLightTransform);
    m_state.lantern->Colour() = Vector4(1.0f, 0.95f, 0.75f, 0.9f);
    m_state.lantern->AmbientIntensity() = 0.02f;
    m_state.lantern->Reach() = 50.0f;

    m_state.flashlight = new SpotLight("playerFlashlight");
    playerCamera->AddChild(m_state.flashlight);
    m_renderer.AddPersistentDataNode(m_state.flashlight);
    m_state.flashlight->SetActive(false);
    m_state.flashlight->SetLocalTransformation(playerLightTransform);
    m_state.flashlight->Colour() = Vector4(0.95f, 0.95f, 1.0f, 1.0f);
    m_state.flashlight->Reach() = 30.0f;
  }

  // HEIGHTMAP
  {
    // Height generation
    PerlinNoise noise;
    FractalBrownianMotion fbm(noise);
    fbm.NumOctaves() = 5;
    fbm.Frequency() = 6.0f;
    fbm.Lacunarity() = 20.0f;
    fbm.Persistence() = 0.2f;
    fbm.ZValue() = 0.8f;

    HeightmapTexture *heightmapTexture = new HeightmapTexture(512, 512);
    heightmapTexture->SetFromFBM(&fbm);
    heightmapTexture->SetRepeating(true);

    ITexture * heightmapMultTex = new Texture();
    heightmapMultTex->LoadFromFile(CW_TEXTURE_DIR "radial_grad.tga");

    // Textures
    ITexture * sandTex = new Texture();
    sandTex->LoadFromFile(CW_TEXTURE_DIR "sand.jpg");
    sandTex->SetRepeating(true);

    ITexture * grassTex = new Texture();
    grassTex->LoadFromFile(CW_TEXTURE_DIR "grass.jpg");
    grassTex->SetRepeating(true);

    ITexture * rockTex = new Texture();
    rockTex->LoadFromFile(CW_TEXTURE_DIR "rock.jpg");
    rockTex->SetRepeating(true);

    // Scene subtree
    auto terrainTextures = globalTransp->AddChild(new TextureNode(
        "terrainTextures", {
          { heightmapTexture, "heightmapTex", 1 },
          { heightmapMultTex, "heightmapMultTex", 2 },
          { sandTex, "levelTex[0]", 3 },
          { grassTex, "levelTex[1]", 4 },
          { rockTex, "levelTex[2]", 5 },
        }));

    auto terrainShader = terrainTextures->AddChild(
        new ShaderNode("terrainShader",
                       new ShaderProgram({new VertexShader(CW_SHADER_DIR "HeightmapVertex.glsl"),
                                          new FragmentShader(CW_SHADER_DIR "HeightmapFragment.glsl"),
                                          new TesselationControlShader(CW_SHADER_DIR "HeightmapTCS.glsl"),
                                          new TesselationEvaluationShader(CW_SHADER_DIR "HeightmapTES.glsl")})));

    auto terrainControlNode = terrainShader->AddChild(
        new GenericControlNode("terrainControlNode", [](ShaderProgram *) { glPatchParameteri(GL_PATCH_VERTICES, 4); }));

    auto terrainTextureMatrix = terrainControlNode->AddChild(new MatrixNode("terrainTextureMatrix", "textureMatrix", Matrix4::Scale(100.0f)));

    auto terrainShaderSync = terrainTextureMatrix->AddChild(new ShaderSyncNode("terrainShaderSync"));

    MeshNode *terrainMesh = new MeshNode("terrainMesh", Mesh::GenerateQuad());
    terrainShaderSync->AddChild(terrainMesh);
    terrainMesh->GetMesh()->SetGLPatches();
    terrainMesh->SetBoundingSphereRadius(-1.0f);
    terrainMesh->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
    terrainMesh->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -15.0f, -8.0f)) *
                                        Matrix4::Scale(m_state.worldBounds));
  }

  // PORTAL
  ITexture *portalBufferColourTex = nullptr;

  {
    // Portal camera
    CameraNode * portalCamera = new CameraNode("portalCamera");
    root->AddChild(portalCamera);
    portalCamera->LockOrientationTo(playerCamera);
    portalCamera->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 50.0f, 0.0f)));

    // Portal FBO
    FramebufferNode *portalBuffer = new FramebufferNode("portalBuffer");
    projection->AddChild(portalBuffer);

    ITexture *portalBufferDepthTex = new DepthStencilTexture(m_state.screenDims.x, m_state.screenDims.y);
    portalBufferColourTex = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);

    portalBuffer->BindTexture(GL_DEPTH_ATTACHMENT, portalBufferDepthTex);
    portalBuffer->BindTexture(GL_STENCIL_ATTACHMENT, portalBufferDepthTex);
    portalBuffer->BindTexture(GL_COLOR_ATTACHMENT0, portalBufferColourTex);

    CameraSelectorNode *portalCameraSelect = new CameraSelectorNode("portalCameraSelect");
    portalBuffer->AddChild(portalCameraSelect);
    portalCameraSelect->SetCamera(portalCamera);

    portalCameraSelect->AddChild(new SubTreeNode("portalSubtree", globalFog));

    // Portal quad
    // TOOD
  }

  // ENVIRONMENT
  {
    auto envShader = globalTransp->AddChild(new ShaderNode(
      "envShader", new ShaderProgram({ new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
        new FragmentShader(CW_SHADER_DIR "PerPixelFragment.glsl") })));
    envShader->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 20.0f, 0.0f)));

    auto envTextures = envShader->AddChild(new TextureNode("envTextures", { { portalBufferColourTex, "diffuseTex", 1 } }));
    auto envShaderSync = envTextures->AddChild(new ShaderSyncNode("envShaderSync"));

    MeshNode *sphere1 = new MeshNode("sphere1", Mesh::GenerateSphere(), true);
    envShaderSync->AddChild(sphere1);
    sphere1->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 5.0f, -20.0f)));
    sphere1->SpecularIntensity() = 0.5f;
    sphere1->SpecularPower() = 100.0f;

    MeshNode *transp1 = new MeshNode("sphere1", Mesh::GenerateQuad(), true);
    envShaderSync->AddChild(transp1);
    transp1->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 3.0f, -10.0f)));
  }

  // WATER
  {
    ITexture *waterTex = new Texture();
    waterTex->LoadFromFile(CW_TEXTURE_DIR "water_surface.jpg");
    waterTex->SetRepeating(true);

    auto waterShader = globalTransp->AddChild(new ShaderNode(
        "waterShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                          new FragmentShader(CW_SHADER_DIR "ReflectFragment.glsl")})));

    auto waterTexture = waterShader->AddChild(
        new TextureNode("waterTexture", {{waterTex, "waterTexture", 1}, {cubeMapTex, "cubeTex", 2}}));
    m_state.waterTexMatrix = (MatrixNode *) waterTexture->AddChild(
        new MatrixNode("waterTexMatrix", "textureMatrix"));

    auto waterShaderSync = m_state.waterTexMatrix->AddChild(new ShaderSyncNode("waterShaderSync"));

    RenderableNode *waterQuad =
        (RenderableNode *)waterShaderSync->AddChild(new MeshNode("waterQuad", Mesh::GenerateQuad()));
    waterQuad->SetBoundingSphereRadius(-1.0f);
    waterQuad->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)) * Matrix4::Scale(1000.0f));
    waterQuad->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
    waterQuad->SpecularPower() = 2.0f;
  }

  // POST PROCESSING
  {
    FramebufferNode *processBuffer = new FramebufferNode("processBuffer");
    root->AddChild(processBuffer);

    auto depthDisable = processBuffer->AddChild(
        new GenericControlNode("depthDisable", [](ShaderProgram *) { glDisable(GL_DEPTH_TEST); },
                               [](ShaderProgram *) { glEnable(GL_DEPTH_TEST); }));

    auto shader = depthDisable->AddChild(
        new ShaderNode("shader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "TexturedVertex.glsl"),
                                                    new FragmentShader(CW_SHADER_DIR "ProcessFragment.glsl")})));

    auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

    auto proj = globalTexMatrixIdentity->AddChild(
        new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

    auto control1 = view->AddChild(new GenericControlNode("control1", [bufferColourTex2](ShaderProgram *s) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex2->GetTextureID(), 0);
    }));

    auto texture1 = control1->AddChild(new TextureNode("texture1", {{bufferColourTex1, "diffuseTex", 1}}));
    auto sync1 = texture1->AddChild(new ShaderSyncNode("sync1"));
    RenderableNode *quad1 = new MeshNode("quad1", Mesh::GenerateQuad());
    sync1->AddChild(quad1);
  }

  // POST PROCESSING PRESENTATION
  {
    auto shader = m_state.screenBuffer->AddChild(
        new ShaderNode("shader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "TexturedVertex.glsl"),
                                                    new FragmentShader(CW_SHADER_DIR "ProcessFragment.glsl")})));

    auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

    auto proj = globalTexMatrixIdentity->AddChild(
        new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

    auto texture = view->AddChild(new TextureNode("texture", {{bufferColourTex1, "diffuseTex", 1}}));
    auto sync = texture->AddChild(new ShaderSyncNode("sync"));
    auto quad = sync->AddChild(new MeshNode("quad", Mesh::GenerateQuad()));
  }
}

void World::Update(float msec)
{
  // Update system monitor at 1Hz
  if (m_state.sysMonitorTimer.GetTimedMS(false) > 1000.0f)
    m_state.sysMonitor.Update(m_state.sysMonitorTimer.GetTimedMS());

  // Update world time
  m_state.timeOfDay += msec * m_state.worldClockSpeed;
  if (m_state.timeOfDay > 1.0f)
    m_state.timeOfDay = 0.0f;

  // Update sun and moon positions and light intensity
  Matrix4 sunMoonTrans = Matrix4::Rotation(360.0f * m_state.timeOfDay, Vector3(0.0f, 0.0f, 1.0f));

  m_state.sun->SetLocalTransformation(sunMoonTrans * Matrix4::Translation(Vector3(0.0f, m_state.worldBounds, 0.0f)) *
                                      Matrix4::Scale(250.0f));
  m_state.sun->Colour().w =
      max(0.1f, m_state.sun->GetLocalTransformation().GetPositionVector().y / m_state.worldBounds);

  m_state.moon->SetLocalTransformation(sunMoonTrans * Matrix4::Translation(Vector3(0.0f, -m_state.worldBounds, 0.0f)) *
                                       Matrix4::Scale(80.0f));
  m_state.moon->Colour().w =
      max(0.1f, m_state.moon->GetLocalTransformation().GetPositionVector().y / m_state.worldBounds);

  // Toggle player lantern
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L))
    m_state.lantern->ToggleActive();

  // Toggle player flashlight
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F))
    m_state.flashlight->ToggleActive();

  // Move water texture
  m_state.waterTexMatrix->Matrix() = Matrix4::Scale(10.0f) * Matrix4::Translation(Vector3(0.0f, -0.1f * m_state.timeOfDay, 0.0f));
}
}