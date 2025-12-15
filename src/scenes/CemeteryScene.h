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
    sf::Sprite grassTile;

    sf::Sprite ghostSprite;
    sf::RectangleShape ghostHitbox;
    sf::Vector2f ghostPos;
    float ghostSpeed = 280.0f;

    sf::RectangleShape grave1Rect;
    sf::RectangleShape grave2Rect;
    sf::RectangleShape grave3Rect;

    bool hasProps = false;
    sf::Sprite grave1Sprite;
    sf::Sprite grave2Sprite;
    sf::Sprite grave3Sprite;
    sf::IntRect graveRect = {0, 0, 32, 32}; 

    sf::Font font;
    sf::Text prompt;

    static bool isOverlapping(const sf::RectangleShape& a, const sf::RectangleShape& b);
    static sf::Vector2f normalizeOrZero(sf::Vector2f v);
};
