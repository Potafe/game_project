#include "weapons/Bullet.h"
#include "Renderer.h"

const float Bullet::MAX_LIFETIME = 3.0f; // 3 seconds

Bullet::Bullet(const Vector2& position, const Vector2& direction, float speed)
    : m_position(position)
    , m_velocity(direction * speed)
    , m_active(true)
    , m_lifetime(0.0f) {
}

void Bullet::Update(float deltaTime) {
    if (!m_active) return;
    
    m_position += m_velocity * deltaTime;
    m_lifetime += deltaTime;
    
    // Deactivate bullet after max lifetime
    if (m_lifetime >= MAX_LIFETIME) {
        m_active = false;
    }
}

void Bullet::Render(Renderer* renderer) const {
    if (!m_active) return;
    
    // Draw bullet as small circle - yellow color
    renderer->DrawCircle(m_position, 3.0f, {128, 128, 128, 255}); // Yellow bullet
}

bool Bullet::CheckCollision(const Vector2& targetPos, float targetRadius) const {
    if (!m_active) return false;
    
    Vector2 diff = m_position - targetPos;
    float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
    return distance <= targetRadius;
}