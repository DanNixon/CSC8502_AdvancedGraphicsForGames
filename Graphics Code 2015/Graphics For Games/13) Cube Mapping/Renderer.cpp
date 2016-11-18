#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
{
  camera = new Camera();
  heightMap = new HeightMap("../ Textures / terrain .raw ");
  quad = Mesh::GenerateQuad();

  camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0 f, 500.0 f, RAW_WIDTH * HEIGHTMAP_X));

  light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
                    Vector4(0.9 f, 0.9 f, 1.0 f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

  reflectShader = new Shader("../ Shaders / PerPixelVertex . glsl ", " reflectFragment . glsl ");
  skyboxShader = new Shader(" skyboxVertex . glsl ", " skyboxFragment . glsl ");
  lightShader = new Shader("../ Shaders / PerPixelVertex . glsl ", "../ Shaders / PerPixelFragment . glsl ");

  if (!reflectShader - > LinkProgram() || !lightShader - > LinkProgram() || !skyboxShader - > LinkProgram())
  {
    return;
  }

  quad - > SetTexture(SOIL_load_OGL_texture("../ Textures / water .TGA ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                            SOIL_FLAG_MIPMAPS));

  heightMap - > SetTexture(SOIL_load_OGL_texture("../ Textures / Barren Reds .JPG ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                                 SOIL_FLAG_MIPMAPS));

  heightMap - > SetBumpMap(SOIL_load_OGL_texture("../ Textures / Barren RedsDOT3 .tga ", SOIL_LOAD_AUTO,
                                                 SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

  cubeMap = SOIL_load_OGL_cubemap("../ Textures / rusted_west .bmp ", "../ Textures / rusted_east .bmp ",
                                  "../ Textures / rusted_up .bmp ", "../ Textures / rusted_down .bmp ",
                                  "../ Textures / rusted_south .bmp ", "../ Textures / rusted_north .bmp ",
                                  SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

  if (!cubeMap || !quad - > GetTexture() || !heightMap - > GetTexture() || !heightMap - > GetBumpMap())
  {
    return;
  }

  SetTextureRepeating(quad - > GetTexture(), true);
  SetTextureRepeating(heightMap - > GetTexture(), true);
  SetTextureRepeating(heightMap - > GetBumpMap(), true);
  init = true;
  waterRotate = 0.0 f;

  projMatrix = Matrix4::Perspective(1.0 f, 15000.0 f, (float)width / (float)height, 45.0 f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  Renderer::~Renderer(void)
  {
    delete camera;
    delete heightMap;
    delete quad;
    delete reflectShader;
    delete skyboxShader;
    delete lightShader;
    delete light;
    currentShader = 0;
  }
  void Renderer::UpdateScene(float msec)
  {
    camera - > UpdateCamera(msec);
    viewMatrix = camera - > BuildViewMatrix();
    waterRotate += msec / 1000.0 f;
  }

  void Renderer::RenderScene()
  {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    DrawSkybox();
    DrawHeightmap();
    DrawWater();

    SwapBuffers();
  }

  void Renderer::DrawSkybox()
  {
    glDepthMask(GL_FALSE);
    SetCurrentShader(skyboxShader);

    UpdateShaderMatrices();
    quad - > Draw();

    glUseProgram(0);
    glDepthMask(GL_TRUE);
  }
  7 void Renderer::DrawHeightmap()
  {
    SetCurrentShader(lightShader);
    SetShaderLight(*light);

    glUniform3fv(glGetUniformLocation(currentShader - > GetProgram(), "cameraPos"), 1,
                 (float *)&camera - > GetPosition());

    glUniform1i(glGetUniformLocation(currentShader - > GetProgram(), "diffuseTex"), 0);
    glUniform1i(glGetUniformLocation(currentShader - > GetProgram(), "bumpTex"), 1);

    modelMatrix.ToIdentity();
    textureMatrix.ToIdentity();

    UpdateShaderMatrices();

    heightMap - > Draw();

    glUseProgram(0);

    void Renderer::DrawWater()
    {
      SetCurrentShader(reflectShader);
      SetShaderLight(*light);
      glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1,
                   (float *)&camera - > GetPosition());

      glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

      glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

      float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

      float heightY = 256 * HEIGHTMAP_Y / 3.0f;

      float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

      modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
                    Matrix4::Scale(Vector3(heightX, 1, heightZ)) * Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

      textureMatrix =
          Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));
      UpdateShaderMatrices();

      quad->Draw();

      glUseProgram(0);
    }
