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
    // Draw coordinate axes
    DrawAxes();
    
    // Draw ground line (x-axis terrain)
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // Black ground line
    
    const auto& terrainPoints = world->GetTerrainPoints();
    
    // Draw straight ground line (x-axis)
    for (size_t i = 0; i < terrainPoints.size() - 1; i++) {
        Vector2 point1 = WorldToScreen(terrainPoints[i]);
        Vector2 point2 = WorldToScreen(terrainPoints[i + 1]);
        
        // Draw thick line for ground
        for (int offset = -2; offset <= 2; offset++) {
            SDL_RenderDrawLine(m_renderer, 
                static_cast<int>(point1.x), 
                static_cast<int>(point1.y + offset),
                static_cast<int>(point2.x), 
                static_cast<int>(point2.y + offset));
        }
    }
    
    // Draw sine waves as climbable terrain in black
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // Black color for wave terrain
    
    const auto& sineWaves = world->GetSineWaves();
    
    // Draw waves as connected lines (terrain) - only if we have waves
    if (sineWaves.size() > 1) {
        for (size_t i = 0; i < sineWaves.size() - 1; i++) {
            Vector2 point1 = WorldToScreen(sineWaves[i]);
            Vector2 point2 = WorldToScreen(sineWaves[i + 1]);
            
            // Check if points are close enough to connect (same wave line)
            if (abs(sineWaves[i + 1].x - sineWaves[i].x) <= 20.0f) {
                // Draw thick lines for wave terrain
                for (int offset = -1; offset <= 1; offset++) {
                    SDL_RenderDrawLine(m_renderer, 
                        static_cast<int>(point1.x), static_cast<int>(point1.y + offset),
                        static_cast<int>(point2.x), static_cast<int>(point2.y + offset));
                }
            }
        }
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

void Renderer::DrawAxes() {
    SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, 255); // Gray color for axes
    
    // Draw X-axis (horizontal line through origin)
    Vector2 xAxisStart = WorldToScreen(Vector2(-2000.0f, 0.0f));
    Vector2 xAxisEnd = WorldToScreen(Vector2(2000.0f, 0.0f));
    SDL_RenderDrawLine(m_renderer, 
        static_cast<int>(xAxisStart.x), static_cast<int>(xAxisStart.y),
        static_cast<int>(xAxisEnd.x), static_cast<int>(xAxisEnd.y));
    
    // Draw Y-axis (vertical line through origin)
    Vector2 yAxisStart = WorldToScreen(Vector2(0.0f, -1000.0f));
    Vector2 yAxisEnd = WorldToScreen(Vector2(0.0f, 1000.0f));
    SDL_RenderDrawLine(m_renderer, 
        static_cast<int>(yAxisStart.x), static_cast<int>(yAxisStart.y),
        static_cast<int>(yAxisEnd.x), static_cast<int>(yAxisEnd.y));
    
    // Draw quadrant labels (optional - can be removed if not needed)
    // You could add text rendering here if SDL_ttf is available
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
