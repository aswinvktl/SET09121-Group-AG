#include "CemeteryScene.h"
#include <iostream>

// setup ghost and 3 graves 
CemeteryScene::CemeteryScene() {
    // ghost/player in middle
    ghost_rect_.setSize(sf::Vector2f(40.f, 40.f));
    ghost_rect_.setFillColor(sf::Color(200, 200, 200, 255));
    ghost_rect_.setOrigin(20.f, 20.f);
    ghost_pos_ = sf::Vector2f(640.f, 600.f);

    // level 1 pool
    grave1_rect_.setSize(sf::Vector2f(60.f, 100.f));
    grave1_rect_.setFillColor(sf::Color(80, 60, 40));
    grave1_rect_.setOrigin(30.f, 50.f);
    grave1_rect_.setPosition(640.f, 500.f);
    
    // level 2 battery
    grave2_rect_.setSize(sf::Vector2f(60.f, 100.f));
    grave2_rect_.setFillColor(sf::Color(80, 60, 40));
    grave2_rect_.setOrigin(30.f, 50.f);
    grave2_rect_.setPosition(640.f, 350.f);
    
    // level 3 guards 
    grave3_rect_.setSize(sf::Vector2f(60.f, 100.f));
    grave3_rect_.setFillColor(sf::Color(80, 60, 40));
    grave3_rect_.setOrigin(30.f, 50.f);
    grave3_rect_.setPosition(640.f, 200.f);

    // dark background for cemetery
    background_rect_.setSize(sf::Vector2f(1280.f, 720.f));
    background_rect_.setFillColor(sf::Color(20, 20, 40));
}


void CemeteryScene::update(float dt) {
    // sync ghost rect to ghost_pos_
    ghost_rect_.setPosition(ghost_pos_);
}

void CemeteryScene::render(sf::RenderWindow& window) {
    window.draw(background_rect_);
    window.draw(ghost_rect_);
    window.draw(grave1_rect_);
    window.draw(grave2_rect_);
    window.draw(grave3_rect_);
}

void CemeteryScene::handleEvent(sf::Event& event) {
    // keyboard input for ghost movement
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            ghost_pos_.y -= ghost_speed_;
        }
        if (event.key.code == sf::Keyboard::S) {
            ghost_pos_.y += ghost_speed_;
        }
        if (event.key.code == sf::Keyboard::A) {
            ghost_pos_.x -= ghost_speed_;
        }
        if (event.key.code == sf::Keyboard::D) {
            ghost_pos_.x += ghost_speed_;
        }

        // interact with graves using E
        if (event.key.code == sf::Keyboard::E) {
            if (is_overlapping(ghost_rect_, grave1_rect_)) {
                std::cout << "Level 1 selected (pool)\n";
            } else if (is_overlapping(ghost_rect_, grave2_rect_)) {
                std::cout << "Level 2 selected (battery)\n";
            } else if (is_overlapping(ghost_rect_, grave3_rect_)) {
                std::cout << "Level 3 selected (guards)\n";
            }
        }
    }
}

bool CemeteryScene::is_overlapping(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}
