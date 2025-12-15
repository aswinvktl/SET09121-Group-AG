#include "scenes/MenuScene.h"

#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"

void MenuScene::centerText(sf::Text& t, sf::Vector2f pos) {
    const auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(pos);
}

bool MenuScene::hit(const sf::Sprite& s, sf::Vector2f p) {
    return s.getGlobalBounds().contains(p);
}

MenuScene::MenuScene() {
    background.setSize({1280.f, 720.f});
    background.setFillColor(sf::Color(10, 10, 20));

    panel.setSize({860.f, 520.f});
    panel.setOrigin(panel.getSize() / 2.f);
    panel.setPosition({640.f, 360.f});
    panel.setFillColor(sf::Color(0, 0, 0, 140));
    panel.setOutlineThickness(4.f);
    panel.setOutlineColor(sf::Color(255, 255, 255, 30));

    hasButtonTexture = Assets::hasTexture("ui_button");
    if (hasButtonTexture) {
        startButton.setTexture(Assets::getTexture("ui_button"));
        quitButton.setTexture(Assets::getTexture("ui_button"));
    }

    (void)font.loadFromFile("resources/fonts/Kenney Future.ttf");

    title.setFont(font);
    title.setString("GHOST REDEMPTION");
    title.setCharacterSize(56);

    startLabel.setFont(font);
    startLabel.setString("START");
    startLabel.setCharacterSize(34);

    quitLabel.setFont(font);
    quitLabel.setString("QUIT");
    quitLabel.setCharacterSize(34);

    hint.setFont(font);
    hint.setString("Enter: start   |   Esc: quit");
    hint.setCharacterSize(20);

    layout();
}

void MenuScene::layout() {
    centerText(title, {640.f, 220.f});
    centerText(hint, {640.f, 610.f});

    if (!hasButtonTexture) {
        centerText(startLabel, {640.f, 400.f});
        centerText(quitLabel, {640.f, 500.f});
        return;
    }

    auto setupBtn = [](sf::Sprite& s, sf::Vector2f pos) {
        const auto b = s.getLocalBounds();
        s.setOrigin(b.width / 2.f, b.height / 2.f);
        s.setPosition(pos);
        s.setScale(3.0f, 3.0f);
    };

    setupBtn(startButton, {640.f, 400.f});
    setupBtn(quitButton, {640.f, 500.f});

    centerText(startLabel, {640.f, 400.f});
    centerText(quitLabel, {640.f, 500.f});
}

void MenuScene::update(float dt) {
    (void)dt;
}

void MenuScene::render(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(panel);

    if (hasButtonTexture) {
        window.draw(startButton);
        window.draw(quitButton);
    }

    window.draw(title);
    window.draw(startLabel);
    window.draw(quitLabel);
    window.draw(hint);
}

void MenuScene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
            GameSystem::setActiveScene(Levels::cemetery);
        } else if (event.key.code == sf::Keyboard::Escape) {
            GameSystem::quit();
        }
    }

    if (!hasButtonTexture) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f m((float)event.mouseButton.x, (float)event.mouseButton.y);
        if (hit(startButton, m)) GameSystem::setActiveScene(Levels::cemetery);
        if (hit(quitButton, m))  GameSystem::quit();
    }
}
