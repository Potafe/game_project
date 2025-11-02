#include "World.h"
#include <random>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

World::World()
    : m_worldWidth(2000)
    , m_worldHeight(1000)
    , m_waveTime(0.0f)
{

}

void World::Update(float deltaTime) {
    // Update wave animation
    m_waveTime += deltaTime;
    
    // Update individual worlds
    m_humanWorld.Update(deltaTime);
    m_animalWorld.Update(deltaTime);
    m_demonWorld.Update(deltaTime);
    m_insectWorld.Update(deltaTime);
    
    UpdateWaves(deltaTime);
}

void World::Generate() {
    m_platforms.clear();
    m_terrainPoints.clear();
    
    // Generate simple ground line (x-axis)
    float groundHeight = 400.0f; // This represents y = 0 (x-axis)
    int terrainStep = 50;
    
    // Create a straight line for the x-axis where the player walks
    for (int x = -m_worldWidth; x <= m_worldWidth; x += terrainStep) {
        m_terrainPoints.push_back(Vector2(static_cast<float>(x), groundHeight));
    }
    
    // Initialize waves from all worlds
    UpdateWaves(0.0f);
}

bool World::IsColliding(const Vector2 &position, float radius) {
    // Check collision with wave terrain first
    float waveHeight = GetWaveHeight(position.x);
    if (position.y + radius >= waveHeight) {
        return true;
    }
    
    // Check collision with base terrain line
    float groundHeight = GetGroundHeight(position.x);
    if (position.y + radius >= groundHeight) {
        return true;
    }
    
    // Check collision with platforms
    for (const auto &platform: m_platforms) {
        float platformWidth = 80.0f;
        float platformHeight = 20.0f;

        if (position.x + radius > platform.x - platformWidth/2 &&
            position.x - radius < platform.x + platformWidth/2 &&
            position.y + radius > platform.y - platformHeight/2 &&
            position.y - radius < platform.y + platformHeight/2) {
            return true;
        }
    }

    return false;
}

float World::GetGroundHeight(float x) {
    // First check terrain points for ground height
    float groundHeight = 400.0f; // Default fallback
    
    // Find the terrain height by interpolating between terrain points
    if (m_terrainPoints.size() > 1) {
        for (size_t i = 0; i < m_terrainPoints.size() - 1; i++) {
            const Vector2& point1 = m_terrainPoints[i];
            const Vector2& point2 = m_terrainPoints[i + 1];
            
            if (x >= point1.x && x <= point2.x) {
                // Linear interpolation between the two points
                float t = (x - point1.x) / (point2.x - point1.x);
                groundHeight = point1.y + t * (point2.y - point1.y);
                break;
            }
        }
    }

    // Also check platforms for higher ground
    for (const auto &platform : m_platforms) {
        float platformWidth = 80.0f;

        if (x >= platform.x - platformWidth / 2 && x <= platform.x + platformWidth / 2) {
            if (platform.y < groundHeight) {
                groundHeight = platform.y;
            }
        }
    }

    return groundHeight;
}

float World::GetWaveHeight(float x) {
    float highestWave = 400.0f; // Start with ground level
    
    // Check all wave points to find the highest one at this x position
    for (const auto& wavePoint : m_allWaves) {
        // Check if this wave point is close to the requested x position
        if (abs(wavePoint.x - x) <= 15.0f) { // Within wave point spacing
            if (wavePoint.y < highestWave) { // Remember: lower y values are higher on screen
                highestWave = wavePoint.y;
            }
        }
    }
    
    return highestWave;
}

const vector<Vector2>& World::GetPlatforms() const {
    return m_platforms;
}

const vector<Vector2>& World::GetTerrainPoints() const {
    return m_terrainPoints;
}

const vector<Vector2>& World::GetSineWaves() const {
    return m_allWaves;
}

void World::UpdateWaves(float deltaTime) {
    // Generate waves from all individual worlds
    m_humanWorld.GenerateWaves(m_waveTime);
    m_animalWorld.GenerateWaves(m_waveTime);
    m_demonWorld.GenerateWaves(m_waveTime);
    m_insectWorld.GenerateWaves(m_waveTime);
    
    // Combine all waves into one vector for rendering
    m_allWaves.clear();
    
    // Add waves from each quadrant
    const auto& humanWaves = m_humanWorld.GetWavePoints();
    const auto& animalWaves = m_animalWorld.GetWavePoints();
    const auto& demonWaves = m_demonWorld.GetWavePoints();
    const auto& insectWaves = m_insectWorld.GetWavePoints();
    
    m_allWaves.insert(m_allWaves.end(), humanWaves.begin(), humanWaves.end());
    m_allWaves.insert(m_allWaves.end(), animalWaves.begin(), animalWaves.end());
    m_allWaves.insert(m_allWaves.end(), demonWaves.begin(), demonWaves.end());
    m_allWaves.insert(m_allWaves.end(), insectWaves.begin(), insectWaves.end());
}

WorldQuadrant World::GetQuadrant(float x, float y) const {
    // Determine quadrant based on mathematical definition
    if (x > 0 && y > 0) {
        return WorldQuadrant::HUMAN_WORLD;   // First quadrant (+x, +y)
    } else if (x < 0 && y > 0) {
        return WorldQuadrant::ANIMAL_WORLD;  // Second quadrant (-x, +y)
    } else if (x < 0 && y < 0) {
        return WorldQuadrant::DEMON_WORLD;   // Third quadrant (-x, -y)
    } else if (x > 0 && y < 0) {
        return WorldQuadrant::INSECT_WORLD;  // Fourth quadrant (+x, -y)
    } else {
        // On axis, default to human world
        return WorldQuadrant::HUMAN_WORLD;
    }
}

string World::GetQuadrantName(WorldQuadrant quadrant) const {
    switch (quadrant) {
        case WorldQuadrant::HUMAN_WORLD:
            return "Human World";
        case WorldQuadrant::ANIMAL_WORLD:
            return "Animal World";
        case WorldQuadrant::DEMON_WORLD:
            return "Demon World";
        case WorldQuadrant::INSECT_WORLD:
            return "Insect World";
        default:
            return "Unknown World";
    }
}

float World::GenerateTerrainHeight(float x, WorldQuadrant quadrant) const {
    switch (quadrant) {
        case WorldQuadrant::HUMAN_WORLD:
            return m_humanWorld.GetTerrainHeight(x);
        case WorldQuadrant::ANIMAL_WORLD:
            return m_animalWorld.GetTerrainHeight(x);
        case WorldQuadrant::DEMON_WORLD:
            return m_demonWorld.GetTerrainHeight(x);
        case WorldQuadrant::INSECT_WORLD:
            return m_insectWorld.GetTerrainHeight(x);
        default:
            return 400.0f; // Default height
    }
}



