#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "World.h"
#include "InputManager.h"
#include "weapons/Gun.h"

#include <iostream>
#include <chrono>

using namespace std;

Game:: Game()
    : m_running(false)
    , m_window(nullptr)
{

}

Game:: ~Game() {
    ShutDown();
}

bool Game:: Init() {
    // we init the SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return 1;
    }
    
    m_window = SDL_CreateWindow(
        "Stick Figure Game Beta",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        return 1;
    }

    m_renderer = make_unique<Renderer>();
    if (!m_renderer->Init(m_window)) {
        cerr << "Failed to initialize renderer!" << endl;
        return 1;
    }

    m_player = make_unique<Player>();    
    m_world = make_unique<World>();
    m_input = make_unique<InputManager>();

    m_world->Generate();
    m_player->SetWorld(m_world.get());

    m_running = true;
    return true;
}

void Game::Run() {
    auto lastTime = chrono::high_resolution_clock::now();

    while (m_running) {
        auto currentTime = chrono::high_resolution_clock::now();
        float deltaTime = chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_running = false;
            }
            m_input->HandleEvent(event);
        }

        Update(deltaTime);

        Render();
    }
}

void Game::Update(float deltaTime) {
    Vector2 movement = m_input->GetMovementInput();
    m_player->Move(movement);
    
    // Handle orientation toggle (R key) with cooldown
    if (m_input->IsKeyPressed(SDL_SCANCODE_R) && m_player->CanToggleOrientation()) {
        m_player->TriggerOrientationToggle();
        cout << "Orientation toggled! Horizontal mode: " << (m_player->IsHorizontalMode() ? "ON" : "OFF") << endl;
    }

    // Handle weapon switching (1 key cycles between weapons)
    if (m_input->IsKeyJustPressed(SDL_SCANCODE_1)) {
        m_player->CycleWeapon();
    }

    // Handle gun firing (Enter key)
    if (m_input->IsKeyJustPressed(SDL_SCANCODE_RETURN)) {
        m_player->FireGun();
    }

    m_player->Update(deltaTime);
    m_world->Update(deltaTime);

    // Debug output for orientation mode
    static bool lastHorizontalMode = false;
    bool currentHorizontalMode = m_player->IsHorizontalMode();
    if (currentHorizontalMode != lastHorizontalMode) {
        cout << "Mode changed! Now in: " << (currentHorizontalMode ? "HORIZONTAL" : "VERTICAL") << " mode" << endl;
        lastHorizontalMode = currentHorizontalMode;
    }

    // // Debug output
    // Vector2 playerPos = m_player->GetPosition();
    // cout << "Player position: (" << playerPos.x << ", " << playerPos.y << ")" << endl;

    m_renderer->SetCamera(m_player->GetPosition());
    
    // Update input manager at the end of the frame
    m_input->Update();
}

void Game::Render() {
    m_renderer->Clear();

    m_renderer->DrawPlatforms(m_world.get());

    // Draw bullets first (behind player)
    if (auto gun = dynamic_cast<Gun*>(m_player->GetCurrentWeapon())) {
        m_renderer->DrawBullets(gun->GetBullets());
    }

    m_renderer->DrawStickFigure(m_player->GetBody());

    // Draw gun on player's hand (only if gun is equipped)
    if (dynamic_cast<Gun*>(m_player->GetCurrentWeapon())) {
        Vector2 handPos = m_player->GetHandPosition();
        m_renderer->DrawGun(handPos, m_player->IsFacingRight(), m_player->IsHorizontalMode());
    }

    // Draw UI elements
    m_renderer->DrawHealthBar(m_player->GetHealth(), WINDOW_WIDTH, WINDOW_HEIGHT);
    m_renderer->DrawWeaponBar(m_player->GetCurrentWeapon(), WINDOW_WIDTH, WINDOW_HEIGHT);

    m_renderer->Present();
}

void Game::ShutDown() {
    m_renderer.reset();
    m_player.reset();
    m_world.reset();
    m_input.reset();

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}
