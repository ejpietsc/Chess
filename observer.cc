#include "util.h"
#include "observer.h"

// notify(pair, Piece *) - Calls the doNotify function to update and print the grid
void Observer::notify(Position pos, Piece *p)
{
    // NVI - Call the doNotify function
    doNotify(pos, p);
}