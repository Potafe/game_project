#include "../../include/world/HumanWorld.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;


HumanWorld::HumanWorld()
    : m_amplitude(60.0f)
    , m_frequency(0.05f)
    , m_worldWidth(2000)
{
}

void HumanWorld::Update(float deltaTime) {
    // Update any human world specific logic
}

void HumanWorld::GenerateWaves(float waveTime) {
    m_wavePoints.clear();
    // No waves - empty quadrant
}

const vector<Vector2>& HumanWorld::GetWavePoints() const {
    return m_wavePoints;
}

float HumanWorld::GetTerrainHeight(float x) const {
    // Sin wave pattern for Human World (First quadrant region)
    float baseHeight = 400.0f;
    float amplitude = 40.0f;      // Wave height
    float frequency = 0.008f;     // Wave frequency
    
    return baseHeight + amplitude * sin(x * frequency);
}
