#include "Projectile.h"
#include "Renderer.h"
#include <cmath>

Projectile::Projectile(const Vector2& position, const Vector2& velocity, float damage, float lifetime)
    : m_position(position)
    , m_velocity(velocity)
    , m_damage(damage)
    , m_lifetime(lifetime)
    , m_maxLifetime(lifetime)
{
}

void Projectile::Update(float deltaTime) {
    // Update position
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    // Update lifetime
    m_lifetime -= deltaTime;
}

void Projectile::Render(Renderer* renderer) const {
    if (renderer && IsAlive()) {
        // Render as a small circle
        SDL_Color color = { 139, 69, 19, 255 }; // Brown color for claw
        renderer->DrawCircle(m_position, 3.0f, color);
    }
}

bool Projectile::CheckCollision(const Vector2& targetPos, float targetRadius) const {
    Vector2 diff = m_position - targetPos;
    float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
    return distance <= targetRadius;
}