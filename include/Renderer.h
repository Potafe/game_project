#pragma once
#include "Vector2.h"
#include <SDL2/SDL.h>
#include "player/Health.h"
#include "Weapon.h"
#include <vector>
#include <memory>

using namespace std;


class World;
class StickFigureBody;
class BodyPart;
class Gun;
class Hammer;
class Bullet;

class World;
class StickFigureBody;
class BodyPart;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Init(SDL_Window *window);
    void Clear();
    void Present();

    void DrawPlatforms(const World* world);
    void DrawStickFigure(const StickFigureBody& body);
    void DrawAxes();
    void SetCamera(const Vector2& position);

    // UI drawing methods
    void DrawHealthBar(const Health& health, int screenWidth, int screenHeight);
    void DrawWeaponBar(const Weapon* weapon, int screenWidth, int screenHeight);
    void DrawStaminaBar(float currentStamina, float maxStamina, int screenWidth, int screenHeight);
    void DrawSpeedLines(const Vector2& playerPosition, bool facingRight, bool isSprinting);
    void DrawWeaponIcon(const Weapon* weapon, float x, float y);
    void DrawCooldownBar(const Weapon* weapon, float x, float y);
    void DrawChargeBar(const Hammer* hammer, float x, float y);
    void DrawAmmoCounter(const Gun* gun, float x, float y);
    
    // Gun and bullet rendering
    void DrawGun(const Vector2& handPosition, bool facingRight, bool horizontalMode);
    void DrawBullets(const vector<unique_ptr<Bullet>>& bullets);
    void DrawCircle(const Vector2& center, float radius, SDL_Color color);

private:
    void DrawLine(const Vector2& start, const Vector2& end);
    void DrawCircle(const Vector2& center, float radius);
    void DrawThickBodyPart(const BodyPart& part);
    void DrawThickLine(const Vector2& start, const Vector2& end, float thickness);
    Vector2 WorldToScreen(const Vector2& worldPos);

    void DrawHeart(float x, float y, bool filled);

    SDL_Renderer *m_renderer;
    Vector2 m_cameraPosition;
};

