#include "Level3Scene.h"
#include "CemeteryScene.h"

Level3Scene::Level3Scene(GameSystem* game) : game_(game) {

    // background
    background_.setSize({1280.f, 720.f});
    background_.setFillColor(sf::Color(40, 30, 20));

    // player
    player_.setSize({40.f, 40.f});
    player_.setFillColor(sf::Color(220, 220, 220));
    player_.setOrigin(20.f, 20.f);
    player_pos_ = {120.f, 600.f};

    // home (goal)
    home_.setSize({80.f, 80.f});
    home_.setFillColor(sf::Color::Green);
    home_.setOrigin(40.f, 40.f);
    home_.setPosition(1180.f, 120.f);

    // key
    key_.setSize({40.f, 40.f});
    key_.setFillColor(sf::Color::Yellow);
    key_.setOrigin(20.f, 20.f);
    key_.setPosition(200.f, 120.f);

    // warehouse
    warehouse_.setSize({100.f, 80.f});
    warehouse_.setFillColor(sf::Color(120, 70, 20));
    warehouse_.setOrigin(50.f, 40.f);
    warehouse_.setPosition(640.f, 120.f);

    // load guard sprite (single image)
    guard_texture_loaded_ =
        guard_texture_.loadFromFile("assets/textures/guards/guard.png") ||
        guard_texture_.loadFromFile("../assets/textures/guards/guard.png");

    // setup guards
    for (int i = 0; i < GUARD_COUNT; i++) {
        // collision box (invisible)
        guards_[i].setSize({50.f, 50.f});
        guards_[i].setOrigin(25.f, 25.f);
        guards_[i].setFillColor(sf::Color(0, 0, 0, 0)); // INVISIBLE
        guards_[i].setOutlineThickness(0.f);

        guard_forward_[i] = true;

        // default: horizontal
        guard_vertical_[i] = false;
        guard_min_y_[i] = 0.f;
        guard_max_y_[i] = 0.f;
    }

    // Guard 0 (HORIZONTAL)
    guard_pos_[0] = {640.f, 200.f};
    guard_speed_[0] = 140.f;
    guard_min_x_[0] = 200.f;
    guard_max_x_[0] = 1100.f;

    // Guard 1 (HORIZONTAL)
    guard_pos_[1] = {640.f, 360.f};
    guard_speed_[1] = 180.f;
    guard_min_x_[1] = 300.f;
    guard_max_x_[1] = 1000.f;

    // Guard 2 (VERTICAL)
    guard_pos_[2] = {900.f, 360.f};
    guard_speed_[2] = 170.f;
    guard_vertical_[2] = true;
    guard_min_y_[2] = 150.f;
    guard_max_y_[2] = 650.f;

    // keep x range valid for guard 2
    guard_min_x_[2] = guard_pos_[2].x;
    guard_max_x_[2] = guard_pos_[2].x;

    // setup sprites (same sprite used for each guard)
    if (guard_texture_loaded_) {
        float texW = (float)guard_texture_.getSize().x;
        float texH = (float)guard_texture_.getSize().y;

        for (int i = 0; i < GUARD_COUNT; i++) {
            guard_sprites_[i].setTexture(guard_texture_);
            guard_sprites_[i].setOrigin(texW / 2.f, texH / 2.f);

            // scale sprite to match 50x50 collision box
            float sx = 120.f / texW;
            float sy = 120.f / texH;
            guard_sprites_[i].setScale(sx, sy);
        }
    }

    // apply initial positions
    for (int i = 0; i < GUARD_COUNT; i++) {
        guards_[i].setPosition(guard_pos_[i]);
        if (guard_texture_loaded_) {
            guard_sprites_[i].setPosition(guard_pos_[i]);
        }
    }
}

void Level3Scene::update(float dt) {
    player_.setPosition(player_pos_);

    // move guards + collision
    for (int i = 0; i < GUARD_COUNT; i++) {

        if (!guard_vertical_[i]) {
            // horizontal
            if (guard_forward_[i]) guard_pos_[i].x += guard_speed_[i] * dt;
            else guard_pos_[i].x -= guard_speed_[i] * dt;

            if (guard_pos_[i].x > guard_max_x_[i]) guard_forward_[i] = false;
            if (guard_pos_[i].x < guard_min_x_[i]) guard_forward_[i] = true;
        } else {
            // vertical
            if (guard_forward_[i]) guard_pos_[i].y += guard_speed_[i] * dt;
            else guard_pos_[i].y -= guard_speed_[i] * dt;

            if (guard_pos_[i].y > guard_max_y_[i]) guard_forward_[i] = false;
            if (guard_pos_[i].y < guard_min_y_[i]) guard_forward_[i] = true;
        }

        guards_[i].setPosition(guard_pos_[i]);
        if (guard_texture_loaded_) {
            guard_sprites_[i].setPosition(guard_pos_[i]);
        }

        // touch guard = reset
        if (overlaps(player_, guards_[i])) {
            resetLevel();
            return;
        }
    }

    // pick up key
    if (!has_key_ && overlaps(player_, key_)) {
        has_key_ = true;
    }

    // warehouse gives food if key collected
    if (has_key_ && !has_food_ && overlaps(player_, warehouse_)) {
        has_food_ = true;
    }

    // win if have food + reach home
    if (has_food_ && overlaps(player_, home_)) {
        game_->setScene(std::make_unique<CemeteryScene>(game_));
    }
}

void Level3Scene::render(sf::RenderWindow& window) {
    window.draw(background_);
    window.draw(home_);

    if (!has_key_) window.draw(key_);
    if (!has_food_) window.draw(warehouse_);

    // draw ONLY sprites (no collision boxes)
    for (int i = 0; i < GUARD_COUNT; i++) {
        if (guard_texture_loaded_) window.draw(guard_sprites_[i]);
        // if texture not loaded, you will see nothing for guards (by design)
        // (optional: draw guards_[i] here if you want fallback)
    }

    window.draw(player_);
}

void Level3Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) player_pos_.y -= move_step_;
        if (event.key.code == sf::Keyboard::S) player_pos_.y += move_step_;
        if (event.key.code == sf::Keyboard::A) player_pos_.x -= move_step_;
        if (event.key.code == sf::Keyboard::D) player_pos_.x += move_step_;
    }
}

bool Level3Scene::overlaps(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

void Level3Scene::resetLevel() {
    player_pos_ = {120.f, 600.f};
    has_key_ = false;
    has_food_ = false;

    for (int i = 0; i < GUARD_COUNT; i++) {
        guard_forward_[i] = true;
    }
}
