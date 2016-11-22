/** @file */

#pragma once

#include "Buffer.h"
#include "OGLRenderer.h"

/**
 * @class Mesh
 * @brief Holds data for a simple renderable mesh.
 */
class Mesh
{
public:
  static Mesh *GenerateTriangle();
  static Mesh *GenerateQuad();
  static Mesh *GenerateSphere(size_t resolution = 64);

public:
  Mesh();
  virtual ~Mesh();

  /**
   * @brief Gets the number of vertices in this mesh.
   * @return Vertex count
   */
  inline size_t NumVertices() const
  {
    return m_numVertices;
  }

  /**
   * @brief Gets the number of indices in this mesh.
   * @return Index count
   */
  inline size_t NumIndices() const
  {
    return m_numIndices;
  }

  virtual void Draw();

  void SetGLPatches();
  void SetUniformColour(const Vector4 &colour);

  /**
   * @brief Gets the texture used with this mesh.
   * @return GL texture handle
   */
  inline GLuint GetTexture()
  {
    return m_texture;
  }

  /**
   * @brief Sets the texture used with this mesh.
   * @param t GL texture handle
   */
  inline void SetTexture(GLuint t)
  {
    m_texture = t;
  }

protected:
  void GenerateNormals();

  void GenerateTangents();
  Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector2 &ta, const Vector2 &tb,
                          const Vector2 &tc);

  void BufferAllData();

  void BufferData(Buffer b, GLuint width, void *data);
  void DeleteBuffer(Buffer b);

protected:
  GLuint m_arrayObject;               //!< GL array object handle
  GLuint m_bufferObjects[MAX_BUFFER]; //!< Handles for each GL buffer

  GLuint m_type;        //!< Draw type
  GLuint m_numVertices; //!< Number of vertices
  GLuint m_numIndices;  //!< Number of indices

  GLuint m_texture; //!< Associated texture

  Vector3 *m_vertices;      //!< Pointer to vertex data
  Vector4 *m_colours;       //!< Pointer to colour data
  Vector2 *m_textureCoords; //!< Pointer to texture coordinate data
  Vector3 *m_normals;       //!< Pointer to normal vector data
  Vector3 *m_tangents;      //!< Pointer to tangent vector data
  unsigned int *m_indices;  //!< Pointer to index data
};
