// Title        : Board.h
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015

#pragma once

#include <vector>         // Vector for board information
#include "PuzzlePiece.h"  // Include the header file



class Board
{

public:
  int boardSize;                   // Holds the grid size for the puzzle
  int patternNum;                  // Number of patterns for the puzzle
  static Board* pInstance;         // Holds the current instance of the class

  std::vector<PuzzlePiece> pieceVec;

  // Pointer to pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVec; 

  Board();

  // Returns the current instance of the class
  //static Board* GetInstance();

  // Creates the static instance for the class, calling the constructor with
  // the puzzle size along with the number of colours and shapes
  //static void CreateInstance(int puzzleSize, int pattern);

  // Creates the board and sets inital tile placement
  void CreateInitialBoard();

  // Destructor that deletes the instance of the class
  ~Board();


};

