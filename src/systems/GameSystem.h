#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../scenes/Scene.h"

class GameSystem {
public:
  static void start();
  static void setActiveScene(const std::shared_ptr<Scene>& scene);

  static void quit();
  static sf::RenderWindow& window();

private:
  static sf::RenderWindow window_;
  static sf::Clock clock_;
  static std::shared_ptr<Scene> current_scene_;

  static void processEvents();
  static void update(float dt);
  static void render();
};
