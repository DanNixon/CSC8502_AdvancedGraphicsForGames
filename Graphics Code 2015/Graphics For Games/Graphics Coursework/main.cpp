#pragma comment(lib, "nclgl.lib")

#include <iostream>

#include "../../NCLGL/Window.h"

#include "Renderer.h"

int main()
{
  Window w("Planet", 800, 600, false);
  if (!w.HasInitialised())
    return 1;

  Renderer r(w);
  if (!r.HasInitialised())
    return 1;

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    // TODO
    r.RenderScene();
  }

  return 0;
}