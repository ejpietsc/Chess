# Questions
- I think Board should be the subject - RESOLVED
- or at the very least if we have piece subjects, the board doesn't
need to observe pieces 
- Move may need to store capture info - TBD
- does notify take state or does the controller (main) communicate it
- return Move or Move&


# Ideas
- Template & Factory Method for Pieces
- may need pieces to have a ptr to Board
- Move stores all move info and it get passed to computer to determine which move 
it prefers based on lvl


# Thoughts
- piece doesn't need to have a ptr to board of know anything abt board
- Big 5 not needed for pieces
- utils is prob bad design - low cohesion

# Changes
- Changed Board to be the subject instead of Pieces - say a Piece moves, we can redraw the Piece by calling Piece.notify(). But how would we tell the display to draw the blank tile over where the Piece used to be? There is no Piece there to notify
- Added PlayerType::NULL_PLR to return from the playerStringToPlayerType method in main.cc when invalid input is given (same reasoning for adding NULL_PIECE)
- Made Board persist between games instead of being destroyed and recreated. This allowed us to keep track of score in the Board instead of in main
    - with this, we had to add whiteScore and blackScore private fields to Board, in addition to public setters/getters
