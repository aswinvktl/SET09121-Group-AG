#include "scenes/Level1Scene.h"
#include "systems/GameSystem.h"
#include "levels.h"
#include <cmath>

static sf::Vector2f normalize(sf::Vector2f v) {
    float len2 = v.x*v.x + v.y*v.y;
    if (len2 <= 0.0001f) return {0.f, 0.f};
    float inv = 1.0f / std::sqrt(len2);
    return {v.x*inv, v.y*inv};
}

Level1Scene::Level1Scene() {
    // Player
    player.setSize({24.f, 24.f});
    player.setFillColor(sf::Color::White);
    player.setOrigin(12.f, 12.f);
    playerStart = {80.f, 620.f};
    player.setPosition(playerStart);

    // Goal
    goal.setSize({100.f, 70.f});
    goal.setFillColor(sf::Color(80, 200, 120));
    goal.setOrigin(50.f, 35.f);
    goal.setPosition({1180.f, 90.f});

    // Pools
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

    // Dogs (simple patrol)
    auto makeDog = [&](sf::Vector2f a, sf::Vector2f b, float speed) {
        Dog d;
        d.body.setSize({28.f, 18.f});
        d.body.setFillColor(sf::Color(200, 170, 60));
        d.body.setOrigin(14.f, 9.f);
        d.a = a;
        d.b = b;
        d.speed = speed;
        d.body.setPosition(a);
        d.dir = normalize(b - a);
        dogs.push_back(d);
    };
    makeDog({140.f, 220.f}, {740.f, 220.f}, 160.f);
    makeDog({980.f, 560.f}, {980.f, 160.f}, 140.f);
}

void Level1Scene::resetLevel() {
    player.setPosition(playerStart);
    for (auto& d : dogs) {
        d.body.setPosition(d.a);
        d.dir = normalize(d.b - d.a);
    }
}

void Level1Scene::win() {
    Levels::level1Complete = true;
    GameSystem::setActiveScene(Levels::cemetery);
}

void Level1Scene::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {

        Levels::pausedFrom = Levels::level1;
        GameSystem::setActiveScene(Levels::pause);
    }
}


void Level1Scene::update(float dt) {
    // Player movement (dt-based)
    sf::Vector2f input(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;

    player.move(normalize(input) * playerSpeed * dt);

    // Dogs patrol
    auto near = [](sf::Vector2f p, sf::Vector2f q) {
        sf::Vector2f dv = p - q;
        return (dv.x*dv.x + dv.y*dv.y) < (12.f*12.f);
    };

    for (auto& d : dogs) {
        d.body.move(d.dir * d.speed * dt);
        sf::Vector2f pos = d.body.getPosition();
        if (near(pos, d.b)) d.dir = normalize(d.a - d.b);
        else if (near(pos, d.a)) d.dir = normalize(d.b - d.a);
    }

    // Fail on pool
    for (const auto& pool : pools) {
        if (player.getGlobalBounds().intersects(pool.getGlobalBounds())) {
            resetLevel();
            return;
        }
    }

    // Fail on dog
    for (const auto& d : dogs) {
        if (player.getGlobalBounds().intersects(d.body.getGlobalBounds())) {
            resetLevel();
            return;
        }
    }

    // Win
    if (player.getGlobalBounds().intersects(goal.getGlobalBounds())) {
        win();
        return;
    }
}

void Level1Scene::render(sf::RenderWindow& window) {
    window.clear(sf::Color(10, 10, 30));
    for (const auto& pool : pools) window.draw(pool);
    for (const auto& d : dogs) window.draw(d.body);
    window.draw(goal);
    window.draw(player);
}
