#include "scenes/Level2Scene.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include "utils/SaveSystem.h"

Level2Scene::Level2Scene() {
    playerRect.setSize({40.f, 40.f});
    playerRect.setOrigin(20.f, 20.f);
    playerRect.setFillColor(sf::Color::Yellow);
    playerRect.setPosition(640.f, 360.f);
}

void Level2Scene::update(float dt) {
    (void)dt;
}

void Level2Scene::render(sf::RenderWindow& window) {
    // DO NOT clear here — GameSystem does it
    window.draw(playerRect);
}

void Level2Scene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // Placeholder completion
    if (event.key.code == sf::Keyboard::E) {
        Levels::level2Complete = true;
        SaveSystem::save(
            Levels::muted,
            Levels::level1Complete,
            Levels::level2Complete
        );
        GameSystem::setActiveScene(Levels::cemetery);
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::pauseMusic();
        Levels::pausedFrom = Levels::level2;
        GameSystem::setActiveScene(Levels::pause);
        return;
    }
}
