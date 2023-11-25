// provides board class
#ifndef BOARD_H
#define BOARD_H

#include "util.h"

using namespace std;

enum class GameState { Setup, Play, Check, Checkmate, Stalemate, CTRL_D };

// class for the game board
//   ! Board now persists between games
class Board {
    vector<vector<Piece *>> board;
    GameState state;
    unique_ptr<Player> blackPlayer;
    unique_ptr<Player> whitePlayer;
    Colour turn;
    float whiteScore, blackScore = 0;

    // Determine GameStates
    bool isPlayerInCheck(Player &plr);
    bool isPlayerCheckmated(Player &plr);
    bool isPlayerStalemated(Player &plr);

    // ! [added] clear the Board, to be run by initBoard
    void clearBoard();

    public:
    // Ctor
    Board(PlayerType whitePt, const int whiteLevel, PlayerType blackPt, const int blackLevel);

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
    void addPiece(Position pos, PieceType pt, Colour clr);
    void delPiece(Position pos);

    // Getters and setters
    GameState getState();
    void setState(GameState state);
    Player *getPlayerByColour(Colour clr);
    Colour getTurn();
    void setTurn(Colour clr);
    Piece *getPiece(Position pos);
    void updateState();
    // ! [added] new setters and getters
    float getScore(Colour clr);
    void incrementScore(Colour clr, float addTo); // add addTo to player's score

    // General gameplay logic
    vector<Move> getValidMoves(Player &plr);
    bool isPlayerMoveValid();
    // Check if plr is checkmated or stalemated, 
    //  if this returns true, main can then call getState() to determine what to do
    bool didPlayerLose(Player &plr);

};

#endif
