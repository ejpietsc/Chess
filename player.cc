#include "player.h"
#include "board.h"

using namespace std;

// TODO

// PUBLIC HELPER
//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isHuman(const Player* p) {
    return (p->getPlayerType() == PlayerType::Human);
}

//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isWhiteTeam(const Player* p) {
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

Move Player::getNextMove(vector<Move>& validMoves) const
{
    return this->doGetNextMove(validMoves);
}

// === HUMAN ===
Human::Human(const Colour team, const PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove(vector<Move>& validMoves) const
{
    return this->getHumanMove(validMoves);
}

//! check start and end are proper format
Move Human::getHumanMove(vector<Move>& validMoves) const
{
    string currLine, start, end;
    cin >> start >> end;

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }
    Move move = Move{start, end};
    if (!moveIsValid(move, validMoves)) {
        move.endPos = Position(illegal_move);
    }
    return move;
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(const Colour team, const PlayerType p, const int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove(vector<Move>& validMoves) const
{
    //! pick from validMoves
    return Move{Position{0, 0}, Position{0, 0}}; // TODO TODO TODO - Incorrect code - PLEASE REPLACE EVENTUALLY
}

LevelOne::LevelOne(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelTwo::LevelTwo(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelThree::LevelThree(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelFour::LevelFour(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}

LevelOne::~LevelOne() {}
LevelTwo::~LevelTwo() {}
LevelThree::~LevelThree() {}
LevelFour::~LevelFour() {}

// TODO !!!!! vvvv
Move LevelOne::generateMove(vector<Move> &moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelTwo::generateMove(vector<Move> &moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelThree::generateMove(vector<Move> &moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelFour::generateMove(vector<Move> &moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}
