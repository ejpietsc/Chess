#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "util.h"
#include "window.h"
#include "board.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;
    virtual void doNotify(Move& move, GameState state) override;

public:
    GraphicsDisplay();
    ~GraphicsDisplay();
};

#endif
