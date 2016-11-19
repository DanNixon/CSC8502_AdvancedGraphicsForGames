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
  /**
   * @brief Used to store the state of a threaded array generation.
   */
  struct FractralArrayGenState
  {
    size_t x; //!< Length of X axis
    size_t y; //!< Length of Y axis
    float w;  //!< Length of step in X axis
    float d;  //!< Length of step in Y axis
  };

public:
  FractalBrownianMotion(const PerlinNoise &perlin);
  virtual ~FractalBrownianMotion();

  float Fractal(float x, float y) const;
  void FractalArrayThreaded(float *out, size_t x, size_t y, size_t numThreads = 0);

  /**
   * @brief Gets or sets the number of octaves.
   * @return Reference to number of octaves
   */
  inline size_t &NumOctaves()
  {
    return m_numOctaves;
  }

  /**
   * @brief Gets or sets the 3D perlin Z coordinate.
   * @return Reference to Z coordinate
   *
   * Z coordinate used in Perlin noise generation. Can be varied with time to generate animations, etc.
   */
  inline float &ZValue()
  {
    return m_zValue;
  }

  /**
   * @brief Gets or sets the frequency.
   * @return Reference to frequency
   */
  inline float &Frequency()
  {
    return m_frequency;
  }

  /**
   * @brief Gets or sets the amplitude.
   * @return Reference to amplitude
   */
  inline float &Amplitude()
  {
    return m_amplitude;
  }

  /**
   * @brief Gets or sets the lacunarity.
   * @return Reference to lacunarity
   */
  inline float &Lacunarity()
  {
    return m_lacunarity;
  }

  /**
   * @brief Gets or sets the persistence.
   * @return Reference to persistence
   */
  inline float &Persistence()
  {
    return m_persistence;
  }

  /**
   * @brief Gets or sets the linear amplitude (uniform multiplicative factor).
   * @return Reference to amplitude
   */
  inline float &UniformAmplitude()
  {
    return m_uniformAmplitude;
  }

  /**
   * @brief Gets or sets the offset (uniform additive factor).
   * @return Reference to offset
   */
  inline float &Offset()
  {
    return m_offset;
  }

protected:
  void FractalArray(float *out, size_t start, size_t end, FractralArrayGenState &state) const;

protected:
  const PerlinNoise &m_perlin; //!< Perlin noise generator

  size_t m_numOctaves; //!< Number of octaves
  float m_zValue;      //!< Z value passed to Perlin noise generator
  float m_frequency;
  float m_amplitude;
  float m_lacunarity;
  float m_persistence;

  float m_uniformAmplitude; //!< Uniform multiplicative factor applied to output
  float m_offset;           //!< Uniform additive factor applied to output
};
}
