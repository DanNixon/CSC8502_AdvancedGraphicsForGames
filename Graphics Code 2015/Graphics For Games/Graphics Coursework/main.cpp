/** @file */

#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/Window.h"

#include "Renderer.h"
#include "SceneNode.h"

#include "World.h"

using namespace GraphicsCoursework;

/**
 * @brief Entry point.
 * @return Exit code
 */
int main()
{
  Window window("Planet", 1440, 720, false);
  if (!window.HasInitialised())
    return 1;

  /* Mouse is inside of window */
  window.LockMouseToWindow(true);
  window.ShowOSPointer(false);

  Renderer r(window);
  if (!r.HasInitialised())
    return 1;

  /* Init world */
  float msec = 0.0f;
  World w(r);

  glClearColor(0.3f, 0.6f, 0.7f, 1.0f);

  /* Build and render loading screen */
  w.BuildLoadingScreen(r.Root());
  r.Root()->Update(msec);
  r.RenderScene();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  /* Build the world */
  w.Build(r.Root());

  /* Output scene tree and mark loading as completed */
  std::cout << r;
  w.State().loadingNode->SetActive(false);

  /* Do rendering and world updates */
  while (window.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
  {
    msec = window.GetTimer()->GetTimedMS();

    w.Update(msec);
    r.Root()->Update(msec);

    r.RenderScene();
  }

  return 0;
}
