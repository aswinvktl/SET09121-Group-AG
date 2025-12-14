#pragma once
#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>

class PauseScene : public Scene {
public:
    PauseScene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::RectangleShape overlay;
    sf::RectangleShape panel;
};
