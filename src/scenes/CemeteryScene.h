#pragma once
#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>

class CemeteryScene : public Scene {
public:
    CemeteryScene();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::RectangleShape backgroundRect;

    sf::RectangleShape ghostRect;
    sf::Vector2f ghostPos;
    float ghostSpeed = 280.0f; // pixels per second (fluid)

    sf::RectangleShape grave1Rect;
    sf::RectangleShape grave2Rect;
    sf::RectangleShape grave3Rect;

    bool isOverlapping(const sf::RectangleShape& a, const sf::RectangleShape& b);

    static sf::Vector2f normalizeOrZero(sf::Vector2f v);
};
