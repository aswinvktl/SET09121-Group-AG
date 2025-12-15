#include "levels.h"
#include "scenes/Scene.h"

// Scene pointers
std::shared_ptr<Scene> Levels::menu        = nullptr;
std::shared_ptr<Scene> Levels::pause       = nullptr;
std::shared_ptr<Scene> Levels::cemetery    = nullptr;
std::shared_ptr<Scene> Levels::level1      = nullptr;
std::shared_ptr<Scene> Levels::level2      = nullptr;
std::shared_ptr<Scene> Levels::ending      = nullptr;
std::shared_ptr<Scene> Levels::pausedFrom  = nullptr;

// Global flags
bool Levels::level1Complete = false;
bool Levels::level2Complete = false;
bool Levels::muted          = false;
