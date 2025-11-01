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
    if (m_position.y >= groundY) {
        m_position.y = groundY;
        m_velocity.y = 0.0f;
        m_onGround = true;
    } else {
        m_onGround = false;
    }

    m_velocity.x *= 0.9f;
}

void Player::Move(const Vector2 &direction) {
    m_velocity.x += direction.x * m_speed;

    if (direction.y > 0 && m_onGround) {
        m_velocity.y = -m_jumpHeight;
        m_onGround = false;
    }

    const float maxSpeed = m_speed;
    if (m_velocity.x > maxSpeed) m_velocity.x = maxSpeed;
    if (m_velocity.x < -maxSpeed) m_velocity.x = -maxSpeed;
}
