#include "enemies/Animal.h"
#include "Player.h"
#include "Projectile.h"
#include "Renderer.h"
#include <iostream>
#include <cmath>

using namespace std;

Animal::Animal()
    : m_attackRange(500.0f) // Maximum attack range
    , m_clawRange(50.0f)   // Close-range claw attack
    , m_throwRange(500.0f) // Long-range thrown claw
{
    // Set animal-specific properties
    m_speed = 400.0f; // 2x player speed (200 * 1.5)
    m_health = Health(3); // 3 hearts instead of player's 5
    m_attackCooldown = 0.2f; // 0.2 second cooldown between attacks
}

void Animal::UpdateAI(float deltaTime, Player* player) {
    if (!player || player->GetHealth().IsDead()) return;

    Vector2 playerPos = player->GetPosition();
    Vector2 direction = playerPos - m_position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Update projectiles
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();) {
        (*it)->Update(deltaTime);
        
        // Check collision with player
        if ((*it)->CheckCollision(playerPos, 15.0f)) { // 15 is approximate player radius
            player->GetHealth().TakeDamage((*it)->GetDamage());
            it = m_projectiles.erase(it);
        } else if (!(*it)->IsAlive()) {
            it = m_projectiles.erase(it);
        } else {
            ++it;
        }
    }

    // AI behavior: approach player but maintain optimal attack distance
    if (distance > m_attackRange) {
        // Too far, move closer
        MoveTowards(playerPos, deltaTime);
    } else if (distance < m_clawRange) {
        // Too close, back away slightly
        Vector2 awayDirection = m_position - playerPos;
        if (sqrt(awayDirection.x * awayDirection.x + awayDirection.y * awayDirection.y) > 0) {
            awayDirection.x /= distance;
            awayDirection.y /= distance;
            m_velocity.x += awayDirection.x * m_speed * 0.5f;
            m_velocity.y += awayDirection.y * m_speed * 0.5f;
        }
    } else {
        // At good distance, circle around player occasionally
        // For now, just stand still and attack
        m_velocity.x *= 0.9f; // Slow down
        m_velocity.y *= 0.9f;
    }

    // Handle attacks (don't call base class attack logic)
    if (CanAttack(player) && m_attackCooldownTimer <= 0.0f) {
        Attack(player);
        m_attackCooldownTimer = m_attackCooldown;
    }
}

void Animal::Attack(Player* player) {
    if (!player || m_attackCooldownTimer > 0.0f) return;

    Vector2 playerPos = player->GetPosition();
    Vector2 direction = playerPos - m_position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= m_clawRange) {
        // Close range: claw attack
        ClawAttack(player);
    } else if (distance <= m_throwRange) {
        // Medium range: throw claw
        ThrowClawAttack(player);
    }
}

bool Animal::CanAttack(Player* player) const {
    if (!player || m_attackCooldownTimer > 0.0f) return false;

    Vector2 playerPos = player->GetPosition();
    Vector2 direction = playerPos - m_position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    return distance <= m_attackRange;
}

void Animal::ClawAttack(Player* player) {
    // Deal damage directly
    player->GetHealth().TakeDamage(1); // 1 heart damage
    m_attackCooldownTimer = m_attackCooldown;
    cout << "Animal CLAW ATTACK! Player health: " << player->GetHealth().GetCurrentHearts() << endl;
}

void Animal::ThrowClawAttack(Player* player) {
    // Create a projectile
    Vector2 playerPos = player->GetPosition();
    Vector2 direction = playerPos - m_position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;
    }

    // Create projectile with some speed
    Vector2 projectileVelocity = direction * 200.0f; // 200 units per second
    m_projectiles.push_back(make_unique<Projectile>(m_position, projectileVelocity, 1.0f, 2.0f));
    
    m_attackCooldownTimer = m_attackCooldown;

    cout << "Animal THROWN CLAW ATTACK! Projectile created." << endl;
}

void Animal::Render(Renderer* renderer) const {
    if (!renderer) return;

    // Render projectiles first
    for (const auto& projectile : m_projectiles) {
        projectile->Render(renderer);
    }

    // Render animal as a simple shape (could be improved with proper sprites)
    SDL_Color animalColor = { 139, 69, 19, 255 }; // Brown color
    
    // Draw a simple animal shape (circle for body, smaller circles for limbs)
    renderer->DrawCircle(m_position, 15.0f, animalColor); // Body
    
    // Draw limbs based on facing direction
    float limbOffset = m_facingRight ? 10.0f : -10.0f;
    renderer->DrawCircle(Vector2(m_position.x + limbOffset, m_position.y - 5.0f), 8.0f, animalColor); // Head
    renderer->DrawCircle(Vector2(m_position.x + limbOffset * 0.5f, m_position.y + 10.0f), 6.0f, animalColor); // Legs
    renderer->DrawCircle(Vector2(m_position.x - limbOffset * 0.5f, m_position.y + 10.0f), 6.0f, animalColor);
}