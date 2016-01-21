// Title        : Board.h
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include <vector>         // Vector for board information
#include "PuzzlePiece.h"  // Create puzzle piece objects


class Board
{

public:

  // Vector of all pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVec; 

  // Destructor that deletes the instance of the class
  ~Board();


};

