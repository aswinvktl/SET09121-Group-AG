#include "scenes/Level1Scene.h"

#include "assets/Assets.h"
#include "levels.h"
#include "systems/GameSystem.h"

#include <cmath>

static sf::Vector2f normalize(sf::Vector2f v) {
    const float len2 = v.x * v.x + v.y * v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    const float inv = 1.0f / std::sqrt(len2);
    return {v.x * inv, v.y * inv};
}

Level1Scene::Level1Scene() {
    floorTile.setTexture(Assets::getTexture("stone"));
    floorTile.setTextureRect({0, 0, 32, 32});
    floorTile.setScale(2.f, 2.f);

    player.setSize({24.f, 24.f});
    player.setFillColor(sf::Color(255, 255, 255, 0));
    player.setOrigin(12.f, 12.f);
    playerStart = {80.f, 620.f};
    player.setPosition(playerStart);

    ghostSprite.setTexture(Assets::getTexture("ghost"));
    ghostSprite.setTextureRect({0, 0, 32, 32});
    ghostSprite.setOrigin(16.f, 16.f);
    ghostSprite.setScale(2.f, 2.f);
    ghostSprite.setPosition(playerStart);

    goal.setSize({100.f, 70.f});
    goal.setFillColor(sf::Color(80, 200, 120));
    goal.setOrigin(50.f, 35.f);
    goal.setPosition({1180.f, 90.f});

    auto makePool = [&](sf::Vector2f pos, sf::Vector2f size) {
        sf::RectangleShape r;
        r.setPosition(pos);
        r.setSize(size);
        r.setFillColor(sf::Color(30, 80, 200));
        pools.push_back(r);
    };
    makePool({200.f, 120.f}, {220.f, 140.f});
    makePool({520.f, 280.f}, {260.f, 160.f});
    makePool({220.f, 430.f}, {170.f, 110.f});

    hasDogTexture = Assets::hasTexture("dog");

    auto makeDog = [&](sf::Vector2f a, sf::Vector2f b, float speed) {
        Dog d;
        d.body.setSize({28.f, 18.f});
        d.body.setFillColor(sf::Color(255, 255, 255, 0));
        d.body.setOrigin(14.f, 9.f);
        d.a = a;
        d.b = b;
        d.speed = speed;
        d.body.setPosition(a);
        d.dir = normalize(b - a);
        dogs.push_back(d);

        if (hasDogTexture) {
            sf::Sprite s;
            s.setTexture(Assets::getTexture("dog"));
            s.setTextureRect({0, 0, 32, 32});
            s.setOrigin(16.f, 16.f);
            s.setScale(2.f, 2.f);
            s.setPosition(a);
            dogSprites.push_back(s);
        }
    };

    makeDog({140.f, 220.f}, {740.f, 220.f}, 160.f);
    makeDog({980.f, 560.f}, {980.f, 160.f}, 140.f);
}

void Level1Scene::resetLevel() {
    player.setPosition(playerStart);
    ghostSprite.setPosition(playerStart);

    for (size_t i = 0; i < dogs.size(); ++i) {
        dogs[i].body.setPosition(dogs[i].a);
        dogs[i].dir = normalize(dogs[i].b - dogs[i].a);
        if (hasDogTexture && i < dogSprites.size()) dogSprites[i].setPosition(dogs[i].a);
    }
}

void Level1Scene::win() {
    Levels::level1Complete = true;
    GameSystem::setActiveScene(Levels::cemetery);
}

void Level1Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        Levels::pausedFrom = Levels::level1;
        GameSystem::setActiveScene(Levels::pause);
    }
}

void Level1Scene::update(float dt) {
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    player.move(normalize(input) * playerSpeed * dt);
    ghostSprite.setPosition(player.getPosition());

    auto near = [](sf::Vector2f p, sf::Vector2f q) {
        const sf::Vector2f dv = p - q;
        return (dv.x * dv.x + dv.y * dv.y) < (12.f * 12.f);
    };

    for (size_t i = 0; i < dogs.size(); ++i) {
        auto& d = dogs[i];
        d.body.move(d.dir * d.speed * dt);
        const sf::Vector2f pos = d.body.getPosition();

        if (near(pos, d.b)) d.dir = normalize(d.a - d.b);
        else if (near(pos, d.a)) d.dir = normalize(d.b - d.a);

        if (hasDogTexture && i < dogSprites.size()) dogSprites[i].setPosition(pos);
    }

    for (const auto& pool : pools) {
        if (player.getGlobalBounds().intersects(pool.getGlobalBounds())) {
            resetLevel();
            return;
        }
    }

    for (const auto& d : dogs) {
        if (player.getGlobalBounds().intersects(d.body.getGlobalBounds())) {
            resetLevel();
            return;
        }
    }

    if (player.getGlobalBounds().intersects(goal.getGlobalBounds())) {
        win();
        return;
    }
}

void Level1Scene::render(sf::RenderWindow& window) {
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 20; ++x) {
            floorTile.setPosition(x * 64.f, y * 64.f);
            window.draw(floorTile);
        }
    }

    for (const auto& pool : pools) window.draw(pool);

    if (hasDogTexture) {
        for (const auto& s : dogSprites) window.draw(s);
    } else {
        for (const auto& d : dogs) window.draw(d.body);
    }

    window.draw(goal);
    window.draw(ghostSprite);
}
