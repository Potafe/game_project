#include "../../include/world/AnimalWorld.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

AnimalWorld::AnimalWorld()
    : m_amplitude(50.0f)
    , m_frequency(0.04f)
    , m_worldWidth(2000)
{
}

void AnimalWorld::Update(float deltaTime) {
    // Update any animal world specific logic
}

void AnimalWorld::GenerateWaves(float waveTime) {
    m_wavePoints.clear();
    // No waves - empty quadrant
}

const vector<Vector2>& AnimalWorld::GetWavePoints() const {
    return m_wavePoints;
}

float AnimalWorld::GetTerrainHeight(float x) const {
    // Cosine wave pattern for Animal World (Second quadrant region)
    float baseHeight = 400.0f;
    float amplitude = 35.0f;
    float frequency = 0.01f;
    
    return baseHeight + amplitude * cos(x * frequency);
}
