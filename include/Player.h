#pragma once
#include "Vector2.h"
#include "World.h"
#include "player/StickFigureBody.h"

class Player {
public:
    Player();

    void Update(float deltaTime);
    void Move(const Vector2 &direction);
    void SetWorld(World* world) { m_world = world; }

    Vector2 GetPosition() const { return m_position; }
    void SetPosition(const Vector2 &position) { m_position = position; }

    float GetAnimationPhase() const { return m_animationTime; }
    bool IsFacingRight() const { return m_facingRight; }
    bool IsMoving() const { return abs(m_velocity.x) > 10.0f; }

    const StickFigureBody& GetBody() const { return m_body; }

private:
    Vector2 m_position;
    Vector2 m_velocity;
    float m_speed;
    float m_jumpHeight;
    bool m_onGround;
    World* m_world;
    bool m_facingRight;

    float m_animationTime;
    float m_animationSpeed;
    StickFigureBody m_body;
};
