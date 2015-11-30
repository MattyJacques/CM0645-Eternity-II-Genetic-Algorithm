#pragma once


class PuzzlePiece
{

private:
  int pieceID;      // Holds the ID of the puzzle piece
  int pieceType;    // Holds the type of the puzzle piece, example corner, edge
  int orientation;  // Holds the rotation of the puzzle piece
  int segments[4];  // Holds the pattern for each segment of the piece
  

public:
  // Enum so the code can use colours for the tile pieces
  enum segLocation { TOP, RIGHT, BOTTOM, LEFT };  

  // Initialises all variables to 0
  PuzzlePiece();

  // Sets the top of the piece to a pattern using the index provided
  void SetTop(int pattern);

  // Sets the right of the piece to a pattern using the index provided
  void SetRight(int pattern);

  // Sets the bottom of the piece to a pattern using the index provided
  void SetBottom(int pattern);

  // Sets the left of the piece to a pattern using the index provided
  void SetLeft(int pattern);

  // Sets the orientation of the piece using the index provided
  void SetOrientation(int ori);

  // Sets the ID for the piece to the integer provided
  void SetPieceID(int theID);

  // Returns the orientation of the puzzle piece
  int  GetOrientation();

  // Returns the piece ID of the puzzle piece
  int  GetPieceID();

};

