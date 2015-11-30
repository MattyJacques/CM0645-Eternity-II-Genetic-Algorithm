#include "PuzzlePiece.h"


PuzzlePiece::PuzzlePiece()
{
  SetTop(0);
  SetRight(0);
  SetBottom(0);
  SetLeft(0);
  SetPieceID(0);
  SetOrientation(0);
} // PuzzlePiece()


void PuzzlePiece::SetTop(int pattern)
{
  segments[0] = pattern;
} // SetTop()


void PuzzlePiece::SetRight(int pattern)
{
  segments[1] = pattern;
} // SetRight()


void PuzzlePiece::SetBottom(int pattern)
{
  segments[2] = pattern;
} // SetBottom()


void PuzzlePiece::SetLeft(int pattern)
{
  segments[3] = pattern;
} // SetLeft()


void PuzzlePiece::SetOrientation(int ori)
{
  orientation = ori;
} // SetOrientation()


int PuzzlePiece::GetOrientation()
{
  return orientation;
} // GetOrientation()


void PuzzlePiece::SetPieceID(int theID)
{
  pieceID = theID;
} // SetPieceID()


int PuzzlePiece::GetPieceID()
{
  return pieceID;
} // GetPieceID()