#pragma once

#include "../nclgl/OBJMesh.h"
#include "../nclgl/SceneNode.h"

class CubeRobot : public SceneNode
{
public:
  static void CreateCube()
  {
    OBJMesh *m = new OBJMesh();
    m->LoadOBJMesh(MESHDIR "cube.obj");
    cube = m;
  }

  static void DeleteCube()
  {
    delete cube;
  }

public:
  CubeRobot();
  ~CubeRobot(){};

  virtual void Update(float msec);

protected:
  static Mesh *cube;
  SceneNode *head;
  SceneNode *leftArm;
  SceneNode *rightArm;
};
