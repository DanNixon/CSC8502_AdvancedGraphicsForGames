#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , filtering(true)
    , repeating(false)
    , blendFactor(0.5f)
{
  mesh = Mesh::GenerateTriangle();

  mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "brick.tga ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

  if (!mesh->GetTexture())
    return;

  currentShader = new Shader(SHADERDIR "TexturedVertex.glsl", SHADERDIR "TexturedFragment.glsl");

  if (!currentShader->LinkProgram())
    return;

  init = true;

  projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, 1, -1);
}

Renderer::~Renderer()
{
  delete mesh;
}

void Renderer::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(currentShader->GetProgram());

  glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "blendFactor"), blendFactor);
  UpdateShaderMatrices();

  glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

  mesh->Draw();

  glUseProgram(0);
  SwapBuffers();
}

void Renderer::UpdateTextureMatrix(float rotation)
{
  Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
  Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
  Matrix4 r = Matrix4::Rotation(rotation, Vector3(0, 0, 1));
  textureMatrix = pushPos * r * popPos;
}

void Renderer::ToggleRepeating()
{
  repeating = !repeating;
  glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ToggleFiltering()
{
  filtering = !filtering;
  glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::OffsetBlendFactor(float offset)
{
  blendFactor += offset;

  if (blendFactor < 0.0f)
    blendFactor = 0.0f;
  else if (blendFactor > 1.0f)
    blendFactor = 1.0f;
}
