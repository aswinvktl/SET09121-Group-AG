#pragma once
#include <SFML/Graphics.hpp>
#include "scenes/Scene.h"

class Level1Scene : public Scene {
public:
    Level1Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::RectangleShape playerRect;
};
