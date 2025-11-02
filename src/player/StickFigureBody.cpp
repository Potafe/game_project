#include "../../include/player/StickFigureBody.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

StickFigureBody::StickFigureBody()
    : m_head(BodyPartType::HEAD, Vector2(0, 0), HEAD_RADIUS * 2, HEAD_RADIUS * 2)
    , m_torso(BodyPartType::TORSO, Vector2(0, 0), TORSO_WIDTH, TORSO_HEIGHT)
    , m_leftArm(BodyPartType::LEFT_ARM, Vector2(0, 0), ARM_WIDTH, ARM_LENGTH)
    , m_rightArm(BodyPartType::RIGHT_ARM, Vector2(0, 0), ARM_WIDTH, ARM_LENGTH)
    , m_leftLeg(BodyPartType::LEFT_LEG, Vector2(0, 0), LEG_WIDTH, LEG_LENGTH)
    , m_rightLeg(BodyPartType::RIGHT_LEG, Vector2(0, 0), LEG_WIDTH, LEG_LENGTH)
    , m_basePosition(0, 0)
    , m_currentAnimationPhase(0.0f)
    , m_currentFacingRight(true)
{
}

void StickFigureBody::SetPosition(const Vector2& position) {
    m_basePosition = position;
}

const std::vector<BodyPart*> StickFigureBody::GetAllParts() const {
    return {
        const_cast<BodyPart*>(&m_head),
        const_cast<BodyPart*>(&m_torso),
        const_cast<BodyPart*>(&m_leftArm),
        const_cast<BodyPart*>(&m_rightArm),
        const_cast<BodyPart*>(&m_leftLeg),
        const_cast<BodyPart*>(&m_rightLeg)
    };
}

void StickFigureBody::UpdateAnimation(float animationPhase, bool facingRight, bool isMoving, float speed) {
    m_currentAnimationPhase = animationPhase;
    m_currentFacingRight = facingRight;
    
    if (isMoving) {
        UpdateWalkingAnimation(animationPhase, facingRight);
    } else {
        UpdateIdlePose();
    }
}

void StickFigureBody::UpdateAnimationWithOrientation(float animationPhase, bool facingRight, bool isMoving, float speed, bool horizontalMode) {
    m_currentAnimationPhase = animationPhase;
    m_currentFacingRight = facingRight;
    
    if (horizontalMode) {
        // In horizontal mode, body parts should be rotated 90 degrees
        if (isMoving) {
            UpdateHorizontalWalkingAnimation(animationPhase, facingRight);
        } else {
            UpdateHorizontalIdlePose();
        }
    } else {
        // Normal vertical mode
        if (isMoving) {
            UpdateWalkingAnimation(animationPhase, facingRight);
        } else {
            UpdateIdlePose();
        }
    }
}

void StickFigureBody::UpdateIdlePose() {
    // Head position - on top of torso with small gap
    Vector2 headPos = Vector2(m_basePosition.x, m_basePosition.y - TORSO_HEIGHT - HEAD_RADIUS - 5.0f); // Added 5px gap
    m_head.SetPosition(headPos);
    m_head.SetRotation(0.0f);
    
    // Torso position (centered, vertical)
    Vector2 torsoPos = Vector2(m_basePosition.x, m_basePosition.y - TORSO_HEIGHT / 2);
    m_torso.SetPosition(torsoPos);
    m_torso.SetRotation(0.0f);
    
    // Calculate shoulder position (top of torso)
    Vector2 shoulderPos = Vector2(m_basePosition.x, m_basePosition.y - TORSO_HEIGHT + 5.0f);
    
    // Left arm - positioned at 45 degrees outward from torso 
    Vector2 leftShoulderPos = Vector2(shoulderPos.x - TORSO_WIDTH/2, shoulderPos.y);
    Vector2 leftArmCenter = Vector2(
        leftShoulderPos.x + (ARM_LENGTH/2) * sin(-0.785f),  // 45 degree angle outward (negative for left)
        leftShoulderPos.y + (ARM_LENGTH/2) * cos(-0.785f)
    );
    m_leftArm.SetPosition(leftArmCenter);
    m_leftArm.SetRotation(-0.785f); // 45 degrees outward to the left
    
    // Right arm - positioned at 45 degrees outward from torso 
    Vector2 rightShoulderPos = Vector2(shoulderPos.x + TORSO_WIDTH/2, shoulderPos.y);
    Vector2 rightArmCenter = Vector2(
        rightShoulderPos.x + (ARM_LENGTH/2) * sin(0.785f),  // 45 degree angle outward (positive for right)
        rightShoulderPos.y + (ARM_LENGTH/2) * cos(0.785f)
    );
    m_rightArm.SetPosition(rightArmCenter);
    m_rightArm.SetRotation(0.785f); // 45 degrees outward to the right
    
    // Calculate hip position (bottom of torso)
    Vector2 hipPos = Vector2(m_basePosition.x, m_basePosition.y - 5.0f);
    
    // Left leg - positioned at 45 degrees outward from torso for wider stance
    Vector2 leftHipPos = Vector2(hipPos.x - TORSO_WIDTH/3, hipPos.y);
    Vector2 leftLegCenter = Vector2(
        leftHipPos.x + (LEG_LENGTH/2) * sin(-0.785f),  // 45 degree spread outward (negative for left)
        leftHipPos.y + (LEG_LENGTH/2) * cos(-0.785f)
    );
    m_leftLeg.SetPosition(leftLegCenter);
    m_leftLeg.SetRotation(-0.785f); // 45 degrees outward to the left
    
    // Right leg - positioned at 45 degrees outward from torso for wider stance
    Vector2 rightHipPos = Vector2(hipPos.x + TORSO_WIDTH/3, hipPos.y);
    Vector2 rightLegCenter = Vector2(
        rightHipPos.x + (LEG_LENGTH/2) * sin(0.785f),  // 45 degree spread outward (positive for right)
        rightHipPos.y + (LEG_LENGTH/2) * cos(0.785f)
    );
    m_rightLeg.SetPosition(rightLegCenter);
    m_rightLeg.SetRotation(0.785f); // 45 degrees outward to the right
}

void StickFigureBody::UpdateWalkingAnimation(float phase, bool facingRight) {
    // Create alternating leg movement with more dramatic swings for running
    float leftLegSwing = sin(phase * 2 * M_PI) * 0.6f; // Increased swing for running
    float rightLegSwing = sin((phase * 2 * M_PI) + M_PI) * 0.6f; // 180 degrees opposite
    
    // Arms swing opposite to legs with dramatic movement
    float leftArmSwing = sin((phase * 2 * M_PI) + M_PI) * 0.6f; // Opposite to left leg
    float rightArmSwing = sin(phase * 2 * M_PI) * 0.6f; // Opposite to right leg
    
    // Calculate leaning based on movement direction
    float leanAngle = facingRight ? -0.14f : 0.14f; // Lean forward in movement direction
    float additionalLean = sin(phase * 4 * M_PI) * 0.05f; // Slight rhythmic lean
    float totalLean = leanAngle + additionalLean;
    
    // Head position with leaning and reduced bob for running (separated from torso)
    float headBob = sin(phase * 4 * M_PI) * 0.5f; // Reduced bob for running
    
    // Torso with pronounced forward lean
    Vector2 torsoBasePos = Vector2(m_basePosition.x, m_basePosition.y - TORSO_HEIGHT / 2);
    float torsoLeanOffset = sin(totalLean) * 6.0f;
    Vector2 actualTorsoPos = Vector2(torsoBasePos.x + torsoLeanOffset, torsoBasePos.y - 2.0f);
    m_torso.SetPosition(actualTorsoPos); // Slightly lower when running
    m_torso.SetRotation(totalLean); // Lean torso in movement direction
    
    // Position head above the actual torso position with additional forward lean
    m_head.SetPosition(Vector2(actualTorsoPos.x + (facingRight ? 2.0f : -2.0f), actualTorsoPos.y - TORSO_HEIGHT/2 - HEAD_RADIUS - 5.0f + headBob));
    m_head.SetRotation(totalLean * 0.5f + (facingRight ? 0.1f : -0.1f)); // Head tilts in motion direction
    
    // Arms connected to torso shoulders with dramatic alternating swing
    Vector2 torsoPos = m_torso.GetPosition();
    float torsoRotation = m_torso.GetRotation();
    
    // Calculate shoulder positions - simplified to stay connected
    Vector2 leftShoulderPos = Vector2(
        torsoPos.x - TORSO_WIDTH/1.9,
        torsoPos.y - TORSO_HEIGHT/1.9 + 5.0f  // Slightly below top of torso
    );
    Vector2 rightShoulderPos = Vector2(
        torsoPos.x + TORSO_WIDTH/1.9,
        torsoPos.y - TORSO_HEIGHT/1.9 + 5.0f  // Slightly below top of torso
    );
    
    // Left arm - positioned so it starts at shoulder and extends outward
    Vector2 leftArmCenter = Vector2(
        leftShoulderPos.x + (ARM_LENGTH/2) * sin(leftArmSwing + totalLean * 0.5f),
        leftShoulderPos.y + (ARM_LENGTH/2) * cos(leftArmSwing + totalLean * 0.5f)
    );
    m_leftArm.SetPosition(leftArmCenter);
    m_leftArm.SetRotation(leftArmSwing + totalLean * 0.5f);
    
    // Right arm - positioned so it starts at shoulder and extends outward
    Vector2 rightArmCenter = Vector2(
        rightShoulderPos.x + (ARM_LENGTH/2) * sin(rightArmSwing + totalLean * 0.5f),
        rightShoulderPos.y + (ARM_LENGTH/2) * cos(rightArmSwing + totalLean * 0.5f)
    );
    m_rightArm.SetPosition(rightArmCenter);
    m_rightArm.SetRotation(rightArmSwing + totalLean * 0.5f);
    
    // Calculate hip positions - simplified to stay connected
    Vector2 leftHipPos = Vector2(
        torsoPos.x - TORSO_WIDTH/2,
        torsoPos.y + TORSO_HEIGHT/1.9 - 5.0f  // Slightly above bottom of torso
    );
    Vector2 rightHipPos = Vector2(
        torsoPos.x + TORSO_WIDTH/2,
        torsoPos.y + TORSO_HEIGHT/1.9 - 5.0f  // Slightly above bottom of torso
    );
    
    // Left leg - positioned so it starts at hip and extends downward
    Vector2 leftLegCenter = Vector2(
        leftHipPos.x + (LEG_LENGTH/2) * sin(leftLegSwing + totalLean * 0.3f),
        leftHipPos.y + (LEG_LENGTH/2) * cos(leftLegSwing + totalLean * 0.3f)
    );
    
    m_leftLeg.SetPosition(leftLegCenter);
    m_leftLeg.SetRotation(leftLegSwing + totalLean * 0.3f);
    
    // Right leg - positioned so it starts at hip and extends downward
    Vector2 rightLegCenter = Vector2(
        rightHipPos.x + (LEG_LENGTH/2) * sin(rightLegSwing + totalLean * 0.3f),
        rightHipPos.y + (LEG_LENGTH/2) * cos(rightLegSwing + totalLean * 0.3f)
    );
    m_rightLeg.SetPosition(rightLegCenter);
    m_rightLeg.SetRotation(rightLegSwing + totalLean * 0.3f);
    
    // Store swing amounts for physics reactions and visibility calculations
    m_leftLeg.SetSwingAmount(leftLegSwing);
    m_rightLeg.SetSwingAmount(rightLegSwing);
    m_leftArm.SetSwingAmount(leftArmSwing);
    m_rightArm.SetSwingAmount(rightArmSwing);
}

void StickFigureBody::UpdatePhysics(float deltaTime) {
    // Update physics for each limb
    UpdateLimbPhysics(m_leftArm, deltaTime);
    UpdateLimbPhysics(m_rightArm, deltaTime);
    UpdateLimbPhysics(m_leftLeg, deltaTime);
    UpdateLimbPhysics(m_rightLeg, deltaTime);
}

void StickFigureBody::ReactToMovement(const Vector2& velocity, float deltaTime) {
    // Make limbs react to sudden changes in movement
    float velocityMagnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    
    if (velocityMagnitude > 50.0f) {
        // Add some physics-based swaying to limbs
        Vector2 inertia = Vector2(-velocity.x * 0.001f, -velocity.y * 0.001f);
        
        m_leftArm.ApplyOffset(inertia);
        m_rightArm.ApplyOffset(inertia);
        m_leftLeg.ApplyOffset(Vector2(inertia.x * 0.5f, 0)); // Legs less affected horizontally
        m_rightLeg.ApplyOffset(Vector2(inertia.x * 0.5f, 0));
    }
}

void StickFigureBody::UpdateLimbPhysics(BodyPart& limb, float deltaTime) {
    // Simple physics simulation for more natural movement
    Vector2 velocity = limb.GetVelocity();
    
    // Apply damping
    velocity.x *= 0.95f;
    velocity.y *= 0.95f;
    
    // Apply velocity to position
    Vector2 currentPos = limb.GetPosition();
    currentPos.x += velocity.x * deltaTime;
    currentPos.y += velocity.y * deltaTime;
    
    limb.SetPosition(currentPos);
    limb.SetVelocity(velocity);
}

void StickFigureBody::ApplyLimbSwing(BodyPart& limb, float swingAmount, float maxAngle) {
    float currentRotation = limb.GetRotation();
    float targetRotation = swingAmount * maxAngle;
    
    // Smooth interpolation towards target
    float newRotation = currentRotation + (targetRotation - currentRotation) * 0.1f;
    limb.SetRotation(newRotation);
}

bool StickFigureBody::IsLimbInFront(BodyPartType limbType, float animationPhase) const {
    // Determine which limbs are in front based on animation phase
    float phase = fmod(animationPhase * 2 * M_PI, 2 * M_PI);
    
    switch (limbType) {
        case BodyPartType::LEFT_LEG:
            // Left leg is in front when it's swinging forward (phase 0 to PI)
            return phase < M_PI;
        case BodyPartType::RIGHT_LEG:
            // Right leg is in front when left leg is back (phase PI to 2*PI)
            return phase >= M_PI;
        case BodyPartType::LEFT_ARM:
            // Left arm is in front when right leg is forward (opposite to left leg)
            return phase >= M_PI;
        case BodyPartType::RIGHT_ARM:
            // Right arm is in front when left leg is forward
            return phase < M_PI;
        default:
            return true; // Head and torso are always visible
    }
}

std::vector<BodyPart*> StickFigureBody::GetDrawingOrder(float animationPhase) const {
    std::vector<BodyPart*> drawOrder;
    
    // Always draw back limbs first, then torso, then front limbs, then head
    
    // Draw back leg first
    if (IsLimbInFront(BodyPartType::LEFT_LEG, animationPhase)) {
        drawOrder.push_back(const_cast<BodyPart*>(&m_rightLeg)); // Right leg is back
    } else {
        drawOrder.push_back(const_cast<BodyPart*>(&m_leftLeg)); // Left leg is back
    }
    
    // Draw back arm
    if (IsLimbInFront(BodyPartType::LEFT_ARM, animationPhase)) {
        drawOrder.push_back(const_cast<BodyPart*>(&m_rightArm)); // Right arm is back
    } else {
        drawOrder.push_back(const_cast<BodyPart*>(&m_leftArm)); // Left arm is back
    }
    
    // Draw torso (middle layer)
    drawOrder.push_back(const_cast<BodyPart*>(&m_torso));
    
    // Draw front arm
    if (IsLimbInFront(BodyPartType::LEFT_ARM, animationPhase)) {
        drawOrder.push_back(const_cast<BodyPart*>(&m_leftArm)); // Left arm is front
    } else {
        drawOrder.push_back(const_cast<BodyPart*>(&m_rightArm)); // Right arm is front
    }
    
    // Draw front leg
    if (IsLimbInFront(BodyPartType::LEFT_LEG, animationPhase)) {
        drawOrder.push_back(const_cast<BodyPart*>(&m_leftLeg)); // Left leg is front
    } else {
        drawOrder.push_back(const_cast<BodyPart*>(&m_rightLeg)); // Right leg is front
    }
    
    // Draw head last (always on top)
    drawOrder.push_back(const_cast<BodyPart*>(&m_head));
    
    return drawOrder;
}
    
void StickFigureBody::UpdateHorizontalIdlePose() {
    // In horizontal mode, just rotate the normal pose by 90 degrees
    // Start with normal idle pose positions
    UpdateIdlePose();
    
    // Get the center point for rotation
    Vector2 center = m_basePosition;
    
    // Rotate all body parts 90 degrees clockwise around the center
    RotateBodyPartAroundCenter(m_head, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_torso, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_leftArm, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_rightArm, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_leftLeg, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_rightLeg, center, M_PI / 2);
}

void StickFigureBody::UpdateHorizontalWalkingAnimation(float phase, bool facingRight) {
    // Start with normal walking animation
    UpdateWalkingAnimation(phase, facingRight);
    
    // Get the center point for rotation
    Vector2 center = m_basePosition;
    
    // Rotate all body parts 90 degrees clockwise around the center
    RotateBodyPartAroundCenter(m_head, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_torso, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_leftArm, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_rightArm, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_leftLeg, center, M_PI / 2);
    RotateBodyPartAroundCenter(m_rightLeg, center, M_PI / 2);
}

void StickFigureBody::RotateBodyPartAroundCenter(BodyPart& part, const Vector2& center, float angle) {
    // Get current position relative to center
    Vector2 currentPos = part.GetPosition();
    Vector2 relative = Vector2(currentPos.x - center.x, currentPos.y - center.y);
    
    // Rotate the relative position
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    Vector2 rotated = Vector2(
        relative.x * cosAngle - relative.y * sinAngle,
        relative.x * sinAngle + relative.y * cosAngle
    );
    
    // Set new position relative to center
    Vector2 newPos = Vector2(center.x + rotated.x, center.y + rotated.y);
    part.SetPosition(newPos);
    
    // Also rotate the body part's own rotation
    float currentRotation = part.GetRotation();
    part.SetRotation(currentRotation + angle);
}
