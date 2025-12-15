#include <fstream>
#include <SFML/Window/Context.hpp>

#include "systems/GameSystem.h"
#include "levels.h"
#include "assets/Assets.h"

#include "scenes/MenuScene.h"
#include "scenes/PauseScene.h"
#include "scenes/CemeteryScene.h"
#include "scenes/Level1Scene.h"
#include "scenes/Level2Scene.h"
#include "scenes/Level3Scene.h"

static void trace(const char* msg) {
    std::ofstream f("trace.txt", std::ios::app);
    f << msg << "\n";
    f.flush();
}

int main() {
    trace("MAIN: entered");

    sf::Context context;
    trace("MAIN: sf::Context created");

    trace("MAIN: loading textures");
    Assets::loadTexture("ghost", "resources/textures/characters/ghost.png");
    Assets::loadTexture("tiles_grass", "resources/textures/tiles/tiles_grass.png");
    Assets::loadTexture("stone", "resources/textures/tiles/stone.png");
    Assets::loadTexture("dog", "resources/textures/characters/dog.png");
    Assets::loadTexture("guard", "resources/textures/characters/knight.png");
    Assets::loadTexture("key", "resources/textures/props/key.png");

    trace("MAIN: textures loaded");

    trace("MAIN: creating MenuScene");
    Levels::menu = std::make_shared<MenuScene>();
    trace("MAIN: MenuScene created");

    trace("MAIN: creating PauseScene");
    Levels::pause = std::make_shared<PauseScene>();
    trace("MAIN: PauseScene created");

    trace("MAIN: creating CemeteryScene");
    Levels::cemetery = std::make_shared<CemeteryScene>();
    trace("MAIN: CemeteryScene created");

    trace("MAIN: creating Level1Scene");
    Levels::level1 = std::make_shared<Level1Scene>();
    trace("MAIN: Level1Scene created");

    trace("MAIN: creating Level2Scene");
    Levels::level2 = std::make_shared<Level2Scene>();
    trace("MAIN: Level2Scene created");

    trace("MAIN: creating Level3Scene");
    Levels::level3 = std::make_shared<Level3Scene>();
    trace("MAIN: Level3Scene created");

    trace("MAIN: setActiveScene");
    GameSystem::setActiveScene(Levels::menu);

    trace("MAIN: calling start");
    GameSystem::start();

    trace("MAIN: returned from start");
    return 0;
}
