#pragma once
#include <SFML/Graphics.hpp>
#include "scenes/Scene.h"

class CemeteryScene : public Scene {
public:
    CemeteryScene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    // background,graves and ghost
    sf::RectangleShape backgroundRect;
    sf::RectangleShape ghostRect;
    sf::RectangleShape grave1Rect;
    sf::RectangleShape grave2Rect;
    sf::RectangleShape grave3Rect;

    sf::Vector2f ghostPos;
    float ghostSpeed = 200.f;

    bool isOverlapping(const sf::RectangleShape& a, const sf::RectangleShape& b);
};
