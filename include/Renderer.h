#pragma once
#include "Vector2.h"
#include <SDL2/SDL.h>

class World;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Init(SDL_Window *window);
    void Clear();
    void Present();

    void DrawPlatforms(const World* world);
    void DrawStickFigure(const Vector2 &position);
    void SetCamera(const Vector2& position);

private:
    void DrawLine(const Vector2& start, const Vector2& end);
    void DrawCircle(const Vector2& center, float radius);
    Vector2 WorldToScreen(const Vector2& worldPos);

    SDL_Renderer *m_renderer;
    Vector2 m_cameraPosition;
};

