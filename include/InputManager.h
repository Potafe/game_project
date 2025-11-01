#pragma once
#include "Vector2.h"
#include <SDL2/SDL.h>

class InputManager {
public:
    InputManager();
    ~InputManager();

    void Update();
    void HandleEvent(const SDL_Event &event);

    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyJustPressed(SDL_Scancode key) const;
    Vector2 GetMovementInput() const;

private:
    const Uint8 *m_currentKeys;
    Uint8 *m_previousKeys;
    int m_numKeys;
};
