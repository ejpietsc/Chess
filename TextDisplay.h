#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "util.h"

class TextDisplay : public Observer
{
    std::vector<std::vector<char>> theDisplay;
    void doNotify(Move &move, GameState state) override;

public:
    TextDisplay(Board *subject); // done
    void updateMsg(ostream &out, string msg);
    void displayScoreBoard(ostream &out);
    ~TextDisplay();

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td); // done
};

SquareColor getSquareColor(int c, int r); //? maybe move somewhere more appropriate - done
char squareCharDisplay(int c, int r);     //! added - done

std::ostream &operator<<(std::ostream &out, const TextDisplay &td); 

#endif
