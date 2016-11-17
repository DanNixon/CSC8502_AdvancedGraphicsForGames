#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
{
  for (size_t i = 0; i < NUM_MESHES; i++)
    meshes[i] = nullptr;

  // meshes[0] = Mesh::GenerateTriangle();
  meshes[1] = Mesh::GenerateQuad();

  currentShader = new Shader(SHADERDIR "BasicVertex.glsl", SHADERDIR "ColourFragment.glsl");

  if (!currentShader->LinkProgram())
    return;

  init = true;
}

Renderer::~Renderer(void)
{
  for (size_t i = 0; i < NUM_MESHES; i++)
    delete meshes[i];
}

void Renderer::RenderScene()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(currentShader->GetProgram());
  for (size_t i = 0; i < NUM_MESHES; i++)
  {
    if (meshes[i] != nullptr)
    {
#if 0
      void *data = meshes[i]->GetBuffer(COLOUR_BUFFER, GL_READ_WRITE);
      if (data)
      {
        for (size_t i = 0; i < meshes[i]->NumVertices() * 4; i++)
          ((float *)data)[i] = 0.5f;
      }
      meshes[i]->ReturnBuffer(COLOUR_BUFFER);
#endif

      meshes[i]->Draw();
    }
  }
  glUseProgram(0);

  SwapBuffers();
}
