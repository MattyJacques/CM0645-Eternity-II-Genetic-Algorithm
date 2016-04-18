// Title        : FitnessFunction.h
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#pragma once


#define CORNERMATCH 10
#define EDGEMATCH   5
#define INNERMATCH  1


#include "BoardManager.h"       // For board size access


class FitnessFunction
{

private:
  Board* pBoard;                        // Holds pointer to current board
  BoardManager* pBoardMan;              // For access to board size

  // Checks to see if the pattern located on right side of piece1 matches the
  // pattern located on the left of piece2
  bool IsMatchSide(int xIndex1,         // *In*
                   int yIndex1,         // *In*
                   int xIndex2,         // *In*
                   int yIndex2);        // *In*

  // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2
  bool IsMatchBottom(int xIndex1,       // *In*
                     int yIndex1,       // *In*
                     int xIndex2,       // *In*
                     int yIndex2);      // *In*

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

public:
  // Initialse board and board manager pointers
  FitnessFunction();

  // Checks the fitness of the candidate provided by the GA
  void CheckFitness(Board* theBoard);   // *In-Out*

}; // FitnessFunction

