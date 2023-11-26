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
    virtual Move doGetNextMove() const = 0;

public:
    Player(Colour team, PlayerType p);

    virtual ~Player() = default;

    // NVI - call getNextMove()
    Move getNextMove() const;
    Colour getColour() const;
    PlayerType getPlayerType() const;
};

class Human final : public Player
{
    Human(Colour team, PlayerType p);

    // get next move from input and return it as a Move object
    //  this will throw if it fails to read from cin - handled in main
    Move getHumanMove() const;

    // call getHumanMove()
    Move doGetNextMove() const override;
};

//! ADDED LVL FIELD
class Computer : public Player
{
    protected:
    int lvl;
    // generate a vector of valid moves to pass to generateMove
    virtual vector<Move> generateValidMoves() const = 0;

    // generate a Move object using algorithm depending on Computer level
    //  pass it generateValidMoves() rvalue
    virtual Move generateMove(vector<Move> &&moves) const = 0;
    public:
    Computer(Colour team, PlayerType p, int lvl);
    int getLvl() const;

    // call generateMove()
    Move doGetNextMove() const override;
};

class LevelOne final : public Computer
{
    Move generateMove(vector<Move> &&moves) const override; // random pick
};

class LevelTwo final : public Computer
{
    Move generateMove(vector<Move> &&moves) const override; // prefers capture & checks
};

class LevelThree final : public Computer
{
    Move generateMove(vector<Move> &&moves) const override; // prefers avoiding capture, capture & checks
};

class LevelFour final : public Computer
{
    Move generateMove(vector<Move> &&moves) const override; // something sophisticated
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
