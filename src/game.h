// Game.h
#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "pieces.h"

using std::vector;

class Game {
  private:
    float cellSize;
    float xOffset;
    float yOffset;
    std::vector<std::vector<ColoredPiece>> boardState;

    // Textures
    std::vector<std::vector<sf::Texture>> pieceTextures;

    // Shapes/Sprite
    vector<sf::RectangleShape> gridCells;
    std::vector<sf::Sprite> pieceSprites;

    // Methods  
    void drawBoard(sf::RenderWindow& window);
    void resetGameBoard();

  public:
    Game(float cellSize = 10, float xOffset = 0, float yOffset = 0);
    void draw(sf::RenderWindow& window);
};