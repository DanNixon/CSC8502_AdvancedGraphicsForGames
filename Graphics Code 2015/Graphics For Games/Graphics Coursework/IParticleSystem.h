/** @file */

#pragma once

#include <../nclgl/Mesh.h>

namespace GraphicsCoursework
{
struct Particle
{
  Vector3 position;
  Vector4 colour;
  Vector3 direction;
};

class IParticleSystem : public Mesh
{
public:
  IParticleSystem();
  virtual ~IParticleSystem();

  inline float GetParticleRate()
  {
    return m_particleRate;
  }

  virtual void SetParticleRate(float rate)
  {
    m_particleRate = rate;
  }

  inline float GetParticleLifetime()
  {
    return m_particleLifetime;
  }

  virtual void SetParticleLifetime(float life)
  {
    m_particleLifetime = life;
  }

  inline float GetParticleSize()
  {
    return m_particleSize;
  }

  virtual void SetParticleSize(float size)
  {
    m_particleSize = size;
  }

  inline float GetParticleVariance()
  {
    return m_particleVariance;
  }

  virtual void SetParticleVariance(float variance)
  {
    m_particleVariance = variance;
  }

  inline float GetParticleSpeed()
  {
    return m_particleSpeed;
  }

  virtual void SetParticleSpeed(float speed)
  {
    m_particleSpeed = speed;
  }

  inline int GetLaunchParticles()
  {
    return m_numLaunchParticles;
  }

  virtual void SetLaunchParticles(int num)
  {
    m_numLaunchParticles = num;
  }

  inline Vector3 GetDirection()
  {
    return m_initialDirection;
  }

  virtual void SetDirection(const Vector3 dir)
  {
    m_initialDirection = dir;
  }

  virtual void Update(float msec);
  virtual void Draw();

protected:
  Particle *GetFreeParticle();
  void ResizeArrays();

protected:
  float m_particleRate;
  float m_particleLifetime;
  float m_particleSize;
  float m_particleVariance;
  float m_particleSpeed;
  int m_numLaunchParticles;

  Vector3 m_initialDirection;

  float m_nextParticleTime; // How long until we next spit out some particles?

  unsigned int m_largestSize; // How large has our particle array become?

  std::vector<Particle *> m_particles; // Active particles stay in here :)
  std::vector<Particle *> m_freeList;  //'Spare' particles stay in here...
};
}