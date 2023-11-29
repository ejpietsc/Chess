#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "util.h"
#include "window.h"
#include "board.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;
    void doNotify(pair<int, int> pos, Piece *p) override;

    public:
        GraphicsDisplay();
        ~GraphicsDisplay();
};

#endif
