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
    sf::Text title;
    sf::Text body;
    sf::Text hint;
    sf::RectangleShape fade;

    static void center(sf::Text& t, sf::Vector2f pos);
};
