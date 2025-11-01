#pragma once
#include "Vector2.h"
#include "World.h"

class Player {
public:
    Player();

    void Update(float deltaTime);
    void Move(const Vector2 &direction);
    void SetWorld(World* world) { m_world = world; }

    Vector2 GetPosition() const { return m_position; }
    void SetPosition(const Vector2 &position) { m_position = position; }

private:
    Vector2 m_position;
    Vector2 m_velocity;
    float m_speed;
    float m_jumpHeight;
    bool m_onGround;
    World* m_world;
};
