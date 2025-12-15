#include "systems/GameSystem.h"
#include "levels.h"
#include "assets/Assets.h"

#include "scenes/MenuScene.h"
#include "scenes/PauseScene.h"
#include "scenes/CemeteryScene.h"
#include "scenes/Level1Scene.h"
#include "scenes/Level2Scene.h"
#include "scenes/EndingScene.h"

#include "utils/SaveSystem.h"

int main() {
    // ------------------------
    // Load textures (required)
    // ------------------------
    Assets::loadTexture("ghost", "resources/textures/characters/ghost.png");
    Assets::loadTexture("dog",   "resources/textures/characters/dog.png");
    Assets::loadTexture("human", "resources/textures/characters/human.png");

    Assets::loadTexture("grass", "resources/textures/tiles/grass.png");
    Assets::loadTexture("stone", "resources/textures/tiles/stone.png");
    Assets::loadTexture("wall",  "resources/textures/tiles/wall.png");

    // ------------------------
    // Optional UI / props
    // ------------------------
    Assets::loadTextureSafe("props",     "resources/textures/props/props.png");
    Assets::loadTextureSafe("ui_button", "resources/textures/ui/button.png");
    Assets::loadTextureSafe("ui_cursor", "resources/textures/ui/cursor.png");

    Assets::loadTextureSafe("grave",       "resources/textures/props/grave.png");
    Assets::loadTextureSafe("flowerbed",   "resources/textures/props/flowerbed.png");
    Assets::loadTextureSafe("closed_door", "resources/textures/props/closed_door.png");
    Assets::loadTextureSafe("open_door",   "resources/textures/props/open_door.png");

    // ------------------------
    // Load saved game state
    // ------------------------
    SaveSystem::load(Levels::muted, Levels::level1Complete, Levels::level2Complete);

    // ------------------------
    // Create scenes (persistent)
    // ------------------------
    Levels::menu     = std::make_shared<MenuScene>();
    Levels::pause    = std::make_shared<PauseScene>();
    Levels::cemetery = std::make_shared<CemeteryScene>();
    Levels::level1   = std::make_shared<Level1Scene>();
    Levels::level2   = std::make_shared<Level2Scene>();
    Levels::ending   = std::make_shared<EndingScene>();

    // ------------------------
    // Start game
    // ------------------------
    GameSystem::setActiveScene(Levels::menu);
    GameSystem::start();
    return 0;
}
