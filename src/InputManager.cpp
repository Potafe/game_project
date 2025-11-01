#include "InputManager.h"
#include <cstring>

using namespace std;

InputManager::InputManager()
    : m_currentKeys(nullptr)
    , m_previousKeys(nullptr)
    , m_numKeys(0)
{
    m_currentKeys = SDL_GetKeyboardState(&m_numKeys);
    m_previousKeys = new Uint8[m_numKeys];

    memset(m_previousKeys, 0, m_numKeys);
}

InputManager::~InputManager() {
    delete[] m_previousKeys;
}

void InputManager::Update() {
    memcpy(m_previousKeys, m_currentKeys, m_numKeys);
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
    return m_currentKeys[key] && !m_previousKeys[key];
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

    // Jumping
    if (IsKeyJustPressed(SDL_SCANCODE_SPACE) || IsKeyJustPressed(SDL_SCANCODE_W) || IsKeyJustPressed(SDL_SCANCODE_UP)) {
        movement.y = 1.0f;
    }
    
    return movement;
}


