#include "Player.h"
#include "weapons/Gun.h"
#include "weapons/Hammer.h"
#include "weapons/Bullet.h"
#include <iostream>
#include <cstdio>

using namespace std;

Player::Player()
    : m_position(0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f)
    , m_speed(200.0f)
    , m_jumpHeight(300.0f)
    , m_onGround(false)
    , m_world(nullptr)
    , m_animationTime(0.0f)
    , m_animationSpeed(2.0f)
    , m_facingRight(true)
    , m_horizontalMode(false)
    , m_orientationCooldown(0.0f)
    , m_health(5)  // 5 hearts
    , m_currentWeapon(make_unique<Gun>())  // Default weapon
{

}

void Player::Update(float deltaTime) {
    // Update orientation cooldown
    if (m_orientationCooldown > 0.0f) {
        m_orientationCooldown -= deltaTime;
    }
    
    // Apply gravity only in normal (vertical) mode
    if (!m_horizontalMode) {
        const float gravity = 800.0f;
        if (!m_onGround) {
            m_velocity.y += gravity * deltaTime;
        }
    }

    if (abs(m_velocity.x) > 10.0f || (m_horizontalMode && abs(m_velocity.y) > 10.0f)) {
        m_animationTime += deltaTime * m_animationSpeed;
    } else {
        m_animationTime = 0.0f;
    }

    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    // Only apply terrain collision in normal (vertical) mode
    if (!m_horizontalMode) {
        // Get both ground height and wave height, use the higher one (lower y value)
        float groundY = m_world ? m_world->GetGroundHeight(m_position.x) : 400.0f;
        float waveY = m_world ? m_world->GetWaveHeight(m_position.x) : 400.0f;
        float terrainY = min(groundY, waveY); // Use the higher terrain (lower y value)
        
        // Adjust for leg length so feet land on terrain (not the body base position)
        float legLength = 30.0f; // Should match LEG_LENGTH from StickFigureBody
        float feetY = m_position.y + legLength;
        
        if (feetY >= terrainY) {
            m_position.y = terrainY - legLength; // Position body so feet are on terrain
            m_velocity.y = 0.0f;
            m_onGround = true;
        } else {
            m_onGround = false;
        }
    }

    // Apply velocity damping
    m_velocity.x *= 0.9f;
    
    // In horizontal mode, also apply damping to y-velocity
    if (m_horizontalMode) {
        m_velocity.y *= 0.9f;
    }

    // Update body position and animation with speed and orientation information
    m_body.SetPosition(m_position);
    float currentSpeed = sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y) / m_speed;
    m_body.UpdateAnimationWithOrientation(m_animationTime, m_facingRight, IsMoving(), currentSpeed, m_horizontalMode);
    
    // Update body physics for realistic movement reactions
    m_body.UpdatePhysics(deltaTime);
    m_body.ReactToMovement(m_velocity, deltaTime);
    
    // Update weapon
    if (m_currentWeapon) {
        m_currentWeapon->Update(deltaTime);
        
        // Special handling for hammer charging
        if (auto hammer = dynamic_cast<Hammer*>(m_currentWeapon.get())) {
            hammer->Charge(deltaTime);
        }
    }
}

void Player::Move(const Vector2 &direction) {
    // Always allow x-axis movement regardless of orientation
    if (direction.x < 0) {
        m_facingRight = false;
    } else if (direction.x > 0) {
        m_facingRight = true;
    }
    
    m_velocity.x += direction.x * m_speed;

    // Y-axis movement depends on orientation mode
    if (m_horizontalMode) {
        // In horizontal mode, y-direction moves the player up/down continuously
        m_velocity.y += direction.y * m_speed * 0.8f; // Slightly slower for control
    } else {
        // In normal mode, only jump when on ground and direction.y > 0
        if (direction.y > 0 && m_onGround) {
            m_velocity.y = -m_jumpHeight;
            m_onGround = false;
        }
        // Ignore downward input in normal mode (no crouching yet)
    }

    const float maxSpeed = m_speed;
    if (m_velocity.x > maxSpeed) m_velocity.x = maxSpeed;
    if (m_velocity.x < -maxSpeed) m_velocity.x = -maxSpeed;
    
    // Apply y-velocity limits in horizontal mode
    if (m_horizontalMode) {
        if (m_velocity.y > maxSpeed * 0.8f) m_velocity.y = maxSpeed * 0.8f;
        if (m_velocity.y < -maxSpeed * 0.8f) m_velocity.y = -maxSpeed * 0.8f;
    }
}

void Player::CycleWeapon() {
    if (dynamic_cast<Gun*>(m_currentWeapon.get())) {
        m_currentWeapon = std::make_unique<Hammer>();
    } else {
        m_currentWeapon = std::make_unique<Gun>();
    }
    cout << "Switched to weapon: " << m_currentWeapon->GetName() << endl;
}

void Player::FireGun() {
    if (auto gun = dynamic_cast<Gun*>(m_currentWeapon.get())) {
        if (gun->IsReady() && !gun->IsOutOfAmmo()) {
            // Calculate firing direction based on facing
            Vector2 fireDirection = m_facingRight ? Vector2(1.0f, 0.0f) : Vector2(-1.0f, 0.0f);
            
            // Get hand position for bullet spawn
            Vector2 handPos = GetHandPosition();
            
            // Create bullet
            gun->GetBullets().push_back(std::make_unique<Bullet>(handPos, fireDirection));
            
            // Use the gun (applies cooldown and reduces ammo)
            gun->Use();
        }
    }
}

Vector2 Player::GetHandPosition() const {
    // Get the right hand position based on current animation
    Vector2 basePos = m_position;
    
    if (m_horizontalMode) {
        // In horizontal mode, adjust hand position accordingly
        return basePos + Vector2(m_facingRight ? 25.0f : -25.0f, 0.0f);
    } else {
        // In vertical mode, hand is at shoulder level
        return basePos + Vector2(m_facingRight ? 15.0f : -15.0f, -40.0f);
    }
}