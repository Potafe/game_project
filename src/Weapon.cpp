#include "Weapon.h"

Weapon::Weapon()
    : m_position(0.0f, 0.0f)
    , m_cooldownTimer(0.0f)
{

}

void Weapon::Update(float deltaTime) {
    if (m_cooldownTimer > 0.0f) {
        m_cooldownTimer -= deltaTime;
        if (m_cooldownTimer < 0.0f) {
            m_cooldownTimer = 0.0f;
        }
    }
}

void Weapon::Use() {
    m_cooldownTimer = GetCooldown();
}

float Weapon::GetCooldownProgress() const {
    if (GetCooldown() <= 0.0f) return 0.0f;
    return m_cooldownTimer / GetCooldown();
}