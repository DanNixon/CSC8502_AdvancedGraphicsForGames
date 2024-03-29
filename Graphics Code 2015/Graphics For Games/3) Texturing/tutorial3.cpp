#pragma comment(lib, "nclgl.lib")

#include "../../nclGL/window.h"
#include "Renderer.h"

int main()
{
  Window w("Texturing!", 800, 600, false);
  if (!w.HasInitialised())
    return -1;

  Renderer renderer(w);
  if (!renderer.HasInitialised())
    return -1;

  float rotate = 0.0f;
  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
    {
      --rotate;
      renderer.UpdateTextureMatrix(rotate);
    }

    if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
    {
      ++rotate;
      renderer.UpdateTextureMatrix(rotate);
    }

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
      renderer.ToggleFiltering();

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
      renderer.ToggleRepeating();

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_MINUS))
      renderer.OffsetBlendFactor(-0.05f);

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PLUS))
      renderer.OffsetBlendFactor(0.05f);

    renderer.RenderScene();
  }

  return 0;
}