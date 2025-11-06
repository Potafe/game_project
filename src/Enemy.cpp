#include "Enemy.h"
#include "Player.h"
#include "World.h"
#include <cmath>

Enemy::Enemy()
    : m_position(0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f)
    , m_speed(150.0f) // Base enemy speed
    , m_facingRight(true)
    , m_animationTime(0.0f)
    , m_animationSpeed(1.5f)
    , m_health(1) // Default 1 heart, can be overridden
    , m_attackCooldown(1.0f) // 1 second base cooldown
    , m_attackCooldownTimer(0.0f)
{
}

void Enemy::Update(float deltaTime, Player* player, World* world) {
    // Update attack cooldown
    if (m_attackCooldownTimer > 0.0f) {
        m_attackCooldownTimer -= deltaTime;
    }

    // Update AI and movement
    UpdateAI(deltaTime, player);

    // Apply movement
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    // Apply basic terrain collision (similar to player)
    if (world) {
        float groundY = world->GetGroundHeight(m_position.x);
        float waveY = world->GetWaveHeight(m_position.x);
        float terrainY = min(groundY, waveY);

        // Simple ground collision
        if (m_position.y >= terrainY - 20.0f) { // 20 is approximate enemy height
            m_position.y = terrainY - 20.0f;
            m_velocity.y = 0.0f;
        }
    }

    // Apply velocity damping
    m_velocity.x *= 0.8f;
    m_velocity.y *= 0.8f;

    // Update animation
    if (abs(m_velocity.x) > 10.0f) {
        m_animationTime += deltaTime * m_animationSpeed;
    } else {
        m_animationTime = 0.0f;
    }

    // Update facing direction based on movement
    if (m_velocity.x > 10.0f) {
        m_facingRight = true;
    } else if (m_velocity.x < -10.0f) {
        m_facingRight = false;
    }
}

void Enemy::UpdateAI(float deltaTime, Player* player) {
    // Base AI: move towards player
    if (player) {
        Vector2 playerPos = player->GetPosition();
        Vector2 direction = playerPos - m_position;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        // Normalize direction
        if (distance > 0.0f) {
            direction.x /= distance;
            direction.y /= distance;
        }

        // Move towards player
        MoveTowards(playerPos, deltaTime);

        // Note: Attack logic is handled by derived classes in their UpdateAI overrides
    }
}

void Enemy::MoveTowards(const Vector2& target, float deltaTime) {
    Vector2 direction = target - m_position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f) {
        // Normalize direction
        direction.x /= distance;
        direction.y /= distance;

        // Apply movement
        m_velocity.x += direction.x * m_speed;
        m_velocity.y += direction.y * m_speed * 0.5f; // Slower vertical movement

        // Limit velocity
        float maxSpeed = m_speed;
        if (m_velocity.x > maxSpeed) m_velocity.x = maxSpeed;
        if (m_velocity.x < -maxSpeed) m_velocity.x = -maxSpeed;
        if (m_velocity.y > maxSpeed * 0.5f) m_velocity.y = maxSpeed * 0.5f;
        if (m_velocity.y < -maxSpeed * 0.5f) m_velocity.y = -maxSpeed * 0.5f;
    }
}