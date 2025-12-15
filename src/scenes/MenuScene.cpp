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

void MenuScene::setHover(sf::Sprite& s, bool on) {
    const float base = 1.75f;
    const float hover = 1.88f;
    s.setScale(on ? hover : base, on ? hover : base);
}

void MenuScene::playClick() {
    if (hasClick && !Levels::muted)
        clickSound.play();
}

MenuScene::MenuScene() {
    grassTile.setTexture(Assets::getTexture("grass"));
    grassTile.setTextureRect({0, 0, 32, 32});
    grassTile.setScale(2.f, 2.f);

    overlay.setSize({1280.f, 720.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));

    // BUTTONS
    hasButtonTexture = Assets::hasTexture("ui_button");
    if (hasButtonTexture) {
        startButton.setTexture(Assets::getTexture("ui_button"));
        quitButton.setTexture(Assets::getTexture("ui_button"));

        auto setup = [](sf::Sprite& s) {
            auto b = s.getLocalBounds();
            s.setOrigin(b.width / 2.f, b.height / 2.f);
            s.setScale(1.75f, 1.75f);
        };
        setup(startButton);
        setup(quitButton);
    }

    // CURSOR
    hasCursorTexture = Assets::hasTexture("ui_cursor");
    if (hasCursorTexture) {
        cursorSprite.setTexture(Assets::getTexture("ui_cursor"));
        cursorSprite.setScale(2.f, 2.f);
        GameSystem::window().setMouseCursorVisible(false);
    }

    // FONT
    font.loadFromFile("resources/fonts/Kenney Future.ttf");

    title.setFont(font);
    title.setString("GHOST REDEMPTION");
    title.setCharacterSize(56);

    startLabel.setFont(font);
    startLabel.setString("START");
    startLabel.setCharacterSize(32);

    quitLabel.setFont(font);
    quitLabel.setString("QUIT");
    quitLabel.setCharacterSize(32);

    hint.setFont(font);
    hint.setString("ENTER: START | ESC: QUIT | M: MUTE");
    hint.setCharacterSize(20);

    // MUSIC (global)
    GameSystem::playMusic("resources/music/menu.wav", true);

    // CLICK
    hasClick = clickBuffer.loadFromFile("resources/sfx/ui/click.ogg");
    if (hasClick) {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(60.f);
    }
}

void MenuScene::update(float) {
    auto& win = GameSystem::window();
    const auto mouse = win.mapPixelToCoords(sf::Mouse::getPosition(win));

    if (hasCursorTexture)
        cursorSprite.setPosition(mouse);

    if (hasButtonTexture) {
        setHover(startButton, hit(startButton, mouse));
        setHover(quitButton, hit(quitButton, mouse));
    }
}

void MenuScene::render(sf::RenderWindow& window) {
    for (int y = 0; y < 12; ++y)
        for (int x = 0; x < 20; ++x) {
            grassTile.setPosition(x * 64.f, y * 64.f);
            window.draw(grassTile);
        }

    window.draw(overlay);

    const float cx = 640.f;

    centerText(title, {cx, 140.f});
    startButton.setPosition({cx, 330.f});
    quitButton.setPosition({cx, 500.f});

    centerText(startLabel, {cx, 330.f});
    centerText(quitLabel, {cx, 500.f});
    centerText(hint, {cx, 650.f});

    window.draw(startButton);
    window.draw(quitButton);
    window.draw(title);
    window.draw(startLabel);
    window.draw(quitLabel);
    window.draw(hint);

    if (hasCursorTexture)
        window.draw(cursorSprite);
}

void MenuScene::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::M) {
        Levels::muted = !Levels::muted;
        GameSystem::applyMute();
        return;
    }

    if (event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Return) {
        playClick();
        GameSystem::playMusic("resources/music/gameplay.wav", true);
        GameSystem::setActiveScene(Levels::cemetery);
        return;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        GameSystem::quit();
        return;
    }
}
