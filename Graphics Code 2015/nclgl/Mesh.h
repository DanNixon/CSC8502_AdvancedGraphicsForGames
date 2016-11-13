#pragma once

#include "Buffer.h"
#include "OGLRenderer.h"

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
    return m_numVertices;
  }

  virtual void Draw();

  virtual void *GetBuffer(Buffer b, GLenum mode);
  virtual bool ReturnBuffer(Buffer b);

  inline GLuint GetTexture()
  {
    return m_texture;
  }

  inline void SetTexture(GLuint t)
  {
    m_texture = t;
  }

protected:
  void BufferData();

protected:
  GLuint m_arrayObject;
  GLuint m_bufferObjects[MAX_BUFFER];

  GLuint m_type;
  GLuint m_numVertices;
  GLuint m_numIndices;

  GLuint m_texture;

  Vector3 *m_vertices;
  Vector4 *m_colours;
  Vector2 *m_textureCoords;
  unsigned int *m_indices;
};