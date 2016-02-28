// Title        : PuzzlePiece.h
// Purpose      : Holds information about the puzzle piece
// Author       : Matthew Jacques
// Date         : 23/11/2015


#pragma once


// Enum so the code can use colours for the tile pieces
enum segLocation { TOP, RIGHT, BOTTOM, LEFT };

// Ease of use for the type of puzzle piece
enum PieceType { CORNER, EDGE, INNER };


class PuzzlePiece
{

private:
  int pieceID;      // Holds the ID of the puzzle piece
  PieceType type;    // Holds the type of the puzzle piece, example corner, edge
  int orientation;  // Holds the rotation of the puzzle piece
  int segments[4];  // Holds the pattern for each segment of the piece


public:
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

  // Sets the type of the piece using the type provided
  void SetType(PieceType aType);

  // Returns the pattern on the top of the puzzle piece
  int GetTop();

  // Returns the pattern on the right of the puzzle piece
  int GetRight();

  // Returns the pattern on the bottom of the puzzle piece
  int GetBottom();

  // Returns the pattern on the left of the puzzle piece
  int GetLeft();

  // Returns the orientation of the puzzle piece
  int GetOrientation();

  // Returns the piece ID of the puzzle piece
  int GetPieceID();

  // Returns the type of piece
  PieceType GetType();

};

