#include "util.h"
#include "observer.h"

// Calls the doNotify function to update and print the grid
void Observer::notify(Position pos, Piece *p)
{
    // NVI - Call the doNotify function
    if (doNotify(pos, p)) doUpdate();
}
void Observer::notify(std::vector<std::pair<Position, Piece *>> vec) {
    bool to_update = false;
    for (std::pair<Position, Piece *> p : vec) {
        to_update = to_update || doNotify(p.first, p.second);
    }
    if (to_update) doUpdate();
}

void Observer::update() {
    doUpdate();
}
