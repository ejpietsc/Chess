// provides board class
#ifndef BOARD_H
#define BOARD_H

#include "util.h"
#include "player.h"
#include "observer.h"
#include "pieces.h"

using namespace std;

enum class GameState
{
    Setup,
    Play,
    Check,
    Checkmate,
    Stalemate,
    NA
};

// class for the game board
//   ! Board now persists between games
class Board
{
    vector<vector<unique_ptr<Piece>>> board; // 2D vector
    vector<unique_ptr<Observer>> observers;  // textDisplay & GraphicsDisplay
    GameState state;                         // ! discuss
    unique_ptr<Player> whitePlayer;
    unique_ptr<Player> blackPlayer; //* player is abstract - must be ptr
    Player *currPlayer;             // non-ownership
    float whiteScore, blackScore = 0;

    // Determine GameStates
    bool isPlayerInCheck(Player *plr) const;
    bool isPlayerCheckmated(Player *plr) const;
    bool isPlayerStalemated(Player *plr) const;

    // called by initBoard()
    void clearBoard(); // done

public:
    Board();                                                                                   // done
    Board(PlayerType whitePl, const int whiteLevel, PlayerType blackPl, const int blackLevel); // done

    // Copy ctor and assignment operator
    Board(const Board &other);
    Board &operator=(const Board &other);

    // Move ctor and assignment operator
    Board(Board &&other);
    Board &operator=(Board &&other);

    // Dtor
    ~Board() = default; // done

    void initBoard(); // done

    // Observer Pattern methods
    void notifyObservers() const;
    void attach(unique_ptr<Observer> o); // done

    // SetUp mode
    void addPiece(PieceType pt, Colour clr, Position pos); // done
    void delPiece(Position pos);                           // done
    bool boardIsValid() const;                             // todo: finish once isInCheckmate done

    // getters
    GameState getState() const;                  // done
    Player *getPlayerByColour(Colour clr) const; // done
    Player *getCurrPlayer() const;               // done
    Piece *getPiece(Position pos);               // done
    Piece *getPieceByCoords(int c, int r);       // done
    float getScore(Colour clr);                  // done

    // setters
    void setState(GameState state); // done
    void setTurn(Colour clr);       // done

    void flipTurn();                              // done
    void incrementScore(Colour clr, float addTo); // done
    // add addTo to player's score

    // Move logic
    bool makeMove();
    vector<Move> getValidMoves(Player *plr);
    bool isPlayerMoveValid();

    // Check if plr is checkmated or stalemated,
    //  if this returns true, main can then call getState() to determine what to do
    bool didPlayerLose(Player *plr);
    // TODO discuss - ideally board has a getMove() method that calls
    // TODO   the current player's getMove() method AND AFTERWARDS sets the next player's turn upon
    // TODO   a SUCCESSFUL turn, instead of main doing this
};
// public fn for entire program to use
bool isKing(Piece *p);
bool isPawn(Piece *p);
bool isWhite(Piece *p);
Colour getOtherColour(Colour clr);

#endif
