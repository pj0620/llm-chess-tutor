// Game.h
#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "pieces.h"
#include <random>

using std::vector;

class Game {
  private:
    float cellSizeX;
    float cellSizeY;
    float xOffset;
    float yOffset;
    float perspectivePointH;
    std::vector<std::vector<ColoredPiece>> boardState;
    PieceColor playerColor;

    // Textures
    std::vector<std::vector<sf::Texture>> pieceTextures;

    // Shapes/Sprite
    vector<sf::ConvexShape> gridCells;
    std::array<std::vector<sf::Sprite>, 8> pieceSprites;

    // Methods  
    void drawBoard(sf::RenderWindow& window);
    void resetGameBoard();
    sf::Vector2f getPoint(float i, float j);
    float getPerspectiveWidth(float j);

    // RNG
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<int> dist;
    
  public:
    Game(float cellSizeX = 10, float cellSizeY = 10, float xOffset = 0, float yOffset = 0, float perspectivePointH=50);
    void draw(sf::RenderWindow& window);
};