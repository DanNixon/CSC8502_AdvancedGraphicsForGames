/** @file */

#include "ParticleSystem.h"

namespace GraphicsCoursework
{
float ParticleSystem::Rand()
{
  return (rand() % 101) / 100.0f;
}

ParticleSystem::ParticleSystem()
    : Mesh()
    , m_particleRate(100.0f)
    , m_particleLifetime(500.0f)
    , m_nextParticleTime(0.0f)
    , m_numLaunchParticles(10)
    , m_largestSize(0)
{
  m_type = GL_POINTS;

  m_newParticleFunc = [](Vector3 &dir, Vector4 &col) {
    col = Vector4(Rand(), Rand(), Rand(), 1.0);

    dir = Vector3(0.0f, 0.0f, 0.0f);
    dir.x += ((Rand() - Rand()) * 0.5f);
    dir.y += ((Rand() - Rand()) * 0.5f);
    dir.z += ((Rand() - Rand()) * 0.5f);
  };

  m_particleUpdateFunc = [this](Particle &p, float msec) { p.position += p.direction * (msec * 0.2); };
}

ParticleSystem::~ParticleSystem()
{
  for (auto it = m_particles.begin(); it != m_particles.end(); ++it)
    delete *it;

  for (auto it = m_freeList.begin(); it != m_freeList.end(); ++it)
    delete *it;
}

void ParticleSystem::Update(float msec)
{
  m_nextParticleTime -= msec;

  while (m_nextParticleTime <= 0)
  {
    m_nextParticleTime += m_particleRate;

    // Add a number of particles to the vector, obtained from the free list.
    for (int i = 0; i < m_numLaunchParticles; ++i)
      m_particles.push_back(GetFreeParticle());
  }

  // Now for the particle 'think' function. Particles are so 'lightweight' there's not
  // much point putting this as a member variable of the Particle struct...

  for (auto it = m_particles.begin(); it != m_particles.end();)
  {
    Particle *p = (*it);

    // We're keeping the particles 'life' in the alpha channel of its colour.
    // This means it'll also fade out as it loses energy. Kinda cool?
    p->colour.w -= (msec / m_particleLifetime);

    // If this particle has ran out of life, remove it from the 'active' list,
    // and put it on the 'free' list for reuse later.
    if (p->colour.w <= 0.0f)
    {
      m_freeList.push_back(p);
      it = m_particles.erase(it);
    }
    else
    {
      m_particleUpdateFunc(*p, msec);
      ++it;
    }
  }

  // If we now have more particles than we have graphics memory for, we
  // must allocate some new space for them, using ResizeArrays.
  if (m_particles.size() > m_largestSize)
    ResizeArrays();
}

void ParticleSystem::Draw()
{
  // Get 2 contiguous sections of memory full of our particle info
  for (unsigned int i = 0; i < m_particles.size(); ++i)
  {
    m_vertices[i] = m_particles.at(i)->position;
    m_colours[i] = m_particles.at(i)->colour;
  }

  glBindVertexArray(m_arrayObject);

  // Bind vertex data
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[VERTEX_BUFFER]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_particles.size() * sizeof(Vector3), (void *)m_vertices);
  glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
  glEnableVertexAttribArray(VERTEX_BUFFER);

  // Bind colour data
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[COLOUR_BUFFER]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_particles.size() * sizeof(Vector4), (void *)m_colours);
  glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
  glEnableVertexAttribArray(COLOUR_BUFFER);

  glDrawArrays(m_type, 0, m_particles.size());

  glBindVertexArray(0);
}

Particle *ParticleSystem::GetFreeParticle()
{
  Particle *p = nullptr;

  // Try to return an existing particle
  if (m_freeList.size() > 0)
  {
    p = m_freeList.back();
    m_freeList.pop_back();
  }
  else
  {
    p = new Particle();
  }

  // Randomise data
  m_newParticleFunc(p->direction, p->colour);

  p->direction.Normalise();
  p->position.ToZero();

  return p;
}

void ParticleSystem::ResizeArrays()
{
  delete[] m_vertices;
  delete[] m_colours;

  glDeleteBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  glDeleteBuffers(1, &m_bufferObjects[COLOUR_BUFFER]);

  m_vertices = new Vector3[m_particles.size()];
  m_colours = new Vector4[m_particles.size()];

  glGenBuffers(1, &m_bufferObjects[VERTEX_BUFFER]);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[VERTEX_BUFFER]);
  glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Vector3), 0, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &m_bufferObjects[COLOUR_BUFFER]);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[COLOUR_BUFFER]);
  glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Vector4), 0, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_largestSize = m_particles.size();
}
}