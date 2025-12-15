#include "scenes/EndingScene.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include "utils/SaveSystem.h"

EndingScene::EndingScene() {
    font.loadFromFile("resources/fonts/Kenney Future.ttf");

    text.setFont(font);
    text.setCharacterSize(26);
    text.setFillColor(sf::Color::White);
    text.setString(
        "REDEMPTION\n\n"
        "The souls are free\n"
        "ENTER  - Main Menu\n"
        "R      - Reset Progress\n"
        "M      - Mute\n"
        "ESC    - Quit"
    );

    // rough center
    text.setPosition(420.f, 220.f);

    GameSystem::playMusic("resources/music/menu.wav", true);
    GameSystem::applyMute();
}

void EndingScene::update(float dt) {
    (void)dt;
}

void EndingScene::render(sf::RenderWindow& window) {
    window.draw(text);
}

void EndingScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        GameSystem::applyMute();
        SaveSystem::save(Levels::muted,
                 Levels::level1Complete,
                 Levels::level2Complete,
                 Levels::level3Complete);

        return;
    }

    // Reset save
    if (event.key.code == sf::Keyboard::R) {
        Levels::level1Complete = false;
Levels::level2Complete = false;
Levels::level3Complete = false;
SaveSystem::save(Levels::muted,
                 Levels::level1Complete,
                 Levels::level2Complete,
                 Levels::level3Complete);

        return;
    }

    if (event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Return) {
        GameSystem::setActiveScene(Levels::menu);
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::quit();
        return;
    }
}
