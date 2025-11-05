#pragma once
#include "Weapon.h"
#include "Bullet.h"
#include <vector>
#include <memory>

class Gun : public Weapon {
public:
    Gun();

    // Gun properties
    const char* GetName() const override { return "Gun"; }
    int GetDamage() const override { return 1; }
    float GetRange() const override { return 300.0f; }
    float GetCooldown() const override { return 0.5f; } // 2 shots per second

    void Use() override;
    void Update(float deltaTime) override;

    // Gun-specific properties
    int GetAmmo() const { return m_ammo; }
    void Reload();
    bool IsOutOfAmmo() const { return m_ammo <= 0; }

    // Bullet management
    std::vector<std::unique_ptr<Bullet>>& GetBullets() { return m_bullets; }
    const std::vector<std::unique_ptr<Bullet>>& GetBullets() const { return m_bullets; }

private:
    int m_ammo;
    static const int MAX_AMMO = 30;
    std::vector<std::unique_ptr<Bullet>> m_bullets;
};