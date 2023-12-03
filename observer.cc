#include "util.h"
#include "observer.h"

// Calls the doNotify function to update and print the grid
void Observer::notify(Position pos, Piece *p)
{
    // NVI - Call the doNotify function
    doNotify(pos, p);
    doUpdate();
}
void Observer::notify(std::vector<std::pair<Position, Piece *>> vec) {
    for (std::pair<Position, Piece *> p : vec) {
        doNotify(p.first, p.second);
    }
    doUpdate();
}

void Observer::update() {
    doUpdate();
}
