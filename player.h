// provides player classes

#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"

using namespace std;

class Player
{
    Colour team;
    PlayerType p;
    virtual Move getNextMove() = 0;

public:
    Player(Colour team, PlayerType p);
    virtual ~Player() = 0;

    // NVI - call getNextMove(), which Human/Computer overwrites
    Move doGetNextMove();

    /* BIG 5 - Not needed given current implementation _______________________
    !IF NEEDED LATER MAKE PROTECTED
    // Copy ctor and assignment operator
    Player(const Player& other);
    Player& operator=(const Player& other);

    // Move ctor and assignment operator
    Player(Player&& other);
    Player& operator=(Player&& other); */
};

class Human final : public Player
{
    // call this in the overwritten getNextMove() method
    Move getHumanMove(string inputLine);
};

class Computer : public Player
{
    // call this in the overwritten getNextMove() method
    virtual Move generateMove(vector<Move> moves) = 0;
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

#endif
