#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    Game game;

    if (!game.Init()) {
        cerr << "Failed to initialize game!" << endl;
        return 1;
    }

    game.Run();
    game.ShutDown();

    return 0;
}
