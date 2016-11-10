#include "Mesh.h"

Mesh *Mesh::GenerateTriangle()
{
  Mesh *m = new Mesh();
  m->m_numVertices = 3;

  m->m_vertices = new Vector3[m->m_numVertices];
  m->m_vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
  m->m_vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
  m->m_vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

  m->m_colours = new Vector4[m->m_numVertices];
  m->m_colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  m->m_colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  m->m_colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

  m->m_textureCoords = new Vector2[m->m_numVertices];
  m->m_textureCoords[0] = Vector2(0.5f, 0.0f);
  m->m_textureCoords[1] = Vector2(1.0f, 1.0f);
  m->m_textureCoords[2] = Vector2(0.0f, 1.0f);

  m->BufferData();

  return m;
}

Mesh *Mesh::GenerateSquare()
{
  Mesh *m = new Mesh();
  m->m_numVertices = 4;
  m->m_type = GL_TRIANGLE_STRIP;

  m->m_vertices = new Vector3[m->m_numVertices];
  m->m_vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
  m->m_vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
  m->m_vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
  m->m_vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

  m->m_colours = new Vector4[m->m_numVertices];
  m->m_colours[0] = Vector4(1.0f, 0.0f, 0.0f, 0.5f);
  m->m_colours[1] = Vector4(0.0f, 1.0f, 0.0f, 0.5f);
  m->m_colours[2] = Vector4(0.0f, 0.0f, 1.0f, 0.5f);
  m->m_colours[3] = Vector4(1.0f, 1.0f, 1.0f, 0.0f);

  m->m_textureCoords = new Vector2[m->m_numVertices];
  m->m_textureCoords[0] = Vector2(0.0f, 1.0f);
  m->m_textureCoords[1] = Vector2(0.0f, 0.0f);
  m->m_textureCoords[2] = Vector2(1.0f, 1.0f);
  m->m_textureCoords[3] = Vector2(1.0f, 0.0f);

  m->BufferData();

  return m;
}

Mesh *Mesh::GenerateSphere(size_t resolution)
{
  Mesh *m = new Mesh();
  m->m_numVertices = resolution * resolution * 2;
  m->m_type = GL_TRIANGLE_STRIP;

  m->m_vertices = new Vector3[m->m_numVertices];
  m->m_colours = new Vector4[m->m_numVertices];

  float deltaTheta = (float)(PI / resolution);
  float deltaPhi = (float)((PI * 2.0f) / resolution);

  size_t n = 0;
  for (size_t i = 0; i < resolution; i++)
  {
    const float theta1 = i * deltaTheta;
    const float theta2 = (i + 1) * deltaTheta;

    for (size_t j = 0; j < resolution; j++)
    {
      const float phi = j * deltaPhi;

      m->m_vertices[n] = Vector3(cos(theta1) * sin(phi), sin(theta1) * sin(phi), cos(phi));
      m->m_colours[n] = Vector4(theta1, phi, 0.0f, 1.0f);
      n++;

      m->m_vertices[n] = Vector3(cos(theta2) * sin(phi), sin(theta2) * sin(phi), cos(phi));
      m->m_colours[n] = Vector4(theta2, phi, 0.0f, 1.0f);
      n++;
    }
  }

  m->BufferData();

  return m;
}

Mesh::Mesh()
    : m_type(GL_TRIANGLES)
    , m_numVertices(0)
    , m_numIndices(0)
    , m_texture(0)
    , m_vertices(nullptr)
    , m_colours(nullptr)
    , m_textureCoords(nullptr)
    , m_indices(nullptr)
{
  for (int i = 0; i < MAX_BUFFER; ++i)
    m_bufferObjects[i] = 0;

  glGenVertexArrays(1, &m_arrayObject);
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &m_arrayObject);
  glDeleteBuffers(MAX_BUFFER, m_bufferObjects);
  glDeleteTextures(1, &m_texture);

  delete[] m_vertices;

  if (m_colours)
    delete[] m_colours;

  if (m_textureCoords)
    delete[] m_textureCoords;

  if (m_indices)
    delete[] m_indices;
}

void Mesh::Draw()
{
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glBindVertexArray(m_arrayObject);

  if (m_bufferObjects[INDEX_BUFFER] > 0)
    glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(m_type, 0, m_numVertices);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void *Mesh::GetBuffer(Buffer b, GLenum mode)
{
  return glMapBuffer(m_bufferObjects[b], mode);
}

bool Mesh::ReturnBuffer(Buffer b)
{
  return glUnmapBuffer(m_bufferObjects[b]) == GL_TRUE;
}

void Mesh::BufferData()
{
  glBindVertexArray(m_arrayObject);

  glGenBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[VERTEX_BUFFER]);
  glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector3), m_vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(VERTEX_BUFFER);

  if (m_textureCoords)
  {
    glGenBuffers(1, &m_bufferObjects[TEXTURE_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[TEXTURE_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector2), m_textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(TEXTURE_BUFFER);
  }

  if (m_colours)
  {
    glGenBuffers(1, &m_bufferObjects[COLOUR_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[COLOUR_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vector4), m_colours, GL_STATIC_DRAW);
    glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(COLOUR_BUFFER);
  }

  if (m_indices)
  {
    glGenBuffers(1, &m_bufferObjects[INDEX_BUFFER]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
}
