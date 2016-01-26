// Title        : Board.h
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include <vector>         // Vector for board information
#include "PuzzlePiece.h"  // Create puzzle piece objects


class Board
{

private:
  int fitScore;          // Holds fitness score for the board
  int boardID;           // Holds the ID of the board

public:
  // Vector of all pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVec; 

  // Initialises the fitness score
  Board();

  // Returns the fitness score of the board
  int GetFitScore();

  // Sets the fitness score for this particular board
  void SetFitScore(int score);

  // Destructor that deletes the instance of the class
  ~Board();


};

