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
    sf::RectangleShape panel;

    bool hasButtonTexture = false;
    sf::Sprite startButton;
    sf::Sprite quitButton;

    sf::Font font;
    sf::Text title;
    sf::Text startLabel;
    sf::Text quitLabel;
    sf::Text hint;

    void layout();
    static void centerText(sf::Text& t, sf::Vector2f pos);
    static bool hit(const sf::Sprite& s, sf::Vector2f p);
};
