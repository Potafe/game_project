#pragma once
#include "Vector2.h"
#include "World.h"
#include "player/StickFigureBody.h"
#include "player/Health.h"
#include "Weapon.h"

class Player {
public:
    Player();

    void Update(float deltaTime);
    void Move(const Vector2 &direction);
    void SetWorld(World* world) { m_world = world; }

    Vector2 GetPosition() const { return m_position; }
    void SetPosition(const Vector2 &position) { m_position = position; }

    float GetAnimationPhase() const { return m_animationTime; }
    bool IsFacingRight() const { return m_facingRight; }
    bool IsMoving() const { return abs(m_velocity.x) > 10.0f || abs(m_velocity.y) > 10.0f; }
    bool IsHorizontalMode() const { return m_horizontalMode; }

    void SetHorizontalMode(bool horizontal) { m_horizontalMode = horizontal; }
    void ToggleOrientation() { m_horizontalMode = !m_horizontalMode; }
    bool CanToggleOrientation() const { return m_orientationCooldown <= 0.0f; }
    void TriggerOrientationToggle() { 
        if (CanToggleOrientation()) {
            ToggleOrientation(); 
            m_orientationCooldown = 0.3f; // 300ms cooldown
        }
    }

    const StickFigureBody& GetBody() const { return m_body; }

    // Health system
    Health& GetHealth() { return m_health; }
    const Health& GetHealth() const { return m_health; }
    
    // Weapon system
    Weapon* GetCurrentWeapon() { return m_currentWeapon.get(); }
    const Weapon* GetCurrentWeapon() const { return m_currentWeapon.get(); }
    void CycleWeapon();

    // Gun firing
    void FireGun();
    Vector2 GetHandPosition() const;

    // Input handling
    void HandleInput(float deltaTime);
    
private:
    Vector2 m_position;
    Vector2 m_velocity;
    float m_speed;
    float m_jumpHeight;
    bool m_onGround;
    World* m_world;
    bool m_facingRight;
    bool m_horizontalMode;      // True when traversing y-axis horizontally

    float m_animationTime;
    float m_animationSpeed;
    StickFigureBody m_body;
    
    // Orientation toggle cooldown
    float m_orientationCooldown;

    // Health and Weapon systems
    Health m_health;
    unique_ptr<Weapon> m_currentWeapon;
};