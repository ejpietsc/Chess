// provides board class
#ifndef BOARD_H
#define BOARD_H

#include "util.h"

using namespace std;

enum class GameState { Setup, Play, Check, Checkmate, Stalemate };

// class for the game board
//   ! Board now persists between games
class Board {
    vector<Observer *> observers; // textDisplay & GraphicsDisplay
    vector<vector<unique_ptr<Piece>>> board; // 2D vector
    GameState state; // ! discuss
    unique_ptr<Player> blackPlayer; // ! TO BE DISCUSSED
    unique_ptr<Player> whitePlayer;
    Player* currPlayer; // non-ownership
    float whiteScore, blackScore = 0;

    // Determine GameStates
    bool isPlayerInCheck(Player &plr);
    bool isPlayerCheckmated(Player &plr);
    bool isPlayerStalemated(Player &plr);

    // ! [added] clear the Board, to be run by initBoard
    void clearBoard();

    public:
    // ! [updated] Ctor
    Board(PlayerType whitePt, const int whiteLevel, PlayerType blackPt, const int blackLevel);

    // Copy ctor and assignment operator
    Board(const Board& other);
    Board& operator=(const Board& other);

    // Move ctor and assignment operator
    Board(Board&& other);
    Board& operator=(Board&& other);

    // Dtor
    ~Board();

    // Observer Pattern methods
    void notifyObservers() const;
    void attach(Observer *o);

    // Change the pieces
    void initBoard();
    // ! Note: addPiece/delPiece are responsible for confirming that
    // !    pos is a valid Position (it will be taken from input)
    void addPiece(Position pos, PieceType pt, Colour clr);
    void delPiece(Position pos);

    // Getters and setters
    GameState getState();
    void setState(GameState state);
    Player *getPlayerByColour(Colour clr);
    Colour getTurn();
    // ! [added] 1 new method (for switching between player turns in main, and for setup '= color' command)
    // TODO discuss - ideally board has a getMove() method that calls
    // TODO   the current player's getMove() method AND AFTERWARDS sets the next player's turn upon
    // TODO   a SUCCESSFUL turn, instead of main doing this
    void setTurn(Colour clr);
    Piece *getPiece(Position pos);
    void updateState();
    // ! [added] 2 new setters and getters
    float getScore(Colour clr);
    void incrementScore(Colour clr, float addTo); // add addTo to player's score

    // General gameplay logic
    // ! [added] 1 new method
    bool boardIsValid() const; // for setup mode, returns true if board has 
                                // valid configuration (1 king, pawns not on 1st/8th rows)
    vector<Move> getValidMoves(Player &plr);
    bool isPlayerMoveValid();
    // Check if plr is checkmated or stalemated, 
    //  if this returns true, main can then call getState() to determine what to do
    bool didPlayerLose(Player &plr);

};

#endif
