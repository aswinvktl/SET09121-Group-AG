#include "systems/GameSystem.h"

sf::RenderWindow GameSystem::window_;
sf::Clock GameSystem::clock_;
std::shared_ptr<Scene> GameSystem::current_scene_ = nullptr;

void GameSystem::setActiveScene(const std::shared_ptr<Scene>& scene) {
    current_scene_ = scene;
}

void GameSystem::start() {
    window_.create(sf::VideoMode(1280, 720), "Ghost Redemption");
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
