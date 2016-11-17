#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
{
  mesh = Mesh::GenerateTriangle();
  camera = new Camera();

  currentShader = new Shader(SHADERDIR "MatrixVertex.glsl", SHADERDIR "ColourFragment.glsl");
  if (!currentShader->LinkProgram())
    return;

  init = true;
  SwitchToOrthographic();

  // glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer(void)
{
  delete mesh;
}

void Renderer::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(currentShader->GetProgram());

  glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false,
                     (float *)&projMatrix);

  glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false,
                     (float *)&viewMatrix);

  for (int i = 0; i < 3; ++i)
  {
    Vector3 tempPos = position;
    tempPos.z += (i * 500.0f);
    tempPos.x -= (i * 100.0f);
    tempPos.y -= (i * 100.0f);

    modelMatrix = Matrix4::Translation(tempPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
                  Matrix4::Scale(Vector3(scale, scale, scale));

    glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false,
                       (float *)&modelMatrix);

    mesh->Draw();
  }

  glUseProgram(0);

  SwapBuffers();
}

void Renderer::UpdateScene(float msec)
{
  // camera->UpdateCamera(msec);
  viewMatrix = camera->BuildViewMatrix();
}

void Renderer::SwitchToPerspective(float fov)
{
  projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, fov);
}

void Renderer::SwitchToOrthographic()
{
  projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f,
                                     -height / 2.0f);
}
