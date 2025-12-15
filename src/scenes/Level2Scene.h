#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include <vector>

class Level2Scene : public Scene {
public:
    Level2Scene();

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    sf::Font font;
    sf::Text pressEText;

    sf::RectangleShape playerRect;
    sf::Sprite playerCyborg;
    sf::Vector2f playerPosition;
    float playerSpeed = 220.0f;

    float energy = 100.f;
    float drainRate = 22.f;
    bool dead = false;

    sf::RectangleShape battery;
    sf::RectangleShape batteryFill;
    std::vector<sf::Sprite> chargers;

    sf::Sprite mechanic;

    std::vector<sf::RectangleShape> walls;
    sf::RectangleShape playerHitbox;
};
