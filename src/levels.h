#pragma once
#include <memory>

class Scene;

// add include if needed for Scene shared_ptr already present

struct Levels {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> pause;
    static std::shared_ptr<Scene> cemetery;
    static std::shared_ptr<Scene> level1;
    static std::shared_ptr<Scene> level2;
    static std::shared_ptr<Scene> ending;     // NEW

    static std::shared_ptr<Scene> pausedFrom;

    static bool level1Complete;
    static bool level2Complete;               // NEW
    static bool muted;
};
