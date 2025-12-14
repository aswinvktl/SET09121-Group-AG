#pragma once
#include <memory>
#include "scenes/Scene.h"

struct Levels {
    static std::shared_ptr<Scene> cemetery;
    static std::shared_ptr<Scene> level1;
    static std::shared_ptr<Scene> level2;
};
