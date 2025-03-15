#include "game.h"
#include <SFML/Graphics.hpp>
#include "string"
#include <format>
#include <iostream>
#include <random>
#include <algorithm>
#include <ranges>
#include <vector>

using std::string;
using std::format;

// Define static members outside the class
std::random_device Game::rd;
std::mt19937 Game::gen(Game::rd());  // Seed generator once
std::uniform_int_distribution<int> Game::dist(0, 1);  // Only needs to be defined once

sf::Vector2f Game::getPoint(float i, float j) {
  // Shift i so that the center of the board (i = 4) is at x=0 in "3D space"
  float x3D = (i - 4.0f) * cellSizeX;

  // We can treat j as "depth" in 3D, so z3D grows with j
  // (You could scale it by cellSizeY if you want deeper or shallower perspective.)
  float z3D = j * cellSizeY;

  // The perspective factor: as z3D grows, factor shrinks
  // perspectivePointH is playing the role of 'focal length'
  float factor = perspectivePointH / (perspectivePointH + z3D);

  // Apply perspective to x3D; then shift back so that
  // when i=4, the board is centered at xOffset + 4*cellSizeX.
  float screenX = xOffset + (x3D * factor) + (4.0f * cellSizeX);

  // For y, do something similar, or simply let j define the vertical spacing in perspective
  float screenY = yOffset - (j * cellSizeY * factor);

  return sf::Vector2f(screenX, screenY);
}

float Game::getPerspectiveWidth(float j) {
  float z3D = j * cellSizeY;
  float factor = perspectivePointH / (perspectivePointH + z3D);
  return cellSizeX * factor;
}

Game::Game(float cellSizeX, float cellSizeY, float xOffset, float yOffset, float perspectivePointH)
    : cellSizeX(cellSizeX), cellSizeY(cellSizeY), xOffset(xOffset), yOffset(yOffset), perspectivePointH(perspectivePointH) {

  // create Game Cells
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      sf::ConvexShape newCell;
      newCell.setPointCount(4);

      newCell.setPoint(0, getPoint(i, j));
      newCell.setPoint(1, getPoint(i + 1, j));
      newCell.setPoint(2, getPoint(i + 1, j + 1));
      newCell.setPoint(3, getPoint(i, j + 1));

      if ((i + j) % 2 == 0) {
        newCell.setFillColor(sf::Color::White);
      } else {
        newCell.setFillColor(sf::Color(128, 128, 128));
      }
      gridCells.push_back(newCell);
    }
  }

  // Load piece textures (must match order in pieces.h)
  const string pieces[] = {"bishop", "king", "knight", "pawn", "queen", "rook"};
  const string colors[] = {"black", "white"};

  pieceTextures.resize(2, std::vector<sf::Texture>(6));
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
  for (int j = 7; j >= 0; j--) {
    pieceSprites[j] = std::vector<sf::Sprite>();
    for (int i = 0; i < 8; i++) {

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
        auto persWidth = 1.2 * getPerspectiveWidth(j);
        sf::Vector2f newScale(persWidth / textureSize.x, persWidth / textureSize.y);
        pieceSprite.setScale(newScale);

        auto pos = getPoint(i + 0.5, j + 0.5);
        pos += sf::Vector2f(persWidth * -0.5, -persWidth * 0.75);
        pieceSprite.setPosition(pos);
        pieceSprites[j].push_back(pieceSprite);

        boardState[i][j] = newPiece;
      }
    }
  }
}

void Game::draw(sf::RenderWindow& window) {
  for (sf::ConvexShape cell : gridCells) {
    window.draw(cell);
  }

  for (std::vector<sf::Sprite> sprites : std::views::reverse(pieceSprites)) {
    for (sf::Sprite sprite : sprites) {
      window.draw(sprite);
    }
  }
}
