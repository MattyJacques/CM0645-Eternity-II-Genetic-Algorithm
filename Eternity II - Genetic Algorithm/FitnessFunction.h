// Title        : FitnessFunction.h
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#pragma once


#define CORNERMATCH 10          // How many points for corner match
#define EDGEMATCH   5           // How many points for edge match
#define INNERMATCH  1           // How many points for inner match


#include "BoardManager.h"       // For board size access, board definition


class FitnessFunction
{

private:
  Board* theBoard;                        // Holds pointer to current board
  BoardManager* boardMan;              // For access to board size

  // Checks to see if the pattern located on right side of piece1 matches the
  // pattern located on the left of piece2
  bool isMatchSide(int xIndex1,         // *In*
                   int yIndex1,         // *In*
                   int xIndex2,         // *In*
                   int yIndex2);        // *In*

  // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2
  bool isMatchBottom(int xIndex1,       // *In*
                     int yIndex1,       // *In*
                     int xIndex2,       // *In*
                     int yIndex2);      // *In*

  // Checks the top left corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces
  void checkTopLeft();

  // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces
  void checkTopRight();

  // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces
  void checkBottomLeft();

  // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces
  void checkBottomRight();

  // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void checkTopEdge();

  // Checks the slots along the left edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces
  void checkLeftEdge();

  // Checks the slots along the right edge to see if the piece placed within 
  // that slot is of the right piece type and checks if any edges of those 
  // pieces match with adjacent pieces
  void checkRightEdge();

  // Checks the slots along the bottom edge to see if the piece placed within 
  // that slot is of the right piece type and checks if any edges of those 
  // pieces match with adjacent pieces
  void checkBottomEdge();

  // Checks the pieces in the inner slots of the board for matching connections
  void checkInnerPieces();

public:
  // Initialse board and board manager pointers
  FitnessFunction();

  // Checks the fitness of the candidate provided by the GA
  void checkFitness(Board* theBoard);   // *In-Out*

}; // FitnessFunction

