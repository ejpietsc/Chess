// provides board class
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "util.h"
#include "pieces.h"
#include "observer.h"
#include "player.h"

using namespace std;

// class for the game board
class Board {
    vector<vector<Piece *>> board;
    GameState state;
    unique_ptr<Player> blackPlayer;
    unique_ptr<Player> whitePlayer;
    Colour turn;

    public:
    // Ctor
    Board();

    // Copy ctor and assignment operator
    Board(const Board& other);
    Board& operator=(const Board& other);

    // Move ctor and assignment operator
    Board(Board&& other);
    Board& operator=(Board&& other);

    // Dtor
    ~Board();

    // Change the pieces
    void initBoard();
    void addPiece(Position pos, PieceType pt, Colour col);
    void delPiece(Position pos);

    // Getters and setters
    GameState getState();
    void setState(GameState state);
    Player *getPlayerByColour(Colour clr);
    Colour getTurn();
    Piece *getPiece(Position p);
    void updateState();

    // General gameplay logic
    vector<Move> getValidMoves(Player &plr);
    bool isPlayerMoveValid();

};

#endif
