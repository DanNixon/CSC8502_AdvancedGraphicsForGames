#pragma once

#include "OGLRenderer.h"

enum MeshBuffer
{
  VERTEX_BUFFER,
  COLOUR_BUFFER,
  TEXTURE_BUFFER,
  MAX_BUFFER
};

class Mesh
{
public:
  static Mesh *GenerateTriangle();
  static Mesh *GenerateSquare();
  static Mesh *GenerateSphere(size_t resolution = 64);

public:
  Mesh();
  ~Mesh();

  inline size_t NumVertices() const
  {
    return numVertices;
  }

  virtual void Draw();

  virtual void *GetBuffer(MeshBuffer b, GLenum mode);
  virtual bool ReturnBuffer(MeshBuffer b);

  inline GLuint GetTexture()
  {
    return texture;
  }

  inline void SetTexture(GLuint t)
  {
    texture = t;
  }

protected:
  void BufferData();

protected:
  GLuint arrayObject;
  GLuint bufferObject[MAX_BUFFER];
  GLuint numVertices;
  GLuint type;
  GLuint texture;

  Vector3 *vertices;
  Vector4 *colours;
  Vector2 *textureCoords;
};