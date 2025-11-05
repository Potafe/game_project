#pragma once
#include "../Vector2.h"

class Bullet {
public:
    Bullet (const Vector2 &position, const Vector2 &direction, float speed = 400.0f);

    void Update(float deltaTime);
    void Render(class Renderer* renderer) const;
    
    Vector2 GetPosition() const { return m_position; }
    bool IsActive() const { return m_active; }
    void Deactivate() { m_active = false; }
    
    // Collision detection
    bool CheckCollision(const Vector2& targetPos, float targetRadius) const;
    
private:
    Vector2 m_position;
    Vector2 m_velocity;
    bool m_active;
    float m_lifetime; // Max time bullet can exist
    static const float MAX_LIFETIME;
};