#include "../../include/player/BodyPart.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

BodyPart::BodyPart(BodyPartType type, const Vector2& position, float width, float height)
    : m_type(type)
    , m_position(position)
    , m_rotation(0.0f)
    , m_width(width)
    , m_height(height)
    , m_velocity(0.0f, 0.0f)
    , m_swingAmount(0.0f)
{
}

Vector2 BodyPart::GetStartPoint() const {
    // For limbs, calculate start point based on rotation
    float halfHeight = m_height / 2.0f;
    float startX = m_position.x - sin(m_rotation) * halfHeight;
    float startY = m_position.y - cos(m_rotation) * halfHeight;
    return Vector2(startX, startY);
}

Vector2 BodyPart::GetEndPoint() const {
    // For limbs, calculate end point based on rotation
    float halfHeight = m_height / 2.0f;
    float endX = m_position.x + sin(m_rotation) * halfHeight;
    float endY = m_position.y + cos(m_rotation) * halfHeight;
    return Vector2(endX, endY);
}

void BodyPart::ApplyOffset(const Vector2& offset) {
    m_position.x += offset.x;
    m_position.y += offset.y;
}

Vector2 BodyPart::GetTopConnectionPoint() const {
    float halfHeight = m_height / 2.0f;
    float topX = m_position.x - sin(m_rotation) * halfHeight;
    float topY = m_position.y - cos(m_rotation) * halfHeight;
    return Vector2(topX, topY);
}

Vector2 BodyPart::GetBottomConnectionPoint() const {
    float halfHeight = m_height / 2.0f;
    float bottomX = m_position.x + sin(m_rotation) * halfHeight;
    float bottomY = m_position.y + cos(m_rotation) * halfHeight;
    return Vector2(bottomX, bottomY);
}

Vector2 BodyPart::GetLeftConnectionPoint() const {
    float halfWidth = m_width / 2.0f;
    float leftX = m_position.x - cos(m_rotation) * halfWidth;
    float leftY = m_position.y + sin(m_rotation) * halfWidth;
    return Vector2(leftX, leftY);
}

Vector2 BodyPart::GetRightConnectionPoint() const {
    float halfWidth = m_width / 2.0f;
    float rightX = m_position.x + cos(m_rotation) * halfWidth;
    float rightY = m_position.y - sin(m_rotation) * halfWidth;
    return Vector2(rightX, rightY);
}
