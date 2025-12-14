#include "scenes/Level2Scene.h"
#include "levels.h"
#include "systems/GameSystem.h"

Level2Scene::Level2Scene() {
    playerRect.setSize(sf::Vector2f(40.f, 40.f));
    playerRect.setOrigin(20.f, 20.f);
    playerRect.setFillColor(sf::Color::Yellow);
    playerRect.setPosition(640.f, 360.f);
}

void Level2Scene::update(float dt) {
    // nothing yet
}

void Level2Scene::render(sf::RenderWindow& window) {
    window.clear(sf::Color(0, 80, 0));
    window.draw(playerRect);
}

void Level2Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {

        Levels::pausedFrom = Levels::level2;
        GameSystem::setActiveScene(Levels::pause);
    }
}
