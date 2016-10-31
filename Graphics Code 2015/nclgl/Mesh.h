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
  static Mesh *GenerateSquare();

public:
  Mesh(void);
  ~Mesh(void);

  inline size_t NumVertices() const
  {
    return numVertices;
  }

  virtual void Draw();

  virtual void *GetBuffer(MeshBuffer b, GLenum mode);
  virtual bool ReturnBuffer(MeshBuffer b);

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