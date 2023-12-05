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

Move Player::getNextMove(vector<Move> &validMoves, Board *b) const
{
    return this->doGetNextMove(validMoves, b);
}

// === HUMAN ===
Human::Human(const Colour team, const PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove(vector<Move> &validMoves, Board *b) const
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
        move.endPos = Position(ILLEGAL_MOVE);
    }
    return move;
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(const Colour team, const PlayerType p, const int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove(vector<Move> &validMoves, Board *b) const
{
    //! pick from validMoves
    return generateMove(validMoves, b);
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
Move LevelOne::generateMove(vector<Move> &moves, Board *b) const
{
    // Pick a random move from the vector of possible legal moves
    return moves[rand() % moves.size()];
}

// Level 2: prefers capturing moves and checks over other moves
Move LevelTwo::generateMove(vector<Move> &moves, Board *b) const
{
    int num_moves = moves.size();
    Move bestMove; // Best move so far
    int bestscore = -1; // Best score so far

    // Iterate through moves
    for (Move m : moves) {
        // Calculate score for move
        int currscore = m.captured ? 10 : 0;

        // Update the best move and score if the score is better
        if (currscore > bestscore) {
            // Check if the move causes a check - increase move score
            if (b->putsPlayerInCheck(m, b->getNextPlayer())) currscore = 100;

            // If the score is better - use that move
            // If the score is better, use RNG to decide whether to use this move
            if (
                (currscore > bestscore) ||
                ((currscore == bestscore) && (num_moves > 2) && ((rand() % num_moves) <= 2))
            ) {
                bestscore = currscore;
                bestMove = m;
            }
        }
    }

    return bestMove; // Return the best move
}

Move LevelThree::generateMove(vector<Move> &moves, Board *b) const
{
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelFour::generateMove(vector<Move> &moves, Board *b) const
{
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}
