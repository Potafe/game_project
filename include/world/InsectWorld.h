#pragma once
#include "../Vector2.h"
#include <vector>

using namespace std;

class InsectWorld {
public:
    InsectWorld();
    
    void Update(float deltaTime);
    void GenerateWaves(float waveTime);
    
    const vector<Vector2>& GetWavePoints() const;
    float GetTerrainHeight(float x) const;
    
private:
    vector<Vector2> m_wavePoints;
    float m_amplitude;
    float m_frequency;
    int m_worldWidth;
};
