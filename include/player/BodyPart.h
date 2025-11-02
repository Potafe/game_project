#pragma once
#include "../Vector2.h"

enum class BodyPartType {
    HEAD,
    TORSO,
    LEFT_ARM,
    RIGHT_ARM,
    LEFT_LEG,
    RIGHT_LEG
};

class BodyPart {
public:
    BodyPart(BodyPartType type, const Vector2& position, float width, float height);

    // Position and dimensions
    void SetPosition(const Vector2& position) { m_position = position; }
    void SetRotation(float angle) { m_rotation = angle; }
    void SetDimensions(float width, float height) { m_width = width; m_height = height; }

    Vector2 GetPosition() const { return m_position; }
    float GetRotation() const { return m_rotation; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }
    BodyPartType GetType() const { return m_type; }

    // For limbs - start and end points based on rotation
    Vector2 GetStartPoint() const;
    Vector2 GetEndPoint() const;

    // Physics properties
    void ApplyOffset(const Vector2& offset);
    void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
    Vector2 GetVelocity() const { return m_velocity; }

    // Animation properties
    void SetSwingAmount(float swing) { m_swingAmount = swing; }
    float GetSwingAmount() const { return m_swingAmount; }

private:
    BodyPartType m_type;
    Vector2 m_position;      // Center position
    float m_rotation;        // Rotation angle in radians
    float m_width;          // Thickness/width
    float m_height;         // Length/height
    Vector2 m_velocity;     // For physics reactions
    float m_swingAmount;    // Current swing offset for animation
};
