/** @file */

#pragma once

namespace GraphicsCoursework
{
class PerlinNoise;

/**
 * @class FractalBrownianMotion
 * @brief Provides functionality for generating Fractal Brownian Motion in two dimensions.
 */
class FractalBrownianMotion
{
public:
  struct FractralArrayGenState
  {
    size_t x;
    size_t y;
    float w;
    float d;
  };

public:
  FractalBrownianMotion(const PerlinNoise &perlin);
  virtual ~FractalBrownianMotion();

  float Fractal(float x, float y) const;
  void FractalArrayThreaded(float *out, size_t x, size_t y, size_t numThreads = 0);

  inline size_t &NumOctaves()
  {
    return m_numOctaves;
  }

  inline float &ZValue()
  {
    return m_zValue;
  }

  inline float &Frequency()
  {
    return m_frequency;
  }

  inline float &Amplitude()
  {
    return m_amplitude;
  }

  inline float &Lacunarity()
  {
    return m_lacunarity;
  }

  inline float &Persistence()
  {
    return m_persistence;
  }

  inline float &UniformAmplitude()
  {
    return m_uniformAmplitude;
  }

  inline float &Offset()
  {
    return m_offset;
  }

protected:
  void FractalArray(float *out, size_t start, size_t end, FractralArrayGenState &state) const;

protected:
  const PerlinNoise &m_perlin; //!< Perlin noise generator

  size_t m_numOctaves; //!< Number of octaves
  float m_zValue; //!< Z value passed to Perlin noise generator
  float m_frequency;
  float m_amplitude;
  float m_lacunarity;
  float m_persistence;

  float m_uniformAmplitude; //!< Uniform multiplicative factor applied to output
  float m_offset; //!< Uniform additive factor applied to output
};
}
