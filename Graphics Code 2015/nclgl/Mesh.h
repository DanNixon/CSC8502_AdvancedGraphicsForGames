#pragma once

#include "Buffer.h"
#include "OGLRenderer.h"

class Mesh
{
public:
  static Mesh *GenerateTriangle();
  static Mesh *GenerateQuad();
  static Mesh *GenerateSphere(size_t resolution = 64);

public:
  Mesh();
  virtual ~Mesh();

  inline size_t NumVertices() const
  {
    return m_numVertices;
  }

  virtual void Draw();

  virtual void *GetBuffer(Buffer b, GLenum mode);
  virtual bool ReturnBuffer(Buffer b);

  void SetUniformColour(const Vector4 &colour);

  inline GLuint GetTexture()
  {
    return m_texture;
  }

  inline void SetTexture(GLuint t)
  {
    m_texture = t;
  }

protected:
  void GenerateNormals();

  void GenerateTangents();
  Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector2 &ta, const Vector2 &tb,
                          const Vector2 &tc);

  void BufferData();
  void RegisterBuffer(Buffer b, GLuint width, void *data);

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
  Vector3 *m_normals;
  Vector3 *m_tangents;
  unsigned int *m_indices;
};