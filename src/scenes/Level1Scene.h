#pragma once

#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Level1Scene : public Scene {
public:
    Level1Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::Sprite floorTile;

    sf::RectangleShape player;
    sf::Vector2f playerStart;
    float playerSpeed = 220.0f;

    sf::Sprite ghostSprite;

    sf::RectangleShape goal;
    std::vector<sf::RectangleShape> pools;

    struct Dog {
        sf::RectangleShape body;
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f dir;
        float speed;
    };
    std::vector<Dog> dogs;

    bool hasDogTexture = false;
    std::vector<sf::Sprite> dogSprites;

    void resetLevel();
    void win();
};
