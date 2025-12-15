// Level3Scene.h
#pragma once

#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Level3Scene : public Scene {
public:
    Level3Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    struct Guard {
        sf::RectangleShape body;
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f dir;
        float speed = 300.f;
    };

    static sf::Vector2f normalize(sf::Vector2f v);

    void resetLevel();
    void win();

    // visuals / tiles
    sf::Sprite floorTile;

    // player (fallback) + ghost sprite (main visual)
    sf::RectangleShape player;
    sf::Sprite ghostSprite;
    sf::Vector2f playerStart;
    float playerSpeed = 240.f;

    // guards
    std::vector<Guard> guards;
    std::vector<sf::Sprite> guardSprites;
    bool hasGuardTexture = false;

    // key
    sf::RectangleShape key;
    sf::Sprite keySprite;
    bool hasKey = false;

    // objectives
    sf::RectangleShape warehouse;
    bool hasFood = false;

    sf::RectangleShape exitZone;

    // texture availability
    bool hasStoneTexture = false;
    bool hasGhostTexture = false;
    bool hasKeyTexture   = false;
};
