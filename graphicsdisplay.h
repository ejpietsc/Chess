#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include <vector>
#include "observer.h"
#include "window.h"

class GraphicsDisplay : public Observer
{
    Xwindow window;
public:
    GraphicsDisplay();
    void notify(Move& move) override; // ! figure out what args it takes
    ~GraphicsDisplay();

    friend std::ostream &operator<<(std::ostream &out, const GraphicsDisplay &td);
};

#endif
