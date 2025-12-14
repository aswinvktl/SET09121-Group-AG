#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class Level2Scene : public Scene {
public:
    Level2Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::RectangleShape playerRect;
};
