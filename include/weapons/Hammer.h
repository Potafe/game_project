#pragma once
#include "Weapon.h"

class Hammer : public Weapon {
public:
    Hammer();

    // Gun properties
    const char* GetName() const override { return "Hammer"; }
    int GetDamage() const override { return 2; }
    float GetRange() const override { return 50.0f; }
    float GetCooldown() const override { return 1; } // 1 hit per second

    void Use() override;

    // Hammer-specific properties
    bool IsCharged() const { return m_chargeLevel >= 1.0f; }
    void Charge(float deltaTime);
    float GetChargeLevel() const { return m_chargeLevel; }

private:
    float m_chargeLevel; // 0.0 to 1.0
    static const float CHARGE_RATE;
};