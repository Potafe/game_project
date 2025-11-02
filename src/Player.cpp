#include "Player.h"

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
{

}

void Player::Update(float deltaTime) {
    const float gravity = 800.0f;
    if (!m_onGround) {
        m_velocity.y += gravity * deltaTime;
    }

    if (abs(m_velocity.x) > 10.0f) {
        m_animationTime += deltaTime * m_animationSpeed;
    } else {
        m_animationTime = 0.0f;
    }

    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    float groundY = m_world ? m_world->GetGroundHeight(m_position.x) : 400.0f;
    
    // Adjust for leg length so feet land on ground (not the body base position)
    float legLength = 30.0f; // Should match LEG_LENGTH from StickFigureBody
    float feetY = m_position.y + legLength;
    
    if (feetY >= groundY) {
        m_position.y = groundY - legLength; // Position body so feet are on ground
        m_velocity.y = 0.0f;
        m_onGround = true;
    } else {
        m_onGround = false;
    }

    m_velocity.x *= 0.9f;

    // Update body position and animation with speed information
    m_body.SetPosition(m_position);
    float currentSpeed = sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y) / m_speed;
    m_body.UpdateAnimation(m_animationTime, m_facingRight, IsMoving(), currentSpeed);
    
    // Update body physics for realistic movement reactions
    m_body.UpdatePhysics(deltaTime);
    m_body.ReactToMovement(m_velocity, deltaTime);
}

void Player::Move(const Vector2 &direction) {
    if (direction.x < 0) {
        m_facingRight = false;
    } else if (direction.x > 0) {
        m_facingRight = true;
    }
    // Note: if direction.x == 0, keep current facing

    m_velocity.x += direction.x * m_speed;

    if (direction.y > 0 && m_onGround) {
        m_velocity.y = -m_jumpHeight;
        m_onGround = false;
    }

    const float maxSpeed = m_speed;
    if (m_velocity.x > maxSpeed) m_velocity.x = maxSpeed;
    if (m_velocity.x < -maxSpeed) m_velocity.x = -maxSpeed;
}
