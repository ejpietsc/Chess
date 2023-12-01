// provides player classes

#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include "pieces.h"

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
    //! [modified] doGetNextMove now takes validMoves
    virtual Move doGetNextMove(vector<Move>& validMoves) const = 0;

public:
    Player(const Colour team, const PlayerType p);

    virtual ~Player() = default;

    // NVI - calls doGetNextMove()
    //! [modified] getNextMove now takes validMoves
    Move getNextMove(vector<Move>& validMoves) const;
    Colour getColour() const;
    PlayerType getPlayerType() const;
};

class Human final : public Player
{
    // get next move from input and return it as a Move object
    //  this will throw if it fails to read from cin - handled in main
    Move getHumanMove(vector<Move>& validMoves) const;

  public:
    Human(const Colour team, const PlayerType p);
        // call getHumanMove()
    Move doGetNextMove(vector<Move>& validMoves) const override;
};

//! ADDED LVL FIELD
class Computer : public Player
{
    protected:
    int lvl;
    // generate a vector of valid moves to pass to generateMove

    // generate a Move object using algorithm depending on Computer level
    //  pass it generateValidMoves() rvalue
    virtual Move generateMove(vector<Move> &moves) const = 0;
    public:
    Computer(const Colour team, const PlayerType p, const int lvl);
    int getLvl() const;

    // NVI - call generateMove()
    Move doGetNextMove(vector<Move>& validMoves) const override;
};

class LevelOne final : public Computer
{
    Move generateMove(vector<Move>& moves) const override; // random pick

  public:
    LevelOne(const Colour team, const PlayerType p, const int lvl);
    ~LevelOne();
};

class LevelTwo final : public Computer
{
    Move generateMove(vector<Move>& moves) const override; // prefers capture & checks

  public:
    LevelTwo(const Colour team, const PlayerType p, const int lvl);
    ~LevelTwo();
};

class LevelThree final : public Computer
{
    Move generateMove(vector<Move>& moves) const override; // prefers avoiding capture, capture & checks

  public:
    LevelThree(const Colour team, const PlayerType p, const int lvl);
    ~LevelThree();
};

class LevelFour final : public Computer
{
    Move generateMove(vector<Move>& moves) const override; // something sophisticated
    
  public:
    LevelFour(const Colour team, const PlayerType p, const int lvl);
    ~LevelFour();
};

//! added both below - used in board (don't make static or private)
bool isHuman(const Player* p);
bool isWhiteTeam(const Player* p);

/* BIG 5 - Not needed given current implementation _____________________
!IF NEEDED LATER MAKE PROTECTED
// Copy ctor and assignment operator
Player(const Player& other);
Player& operator=(const Player& other);

// Move ctor and assignment operator
Player(Player&& other);
Player& operator=(Player&& other); */

#endif
