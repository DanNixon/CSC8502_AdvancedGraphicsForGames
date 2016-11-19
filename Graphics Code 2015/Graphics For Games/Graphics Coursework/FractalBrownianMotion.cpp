/** @file */

#include "FractalBrownianMotion.h"

#include <thread>

#include "PerlinNoise.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new Fractal Brownian motion generator.
 * @param perlin Perlin Noise generator to use
 */
FractalBrownianMotion::FractalBrownianMotion(const PerlinNoise &perlin)
    : m_perlin(perlin)
    , m_numOctaves(1)
    , m_zValue(0.5f)
    , m_frequency(1.0f)
    , m_amplitude(1.0f)
    , m_lacunarity(2.0f)
    , m_persistence(0.5f)
    , m_uniformAmplitude(1.0f)
    , m_offset(0.0f)
{
}

FractalBrownianMotion::~FractalBrownianMotion()
{
}

/**
 * @brief Gets the intensity of a given position.
 * @param x X position (0.0 - 1.0)
 * @param y Y position (0.0 - 1.0)
 * @return Intensity
 */
float FractalBrownianMotion::Fractal(float x, float y) const
{
  float retVal = 0.0f;

  float denom = 0.0f;
  float frequency(m_frequency);
  float amplitude(m_amplitude);

  for (size_t i = 0; i < m_numOctaves; i++)
  {
    retVal += (amplitude * m_perlin.Noise(x * frequency, y * frequency, m_zValue * frequency));

    denom += amplitude;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;
  }

  retVal /= denom;
  retVal *= m_uniformAmplitude;
  retVal += m_offset;

  return retVal;
}

/**
 * @brief Fills an array of floats with data fromt he generator, splitting the load over several threads.
 * @param out Output data array
 * @param x X dimension
 * @param y Y dimension
 * @param numThreads NUmber of threads (default is number of cores)
 */
void FractalBrownianMotion::FractalArrayThreaded(float *out, size_t x, size_t y, size_t numThreads)
{
  if (numThreads == 0)
    numThreads = std::thread::hardware_concurrency();

  const size_t len = x * y;

  size_t pointsPerThread = len / numThreads;
  if ((len % numThreads) > 0)
    pointsPerThread++;

  FractralArrayGenState state;
  state.x = x;
  state.y = y;
  state.w = 1.0f / (float)x;
  state.d = 1.0f / (float)y;

  std::vector<std::thread> threads;

  size_t start = 0;
  size_t end = pointsPerThread;
  for (size_t i = 0; i < numThreads; i++)
  {
    threads.push_back(std::thread(&FractalBrownianMotion::FractalArray, this, out, start, end, std::ref(state)));

    start = end;
    end += pointsPerThread;

    if (end > len)
      end = len;
  }

  for (auto it = threads.begin(); it != threads.end(); ++it)
  {
    if (it->joinable())
      it->join();
  }
}

/**
 * @brief Worker function for FractalArrayThreaded.
 * @param out Output data array
 * @param start Start index
 * @param end End index
 * @param state Generation state
 * @see FractalBrownianMotion::FractalArrayThreaded()
 */
void FractalBrownianMotion::FractalArray(float *out, size_t start, size_t end, FractralArrayGenState &state) const
{
  for (size_t i = start; i < end; i++)
    out[i] = Fractal((float)(i % state.x) * state.d, (float)(i / state.x) * state.w);
}
}
