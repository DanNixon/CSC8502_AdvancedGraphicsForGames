#include "Mesh.h"

Mesh *Mesh::GenerateTriangle()
{
  Mesh *m = new Mesh();
  m->numVertices = 3;

  m->vertices = new Vector3[m->numVertices];
  m->vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
  m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
  m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

  m->colours = new Vector4[m->numVertices];
  m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

  m->textureCoords = new Vector2[m->numVertices];
  m->textureCoords[0] = Vector2(0.5f, 0.0f);
  m->textureCoords[1] = Vector2(1.0f, 1.0f);
  m->textureCoords[2] = Vector2(0.0f, 1.0f);

  m->BufferData();

  return m;
}

Mesh *Mesh::GenerateSquare()
{
  Mesh *m = new Mesh();
  m->numVertices = 4;
  m->type = GL_TRIANGLE_STRIP;

  m->vertices = new Vector3[m->numVertices];
  m->vertices[0] = Vector3(-0.5f, 0.5f, 0.0f);
  m->vertices[1] = Vector3(-0.5f, -0.5f, 0.0f);
  m->vertices[2] = Vector3(0.5f, 0.5f, 0.0f);
  m->vertices[3] = Vector3(0.5f, -0.5f, 0.0f);

  m->colours = new Vector4[m->numVertices];
  m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  m->colours[3] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

  m->BufferData();

  return m;
}

Mesh *Mesh::GenerateSphere(size_t resolution)
{
  Mesh *m = new Mesh();
  m->numVertices = resolution * resolution * 2;
  m->type = GL_TRIANGLE_STRIP;

  m->vertices = new Vector3[m->numVertices];
  m->colours = new Vector4[m->numVertices];

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

      m->vertices[n] = Vector3(cos(theta1) * sin(phi), sin(theta1) * sin(phi), cos(phi));
      m->colours[n] = Vector4(theta1, phi, 0.0f, 1.0f);
      n++;

      m->vertices[n] = Vector3(cos(theta2) * sin(phi), sin(theta2) * sin(phi), cos(phi));
      m->colours[n] = Vector4(theta2, phi, 0.0f, 1.0f);
      n++;
    }
  }

  m->BufferData();

  return m;
}

Mesh::Mesh()
{
  for (int i = 0; i < MAX_BUFFER; ++i)
    bufferObject[i] = 0;

  glGenVertexArrays(1, &arrayObject);
  numVertices = 0;
  vertices = NULL;
  colours = NULL;
  textureCoords = NULL;
  type = GL_TRIANGLES;
  texture = 0;
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &arrayObject);
  glDeleteBuffers(MAX_BUFFER, bufferObject);
  glDeleteTextures(1, &texture);

  delete[] vertices;

  if (colours)
    delete[] colours;

  if (textureCoords)
    delete[] textureCoords;
}

void Mesh::Draw()
{
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(arrayObject);
  glDrawArrays(type, 0, numVertices);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void *Mesh::GetBuffer(MeshBuffer b, GLenum mode)
{
  return glMapBuffer(bufferObject[b], mode);
}

bool Mesh::ReturnBuffer(MeshBuffer b)
{
  return glUnmapBuffer(bufferObject[b]) == GL_TRUE;
}

void Mesh::BufferData()
{
  glBindVertexArray(arrayObject);
  glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
  glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(VERTEX_BUFFER);

  if (textureCoords)
  {
    glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(TEXTURE_BUFFER);
  }

  if (colours)
  {
    glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), colours, GL_STATIC_DRAW);
    glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(COLOUR_BUFFER);
  }

  glBindVertexArray(0);
}
