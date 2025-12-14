#pragma once
#include <SFML/Graphics.hpp>
#include "../GameSystem.h"

class Level3Scene : public Scene {
public:
    Level3Scene(GameSystem* game);

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event) override;

private:
    GameSystem* game_;

    sf::RectangleShape background_;

    // player
    sf::RectangleShape player_;
    sf::Vector2f player_pos_;
    float move_step_ = 50.f;

    // guards (collision boxes)
    static const int GUARD_COUNT = 3;

    sf::RectangleShape guards_[GUARD_COUNT];
    sf::Vector2f guard_pos_[GUARD_COUNT];
    float guard_speed_[GUARD_COUNT];
    bool guard_forward_[GUARD_COUNT];

    // horizontal range
    float guard_min_x_[GUARD_COUNT];
    float guard_max_x_[GUARD_COUNT];

    // vertical range
    bool guard_vertical_[GUARD_COUNT];
    float guard_min_y_[GUARD_COUNT];
    float guard_max_y_[GUARD_COUNT];

    // guard sprite (visual)
    sf::Texture guard_texture_;
    sf::Sprite guard_sprites_[GUARD_COUNT];
    bool guard_texture_loaded_ = false;

    // key + warehouse
    sf::RectangleShape key_;
    sf::RectangleShape warehouse_;
    bool has_key_ = false;
    bool has_food_ = false;

    // home (goal)
    sf::RectangleShape home_;

    bool overlaps(const sf::RectangleShape& a, const sf::RectangleShape& b);
    void resetLevel();
};
