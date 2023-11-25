// provides player classes

#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include <string>

using namespace std;

// ! NULL_PLR added to return from stringToPlayerType in main.cc
enum class PlayerType
{
    Human,
    Computer,
    NULL_PLR,
};

class Player
{
    Colour team;
    PlayerType p;

    // Computer overwrites to call generateMove, Human overwrites to call getHumanMove()
    virtual Move doGetNextMove() = 0;

public:
    Player(Colour team, PlayerType p);

    virtual ~Player() = default;

    // NVI - call getNextMove()
    Move getNextMove();
    Colour getColour() const;
    PlayerType getPlayerType() const;
};

class Human final : public Player
{
    // get next move from input and return it as a Move object
    Move getHumanMove();

    // call getHumanMove()
    Move doGetNextMove() override;
};

class Computer : public Player
{
    // generate a Move object using algorithm depending on Computer level
    virtual Move generateMove(vector<Move> moves) = 0;

    // call generateMove()
    Move doGetNextMove() override;
};

class LevelOne final : public Computer
{
    Move generateMove(vector<Move> moves) override; // random pick
};

class LevelTwo final : public Computer
{
    Move generateMove(vector<Move> moves) override; // prefers capture & checks
};

class LevelThree final : public Computer
{
    Move generateMove(vector<Move> moves) override; // prefers avoiding capture, capture & checks
};

class LevelFour final : public Computer
{
    Move generateMove(vector<Move> moves) override; // something sophisticated
};

/* BIG 5 - Not needed given current implementation _____________________
!IF NEEDED LATER MAKE PROTECTED
// Copy ctor and assignment operator
Player(const Player& other);
Player& operator=(const Player& other);

// Move ctor and assignment operator
Player(Player&& other);
Player& operator=(Player&& other); */

#endif
