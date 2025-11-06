#pragma once
#include <memory>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

class Renderer;
class Player;
class World;
class InputManager;
class Enemy;

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();
    void ShutDown();

private:
    void Update(float deltaTime);
    void Render();
    
    bool m_running;
    SDL_Window *m_window;

    unique_ptr<Renderer> m_renderer;
    unique_ptr<Player> m_player;
    unique_ptr<World> m_world;
    unique_ptr<InputManager> m_input;

    // Enemies
    vector<unique_ptr<Enemy>> m_enemies;

    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;
};
