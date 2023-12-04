#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "util.h"
#include "board.h"
#include "window.h"
#include "observer.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;

    // Helper functions for displaying things
    void displayTile(int c, int r);
    void displayPiece(int c, int r, Piece *p);
    void displayGridLoc(int c, int r);

    // doNotify - Updates a tile on the board
    bool doNotify(Position pos, Piece *p) override;

    // doDisplay - Does nothing (it's for the TextDisplay only)
    void doDisplay() override;

    // Force refresh everything
    void doRefresh(const Board *subject) override;

    public:
        GraphicsDisplay(); //! used in Board copy/move- don't delete
        GraphicsDisplay(const Board *subject);
        ~GraphicsDisplay();
};

#endif
