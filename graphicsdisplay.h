#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "util.h"
#include "window.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;
public:
    GraphicsDisplay();
    void doNotify(Move& move, GameState state) override;
    ~GraphicsDisplay();

    friend std::ostream &operator<<(std::ostream &out, const GraphicsDisplay &td);
};

#endif
