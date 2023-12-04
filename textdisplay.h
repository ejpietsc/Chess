#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "util.h"
#include "board.h"
#include "observer.h"

class TextDisplay : public Observer
{
    std::vector<std::vector<char>> theDisplay;

    // doNotify - Updates a tile on the board
    bool doNotify(Position pos, Piece *p) override;

    // doDisplay - Prints out the board again
    void doDisplay() override;

    // Force refresh everything
    void doRefresh(const Board *subject) override;

    public:
        TextDisplay() = default; //! used in Board copy/move- don't delete
        TextDisplay(const Board *subject);
        ~TextDisplay();

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td); 

#endif
