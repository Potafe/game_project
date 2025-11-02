#include "Renderer.h"
#include "World.h"
#include "player/StickFigureBody.h"
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
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::DrawPlatforms(const World *world) {
    // Draw terrain line
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // Black terrain line
    
    const auto& terrainPoints = world->GetTerrainPoints();
    
    // Draw rugged terrain line
    for (size_t i = 0; i < terrainPoints.size() - 1; i++) {
        Vector2 point1 = WorldToScreen(terrainPoints[i]);
        Vector2 point2 = WorldToScreen(terrainPoints[i + 1]);
        
        // Draw thick line for terrain (3 pixels thick)
        for (int offset = -1; offset <= 1; offset++) {
            SDL_RenderDrawLine(m_renderer, 
                static_cast<int>(point1.x), 
                static_cast<int>(point1.y + offset),
                static_cast<int>(point2.x), 
                static_cast<int>(point2.y + offset));
        }
    }
    
    // Draw remaining platforms as rectangles
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

void Renderer::DrawStickFigure(const StickFigureBody& body) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    // Get drawing order for proper depth rendering
    auto drawOrder = body.GetDrawingOrder(body.GetAnimationPhase());
    
    // Draw body parts in correct order (back to front)
    for (const BodyPart* part : drawOrder) {
        if (part->GetType() == BodyPartType::HEAD) {
            // Draw head as circle
            Vector2 headPos = WorldToScreen(part->GetPosition());
            DrawCircle(headPos, part->GetWidth() / 2.0f);
        } else {
            // Draw other parts as thick lines
            DrawThickBodyPart(*part);
        }
    }
}

void Renderer::DrawThickBodyPart(const BodyPart& part) {
    Vector2 startPoint = WorldToScreen(part.GetStartPoint());
    Vector2 endPoint = WorldToScreen(part.GetEndPoint());
    float width = part.GetWidth();
    
    // Calculate the direction vector of the line
    Vector2 direction = Vector2(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    // Normalize the direction vector
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Calculate perpendicular vector (rotate direction by 90 degrees)
    Vector2 perpendicular = Vector2(-direction.y, direction.x);
    
    // Draw multiple parallel lines to create filled effect
    for (int offset = -static_cast<int>(width/2); offset <= static_cast<int>(width/2); offset++) {
        float perpX = perpendicular.x * offset;
        float perpY = perpendicular.y * offset;
        
        SDL_RenderDrawLine(m_renderer, 
            static_cast<int>(startPoint.x + perpX), 
            static_cast<int>(startPoint.y + perpY),
            static_cast<int>(endPoint.x + perpX), 
            static_cast<int>(endPoint.y + perpY));
    }
}

void Renderer::DrawThickLine(const Vector2& start, const Vector2& end, float thickness) {
    // Calculate perpendicular vector for thickness
    Vector2 direction = Vector2(end.x - start.x, end.y - start.y);
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
        
        // Perpendicular vector
        Vector2 perp = Vector2(-direction.y, direction.x);
        float halfThickness = thickness / 2.0f;
        
        // Calculate the four corners of the thick line
        Vector2 p1 = Vector2(start.x + perp.x * halfThickness, start.y + perp.y * halfThickness);
        Vector2 p2 = Vector2(start.x - perp.x * halfThickness, start.y - perp.y * halfThickness);
        Vector2 p3 = Vector2(end.x - perp.x * halfThickness, end.y - perp.y * halfThickness);
        Vector2 p4 = Vector2(end.x + perp.x * halfThickness, end.y + perp.y * halfThickness);
        
        // Draw the thick line as multiple thin lines for thickness
        for (int i = 0; i < static_cast<int>(thickness); ++i) {
            float t = (i / thickness) - 0.5f;
            Vector2 offset = Vector2(perp.x * t, perp.y * t);
            Vector2 lineStart = Vector2(start.x + offset.x, start.y + offset.y);
            Vector2 lineEnd = Vector2(end.x + offset.x, end.y + offset.y);
            DrawLine(lineStart, lineEnd);
        }
    }
}

void Renderer::DrawLine(const Vector2& start, const Vector2& end) {
    SDL_RenderDrawLine(m_renderer, 
                          static_cast<int>(start.x), static_cast<int>(start.y),
                          static_cast<int>(end.x), static_cast<int>(end.y));
}

void Renderer::DrawCircle(const Vector2& center, float radius) {
    // Fill the circle with black color using horizontal lines
    int intRadius = static_cast<int>(radius);
    for (int y = -intRadius; y <= intRadius; y++) {
        int width = static_cast<int>(sqrt(radius * radius - y * y));
        for (int x = -width; x <= width; x++) {
            SDL_RenderDrawPoint(m_renderer, 
                static_cast<int>(center.x + x), 
                static_cast<int>(center.y + y));
        }
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
