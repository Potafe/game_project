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
    
    for (int x = -m_worldWidth; x < m_worldWidth; x += 100) {
        m_platforms.push_back(Vector2(static_cast<float>(x), 400.0f));
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> xDist(-static_cast<float>(m_worldWidth), static_cast<float>(m_worldWidth));
    uniform_real_distribution<float> yDist(200.0f, 350.0f);

    for (int i = 0; i < 20; i++) {
        float x = xDist(gen);
        float y = yDist(gen);
        m_platforms.push_back(Vector2(x, y));
    }
}

bool World::IsColliding(const Vector2 &position, float radius) {
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
    float groundHeight = 400.0f;

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



