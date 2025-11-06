#pragma once
#include "Vector2.h"

class Projectile {
public:
    Projectile(const Vector2& position, const Vector2& velocity, float damage = 1.0f, float lifetime = 3.0f);
    
    void Update(float deltaTime);
    void Render(class Renderer* renderer) const;
    
    Vector2 GetPosition() const { return m_position; }
    bool IsAlive() const { return m_lifetime > 0.0f; }
    float GetDamage() const { return m_damage; }
    
    bool CheckCollision(const Vector2& targetPos, float targetRadius = 10.0f) const;

private:
    Vector2 m_position;
    Vector2 m_velocity;
    float m_damage;
    float m_lifetime;
    float m_maxLifetime;
};