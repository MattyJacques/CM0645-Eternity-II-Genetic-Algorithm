// Title        : PuzzlePiece.cpp
// Purpose      : Holds information about the puzzle piece
// Author       : Matthew Jacques
// Date         : 23/11/2015


#include "PuzzlePiece.h" // Include the header for the class


PuzzlePiece::PuzzlePiece()
{ // Initialises all variables of the puzzle piece to 0

  SetTop(0);
  SetRight(0);
  SetBottom(0);
  SetLeft(0);
  SetPieceID(0);
  SetOrientation(0);

} // PuzzlePiece()


void PuzzlePiece::SetTop(int pattern)
{ // Sets the top of the piece to a pattern using the index provided

  segments[0] = pattern;

} // SetTop()


void PuzzlePiece::SetRight(int pattern)
{ // Sets the right of the piece to a pattern using the index provided

  segments[1] = pattern;

} // SetRight()


void PuzzlePiece::SetBottom(int pattern)
{ // Sets the bottom of the piece to a pattern using the index provided

  segments[2] = pattern;

} // SetBottom()


void PuzzlePiece::SetLeft(int pattern)
{ // Sets the left of the piece to a pattern using the index provided

  segments[3] = pattern;

} // SetLeft()


void PuzzlePiece::SetOrientation(int ori)
{ // Sets the orientation of the piece using the index provided

  orientation = ori;

} // SetOrientation()


void PuzzlePiece::SetPieceID(int theID)
{ // Sets the ID for the piece to the integer provided

  pieceID = theID;

} // SetPieceID()


void PuzzlePiece::SetType(PieceType aType)
{ // Sets the type of the piece using the type provided

  type = aType;

} // SetType()


int PuzzlePiece::GetTop()
{ // Returns the pattern on the top of the puzzle piece

  return segments[TOP];

} // GetTop()


int PuzzlePiece::GetRight()
{ // Returns the pattern on the right of the puzzle piece

  return segments[RIGHT];

} // GetRight()


int PuzzlePiece::GetBottom()
{ // Returns the pattern on the bottom of the puzzle piece

  return segments[BOTTOM];

} // GetBottom()


int PuzzlePiece::GetLeft()
{ // Returns the pattern on the left of the puzzle piece

  return segments[LEFT];

} // GetLeft()


int PuzzlePiece::GetOrientation()
{ // Returns the orientation of the puzzle piece

  return orientation;

} // GetOrientation()


int PuzzlePiece::GetPieceID()
{ // Returns the piece ID of the puzzle piece

  return pieceID;

} // GetPieceID()


PieceType PuzzlePiece::GetType()
{ // Returns the type of piece

  return type;

} // GetType()