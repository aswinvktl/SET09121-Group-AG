#include <cstdio>
#include <fstream>
#include "systems/GameSystem.h"
#include "assets/Assets.h"

sf::RenderWindow GameSystem::window_;
sf::Clock GameSystem::clock_;
std::shared_ptr<Scene> GameSystem::current_scene_ = nullptr;

void GameSystem::setActiveScene(const std::shared_ptr<Scene>& scene) {
    current_scene_ = scene;
}

void GameSystem::quit() {
    window_.close();
}

void GameSystem::start() {
    printf("START CALLED\n");
    fflush(stdout);

    sf::ContextSettings settings;
    settings.majorVersion = 2;
    settings.minorVersion = 1;

    window_.create(
        sf::VideoMode(1280, 720),
        "Ghost Redemption",
        sf::Style::Default,
        settings
    );

    Assets::loadTexture(
        "ghost",
        "resources/textures/characters/ghost.png"
    );
    Assets::loadTexture(
        "tiles_grass",
        "resources/textures/tiles/tiles_grass.png"
    );

    window_.setFramerateLimit(60);

    while (window_.isOpen()) {
        float dt = clock_.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void GameSystem::processEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();
        if (current_scene_)
            current_scene_->handleEvent(event);
    }
}

void GameSystem::update(float dt) {
    if (current_scene_)
        current_scene_->update(dt);
}

void GameSystem::render() {
    window_.clear();
    if (current_scene_)
        current_scene_->render(window_);
    window_.display();
}
