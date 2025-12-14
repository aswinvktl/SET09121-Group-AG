#include "GameSystem.h"
#include "scenes/CemeteryScene.h"

// create window and load first scene
GameSystem::GameSystem() : 
    window_(sf::VideoMode(1280, 720), "Ghost Redemption:Hub") {
    window_.setFramerateLimit(60);
    current_scene_ = std::make_unique<CemeteryScene>(this);
}

void GameSystem::run() {
    while (window_.isOpen()) {
        // Frame independent movement
        float dt = clock_.restart().asSeconds();
        
        processEvents();
        if (current_scene_) {
            current_scene_->update(dt);
        }
        render();
    }
}

// pass events
void GameSystem::processEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        if (current_scene_) {
            current_scene_->handleEvent(event);
        }
    }
}

void GameSystem::update(float dt) {
    // forward update to current scene
    if (current_scene_) {
        current_scene_->update(dt);
    }
}

void GameSystem::render() {
    // clear, let scene draw, then display
    window_.clear(sf::Color(10, 10, 30));  // dark sky for cemetery
    if (current_scene_) {
        current_scene_->render(window_);
    }
    window_.display();
}

void GameSystem::setScene(std::unique_ptr<Scene> scene) {
    current_scene_ = std::move(scene);
}