#include "../../include/world/InsectWorld.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

InsectWorld::InsectWorld()
    : m_amplitude(55.0f)
    , m_frequency(0.045f)
    , m_worldWidth(2000)
{
}

void InsectWorld::Update(float deltaTime) {
    // Update any insect world specific logic
}

void InsectWorld::GenerateWaves(float waveTime) {
    m_wavePoints.clear();
    // No waves - empty quadrant
}

const std::vector<Vector2>& InsectWorld::GetWavePoints() const {
    return m_wavePoints;
}

float InsectWorld::GetTerrainHeight(float x) const {
    // Complex trigonometric pattern for Insect World (Fourth quadrant region)
    float baseHeight = 400.0f;
    float amplitude = 30.0f;
    float frequency = 0.012f;
    
    // Combination of sin and cos for complex pattern
    return baseHeight + amplitude * (sin(x * frequency) * cos(x * frequency * 0.5f));
}
