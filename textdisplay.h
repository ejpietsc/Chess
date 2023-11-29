#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "util.h"
#include "board.h"
#include "observer.h"

class TextDisplay : public Observer
{
    std::vector<std::vector<char>> theDisplay;
    void doNotify(pair<int, int> pos, Piece *p) override;

    public:
        TextDisplay(Board *subject);
        // void updateMsg(ostream &out, string msg);
        // void displayScoreBoard(ostream &out);
        ~TextDisplay();

        friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td); 

#endif
