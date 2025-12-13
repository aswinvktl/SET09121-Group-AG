#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Scene {
public:
    // Pure virtual base class for all scenes to inherit
    virtual ~Scene() = default;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleEvent(sf::Event& event) = 0;
};


// has main game loop
class GameSystem {
public:
    GameSystem();
    void run();

private:
    sf::RenderWindow window_;
    std::unique_ptr<Scene> current_scene_;
    sf::Clock clock_;

    void processEvents();
    void update(float dt);
    void render();
};
