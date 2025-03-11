#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({700u, 400u}), "CMake SFML Project");
  window.setFramerateLimit(144);

  while (window.isOpen())
  {
      while (const std::optional event = window.pollEvent())
      {
          if (event->is<sf::Event::Closed>())
          {
              window.close();
          }
      }

      window.clear();
      window.display();
  }
}