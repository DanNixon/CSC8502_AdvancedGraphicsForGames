#pragma once

#include "OGLRenderer.h"

enum MeshBuffer
{
  VERTEX_BUFFER,
  COLOUR_BUFFER,
  MAX_BUFFER
};

class Mesh
{
public:
  static Mesh *GenerateTriangle();

public:
  Mesh(void);
  ~Mesh(void);

  virtual void Draw();

protected:
  void BufferData();

protected:
  GLuint arrayObject;
  GLuint bufferObject[MAX_BUFFER];
  GLuint numVertices;
  GLuint type;

  Vector3 *vertices;
  Vector4 *colours;
};