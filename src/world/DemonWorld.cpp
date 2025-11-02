#include "../../include/world/DemonWorld.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

DemonWorld::DemonWorld()
    : m_amplitude(45.0f)
    , m_frequency(0.06f)
    , m_worldWidth(2000)
{
}

void DemonWorld::Update(float deltaTime) {
    // Update any demon world specific logic
}

void DemonWorld::GenerateWaves(float waveTime) {
    m_wavePoints.clear();
    // No waves - empty quadrant
}

const std::vector<Vector2>& DemonWorld::GetWavePoints() const {
    return m_wavePoints;
}

float DemonWorld::GetTerrainHeight(float x) const {
    // Tangent-based pattern for Demon World (Third quadrant region)
    float baseHeight = 400.0f;
    float amplitude = 25.0f;
    float frequency = 0.015f;
    
    // Use atan to create a different wave pattern (bounded tangent)
    return baseHeight + amplitude * atan(sin(x * frequency)) * 2.0f;
}
