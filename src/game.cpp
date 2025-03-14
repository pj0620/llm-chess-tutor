#include "game.h"
#include <SFML/Graphics.hpp>
#include "string"
#include <format>
#include <iostream>
#include <random>

using std::string;
using std::format;

// Define static members outside the class
std::random_device Game::rd;
std::mt19937 Game::gen(Game::rd());  // Seed generator once
std::uniform_int_distribution<int> Game::dist(0, 1);  // Only needs to be defined once

Game::Game(float cellSize, float xOffset, float yOffset)
    : cellSize(cellSize), xOffset(xOffset), yOffset(yOffset) {

  // create Game Cells
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      sf::RectangleShape newCell({cellSize, cellSize});

      // set color
      if ((i + j) % 2 == 0) {
        newCell.setFillColor(sf::Color::White);
      } else {
        newCell.setFillColor(sf::Color(128, 128, 128));
      }

      // move
      newCell.setPosition({xOffset + i * cellSize, yOffset + j * cellSize});

      gridCells.push_back(newCell);
    }
  }

  // Load piece textures (must match order in pieces.h)
  const string pieces[] = {"bishop", "king", "knight", "pawn", "queen", "rook"};
  const string colors[] = {"black", "white"};

  pieceTextures.resize(2, std::vector<sf::Texture>(6));
  pieceSprites.reserve(32);
  boardState.resize(8, std::vector<ColoredPiece>(8));

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {

      std::string filename = format("../assets/chess_pieces/{}/{}.png", colors[i], pieces[j]);

      sf::Texture pieceTexture(filename);
      if (!pieceTextures[i][j].loadFromFile(filename)) {
        std::cerr << "Error loading texture: " << filename << std::endl;
      }

      pieceTexture.setSmooth(true);
      pieceTexture.setRepeated(true);
      pieceTextures[i][j] = pieceTexture;
    }
  }

  resetGameBoard();
}

//TODO: will need player's current color
void Game::resetGameBoard() {
  playerColor = (PieceColor) dist(gen);
  PieceColor opponentColor = (PieceColor) (1 - playerColor);

  static std::array<Piece, 3> piecesFromCenter = {BISHOP, KNIGHT, ROOK};
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      ColoredPiece newPiece = {EMPTY, BLACK};

      if (j <= 1) {
        newPiece.color = opponentColor;
      } else {
        newPiece.color = playerColor;
      }

      if (j == 1 || j == 6) {
        newPiece.piece = PAWN;
      }

      if (j == 0 || j == 7) {
        int centerDist = i <= 3 ? abs(i - 4) - 1 : i - 4;
        if (centerDist > 0) {
          newPiece.piece = piecesFromCenter[centerDist - 1];
        } else if ((j == 0 && i == 3) || (j == 7 && i == 4))  {
          newPiece.piece = QUEEN;
        } else if ((j == 0 && i == 4) || (j == 7 && i == 3)) {
          newPiece.piece = KING;
        }
      }

      if (newPiece.piece != EMPTY) {
        sf::Texture& pieceTexture = pieceTextures[newPiece.color][newPiece.piece];
        sf::Vector2u textureSize = pieceTexture.getSize();
        sf::Sprite pieceSprite(pieceTexture);
        sf::Vector2f newScale(cellSize / textureSize.x, cellSize / textureSize.y);
        pieceSprite.setScale(newScale);
        pieceSprite.setPosition(sf::Vector2f(xOffset + i * cellSize, yOffset + j * cellSize));
        pieceSprites.push_back(pieceSprite);

        boardState[i][j] = newPiece;
      }
    }
  }
}

void Game::draw(sf::RenderWindow& window) {
  for (sf::RectangleShape cell : gridCells) {
    window.draw(cell);
  }

  for (sf::Sprite sprite : pieceSprites) {
    window.draw(sprite);
  }
}
