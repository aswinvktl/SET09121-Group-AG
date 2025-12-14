#pragma once
#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>

class MenuScene : public Scene {
public:
    MenuScene();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::RectangleShape background;
    sf::RectangleShape titleBar;
    sf::RectangleShape startButton;
};
