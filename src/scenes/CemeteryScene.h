#pragma once
#include <SFML/Graphics.hpp>
#include "../GameSystem.h"  // FIXED: ../GameSystem.h (not ../../)

class CemeteryScene : public Scene {
public:
    CemeteryScene();  // Load assets + setup

    // LAB INTERFACE - Pong style
    void update(float dt) override;      
    void render(sf::RenderWindow& window) override;  
    void handleEvent(sf::Event& event) override;     

private:
    // SIMPLE RECTANGLES - Pong lab style
    sf::RectangleShape background_rect_;    // Dark cemetery bg
    sf::RectangleShape ghost_rect_;         // Player ghost WASD
    sf::RectangleShape grave1_rect_;        // Level 1 grave
    sf::RectangleShape grave2_rect_;        // Level 2 grave  
    sf::RectangleShape grave3_rect_;        // Level 3 grave
    
    sf::Vector2f ghost_pos_;                // Ghost position
    float ghost_speed_ = 200.f;             // Pixels per second
};
