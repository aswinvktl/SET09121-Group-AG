#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

#include "scenes/Scene.h"

class GameSystem {
public:
    static void start();
    static void setActiveScene(const std::shared_ptr<Scene>& scene);

    static void quit();
    static sf::RenderWindow& window();

    // Global (single-owner) music control
    static void playMusic(const std::string& path, bool loop = true);
    static void stopMusic();
    static void pauseMusic();
    static void resumeMusic();
    static void applyMute(); // reads Levels::muted

private:
    static sf::RenderWindow window_;
    static sf::Clock clock_;
    static std::shared_ptr<Scene> current_scene_;

    static sf::Music music_;
    static std::string current_track_;

    static void processEvents();
    static void update(float dt);
    static void render();
};
