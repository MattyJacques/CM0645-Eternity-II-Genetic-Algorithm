// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#pragma once


#include "PuzzlePiece.h"
#include "Board.h"
#include <vector>


class BoardManager
{

private:
  int patternNum;
  static BoardManager* pInstance;

  BoardManager();

public:
  std::vector<PuzzlePiece> pieceVec;
  std::vector<Board> boards;
  int boardSize;

  static BoardManager* GetInstance();
  void InitialiseData(int size, int patNum);
  void CreateInitialBoard(Board* theBoard);
  ~BoardManager();

};

