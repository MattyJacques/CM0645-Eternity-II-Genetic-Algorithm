// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#pragma once


#include "PuzzlePiece.h"        // For piece collection vector
#include "Board.h"              // For board collection vector 
#include <vector>               // Creating vectors


class BoardManager
{

private:
  int patternNum;                     // Amount of patterns within board
  static BoardManager* pInstance;     // Current instance of class

  // Private constructor for singleton
  BoardManager();

public:
  std::vector<PuzzlePiece> pieceVec;      // Collection of all pieces to be used
  std::vector<Board> boards;              // Collection of all board in generation
  int boardSize;                          // How many pieces per board

  // Returns the instance to the class, if none currently exists, creates one
  static BoardManager* GetInstance();

  // Sets how many patterns to be used and how many pieces there are per board
  void InitialiseData(int size, int patNum);

  // Creates the inital generation of boards
  void CreateInitialBoard(Board* theBoard);

  // Cleans up
  ~BoardManager();

};

