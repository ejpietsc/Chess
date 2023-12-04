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

enum class SquareColor
{
    Dark,
    Light
};

int getRowForOutput(int r);

// class for the game board
//   ! Board now persists between games
class Board
{
    vector<vector<unique_ptr<Piece>>> board; // 2D vector
    vector<unique_ptr<Observer>> observers;  // textDisplay & GraphicsDisplay
    GameState state;                         // ! GET RIDE OF THIS FIELD
    unique_ptr<Player> whitePlayer;
    unique_ptr<Player> blackPlayer; //* player is abstract - must be ptr
    Player *currPlayer;             // non-ownership
    float whiteScore = 0;
    float blackScore = 0;

    // Determine GameStates
    bool isPlayerInCheck(const Player *plr) const;
    bool isPlayerCheckmated(const Player *plr) const;
    bool isPlayerStalemated(const Player *plr) const;

    // called by initBoard()
    void clearBoard(); // done

public:
    Board();                                                                                               // done
    Board(const PlayerType whitePl, const int whiteLevel, const PlayerType blackPl, const int blackLevel); // done

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
    void notifyObservers(Position pos) const;
    void notifyObservers(std::vector<Position> vec) const;
    void displayObservers() const;
    void refreshObservers() const;
    void attach(unique_ptr<Observer> o); // done

    // SetUp mode
    void addPiece(const PieceType &pt, const Colour &clr, const Position &pos); // done
    void delPiece(const Position &pos);                                         // done
    bool boardIsValid() const;                                                  // todo: finish once isInCheckmate done

    // getters
    GameState getState() const;                  // done
    Player *getPlayerByColour(Colour clr) const; // done
    Player *getCurrPlayer() const;               // done
    Piece *getPiece(const Position &pos) const;  // done
    Piece *getPieceByCoords(int c, int r) const; // done
    float getScore(Colour clr);
    vector<Piece *> getPlayerPieces(const Player *plr) const;
    Player *getNextPlayer() const;
    int Board::getPieceTypeCount(const PieceType &pt, const Colour &col) const;

    // done

    // setters
    void setState(GameState state); // done
    void setTurn(Colour clr);       // done

    void flipTurn();                              // done
    void incrementScore(Colour clr, float addTo); // done
    // add addTo to player's score

    // Move logic
    vector<Move> getMovesToUncheck(vector<Move> &moves) const; // todo
    bool putsPlayerInCheck(const Move &m, const Player *plr) const;
    //! [updated] returns position just for the sake of error reporting
    Position makeMove();
    void doMove(const Move &m);                                                   //! [added]
    bool checkMovePiece(const Move &m) const;                                     // Checks if the piece in the location can make the move
    bool checkMoveEndPos(const Move &m) const;                                    // Checks if the end location is within bounds and can be occupied
    bool isLegalMove(Move m) const;                                               // Checks if the move doesn't put the player in checkmate
    vector<Move> getValidMoves(const Player *plr, bool experiment = false) const; // Gets a list of valid moves that the player can make
    // vector<Move> getLegalMoves(const Player *plr) const; // Gets a list of legal moves that the player can make
    // NOTE: Valid move: Within bounds, acceptable end position
    // Legal move: Valid move AND doesn't put player in check

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
Colour getNextColour(Colour clr);
bool moveIsValid(Move &move, vector<Move> &validMoves);

#endif
