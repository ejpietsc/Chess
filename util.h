// provides various structs, classes, and helper functions

#ifndef UTIL_H
#define UTIL_H

using namespace std;

enum class PieceType { King, Queen, Bishop, Rook, Knight, Pawn };
enum class Colour { White, Black };
enum class GameState { Setup, Play, Check, Checkmate, Stalemate };

struct Position {
    int row, col;
};

struct Move {
    Position position1, position2;
};

#endif
