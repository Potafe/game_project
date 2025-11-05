#include "InputManager.h"
#include <cstring>
#include <iostream>

using namespace std;

InputManager::InputManager()
    : m_currentKeys(nullptr)
    , m_previousKeys(nullptr)
    , m_numKeys(0)
{
    m_currentKeys = SDL_GetKeyboardState(&m_numKeys);
    m_previousKeys = new Uint8[m_numKeys];

    // Initialize both arrays to 0
    memset(m_previousKeys, 0, m_numKeys);
}

InputManager::~InputManager() {
    delete[] m_previousKeys;
}

void InputManager::Update() {
    // Copy current state to previous state before SDL updates current
    memcpy(m_previousKeys, m_currentKeys, m_numKeys);
    // Force SDL to update the keyboard state
    SDL_PumpEvents();
}

void InputManager::HandleEvent(const SDL_Event &event) {
    // Handle any specific events here if needed
    // For now, we'll rely on SDL_GetKeyboardState for key input
}

bool InputManager::IsKeyPressed(SDL_Scancode key) const {
    if (key < 0 || key >= m_numKeys) return false;
    return m_currentKeys[key] != 0;
}

bool InputManager::IsKeyJustPressed(SDL_Scancode key) const {
    if (key < 0 || key >= m_numKeys) return false;
    bool currentPressed = m_currentKeys[key] != 0;
    bool previousPressed = m_previousKeys[key] != 0;
    bool justPressed = currentPressed && !previousPressed;
    
    return justPressed;
}

bool InputManager::IsOrientationTogglePressed() const {
    // Use R key for orientation toggle (R for Rotate)
    return IsKeyJustPressed(SDL_SCANCODE_R);
}

bool InputManager::IsShiftPressed() const {
    return m_currentKeys[SDL_SCANCODE_LSHIFT] || m_currentKeys[SDL_SCANCODE_RSHIFT];
}

Vector2 InputManager::GetMovementInput() const {
    Vector2 movement(0.0f, 0.0f);

    // Horizontal movement
    if (IsKeyPressed(SDL_SCANCODE_A) || IsKeyPressed(SDL_SCANCODE_LEFT)) {
        movement.x -= 1.0f;
    }
    if (IsKeyPressed(SDL_SCANCODE_D) || IsKeyPressed(SDL_SCANCODE_RIGHT)) {
        movement.x += 1.0f;
    }

    // Vertical movement - support both jumping and continuous y-axis movement
    if (IsKeyJustPressed(SDL_SCANCODE_SPACE) || IsKeyJustPressed(SDL_SCANCODE_W) || IsKeyJustPressed(SDL_SCANCODE_UP)) {
        movement.y = 1.0f; // For jumping in normal mode
    }
    
    // Continuous y-axis movement for horizontal mode
    if (IsKeyPressed(SDL_SCANCODE_W) || IsKeyPressed(SDL_SCANCODE_UP)) {
        movement.y = 1.0f;
    }
    if (IsKeyPressed(SDL_SCANCODE_S) || IsKeyPressed(SDL_SCANCODE_DOWN)) {
        movement.y = -1.0f;
    }
    
    return movement;
}


