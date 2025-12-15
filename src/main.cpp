#include "systems/GameSystem.h"
#include "levels.h"
#include "assets/Assets.h"
#include "scenes/MenuScene.h"
#include "scenes/PauseScene.h"
#include "scenes/CemeteryScene.h"
#include "scenes/Level1Scene.h"
#include "scenes/Level2Scene.h"

int main() {
    // required textures
    Assets::loadTexture("ghost", "resources/textures/characters/ghost.png");
    Assets::loadTexture("dog",   "resources/textures/characters/dog.png");
    Assets::loadTexture("human", "resources/textures/characters/human.png");

    Assets::loadTexture("grass", "resources/textures/tiles/grass.png");
    Assets::loadTexture("stone", "resources/textures/tiles/stone.png");
    Assets::loadTexture("wall",  "resources/textures/tiles/wall.png");

    // optional UI / old props sheet
    Assets::loadTextureSafe("props",     "resources/textures/props/props.png");
    Assets::loadTextureSafe("ui_button", "resources/textures/ui/button.png");
    Assets::loadTextureSafe("ui_cursor", "resources/textures/ui/cursor.png");

    // NEW: per-item props
    Assets::loadTextureSafe("grave",       "resources/textures/props/grave.png");
    Assets::loadTextureSafe("flowerbed",   "resources/textures/props/flowerbed.png");
    Assets::loadTextureSafe("closed_door", "resources/textures/props/closed_door.png");
    Assets::loadTextureSafe("open_door",   "resources/textures/props/open_door.png");

    //Second Level Assets
    Assets::loadTexture("cyborg", "resources/textures/characters/cyborg.png");
    Assets::loadTexture("mechanic", "resources/textures/characters/mechanic.png");
    Assets::loadTextureSafe("charger", "resources/textures/props/charger.png");

    // create scenes after assets load
    Levels::menu     = std::make_shared<MenuScene>();
    Levels::pause    = std::make_shared<PauseScene>();
    Levels::cemetery = std::make_shared<CemeteryScene>();
    Levels::level1   = std::make_shared<Level1Scene>();
    Levels::level2   = std::make_shared<Level2Scene>();

    GameSystem::setActiveScene(Levels::menu);
    GameSystem::start();
    return 0;
}
