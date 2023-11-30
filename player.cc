#include "player.h"

using namespace std;

// TODO

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

Move Player::getNextMove() const
{
    return this->doGetNextMove();
}

// === HUMAN ===
Human::Human(const Colour team, const PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove() const
{
    return this->getHumanMove();
}

Move Human::getHumanMove() const
{
    string currLine, start, end;
    cin >> start >> end;

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }

    Position startPos{start};
    Position endPos{end};

    return Move{startPos, endPos};
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(const Colour team, const PlayerType p, const int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove() const
{
    return this->generateMove(this->generateValidMoves());
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
Move LevelOne::generateMove(vector<Move> &&moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelTwo::generateMove(vector<Move> &&moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelThree::generateMove(vector<Move> &&moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}

Move LevelFour::generateMove(vector<Move> &&moves) const {
    cout << "-Incomplete method-" << endl;
    Move m{};
    return m;
}
