#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "util.h"

class TextDisplay : public Observer
{
    std::vector<std::vector<char>> theDisplay;
    void doNotify(Move& move, GameState state) override;
    
public:
    TextDisplay();
    void updateMsg(ostream& out, string msg);
    void displayScoreBoard(ostream& out);
    ~TextDisplay();

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td);

#endif
