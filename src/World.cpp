#include "World.h"
#include <random>

using namespace std;

World::World()
    : m_worldWidth(2000)
    , m_worldHeight(1000)
{

}

void World::Update(float deltaTime) {
    // World updates (will add the needed animations, moving platforms, etc.)
}

void World::Generate() {
    m_platforms.clear();
    m_terrainPoints.clear();
    
    // Generate rugged terrain line
    float baseGroundHeight = 400.0f;
    int terrainStep = 20; // Distance between terrain points
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> heightVariation(-15.0f, 15.0f);
    
    // Generate terrain points across the world width
    for (int x = -m_worldWidth; x <= m_worldWidth; x += terrainStep) {
        float terrainHeight = baseGroundHeight + heightVariation(gen);
        m_terrainPoints.push_back(Vector2(static_cast<float>(x), terrainHeight));
    }
    
    // No floating platforms - just the terrain line
}

bool World::IsColliding(const Vector2 &position, float radius) {
    // Check collision with terrain line
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

const vector<Vector2>& World::GetPlatforms() const {
    return m_platforms;
}

const vector<Vector2>& World::GetTerrainPoints() const {
    return m_terrainPoints;
}



