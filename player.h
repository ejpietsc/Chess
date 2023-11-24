// provides player classes

#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include <string>

using namespace std;

enum class PlayerType
{
    Human,
    Computer
};

class Player
{
    Colour team;
    PlayerType p;

    // Computer overwrites to call generateMove, Human overwrites to call getHumanMove()
    virtual Move getNextMove() = 0;

public:
    Player(Colour team, PlayerType p);

    virtual ~Player() = 0;
    
    // NVI - call getNextMove()
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
    // get next move from input and return it as a Move object
    Move getHumanMove();

    // call getHumanMove()
    Move getNextMove() override;
};

class Computer : public Player
{
    // generate a Move object using algorithm depending on Computer level
    virtual Move generateMove(vector<Move> moves) = 0;

    // call generateMove()
    Move getNextMove() override;
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
