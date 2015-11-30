// Title        : Board.h
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015

#pragma once

#include "PuzzlePiece.h"  // Include the header file


class Board
{

public:
  int boardSize;                   // Holds the grid size for the puzzle
  int patternNum;                  // Number of patterns for the puzzle
  static Board* pInstance;         // Holds the current instance of the class
  static PuzzlePiece* pBoardArray; // Pointer to pieces within board
  static PuzzlePiece* pPieceArray; // Pointer to collection of all pieces

  // Constructor that sets puzzle size, number of colours and number of shapes
  Board(int puzzleSize, int pattern);

  // Returns the current instance of the class
  static Board* GetInstance();

  // Creates the static instance for the class, calling the constructor with
  // the puzzle size along with the number of colours and shapes
  static void CreateInstance(int puzzleSize, int pattern);

  // Creates the board and sets inital tile placement
  void CreateInitialBoard();

  // Reads puzzle piece information from file
  void ReadPieceFile(const char* fileName);

  // Destructor that deletes the instance of the class
  ~Board();


};

