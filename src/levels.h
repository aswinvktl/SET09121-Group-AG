#pragma once
#include <memory>

class Scene;

struct Levels {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> pause;
    static std::shared_ptr<Scene> cemetery;
    static std::shared_ptr<Scene> level1;
    static std::shared_ptr<Scene> level2;
    static std::shared_ptr<Scene> level3;
    static std::shared_ptr<Scene> ending;

    static std::shared_ptr<Scene> pausedFrom;

    static bool level1Complete;
    static bool level2Complete;
    static bool level3Complete;
    static bool muted;
};
