/** @file */

#pragma once

#include <../nclgl/Mesh.h>
#include <functional>

namespace GraphicsCoursework
{
/**
 * @brief Structure of state data for a single particle.
 */
struct Particle
{
  Vector3 position;
  Vector3 direction;
  Vector4 colour;
};

/**
 * @class ParticleSystem
 * @brief Basic renderable particle emitter.
 */
class ParticleSystem : public Mesh
{
public:
  static float Rand();

public:
  typedef std::function<void(Particle &p)> NewParticleFunction;
  typedef std::function<void(Particle &p, float msec)> ParticleUpdateFunction;

public:
  ParticleSystem();
  virtual ~ParticleSystem();

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

  inline int GetLaunchParticles()
  {
    return m_numLaunchParticles;
  }

  virtual void SetLaunchParticles(int num)
  {
    m_numLaunchParticles = num;
  }

  NewParticleFunction &NewFunction()
  {
    return m_newParticleFunc;
  }

  ParticleUpdateFunction &UpdateFunction()
  {
    return m_particleUpdateFunc;
  }

  virtual void Update(float msec);
  virtual void Draw();

protected:
  virtual Particle *GetFreeParticle();
  void ResizeArrays();

protected:
  float m_particleRate;
  float m_particleLifetime;
  int m_numLaunchParticles;

  float m_nextParticleTime; //!< Time until next particles spawn

  size_t m_largestSize; //!< Largest size of particle array

  std::vector<Particle *> m_particles; //!< Active particles stay
  std::vector<Particle *> m_freeList;  //!< 'Spare' particles stay

  NewParticleFunction m_newParticleFunc;
  ParticleUpdateFunction m_particleUpdateFunc;
};
}
