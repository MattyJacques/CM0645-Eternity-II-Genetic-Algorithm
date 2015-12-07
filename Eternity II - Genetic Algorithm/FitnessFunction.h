// Title        : FitnessFunction.h
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#pragma once


#include "Board.h"
#include "BoardManager.h"       // For board size access


class FitnessFunction
{

private:
  int score;
  static FitnessFunction* pInstance;    // Holds the current instance of class
  Board* pBoard;
  BoardManager* pBoardMan;          // For access to board size

  // Protected to avoid unauth creation of instances
  FitnessFunction();

public:
  // Returns the current instance of the class, creates if needed
  static FitnessFunction* GetInstance();

  // Checks the fitness of the candidate provided by the GA
  int CheckFitness(Board* theBoard);

  // Checks if the given slot is on a corner of the board using the indexes
  //bool IsCornerSlot(int x, int y);

  // Checks if the given slot is on a edge of the board using the indexes
  //bool IsEdgeSlot(int x, int y);

  // Checks the right side of the piece to see if it matches the left side of
  // the piece to the right
  //bool CheckRight(int x, int y, Board* theBoard);

  // Checks the bottom side of the piece to see if it matches the top side of
  // the piece underneath
  //bool CheckBottom(int x, int y, Board* theBoard);

  // Checks to see if the pattern located on right side of piece1 matches the
  // pattern located on the left of piece2
  bool IsMatchSide(PuzzlePiece piece1, PuzzlePiece piece2);

  // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2
  bool IsMatchBottom(PuzzlePiece piece1, PuzzlePiece piece2);

  // Checks the top left corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces
  void CheckTopLeft();

  // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces
  void CheckTopRight();

  // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces
  void CheckBottomLeft();

  // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces
  void CheckBottomRight();

  // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void CheckTopEdge();

  // Checks the slots along the left edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void CheckLeftEdge();

  // Checks the slots along the right edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void CheckRightEdge();

  // Checks the slots along the bottom edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void CheckBottomEdge();

  // Checks the pieces in the inner slots of the board for matching connections
  void CheckInnerPieces();

};

