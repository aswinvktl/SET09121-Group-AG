#include "systems/GameSystem.h"
#include "levels.h"
#include "assets/Assets.h"
#include "scenes/MenuScene.h"
#include "scenes/PauseScene.h"
#include "scenes/CemeteryScene.h"
#include "scenes/Level1Scene.h"
#include "scenes/Level2Scene.h"

int main() {
    Levels::menu     = std::make_shared<MenuScene>();
    Levels::pause    = std::make_shared<PauseScene>();
    Assets::loadTexture(
        "ghost",
        "resources/textures/characters/ghost.png"
    );
    Levels::cemetery = std::make_shared<CemeteryScene>();
    Levels::level1   = std::make_shared<Level1Scene>();
    Levels::level2   = std::make_shared<Level2Scene>();

    GameSystem::setActiveScene(Levels::menu);
    GameSystem::start();
    return 0;
}
