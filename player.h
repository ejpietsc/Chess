// provides player classes

#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include <string>

using namespace std;

class Player {
    public:
    Player();

    // Copy ctor and assignment operator
    Player(const Player& other);
    Player& operator=(const Player& other);

    // Move ctor and assignment operator
    Player(Player&& other);
    Player& operator=(Player&& other);

    virtual ~Player();

    virtual Move getNextMove() = 0; // return next Move

};

class Human final : public Player {

};

class Computer : public Player {
    virtual Move generateMove() = 0; // generate next move, to be overwritten by
                                    // different level AI's 
};

class LevelOne final : public Computer {

};

class LevelTwo final : public Computer {

};

class LevelThree final : public Computer {

};

class LevelFour final : public Computer {

};

#endif
