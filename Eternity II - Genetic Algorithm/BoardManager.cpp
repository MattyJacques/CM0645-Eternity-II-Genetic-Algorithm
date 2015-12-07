#include "BoardManager.h"


BoardManager* BoardManager::pInstance = nullptr;


BoardManager::BoardManager()
{
}


BoardManager * BoardManager::GetInstance()
{
  if (!pInstance)
    pInstance = new BoardManager;

  return pInstance;
}

void BoardManager::InitialiseData(int size, int patNum)
{
  boardSize = size - 1;
  patternNum = patNum;
}

void BoardManager::CreateInitialBoard(Board * theBoard)
{

  int index = 0;

  for (int i = 0; i <= boardSize; i++)
  {
    std::vector<PuzzlePiece> newVec;
    theBoard->boardVec.push_back(newVec);
  }

  for (PuzzlePiece piece : pieceVec)
  { // Loop through the collection of pieces

    theBoard->boardVec[index].push_back(piece);
    index++;

    if (index > boardSize)
      index = 0;
  }

}

BoardManager::~BoardManager()
{
}
