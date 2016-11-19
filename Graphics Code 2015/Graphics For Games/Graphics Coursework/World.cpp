/** @file */

#include "World.h"

#include <../nclgl/Window.h>

#include "SceneNode.h"
#include "Font.h"
#include "CameraSelectorNode.h"
#include "MatrixNode.h"
#include "CameraNode.h"
#include "ShaderProgram.h"
#include "Shaders.h"
#include "ShaderNode.h"
#include "ShaderSyncNode.h"
#include "TextureNode.h"
#include "PerformanceMonitorNode.h"
#include "FramebufferNode.h"
#include "PositionableCamera.h"
#include "CubeMapTexture.h"
#include "DepthStencilTexture.h"
#include "RGBATexture.h"
#include "Light.h"
#include "PerlinNoise.h"
#include "FractalBrownianMotion.h"
#include "GenericControlNode.h"
#include "HeightmapTexture.h"
#include "Renderer.h"

namespace GraphicsCoursework
{
  World::World(Renderer & renderer)
    : m_renderer(renderer)
  {
    m_state.screenDims = m_renderer.ParentWindow().GetScreenSize();
    m_state.screenBuffer = new FramebufferNode("screenBuffer", false);
    m_renderer.Root()->AddChild(m_state.screenBuffer);
  }

  void World::BuildLoadingScreen(SceneNode * root)
  {
    Font *sysMonFont = new Font(16, 16);
    sysMonFont->LoadFromFile(TEXTUREDIR "tahoma.tga", SOIL_FLAG_COMPRESS_TO_DXT);

    root->AddChild(new CameraNode("sysMonCamera"));

    CameraSelectorNode *sysMonCameraSelect = new CameraSelectorNode("sysMonCameraSelect");
    m_state.screenBuffer->AddChild(sysMonCameraSelect);
    sysMonCameraSelect->SetCamera("sysMonCamera");

    auto sysMonProj = sysMonCameraSelect->AddChild(new MatrixNode(
      "sysMonProj", "projMatrix", Matrix4::Orthographic(-1.0f, 1.0f, m_state.screenDims.x, 0.0f, m_state.screenDims.y, 0.0f)));

    auto sysMonShader = sysMonProj->AddChild(new ShaderNode(
      "sysMonShader", new ShaderProgram({ new VertexShader(SHADERDIR "coursework/BasicTextureVertex.glsl"),
        new FragmentShader(SHADERDIR "coursework/BasicTextureFragment.glsl") })));

    auto sysMonFontTex = sysMonShader->AddChild(new TextureNode("sysMonFontTex", { { sysMonFont, "diffuseTex", 1 } }));

    auto sysMonShaderSync = sysMonFontTex->AddChild(new ShaderSyncNode("sysMonShaderSync"));

    TextNode *sysMonNode = new PerformanceMonitorNode("sysMonNode", sysMonFont, &m_state.sysMonitor);
    sysMonShaderSync->AddChild(sysMonNode);
    sysMonNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 1.0f, 0.0f)));

    m_state.loadingNode = sysMonShaderSync->AddChild(new TextNode("loadingNode", sysMonFont, "Loading..."));
    m_state.loadingNode->SetLocalTransformation(Matrix4::Scale(16.0f) * Matrix4::Translation(Vector3(0.0f, 2.0f, 0.0f)));
  }

  void World::Build(SceneNode * root)
  {
    ITexture *tex1 = new Texture();
    tex1->LoadFromFile(TEXTUREDIR "brick.tga");

    ITexture *tex2 = new Texture();
    tex2->LoadFromFile(TEXTUREDIR "Barren Reds.jpg");

    ITexture *tex3 = new Texture();
    tex3->LoadFromFile(TEXTUREDIR "stainedglass.tga");

    ShaderProgram *shader1 = new ShaderProgram(
    { new VertexShader(SHADERDIR "PerPixelVertex.glsl"), new FragmentShader(SHADERDIR "PerPixelFragment.glsl") });

    ShaderProgram *shader2 = new ShaderProgram(
    { new VertexShader(SHADERDIR "TexVertex.glsl"), new FragmentShader(SHADERDIR "TexTranspFrag.glsl") });

    PositionableCamera *cam1 = new PositionableCamera("cam1");
    root->AddChild(cam1);
    cam1->SetLocalTransformation(Matrix4::Translation(Vector3(1.0f, 1.0f, -8.0f)));
    cam1->LinearSpeed() = 0.01f;

    CameraSelectorNode *cs1 = new CameraSelectorNode("cs1");
    root->AddChild(cs1);
    cs1->SetCamera("cam1");

    auto globalTexMatrixIdentity = cs1->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));
    auto proj1 = globalTexMatrixIdentity->AddChild(
      new MatrixNode("proj1", "projMatrix", Matrix4::Perspective(1.0f, 10000.0f, m_state.screenDims.x / m_state.screenDims.y, 45.0f)));

    // Scene FBO
    FramebufferNode *sceneBuffer = new FramebufferNode("sceneBuffer");
    proj1->AddChild(sceneBuffer);

    ITexture *bufferDepthTex = new DepthStencilTexture(m_state.screenDims.x, m_state.screenDims.y);
    ITexture *bufferColourTex1 = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);
    ITexture *bufferColourTex2 = new RGBATexture(m_state.screenDims.x, m_state.screenDims.y);

    sceneBuffer->BindTexture(GL_DEPTH_ATTACHMENT, bufferDepthTex);
    sceneBuffer->BindTexture(GL_STENCIL_ATTACHMENT, bufferDepthTex);
    sceneBuffer->BindTexture(GL_COLOR_ATTACHMENT0, bufferColourTex1);

    ITexture *cubeMapTex = new CubeMapTexture();
    cubeMapTex->LoadFromFiles({
      TEXTUREDIR "rusted_west.jpg", TEXTUREDIR "rusted_east.jpg", TEXTUREDIR "rusted_up.jpg",
      TEXTUREDIR "rusted_down.jpg", TEXTUREDIR "rusted_south.jpg", TEXTUREDIR "rusted_north.jpg",
    });
    cubeMapTex->SetRepeating(true);

    // SKYBOX
    {
      auto skyboxGLcontrol =
        sceneBuffer->AddChild(new GenericControlNode("skyboxGLcontrol", [](ShaderProgram *) { glDepthMask(GL_FALSE); },
          [](ShaderProgram *) { glDepthMask(GL_TRUE); }));

      auto skyboxShader = skyboxGLcontrol->AddChild(
        new ShaderNode("skyboxShader", new ShaderProgram({ new VertexShader(SHADERDIR "SkyboxVertex.glsl"),
          new FragmentShader(SHADERDIR "SkyboxFragment.glsl") })));
      auto skyboxTexture = skyboxShader->AddChild(new TextureNode("skyboxTexture", { { cubeMapTex, "skyboxTexture", 1 } }));
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
          new ShaderProgram({ new VertexShader(SHADERDIR "coursework/PerPixelVertex.glsl"),
            new FragmentShader(SHADERDIR "coursework/PlanetLightSOurceFragment.glsl") })));
      auto lightShaderSync = lightRenderShader->AddChild(new ShaderSyncNode("lightShaderSync"));

      sun = new Light("sun");
      lightShaderSync->AddChild(sun);
      m_renderer.AddPersistentDataNode(sun);
      sun->Radius() = 1000.0f;
      sun->AmbientIntensity() = 0.3f;
      sun->Colour() = sunColour;
      sun->SetLocalTransformation(Matrix4::Translation(Vector3(50.0f, 50.0f, -50.0f)));

      MeshNode *sunMesh = new MeshNode("sunMesh", Mesh::GenerateSphere());
      sun->AddChild(sunMesh);
      sunMesh->GetMesh()->SetUniformColour(sunColour);

      moon = new Light("moon");
      lightShaderSync->AddChild(moon);
      m_renderer.AddPersistentDataNode(moon);
      moon->Radius() = 80.0f;
      moon->AmbientIntensity() = 0.3f;
      moon->Colour() = moonColour;
      moon->SetLocalTransformation(Matrix4::Translation(Vector3(-50.0f, 50.0f, -50.0f)));

      MeshNode *moonMesh = new MeshNode("moonMesh", Mesh::GenerateSphere());
      moon->AddChild(moonMesh);
      moonMesh->GetMesh()->SetUniformColour(moonColour);
    }
    // END LIGHTS

    auto envShader = sceneBuffer->AddChild(new ShaderNode("envShader", shader1));
    auto envTextures = envShader->AddChild(new TextureNode("envTextures", { { tex1, "diffuseTex", 1 } }));
    auto envShaderSync = envTextures->AddChild(new ShaderSyncNode("envShaderSync"));

    MeshNode *sphere1 = new MeshNode("sphere1", Mesh::GenerateSphere());
    envShaderSync->AddChild(sphere1);
    sphere1->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, 5.0f, -20.0f)));
    sphere1->SpecularIntensity() = 0.5f;
    sphere1->SpecularPower() = 100.0f;

    // HEIGHTMAP
    {
      PerlinNoise noise;
      FractalBrownianMotion fbm(noise);
      fbm.NumOctaves() = 3;
      fbm.Frequency() = 15.0f;
      fbm.ZValue() = 0.8f;

      HeightmapTexture *heightmapTexture = new HeightmapTexture(200, 200);
      heightmapTexture->SetFromFBM(&fbm);

      auto terrainTexture = sceneBuffer->AddChild(
        new TextureNode("terrainTexture", { { tex2, "diffuseTex", 1 },{ heightmapTexture, "heightmapTex", 2 } }));

      auto terrainShader = terrainTexture->AddChild(
        new ShaderNode("terrainShader",
          new ShaderProgram({ new VertexShader(SHADERDIR "coursework/HeightmapVertex.glsl"),
            new FragmentShader(SHADERDIR "PerPixelFragment.glsl"),
            new TesselationControlShader(SHADERDIR "coursework/HeightmapTCS.glsl"),
            new TesselationEvaluationShader(SHADERDIR "coursework/HeightmapTES.glsl") })));

      auto terrainControlNode = terrainShader->AddChild(
        new GenericControlNode("terrainControlNode", [](ShaderProgram *) { glPatchParameteri(GL_PATCH_VERTICES, 4); }));

      auto terrainShaderSync = terrainControlNode->AddChild(new ShaderSyncNode("terrainShaderSync"));

      MeshNode *terrainMesh = new MeshNode("terrainMesh", Mesh::GenerateQuad());
      terrainShaderSync->AddChild(terrainMesh);
      terrainMesh->GetMesh()->SetGLPatches();
      terrainMesh->SetLocalRotation(Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
      terrainMesh->SetLocalTransformation(Matrix4::Translation(Vector3(0.0f, -3.0f, -8.0f)) * Matrix4::Scale(1000.0f));
    }
    // END HEIGHTMAP

    // WATER
    {
      ITexture *waterTex = new Texture();
      waterTex->LoadFromFile(TEXTUREDIR "water_surface.jpg");
      waterTex->SetRepeating(true);

      auto waterShader = sceneBuffer->AddChild(
        new ShaderNode("waterShader", new ShaderProgram({ new VertexShader(SHADERDIR "PerPixelVertex.glsl"),
          new FragmentShader(SHADERDIR "ReflectFragment.glsl") })));

      auto waterTexture = waterShader->AddChild(
        new TextureNode("waterTexture", { { waterTex, "waterTexture", 1 },{ cubeMapTex, "cubeTex", 2 } }));
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

    // POST PROCESSING
    {
      FramebufferNode *processBuffer = new FramebufferNode("processBuffer");
      root->AddChild(processBuffer);

      auto depthDisable = processBuffer->AddChild(
        new GenericControlNode("depthDisable", [](ShaderProgram *) { glDisable(GL_DEPTH_TEST); },
          [](ShaderProgram *) { glEnable(GL_DEPTH_TEST); }));

      auto shader = depthDisable->AddChild(
        new ShaderNode("shader", new ShaderProgram({ new VertexShader(SHADERDIR "TexturedVertex.glsl"),
          new FragmentShader(SHADERDIR "ProcessFragment.glsl") })));

      auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

      auto proj = globalTexMatrixIdentity->AddChild(
        new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
      auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

      auto control1 = view->AddChild(new GenericControlNode("control1", [bufferColourTex2](ShaderProgram *s) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex2->GetTextureID(), 0);
      }));

      auto texture1 = control1->AddChild(new TextureNode("texture1", { { bufferColourTex1, "diffuseTex", 1 } }));
      auto sync1 = texture1->AddChild(new ShaderSyncNode("sync1"));
      RenderableNode *quad1 = new MeshNode("quad1", Mesh::GenerateQuad());
      sync1->AddChild(quad1);
    }

    // POST PROCESSING PRESENTATION
    {
      auto shader = m_state.screenBuffer->AddChild(
        new ShaderNode("shader", new ShaderProgram({ new VertexShader(SHADERDIR "TexturedVertex.glsl"),
          new FragmentShader(SHADERDIR "ProcessFragment.glsl") })));

      auto globalTexMatrixIdentity = shader->AddChild(new MatrixNode("globalTexMatrixIdentity", "textureMatrix"));

      auto proj = globalTexMatrixIdentity->AddChild(
        new MatrixNode("proj", "projMatrix", Matrix4::Orthographic(-1, 1, 1, -1, 1, -1)));
      auto view = proj->AddChild(new MatrixNode("view", "viewMatrix", Matrix4()));

      auto texture = view->AddChild(new TextureNode("texture", { { bufferColourTex1, "diffuseTex", 1 } }));
      auto sync = texture->AddChild(new ShaderSyncNode("sync"));
      auto quad = sync->AddChild(new MeshNode("quad", Mesh::GenerateQuad()));
    }
  }

  void World::Update(float msec)
  {
    if (m_state.sysMonitorTimer.GetTimedMS(false) > 1000.0f)
      m_state.sysMonitor.Update(m_state.sysMonitorTimer.GetTimedMS());

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
      m_state.sun->SetActive(!m_state.sun->IsActive(), true);

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
      m_state.moon->SetActive(!m_state.moon->IsActive(), true);
  }
}