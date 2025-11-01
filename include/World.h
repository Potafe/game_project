#pragma once
#include "Vector2.h"
#include <vector>

using namespace std;

class World {
public:
    World();

    void Update(float deltaTime);
    void Generate();

    bool IsColliding(const Vector2 &position, float radius);
    float GetGroundHeight(float x);
    const vector<Vector2>& GetPlatforms() const;

private:
    vector<Vector2> m_platforms;
    int m_worldWidth;
    int m_worldHeight;
};
