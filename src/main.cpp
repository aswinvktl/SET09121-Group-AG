#include "systems/GameSystem.h"
#include "levels.h"

#include "scenes/CemeteryScene.h"
#include "scenes/Level1Scene.h"
#include "scenes/Level2Scene.h"

int main() {
    Levels::cemetery = std::make_shared<CemeteryScene>();
    Levels::level1   = std::make_shared<Level1Scene>();
    Levels::level2   = std::make_shared<Level2Scene>();

    GameSystem::setActiveScene(Levels::cemetery);
    GameSystem::start();
    return 0;
}
