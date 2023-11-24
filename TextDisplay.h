#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "pieces.h"
#include <vector>

class TextDisplay : public Observer
{
    std::vector<std::vector<char>> theDisplay;
public:
    TextDisplay();
    void notify() override; // ! figure out what args it takes
    ~TextDisplay();

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
