#pragma once
#include <SFML/Graphics.hpp>
#include "../GameSystem.h"

class CemeteryScene : public Scene {
public:
    CemeteryScene(GameSystem* game);  // setup ghost and graves
    void update(float dt) override;      // update ghost position
    void render(sf::RenderWindow& window) override;  // draw cemetery
    void handleEvent(sf::Event& event) override;     // WASD input here
    
private:
    sf::RectangleShape background_rect_;
    sf::RectangleShape ghost_rect_;
    sf::RectangleShape grave1_rect_;
    sf::RectangleShape grave2_rect_;
    sf::RectangleShape grave3_rect_;
    
    sf::Vector2f ghost_pos_;
    float ghost_speed_ = 50.f;

    GameSystem* game_;

    bool is_overlapping(const sf::RectangleShape& a, const sf::RectangleShape& b);
};
