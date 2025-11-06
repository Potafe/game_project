#pragma once
#include "Vector2.h"
#include "player/Health.h"
#include <memory>

class Player;
class World;

class Enemy {
public:
    Enemy();
    virtual ~Enemy() = default;

    // Core enemy properties
    void Update(float deltaTime, Player* player, World* world);
    void SetPosition(const Vector2& position) { m_position = position; }
    Vector2 GetPosition() const { return m_position; }

    // Movement and AI
    virtual void UpdateAI(float deltaTime, Player* player);
    virtual void MoveTowards(const Vector2& target, float deltaTime);

    // Combat
    virtual void Attack(Player* player) = 0; // Pure virtual - must be implemented by subclasses
    virtual bool CanAttack(Player* player) const = 0;
    virtual float GetAttackRange() const = 0;

    // Health system
    Health& GetHealth() { return m_health; }
    const Health& GetHealth() const { return m_health; }
    bool IsDead() const { return m_health.IsDead(); }

    // Rendering
    virtual void Render(class Renderer* renderer) const = 0;

    // Animation
    float GetAnimationPhase() const { return m_animationTime; }
    bool IsFacingRight() const { return m_facingRight; }

protected:
    Vector2 m_position;
    Vector2 m_velocity;
    float m_speed;
    bool m_facingRight;

    float m_animationTime;
    float m_animationSpeed;

    Health m_health;

    // Attack cooldown
    float m_attackCooldown;
    float m_attackCooldownTimer;
};
