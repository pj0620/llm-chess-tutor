#pragma once

enum Piece {
  BISHOP,
  KING,
  KNIGHT,
  PAWN,
  QUEEN,
  ROOK,
  EMPTY
};

enum PieceColor {
  BLACK,
  WHITE
};

struct ColoredPiece {
  Piece piece;
  PieceColor color;
};
