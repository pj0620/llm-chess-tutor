#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "pieces.h"

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({700u, 400u}), "CMake SFML Project");
  window.setFramerateLimit(144);

  Game game(50, 0, 0);

  while (window.isOpen())
  {
      while (const std::optional event = window.pollEvent())
      {
          if (event->is<sf::Event::Closed>())
          {
              window.close();
          }
      }

      window.clear(sf::Color::Black);
      game.draw(window);
      window.display();
  }
}