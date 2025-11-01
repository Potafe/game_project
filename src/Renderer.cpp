#include "Renderer.h"
#include "World.h"
#include <iostream>
#include <cmath>

using namespace std;

Renderer::Renderer()
    : m_renderer(nullptr)
    , m_cameraPosition(0.0f, 0.0f)
{

}

Renderer::~Renderer() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
}

bool Renderer::Init(SDL_Window *window) {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_SetRenderDrawColor(m_renderer, 135, 206, 235, 255); // Sky blue background
    
    return true;
}

void Renderer::Clear() {
    SDL_SetRenderDrawColor(m_renderer, 135, 206, 235, 255); // Sky blue
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::DrawPlatforms(const World *world) {
    SDL_SetRenderDrawColor(m_renderer, 139, 69, 19, 255); // Brown color for platforms
    
    const auto& platforms = world->GetPlatforms();
    
    for (const auto& platform : platforms) {
        // Draw platform as a rectangle
        float platformWidth = 80.0f;
        float platformHeight = 20.0f;
        
        Vector2 topLeft = WorldToScreen(Vector2(platform.x - platformWidth/2, platform.y - platformHeight/2));
        Vector2 bottomRight = WorldToScreen(Vector2(platform.x + platformWidth/2, platform.y + platformHeight/2));
        
        SDL_Rect rect = {
            static_cast<int>(topLeft.x),
            static_cast<int>(topLeft.y),
            static_cast<int>(bottomRight.x - topLeft.x),
            static_cast<int>(bottomRight.y - topLeft.y)
        };
        SDL_RenderFillRect(m_renderer, &rect);
    }
}

void Renderer::DrawStickFigure(const Vector2 &position) {
    Vector2 screenPosition = WorldToScreen(position);

    // Debug output
    // cout << "World pos: (" << position.x << ", " << position.y << ") -> Screen pos: (" 
    //          << screenPosition.x << ", " << screenPosition.y << ")" << std::endl;

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    
    // Legs (from feet up)
    DrawLine(screenPosition, Vector2(screenPosition.x, screenPosition.y - 20));

    // Body (from hips up)
    DrawLine(Vector2(screenPosition.x, screenPosition.y - 20), Vector2(screenPosition.x, screenPosition.y - 60));

    // Head (above body)
    DrawCircle(Vector2(screenPosition.x, screenPosition.y - 70), 10);

    // Arms (at shoulder level)
    DrawLine(Vector2(screenPosition.x - 15, screenPosition.y - 40),
             Vector2(screenPosition.x + 15, screenPosition.y - 40));

    // No separate leg lines since the body line serves as the legs
}void Renderer::DrawLine(const Vector2& start, const Vector2& end) {
    SDL_RenderDrawLine(m_renderer, 
                          static_cast<int>(start.x), static_cast<int>(start.y),
                          static_cast<int>(end.x), static_cast<int>(end.y));
}

void Renderer::DrawCircle(const Vector2& center, float radius) {
    for (int angle = 0; angle < 360; angle += 5) {
        float rad = angle * M_PI / 180.0f;
        int x = static_cast<int>(center.x + radius * cos(rad));
        int y = static_cast<int>(center.y + radius * sin(rad));
        SDL_RenderDrawPoint(m_renderer, x, y);
    }
}

void Renderer::SetCamera(const Vector2& position) {
    m_cameraPosition = position;
}

Vector2 Renderer::WorldToScreen(const Vector2& worldPos) {
    // Debug the camera position (reduced frequency)
    // cout << "Camera pos: (" << m_cameraPosition.x << ", " << m_cameraPosition.y << ")" << endl;

    // Center the camera on the player
    return Vector2(
        (worldPos.x - m_cameraPosition.x) + 640.0f, // 640 = WINDOW_WIDTH / 2
        (worldPos.y - m_cameraPosition.y) + 360.0f  // 360 = WINDOW_HEIGHT / 2, corrected Y
    );
}
