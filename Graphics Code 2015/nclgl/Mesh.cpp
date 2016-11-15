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

  m->GenerateNormals();
  m->BufferData();

  return m;
}

Mesh *Mesh::GenerateQuad()
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

  m->GenerateNormals();
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

  m->GenerateNormals();
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
    , m_normals(nullptr)
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

  if (m_colours != nullptr)
    delete[] m_colours;

  if (m_textureCoords != nullptr)
    delete[] m_textureCoords;

  if (m_normals != nullptr)
    delete[] m_normals;

  if (m_indices != nullptr)
    delete[] m_indices;
}

void Mesh::Draw()
{
  if (m_texture != 0)
    glBindTexture(GL_TEXTURE_2D, m_texture);

  glBindVertexArray(m_arrayObject);

  if (m_bufferObjects[INDEX_BUFFER] > 0)
    glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(m_type, 0, m_numVertices);

  glBindVertexArray(0);

  if (m_texture != 0)
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

void Mesh::GenerateNormals()
{
  if (m_type != GL_TRIANGLE_STRIP)
    return;

  if (m_normals == nullptr)
    m_normals = new Vector3[m_numVertices];

  for (GLuint i = 0; i < m_numVertices; ++i)
    m_normals[i] = Vector3();

  // Per vertex normals
  if (m_indices != nullptr)
  {
    GLuint i = 0;
    unsigned int a = m_indices[i++];
    unsigned int b = m_indices[i++];
    unsigned int c;

    for (; i < m_numIndices; i++)
    {
      c = m_indices[i++];

      Vector3 normal =
          Vector3::Cross((m_vertices[b] - m_vertices[a]), (m_vertices[c] - m_vertices[a]));

      m_normals[a] += normal;
      m_normals[b] += normal;
      m_normals[c] += normal;

      a = b;
      b = c;
    }
  }
  // Face normals
  else
  {
    for (GLuint i = 0; i < m_numVertices - 2; i++)
    {
      Vector3 &a = m_vertices[i];
      Vector3 &b = m_vertices[i + 1];
      Vector3 &c = m_vertices[i + 2];

      Vector3 normal = Vector3::Cross(b - a, c - a);

      m_normals[i] += normal;
      m_normals[i + 1] += normal;
      m_normals[i + 2] += normal;
    }
  }

  // Normalise all normals
  for (GLuint i = 0; i < m_numVertices; ++i)
    m_normals[i].Normalise();
}

void Mesh::BufferData()
{
  glBindVertexArray(m_arrayObject);

  RegisterBuffer(VERTEX_BUFFER, 3, m_vertices);

  if (m_textureCoords != nullptr)
    RegisterBuffer(TEXTURE_BUFFER, 2, m_textureCoords);

  if (m_colours != nullptr)
    RegisterBuffer(COLOUR_BUFFER, 4, m_colours);

  if (m_normals != nullptr)
    RegisterBuffer(NORMAL_BUFFER, 3, m_normals);

  if (m_indices != nullptr)
  {
    glGenBuffers(1, &m_bufferObjects[INDEX_BUFFER]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
}

void Mesh::RegisterBuffer(Buffer b, GLuint width, void *data)
{
  glGenBuffers(1, &m_bufferObjects[b]);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[b]);
  glBufferData(GL_ARRAY_BUFFER, m_numVertices * width * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(b, width, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(b);
}
