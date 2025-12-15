#pragma once

#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>

class EndingScene : public Scene {
public:
    EndingScene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::Font font;
    sf::Text text;
};
