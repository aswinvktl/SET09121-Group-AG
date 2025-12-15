#include "scenes/Level3Scene.h"

#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"
#include "utils/SaveSystem.h"

#include <cmath>

sf::Vector2f Level3Scene::normalize(sf::Vector2f v) {
    const float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    const float inv = 1.f / std::sqrt(len2);
    return {v.x * inv, v.y * inv};
}

Level3Scene::Level3Scene() {
    floorTile.setTexture(Assets::getTexture("stone"));
    floorTile.setTextureRect({0, 0, 32, 32});
    floorTile.setScale(2.f, 2.f);

    player.setSize({24.f, 24.f});
    player.setOrigin(12.f, 12.f);
    player.setFillColor(sf::Color(255, 255, 255, 0));
    playerStart = {80.f, 640.f};
    player.setPosition(playerStart);

    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect({0, 0, 32, 32});
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);
    ghostSprite.setPosition(playerStart);

    key.setSize({24.f, 24.f});
    key.setOrigin(12.f, 12.f);
    key.setPosition({300.f, 150.f});
    key.setFillColor(sf::Color(255, 255, 255, 0));

    hasKey = false;
    if (Assets::hasTexture("key")) {
        keySprite.setTexture(Assets::getTexture("key"));
        keySprite.setTextureRect({0, 0, 32, 32});
        keySprite.setOrigin(16.f, 16.f);
        keySprite.setScale(2.f, 2.f);
        keySprite.setPosition(key.getPosition());
    }

    warehouse.setSize({140.f, 90.f});
    warehouse.setOrigin(70.f, 45.f);
    warehouse.setPosition({640.f, 200.f});
    warehouse.setFillColor(sf::Color(150, 100, 50));

    exitZone.setSize({110.f, 80.f});
    exitZone.setOrigin(55.f, 40.f);
    exitZone.setPosition({1180.f, 650.f});
    exitZone.setFillColor(sf::Color(80, 200, 120));

    hasGuardTexture = Assets::hasTexture("guard");

    auto makeGuard = [&](sf::Vector2f a, sf::Vector2f b, float speed) {
        Guard g;
        g.body.setSize({28.f, 28.f});
        g.body.setOrigin(14.f, 14.f);

        if (hasGuardTexture) {
            g.body.setFillColor(sf::Color(255, 255, 255, 0));
        } else {
            g.body.setFillColor(sf::Color(200, 30, 30, 180));
        }

        g.a = a;
        g.b = b;
        g.dir = normalize(b - a);
        g.speed = speed;
        g.body.setPosition(a);
        guards.push_back(g);

        if (hasGuardTexture) {
            sf::Sprite s;
            s.setTexture(Assets::getTexture("guard"));
            s.setTextureRect({0, 0, 32, 32});
            s.setOrigin(16.f, 16.f);
            s.setScale(2.f, 2.f);
            s.setPosition(a);
            guardSprites.push_back(s);
        }
    };

    makeGuard({140.f, 560.f}, {1140.f, 560.f}, 360.f);
    makeGuard({140.f, 400.f}, {1140.f, 400.f}, 380.f);
    makeGuard({320.f, 140.f}, {320.f, 660.f}, 400.f);
    makeGuard({720.f, 140.f}, {720.f, 660.f}, 420.f);

    hasFood = false;
}

void Level3Scene::resetLevel() {
    hasKey = false;
    hasFood = false;

    player.setPosition(playerStart);
    ghostSprite.setPosition(playerStart);

    for (size_t i = 0; i < guards.size(); ++i) {
        guards[i].body.setPosition(guards[i].a);
        guards[i].dir = normalize(guards[i].b - guards[i].a);

        if (hasGuardTexture && i < guardSprites.size()) {
            guardSprites[i].setPosition(guards[i].a);
        }
    }
}

void Level3Scene::win() {
    Levels::level3Complete = true;
    SaveSystem::save(Levels::muted,
                     Levels::level1Complete,
                     Levels::level2Complete,
                     Levels::level3Complete);
    GameSystem::setActiveScene(Levels::cemetery);
}

void Level3Scene::update(float dt) {
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    player.move(normalize(input) * playerSpeed * dt);
    ghostSprite.setPosition(player.getPosition());

    for (size_t i = 0; i < guards.size(); ++i) {
        auto& g = guards[i];
        g.body.move(g.dir * g.speed * dt);

        const sf::Vector2f pos = g.body.getPosition();
        const sf::Vector2f db = pos - g.b;
        const sf::Vector2f da = pos - g.a;

        if (db.x * db.x + db.y * db.y < 100.f) g.dir = normalize(g.a - g.b);
        else if (da.x * da.x + da.y * da.y < 100.f) g.dir = normalize(g.b - g.a);

        if (hasGuardTexture && i < guardSprites.size()) {
            guardSprites[i].setPosition(pos);
        }

        if (player.getGlobalBounds().intersects(g.body.getGlobalBounds())) {
            resetLevel();
            return;
        }
    }

    if (!hasKey && player.getGlobalBounds().intersects(key.getGlobalBounds())) {
        hasKey = true;
    }

    if (hasKey && !hasFood &&
        player.getGlobalBounds().intersects(warehouse.getGlobalBounds())) {
        hasFood = true;
    }

    if (hasFood && player.getGlobalBounds().intersects(exitZone.getGlobalBounds())) {
        win();
    }
}

void Level3Scene::render(sf::RenderWindow& window) {
    sf::RectangleShape bg({1280.f, 720.f});
    bg.setFillColor(sf::Color(10, 10, 15));
    window.draw(bg);

    for (int y = 0; y < 12; ++y)
        for (int x = 0; x < 20; ++x) {
            floorTile.setPosition(x * 64.f, y * 64.f);
            window.draw(floorTile);
        }

    if (hasGuardTexture) {
        for (const auto& s : guardSprites) window.draw(s);
    } else {
        for (const auto& g : guards) window.draw(g.body);
    }

    if (!hasKey && Assets::hasTexture("key")) window.draw(keySprite);
    if (!hasFood) window.draw(warehouse);
    window.draw(exitZone);

    window.draw(ghostSprite);
}

void Level3Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        Levels::pausedFrom = Levels::level3;
        GameSystem::pauseMusic();
        GameSystem::setActiveScene(Levels::pause);
    }
}
