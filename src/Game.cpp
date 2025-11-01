#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "World.h"
#include "InputManager.h"
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
    m_input->Update();

    Vector2 movement = m_input->GetMovementInput();
    m_player->Move(movement);

    m_player->Update(deltaTime);
    m_world->Update(deltaTime);

    // Debug output
    Vector2 playerPos = m_player->GetPosition();
    cout << "Player position: (" << playerPos.x << ", " << playerPos.y << ")" << endl;

    m_renderer->SetCamera(m_player->GetPosition());
}

void Game::Render() {
    m_renderer->Clear();

    m_renderer->DrawPlatforms(m_world.get());

    m_renderer->DrawStickFigure(m_player->GetPosition(), m_player->GetAnimationPhase());

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
