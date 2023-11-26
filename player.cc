#include "player.h"

using namespace std;

// TODO

// === PLAYER ===
Player::Player(Colour team, PlayerType p)
    : team{team}, p{p} {}

Move Player::getNextMove() const
{
    return this->doGetNextMove();
}

// === HUMAN ===
Human::Human(Colour team, PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove() const
{
    return this->getHumanMove();
}

Move Human::getHumanMove() const
{
    string currLine, start, end;
    getline(cin, currLine);

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }

    stringstream ss{currLine};
    ss >> start >> end;

    // handle read fail
    if (ss.fail())
    {
        return Move{{-1, -1}, {-1, -1}}; // return a trash move that getValidMoves will not contain
    }

    Position startPos{start};
    Position endPos{end};

    return Move{startPos, endPos};
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(Colour team, PlayerType p, int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove() const
{
    return this->generateMove(this->generateValidMoves());
}
