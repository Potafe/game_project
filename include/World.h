#pragma once
#include "Vector2.h"
#include "world/HumanWorld.h"
#include "world/AnimalWorld.h"
#include "world/DemonWorld.h"
#include "world/InsectWorld.h"
#include <vector>
#include <string>

using namespace std;

enum class WorldQuadrant {
    HUMAN_WORLD,    // First quadrant (+x, +y)
    ANIMAL_WORLD,   // Second quadrant (-x, +y)
    DEMON_WORLD,    // Third quadrant (-x, -y)
    INSECT_WORLD    // Fourth quadrant (+x, -y)
};

class World {
public:
    World();

    void Update(float deltaTime);
    void Generate();

    bool IsColliding(const Vector2 &position, float radius);
    float GetGroundHeight(float x);
    float GetWaveHeight(float x); // New method to get wave terrain height
    const vector<Vector2>& GetPlatforms() const;
    const vector<Vector2>& GetTerrainPoints() const;
    const vector<Vector2>& GetSineWaves() const; // All waves combined
    
    WorldQuadrant GetQuadrant(float x, float y) const;
    string GetQuadrantName(WorldQuadrant quadrant) const;
    void UpdateWaves(float deltaTime); // Update wave animation

private:
    vector<Vector2> m_platforms;
    vector<Vector2> m_terrainPoints;  // Points defining the ground terrain line
    vector<Vector2> m_allWaves;       // Combined waves from all quadrants
    int m_worldWidth;
    int m_worldHeight;
    float m_waveTime;                 // Time for wave animation
    
    // Individual world instances
    HumanWorld m_humanWorld;
    AnimalWorld m_animalWorld;
    DemonWorld m_demonWorld;
    InsectWorld m_insectWorld;
    
    float GenerateTerrainHeight(float x, WorldQuadrant quadrant) const;
};
