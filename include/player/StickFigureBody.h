#pragma once
#include "BodyPart.h"
#include "../Vector2.h"
#include <memory>
#include <vector>

using namespace std;

class StickFigureBody {
public:
    StickFigureBody();

    void UpdateAnimation(float animationPhase, bool facingRight, bool isMoving, float speed = 1.0f);
    void UpdateAnimationWithOrientation(float animationPhase, bool facingRight, bool isMoving, float speed, bool horizontalMode);
    void SetPosition(const Vector2 &position);

    // Get individual body parts
    const BodyPart& GetHead() const { return m_head; }
    const BodyPart& GetTorso() const { return m_torso; }
    const BodyPart& GetLeftArm() const { return m_leftArm; }
    const BodyPart& GetRightArm() const { return m_rightArm; }
    const BodyPart& GetLeftLeg() const { return m_leftLeg; }
    const BodyPart& GetRightLeg() const { return m_rightLeg; }

    // Get all body parts for easy iteration
    const vector<BodyPart*> GetAllParts() const;
    
    // Get limb drawing order and visibility
    bool IsLimbInFront(BodyPartType limbType, float animationPhase) const;
    vector<BodyPart*> GetDrawingOrder(float animationPhase) const;
    float GetAnimationPhase() const { return m_currentAnimationPhase; }

    // Physics simulation
    void UpdatePhysics(float deltaTime);
    void ReactToMovement(const Vector2& velocity, float deltaTime);

private:
    // Body parts with proper dimensions
    BodyPart m_head;       // Circular head
    BodyPart m_torso;      // Main body
    BodyPart m_leftArm;    // Left arm with thickness
    BodyPart m_rightArm;   // Right arm with thickness
    BodyPart m_leftLeg;    // Left leg with thickness
    BodyPart m_rightLeg;   // Right leg with thickness

    Vector2 m_basePosition;
    
    // Animation states
    void UpdateIdlePose();
    void UpdateWalkingAnimation(float phase, bool facingRight);
    void UpdateJumpingPose();
    
    // Horizontal orientation animation states
    void UpdateHorizontalIdlePose();
    void UpdateHorizontalWalkingAnimation(float phase, bool facingRight);
    
    // Animation phase for drawing order
    float m_currentAnimationPhase;
    bool m_currentFacingRight;
    
    // Physics helpers
    void ApplyLimbSwing(BodyPart& limb, float swingAmount, float maxAngle);
    void UpdateLimbPhysics(BodyPart& limb, float deltaTime);
    void RotateBodyPartAroundCenter(BodyPart& part, const Vector2& center, float angle);
    
    // Body proportions
    static constexpr float HEAD_RADIUS = 12.0f;
    static constexpr float TORSO_WIDTH = 12.0f;  // Increased thickness
    static constexpr float TORSO_HEIGHT = 40.0f;
    static constexpr float ARM_WIDTH = 8.0f;     // Increased thickness
    static constexpr float ARM_LENGTH = 25.0f;
    static constexpr float LEG_WIDTH = 10.0f;    // Increased thickness
    static constexpr float LEG_LENGTH = 30.0f;
};
