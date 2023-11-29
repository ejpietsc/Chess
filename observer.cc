#include "util.h"
#include "observer.h"

void Observer::notify(pair<int, int> pos, Piece *p)
{
    doNotify(pos, p);
}