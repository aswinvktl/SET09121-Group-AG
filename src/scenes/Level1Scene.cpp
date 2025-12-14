#include "scenes/Level1Scene.h"
#include "levels.h"
#include "systems/GameSystem.h"

Level1Scene::Level1Scene() {
    playerRect.setSize(sf::Vector2f(40.f, 40.f));
    playerRect.setOrigin(20.f, 20.f);
    playerRect.setFillColor(sf::Color::White);
    playerRect.setPosition(640.f, 360.f);
}

void Level1Scene::update(float dt) {
    // nothing yet
}

void Level1Scene::render(sf::RenderWindow& window) {
    window.clear(sf::Color(0, 0, 80));
    window.draw(playerRect);
}

void Level1Scene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::setActiveScene(Levels::cemetery);
    }
}
