#pragma once
#include "../Vector2.h"
#include <vector>

class InsectWorld {
public:
    InsectWorld();
    
    void Update(float deltaTime);
    void GenerateWaves(float waveTime);
    
    const std::vector<Vector2>& GetWavePoints() const;
    float GetTerrainHeight(float x) const;
    
private:
    std::vector<Vector2> m_wavePoints;
    float m_amplitude;
    float m_frequency;
    int m_worldWidth;
};
