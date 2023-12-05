#include "util.h"
#include "observer.h"

// Calls the doNotify function to update and print the grid
void Observer::notify(Position pos, Piece *p)
{
    // NVI - Call the doNotify function
    if (doNotify(pos, p)) doDisplay();
}
void Observer::notify(std::vector<std::pair<Position, Piece *>> vec) {
    for (std::pair<Position, Piece *> p : vec) {
        doNotify(p.first, p.second);
    }
    doDisplay();
}

void Observer::display() {
    doDisplay();
}

void Observer::refresh(const Board *subject) {
    doRefresh(subject);
}
