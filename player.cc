#include "player.h"
#include "board.h"
#include "util.h"


using namespace std;


// Constants for the computer players

// Values of different piece types
std::map<PieceType, int> PIECE_VALUES = {
    {PieceType::King, 500},
    {PieceType::Queen, 250},
    {PieceType::Bishop, 100},
    {PieceType::Rook, 100},
    {PieceType::Knight, 75},
    {PieceType::Pawn, 25}
};

// How many times the piece value is it to capture any enemy piece of the same type
std::map<PieceType, int> PIECE_CAPTURE_MULTIPLIERS = {
    {PieceType::King, 5},
    {PieceType::Queen, 3},
    {PieceType::Bishop, 1},
    {PieceType::Rook, 2},
    {PieceType::Knight, 1},
    {PieceType::Pawn, 2}
};

int CHECK_VALUE = 250; // Value of a check
int CHECKMATE_VALUE = 2500; // Value of a checkmate
int CHECK_CAPTURE_MULTIPLIER = 2; // How much more valuable it is to check the enemy


// TODO

// PUBLIC HELPER
//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isHuman(const Player *p)
{
    return (p->getPlayerType() == PlayerType::Human);
}

//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isWhiteTeam(const Player *p)
{
    return (p->getColour() == Colour::White);
}

// === PLAYER ===
Player::Player(const Colour team, const PlayerType p)
    : team{team}, p{p} {}

Colour Player::getColour() const
{
    return this->team;
}

PlayerType Player::getPlayerType() const
{
    return this->p;
}

Move Player::getNextMove(vector<Move> &validMoves) const
{
    return this->doGetNextMove(validMoves);
}

// === HUMAN ===
Human::Human(const Colour team, const PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove(vector<Move> &validMoves) const
{
    return this->getHumanMove(validMoves);
}

//! check start and end are proper format
Move Human::getHumanMove(vector<Move> &validMoves) const
{
    string currLine, start, end;
    cin >> start >> end;

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }
    Move move = Move{start, end};
    if (!moveIsValid(move, validMoves))
    {
        move.endPos = Position(illegal_move);
    }
    return move;
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(const Colour team, const PlayerType p, const int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove(vector<Move> &validMoves) const
{
    //! pick from validMoves
    return Move{Position{0, 0}, Position{0, 0}}; // TODO TODO TODO - Incorrect code - PLEASE REPLACE EVENTUALLY
}

int Computer::getLvl() const {
    return lvl;
}

LevelOne::LevelOne(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelTwo::LevelTwo(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelThree::LevelThree(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelFour::LevelFour(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}

LevelOne::~LevelOne() {}
LevelTwo::~LevelTwo() {}
LevelThree::~LevelThree() {}
LevelFour::~LevelFour() {}

// Level 1: random legal moves.
Move LevelOne::generateMove(vector<Move> &moves) const
{
    // Pick a random move from the vector of possible legal moves
    return moves[rand() % moves.size()];
}

// Level 2: prefers capturing moves and checks over other moves
Move LevelTwo::generateMove(vector<Move> &moves) const
{
    Move bestMove; // Best move so far
    int bestscore = -1; // Best score so far

    // Iterate through moves
    for (Move m : moves) {
        // Calculate score for move
        int currscore = m.captured ? PIECE_VALUES[m.capturedPt] * PIECE_CAPTURE_MULTIPLIERS[m.capturedPt] : 0;
        // TODO: Update score for checks and checkmates

        // Update the best move and score if the score is better
        if (currscore > bestscore) {
            bestscore = currscore;
            bestMove = m;
        }
    }

    return bestMove; // Return the best move
}

Move LevelThree::generateMove(vector<Move> &moves) const
{
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelFour::generateMove(vector<Move> &moves) const
{
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}
