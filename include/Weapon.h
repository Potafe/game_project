#pragma once
#include "Vector2.h"

class Weapon {
public:
    Weapon();
    virtual ~Weapon() = default;
    
    // Weapon properties
    virtual const char* GetName() const = 0;
    virtual int GetDamage() const = 0;
    virtual float GetRange() const = 0;
    virtual float GetCooldown() const = 0;

    // For rendering
    void SetPosition(const Vector2& position) { m_position = position; }
    Vector2 GetPosition() const { return m_position; }
    
    // Animation
    virtual void Update(float deltaTime);
    virtual void Use();

    bool IsReady() const { return m_cooldownTimer <= 0.0f; }
    float GetCooldownProgress() const; // 0.0 to 1.0
    
public:
    Vector2 m_position;
    float m_cooldownTimer;
};
