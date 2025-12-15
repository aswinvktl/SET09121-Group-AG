#include "levels.h"
#include "scenes/Scene.h"

std::shared_ptr<Scene> Levels::menu;
std::shared_ptr<Scene> Levels::pause;
std::shared_ptr<Scene> Levels::cemetery;
std::shared_ptr<Scene> Levels::level1;
std::shared_ptr<Scene> Levels::level2;
std::shared_ptr<Scene> Levels::pausedFrom;

bool Levels::level1Complete = false;
bool Levels::muted = false;
