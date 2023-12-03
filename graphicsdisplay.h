#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "util.h"
#include "window.h"
#include "board.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;

    bool doNotify(Position pos, Piece *p) override;
    void doUpdate() override;

    public:
        GraphicsDisplay(); //! used in Board copy/move- don't delete
        GraphicsDisplay(Board *subject);
        ~GraphicsDisplay();
};

#endif
