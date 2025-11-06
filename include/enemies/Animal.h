#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>

using namespace std;

class Projectile; // Forward declaration for thrown claws

class Animal : public Enemy {
public:
    Animal();

    // Override Enemy methods
    void UpdateAI(float deltaTime, Player* player) override;
    void Attack(Player* player) override;
    bool CanAttack(Player* player) const override;
    float GetAttackRange() const override { return m_attackRange; }
    void Render(class Renderer* renderer) const override;

    // Animal-specific attacks
    void ClawAttack(Player* player); // Close-range claw attack
    void ThrowClawAttack(Player* player); // Long-range thrown claw

    // Get projectiles for rendering
    const vector<unique_ptr<Projectile>>& GetProjectiles() const { return m_projectiles; }

private:
    float m_attackRange; // Maximum range for attacks
    float m_clawRange; // Range for close claw attack
    float m_throwRange; // Range for thrown claw attack

    // Projectiles for thrown claws
    vector<unique_ptr<Projectile>> m_projectiles;
};