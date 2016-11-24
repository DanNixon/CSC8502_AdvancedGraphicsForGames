/** @file */

#include "World.h"

#include <../nclgl/OBJMesh.h>
#include <../nclgl/Window.h>

#include "CameraNode.h"
#include "CameraSelectorNode.h"
#include "CubeMapTexture.h"
#include "DepthStencilTexture.h"
#include "Font.h"
#include "FractalBrownianMotion.h"
#include "FramebufferNode.h"
#include "HeightmapTexture.h"
#include "Math.h"
#include "MatrixNode.h"
#include "ParticleSystem.h"
#include "ParticleSystemNode.h"
#include "PerformanceMonitorNode.h"
#include "PerlinNoise.h"
#include "PointLight.h"
#include "PositionableCamera.h"
#include "RGBATexture.h"
#include "Renderer.h"
#include "SceneNode.h"
#include "ShaderControlNode.h"
#include "ShaderNode.h"
#include "ShaderProgram.h"
#include "ShaderSyncNode.h"
#include "Shaders.h"
#include "SpotLight.h"
#include "SubTreeNode.h"
#include "TextureNode.h"
#include "TransparentRenderingNode.h"
#include "TreeControlNode.h"
#include "directories.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new world.
 * @param renderer Renderer in use
 */
World::World(Renderer &renderer)
    : m_renderer(renderer)
{
  m_state.screenDims = m_renderer.ParentWindow().GetScreenSize();
  m_state.screenBuffer = new FramebufferNode("screenBuffer", false);
  m_renderer.Root()->AddChild(m_state.screenBuffer);
}

/**
 * @brief Builds the loading screen.
 * @param root Root node of the scene graph
 *
 * This part is done seperately to allow the loading screen to be rendered while the rest of the world is being built.
 */
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

/**
 * @brief Builds the world.
 * @param root Root node of the scene graph
 */
void World::Build(SceneNode *root)
{
  // Textures
  ITexture *brickTexture = new Texture();
  brickTexture->LoadFromFile(CW_TEXTURE_DIR "brick.tga");
  brickTexture->SetRepeating(true);

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

  auto projection = globalTexMatrixIdentity->AddChild(
      new MatrixNode("projection", "projMatrix",
                     Matrix4::Perspective(1.0f, 10000.0f, m_state.screenDims.x / m_state.screenDims.y, 45.0f)));
  projection->SetProcessMode(SceneNode::PM_BOTH);

  // Scene FBO
  FramebufferNode *sceneBuffer = new FramebufferNode("sceneBuffer");
  projection->AddChild(sceneBuffer);
  auto globalFog = sceneBuffer->AddChild(new ShaderControlNode("globalFog", [](ShaderProgram *s) {
    glUniform3f(glGetUniformLocation(s->Program(), "fogBaseColour"), 0.4f, 0.4f, 0.55f);
    glUniform1f(glGetUniformLocation(s->Program(), "fogExp"), 0.0005f);
    glUniform1f(glGetUniformLocation(s->Program(), "fogAtten"), 0.4f);
  }));
  auto globalTransp = globalFog->AddChild(new TransparentRenderingNode("globalTransp"));

  ITexture *bufferDepthTex = new DepthStencilTexture(m_state.screenDims.x, m_state.screenDims.y);
  ITexture *bufferColourTex = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);
  ITexture *bufferColourTexOut = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);

  bufferDepthTex->SetRepeating(true);
  bufferColourTex->SetRepeating(false);
  bufferColourTexOut->SetRepeating(true);

  sceneBuffer->BindTexture(GL_DEPTH_ATTACHMENT, bufferDepthTex);
  sceneBuffer->BindTexture(GL_STENCIL_ATTACHMENT, bufferDepthTex);
  sceneBuffer->BindTexture(GL_COLOR_ATTACHMENT0, bufferColourTex);

  ITexture *cubeMapTex = new CubeMapTexture();
  cubeMapTex->LoadFromFiles(
      {CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_px.jpg", CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_mx.jpg",
       CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_py.jpg", CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_my.jpg",
       CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_pz.jpg",
       CW_TEXTURE_DIR "TychoSkymapII.t3_08192x04096_80_mz.jpg"},
      SOIL_FLAG_INVERT_Y);

  // SKYBOX
  {
    auto skyboxGLcontrol =
        globalTransp->AddChild(new ShaderControlNode("skyboxGLcontrol", [](ShaderProgram *) { glDepthMask(GL_FALSE); },
                                                     [](ShaderProgram *) { glDepthMask(GL_TRUE); }));
    skyboxGLcontrol->SetProcessMode(SceneNode::PM_BOTH);

    auto skyboxShader = skyboxGLcontrol->AddChild(
        new ShaderNode("skyboxShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "SkyboxVertex.glsl"),
                                                          new FragmentShader(CW_SHADER_DIR "SkyboxFragment.glsl")})));
    auto skyboxTexture = skyboxShader->AddChild(new TextureNode("skyboxTexture", {{cubeMapTex, "skyboxTexture", 1}}));
    auto skyboxShaderSync = skyboxTexture->AddChild(new ShaderSyncNode("skyboxShaderSync"));
    RenderableNode *skyboxQuadMesh =
        (RenderableNode *)skyboxShaderSync->AddChild(new MeshNode("skyboxQuadMesh", Mesh::GenerateQuad()));
    skyboxQuadMesh->SetBoundingSphereRadius(-1.0f);
  }

  // LIGHTS
  {
    Vector4 sunColour(1.0f, 0.9f, 0.6f, 1.0f);
    Vector4 moonColour(0.6f, 0.7f, 1.0f, 1.0f);

    auto lightRenderShader = globalTransp->AddChild(new ShaderNode(
        "lightRenderShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
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
    m_state.flashlight->InitShadows(2048, projection, m_state.screenDims);
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

    ITexture *heightmapMultTex = new Texture();
    heightmapMultTex->LoadFromFile(CW_TEXTURE_DIR "radial_grad.tga");

    // Textures
    ITexture *sandTex = new Texture();
    sandTex->LoadFromFile(CW_TEXTURE_DIR "sand.jpg");
    sandTex->SetRepeating(true);

    ITexture *grassTex = new Texture();
    grassTex->LoadFromFile(CW_TEXTURE_DIR "grass.jpg");
    grassTex->SetRepeating(true);

    ITexture *rockTex = new Texture();
    rockTex->LoadFromFile(CW_TEXTURE_DIR "rock.jpg");
    rockTex->SetRepeating(true);

    // Scene subtree
    auto terrainTextures =
        globalTransp->AddChild(new TextureNode("terrainTextures", {
                                                                      {heightmapTexture, "heightmapTex", 1},
                                                                      {heightmapMultTex, "heightmapMultTex", 2},
                                                                      {sandTex, "levelTex[0]", 3},
                                                                      {grassTex, "levelTex[1]", 4},
                                                                      {rockTex, "levelTex[2]", 5},
                                                                  }));

    auto terrainShader = terrainTextures->AddChild(new ShaderNode(
        "terrainShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "HeightmapVertex.glsl"),
                                            new FragmentShader(CW_SHADER_DIR "HeightmapFragment.glsl"),
                                            new TesselationControlShader(CW_SHADER_DIR "HeightmapTCS.glsl"),
                                            new TesselationEvaluationShader(CW_SHADER_DIR "HeightmapTES.glsl")})));

    auto terrainControlNode = terrainShader->AddChild(
        new ShaderControlNode("terrainControlNode", [](ShaderProgram *) { glPatchParameteri(GL_PATCH_VERTICES, 4); }));
    terrainControlNode->SetProcessMode(SceneNode::PM_BOTH);

    auto terrainTextureMatrix =
        terrainControlNode->AddChild(new MatrixNode("terrainTextureMatrix", "textureMatrix", Matrix4::Scale(100.0f)));

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
  {
    // Portal camera
    CameraNode *portalCamera = new CameraNode("portalCamera");
    sceneBuffer->AddChild(portalCamera);
    portalCamera->LockOrientationTo(playerCamera, Matrix4::Rotation(-10.0f, Vector3(1.0f, 0.0f, 0.0f)));
    portalCamera->SetLocalTransformation(Matrix4::Translation(Vector3(30.0f, 30.0f, 600.0f)));

    // Portal camera quad
    auto portalCameraShader = portalCamera->AddChild(new ShaderNode(
        "portalCameraShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                                 new FragmentShader(CW_SHADER_DIR "PerPixelFragment.glsl")})));

    ITexture *portalCameraTexture = new Texture();
    portalCameraTexture->LoadFromFile(CW_TEXTURE_DIR "portal.png");

    auto portalCameraTextures =
        portalCameraShader->AddChild(new TextureNode("portalCameraTextures", {{portalCameraTexture, "diffuseTex", 1}}));
    auto protalCameraShaderSync = portalCameraTextures->AddChild(new ShaderSyncNode("portalCameraShaderSync"));

    MeshNode *portalCameraQuad = new MeshNode("portalCameraQuad", Mesh::GenerateQuad());
    protalCameraShaderSync->AddChild(portalCameraQuad);
    portalCameraQuad->SetLocalTransformation(Matrix4::Scale(5.0f));

    // Portal FBO
    FramebufferNode *portalBuffer = new FramebufferNode("portalBuffer");
    projection->AddChild(portalBuffer);

    ITexture *portalBufferDepthTex = new DepthStencilTexture(m_state.screenDims.x, m_state.screenDims.y);
    ITexture *portalBufferColourTex = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);

    portalBuffer->BindTexture(GL_DEPTH_ATTACHMENT, portalBufferDepthTex);
    portalBuffer->BindTexture(GL_STENCIL_ATTACHMENT, portalBufferDepthTex);
    portalBuffer->BindTexture(GL_COLOR_ATTACHMENT0, portalBufferColourTex);

    CameraSelectorNode *portalCameraSelect = new CameraSelectorNode("portalCameraSelect");
    portalBuffer->AddChild(portalCameraSelect);
    portalCameraSelect->SetCamera(portalCamera);

    portalCameraSelect->AddChild(new SubTreeNode("portalSubtree", globalFog));

    // Portal quad
    auto portalShader = globalTransp->AddChild(
        new ShaderNode("portalShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                                          new FragmentShader(CW_SHADER_DIR "PortalFragment.glsl")})));
    portalShader->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 20.0f, 0.0f)));

    ITexture *brokenGlassTex = new Texture();
    brokenGlassTex->LoadFromFile(CW_TEXTURE_DIR "brokenglass.jpg");

    auto portalTextures = portalShader->AddChild(new TextureNode(
        "portalTextures", {{portalBufferColourTex, "portalViewTex", 1}, {brokenGlassTex, "brokenGlassTex", 2}}));
    auto portalShaderSync = portalTextures->AddChild(new ShaderSyncNode("protalShaderSync"));

    MeshNode *portalQuad = new MeshNode("portalQuad", Mesh::GenerateQuad());
    portalShaderSync->AddChild(portalQuad);
    portalQuad->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 3.0f, -10.0f)));
  }

  // ENVIRONMENT
  {
    auto envShader = globalTransp->AddChild(
        new ShaderNode("envShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                                       new FragmentShader(CW_SHADER_DIR "PerPixelFragment.glsl")})));
    envShader->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 20.0f, 0.0f)));

    auto envTextures = envShader->AddChild(new TextureNode("envTextures", {{brickTexture, "diffuseTex", 1}}));
    auto envShaderSync = envTextures->AddChild(new ShaderSyncNode("envShaderSync"));

    MeshNode *sphere1 = new MeshNode("sphere1", Mesh::GenerateSphere());
    envShaderSync->AddChild(sphere1);
    sphere1->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 5.0f, -20.0f)));
    sphere1->SpecularIntensity() = 0.5f;
    sphere1->SpecularPower() = 100.0f;
  }

  // STREET LIGHT
  {
    auto streetLightShader = globalTransp->AddChild(new ShaderNode(
        "streetLightShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                                new FragmentShader(CW_SHADER_DIR "PerPixelFragmentColour.glsl")})));

    auto streetLightShaderSync = streetLightShader->AddChild(new ShaderSyncNode("streetLightShaderSync"));
    streetLightShaderSync->SetLocalTransformation(Matrix4::Translation(Vector3(-20.0f, 16.0f, -20.0f)));

    OBJMesh *lampMesh = new OBJMesh();
    lampMesh->LoadOBJMesh(CW_MESH_DIR "lamp_post.obj", false);
    lampMesh->SetUniformColour(Vector4(0.9f, 0.8f, 0.8f, 1.0f));

    MeshNode *streetLightRenderable = new MeshNode("streetLightRenderable", lampMesh);
    streetLightShaderSync->AddChild(streetLightRenderable);
    streetLightRenderable->SetLocalTransformation(Matrix4::Scale(0.05f));
    streetLightRenderable->SpecularIntensity() = 0.5f;
    streetLightRenderable->SpecularPower() = 100.0f;

    ILight *light = new PointLight("streetLightLight");
    streetLightShaderSync->AddChild(light);
    m_renderer.AddPersistentDataNode(light);
    light->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 12.0f, 0.0f)));
    light->Reach() = 250.0f;
  }

  // WATER
  {
    ITexture *waterTex = new Texture();
    waterTex->LoadFromFile(CW_TEXTURE_DIR "water_surface.jpg");
    waterTex->SetRepeating(true);

    auto waterShader = globalTransp->AddChild(
        new ShaderNode("waterShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "PerPixelVertex.glsl"),
                                                         new FragmentShader(CW_SHADER_DIR "ReflectFragment.glsl")})));

    auto waterTexture = waterShader->AddChild(
        new TextureNode("waterTexture", {{waterTex, "waterTexture", 1}, {cubeMapTex, "cubeTex", 2}}));
    m_state.waterTexMatrix = (MatrixNode *)waterTexture->AddChild(new MatrixNode("waterTexMatrix", "textureMatrix"));

    auto waterShaderSync = m_state.waterTexMatrix->AddChild(new ShaderSyncNode("waterShaderSync"));

    RenderableNode *waterQuad =
        (RenderableNode *)waterShaderSync->AddChild(new MeshNode("waterQuad", Mesh::GenerateQuad()));
    waterQuad->SetBoundingSphereRadius(-1.0f);
    waterQuad->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)) *
                                      Matrix4::Scale(m_state.worldBounds));
    waterQuad->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
    waterQuad->SpecularPower() = 2.0f;
  }

  // GEYSER PARTICLES
  {
    auto particleShader = globalTransp->AddChild(new ShaderNode(
        "geyserParticleShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "ParticleVertex.glsl"),
                                                   new FragmentShader(CW_SHADER_DIR "ParticleColourFragment.glsl"),
                                                   new GeometryShader(CW_SHADER_DIR "ParticleGeometry.glsl")})));

    ParticleSystem *particle = new ParticleSystem();
    particle->SetLaunchParticles(50);
    particle->SetParticleLifetime(5000.0f);

    particle->NewFunction() = [](Vector3 &dir, Vector4 &col) {
      col = Vector4(Math::Lerp(ParticleSystem::Rand(), 0.1f, 0.2f), Math::Lerp(ParticleSystem::Rand(), 0.1f, 0.2f),
                    Math::Lerp(ParticleSystem::Rand(), 0.5f, 1.0f), 1.0f);

      dir = Vector3(0.05f, 1.0f, 1.0f);
      dir.x += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 0.4f);
      dir.y += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 0.4f);
      dir.z += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 0.4f);
    };

    particle->UpdateFunction() = [](Particle &p, float msec) {
      p.colour.x += ParticleSystem::Rand() * 0.01f;
      p.colour.y += ParticleSystem::Rand() * 0.01f;
      p.colour.z += ParticleSystem::Rand() * 0.001f;

      p.direction.y -= msec * 0.0005f;
      p.direction.Normalise();

      p.position += p.direction * (msec * 0.01f);
    };

    auto particleControl = particleShader->AddChild(
        new ShaderControlNode("geyserParticleControl",
                              [](ShaderProgram *s) {
                                glEnable(GL_BLEND);
                                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                                glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.2f);
                              },
                              [](ShaderProgram *s) {
                                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.0f);
                              }));

    auto particleShaderSync = particleControl->AddChild(new ShaderSyncNode("geyserParticleShaderSync"));

    ParticleSystemNode *p = new ParticleSystemNode("geyserParticleRenderable", particle);
    particleShaderSync->AddChild(p);
    p->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 5.0f, 400.0f)));
    p->SetBoundingSphereRadius(50.0f);
  }

  // WEATHER PARICLES
  {
    CameraNode *particleCamera = new CameraNode("particleCamera");
    root->AddChild(particleCamera);
    particleCamera->LockOrientationTo(playerCamera);
    particleCamera->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -15.0f, 0.0f)));

    auto particleProj = sceneBuffer->AddChild(
        new MatrixNode("particleProj", "projMatrix",
                       Matrix4::Perspective(1.0f, 100.0f, m_state.screenDims.x / m_state.screenDims.y, 45.0f)));
    particleProj->SetProcessMode(SceneNode::PM_BOTH);

    auto particleTransp = particleProj->AddChild(new TransparentRenderingNode("particleTransp"));

    CameraSelectorNode *particleCameraSelect = new CameraSelectorNode("particleCameraSelect");
    particleTransp->AddChild(particleCameraSelect);
    particleCameraSelect->SetCamera(particleCamera);

    // RAIN PARTICLES
    {
      ITexture *raindropTexture = new Texture();
      raindropTexture->LoadFromFile(CW_TEXTURE_DIR "raindrop.tga");

      auto particleShader = particleCameraSelect->AddChild(new ShaderNode(
          "rainParticleShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "ParticleVertex.glsl"),
                                                   new FragmentShader(CW_SHADER_DIR "ParticleMixFragment.glsl"),
                                                   new GeometryShader(CW_SHADER_DIR "ParticleGeometry.glsl")})));

      auto particleTextures =
          particleShader->AddChild(new TextureNode("rainParticleTextures", {{raindropTexture, "diffuseTex", 1}}));

      ParticleSystem *particle = new ParticleSystem();
      particle->SetParticleLifetime(6000.0f);
      particle->SetLaunchParticles(25);

      particle->NewFunction() = [](Vector3 &dir, Vector4 &col) {
        col = Vector4(Math::Lerp(ParticleSystem::Rand(), 0.1f, 0.2f), Math::Lerp(ParticleSystem::Rand(), 0.1f, 0.2f),
                      Math::Lerp(ParticleSystem::Rand(), 0.5f, 1.0f), 1.0f);

        dir = Vector3(0.0f, -1.0f, 0.0f);
        dir.x += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
        dir.y -= abs((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
        dir.z += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
      };

      particle->UpdateFunction() = [](Particle &p, float msec) {
        p.direction.x *= 0.95f;
        p.direction.z *= 0.95f;

        p.position += p.direction * (msec * 0.1f);
      };

      auto particleControl = particleTextures->AddChild(
          new ShaderControlNode("rainParticleControl",
                                [](ShaderProgram *s) {
                                  glEnable(GL_BLEND);
                                  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                                  glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.1f);
                                },
                                [](ShaderProgram *s) {
                                  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                  glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.0f);
                                }));

      auto particleShaderSync = particleControl->AddChild(new ShaderSyncNode("rainParticleShaderSync"));

      m_state.rain = new ParticleSystemNode("rainParticleRenderable", particle);
      particleShaderSync->AddChild(m_state.rain);
      m_state.rain->SetActive(false);
    }

    // SNOW PARTICLES
    {
      ITexture *snowflakeTexture = new Texture();
      snowflakeTexture->LoadFromFile(CW_TEXTURE_DIR "snowflake_1.tga");

      auto particleShader = particleCameraSelect->AddChild(new ShaderNode(
          "snowParticleShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "ParticleVertex.glsl"),
                                                   new FragmentShader(CW_SHADER_DIR "ParticleFragment.glsl"),
                                                   new GeometryShader(CW_SHADER_DIR "ParticleGeometry.glsl")})));

      auto particleTextures =
          particleShader->AddChild(new TextureNode("snowParticleTextures", {{snowflakeTexture, "diffuseTex", 1}}));

      ParticleSystem *particle = new ParticleSystem();
      particle->SetParticleLifetime(5000.0f);

      particle->NewFunction() = [](Vector3 &dir, Vector4 &col) {
        col = Vector4(ParticleSystem::Rand(), ParticleSystem::Rand(), ParticleSystem::Rand(), 1.0);

        dir = Vector3(0.0f, -1.0f, 0.0f);
        dir.x += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
        dir.y -= abs((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
        dir.z += ((ParticleSystem::Rand() - ParticleSystem::Rand()) * 1.5f);
      };

      particle->UpdateFunction() = [](Particle &p, float msec) { p.position += p.direction * (msec * 0.01f); };

      auto particleControl = particleTextures->AddChild(
          new ShaderControlNode("snowParticleControl",
                                [](ShaderProgram *s) {
                                  glEnable(GL_BLEND);
                                  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                                  glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.2f);
                                },
                                [](ShaderProgram *s) {
                                  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                  glUniform1f(glGetUniformLocation(s->Program(), "particleSize"), 0.0f);
                                }));

      auto particleShaderSync = particleControl->AddChild(new ShaderSyncNode("snowParticleShaderSync"));

      m_state.snow = new ParticleSystemNode("snowParticleRenderable", particle);
      particleShaderSync->AddChild(m_state.snow);
      m_state.snow->SetActive(false);
    }
  }

  // POST PROCESSING
  {
    FramebufferNode *processBuffer = new FramebufferNode("processingBuffer");
    processBuffer->BindTexture(GL_COLOR_ATTACHMENT0, bufferColourTexOut);
    root->AddChild(processBuffer);

    auto depthDisable = processBuffer->AddChild(
        new ShaderControlNode("processingDepthDisable", [](ShaderProgram *) { glDisable(GL_DEPTH_TEST); },
                              [](ShaderProgram *) { glEnable(GL_DEPTH_TEST); }));

    auto shader = depthDisable->AddChild(new ShaderNode(
        "processingShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "ProcessVertex.glsl"),
                                               new FragmentShader(CW_SHADER_DIR "ProcessFragment.glsl")})));

    MatrixNode *texMatrix = new MatrixNode("processingTexMatrix", "textureMatrix");
    shader->AddChild(texMatrix);

    auto proj =
        texMatrix->AddChild(new MatrixNode("processingProj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

    auto control = view->AddChild(new ShaderControlNode("processingControl", [this](ShaderProgram *s) {
      glUniform1i(glGetUniformLocation(s->Program(), "shake"), 0);
      glUniform1f(glGetUniformLocation(s->Program(), "time"), this->m_state.timeOfDay * 32.0f);
      glUniform1f(glGetUniformLocation(s->Program(), "colourTemp"), this->m_state.colourTemp);
    }));

    auto texture = control->AddChild(new TextureNode("processingTexture", {{bufferColourTex, "diffuseTex", 1}}));
    auto sync = texture->AddChild(new ShaderSyncNode("processingSync"));
    auto processQuad = sync->AddChild(new MeshNode("processingQuad", Mesh::GenerateQuad()));
    processQuad->SetLocalTransformation(Matrix4::Scale(1.2f));
  }

  // POST PROCESSING PRESENTATION
  {
    auto shader = m_state.screenBuffer->AddChild(new ShaderNode(
        "processingPresentShader", new ShaderProgram({new VertexShader(CW_SHADER_DIR "TexturedVertex.glsl"),
                                                      new FragmentShader(CW_SHADER_DIR "TexturedFragment.glsl")})));

    auto globalTexMatrixIdentity =
        shader->AddChild(new MatrixNode("processingPresentTexMatrixIdentity", "textureMatrix"));

    auto proj = globalTexMatrixIdentity->AddChild(
        new MatrixNode("processingPresentProj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
    auto view = proj->AddChild(new MatrixNode("processingPresentView", "viewMatrix", Matrix4()));

    auto texture = view->AddChild(new TextureNode("processingPresentTexture", {{bufferColourTexOut, "diffuseTex", 1}}));
    auto sync = texture->AddChild(new ShaderSyncNode("processingPresentSync"));
    sync->AddChild(new MeshNode("processingPresentQuad", Mesh::GenerateQuad()));
  }
}

/**
 * @brief Perform world updates.
 * @param msec Time in milliseconds since last world update
 */
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
  Matrix4 sunMoonTrans = Matrix4::Rotation(90.0f + (360.0f * m_state.timeOfDay), Vector3(0.0f, 0.0f, 1.0f));

  m_state.sun->SetLocalTransformation(sunMoonTrans * Matrix4::Translation(Vector3(0.0f, -m_state.worldBounds, 0.0f)) *
                                      Matrix4::Scale(250.0f));
  m_state.sun->Colour().w =
      max(0.15f, m_state.sun->GetLocalTransformation().GetPositionVector().y / m_state.worldBounds);

  m_state.moon->SetLocalTransformation(sunMoonTrans * Matrix4::Translation(Vector3(0.0f, m_state.worldBounds, 0.0f)) *
                                       Matrix4::Scale(80.0f));
  m_state.moon->Colour().w =
      max(0.1f, m_state.moon->GetLocalTransformation().GetPositionVector().y / m_state.worldBounds);

  // Toggle player lantern
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L))
    std::cout << "Lantern: " << m_state.lantern->ToggleActive() << '\n';

  // Toggle player flashlight
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F))
    std::cout << "Flashlight: " << m_state.flashlight->ToggleActive() << '\n';

  // Toggle rain
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
    std::cout << "Rain: " << m_state.rain->ToggleActive() << '\n';

  // Toggle snow
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
    std::cout << "Snow: " << m_state.snow->ToggleActive() << '\n';

  // Move water texture
  m_state.waterTexMatrix->Matrix() =
      Matrix4::Scale(10.0f) * Matrix4::Translation(Vector3(0.0f, -0.1f * m_state.timeOfDay, 0.0f));
}
}
