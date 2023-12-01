# Questions
- Move may need to store capture info - TBD
- does notify take state or does the controller (main) communicate it
- return Move or Move&
- keep each player score field in player objects instead of board? - doesn't matter

# Improvements
- invalid moves error reporting
- getHuman Move() reports if invalid bc of syntax or cz not allowed
- controller



# Ideas
- Template & Factory Method for Pieces
- may need pieces to have a ptr to Board
- Move stores all move info and it gets passed to computer to determine which move 
it prefers based on lvl

# Thoughts
- piece doesn't need to have a ptr to board of know anything abt board
- Big 5 not needed for pieces
- utils is prob bad design - low cohesion
- indexing pieces is a bit confusing but we will go with what is intuitive given
vector indexing

# Changes
- Changed Board to be the subject instead of Pieces - say a Piece moves, we can redraw the Piece by calling Piece.notify(). But how would we tell the display to draw the blank tile over where the Piece used to be? There is no Piece there to notify
- Added PlayerType::NULL_PLR to return from the playerStringToPlayerType method in main.cc when invalid input is given (same reasoning for adding NULL_PIECE)
- Made Board persist between games instead of being destroyed and recreated. This allowed us to keep track of score in the Board instead of in main
    - with this, we had to add whiteScore and blackScore private fields to Board, in addition to public setters/getters

# THINGS TO CONSIDER (POST TALK WITH ROSS):
- remove static from non-helper fns that are on .h
- createPiece is useless and getPiecePtr
- fine to keep same players
- GET RIDE OF STATE field
- get rid of the ton on imports in util.h
- std::array takes size (no resizing)

# Move Logic:
- board fetches from piece (based on piece type)
- board filter the returned vec of moves:
    - landing spot is not occupied by same color piece
    - doesn't put king in check
    - if curr player in check
