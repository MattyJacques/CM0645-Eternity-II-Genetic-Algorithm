// Title        : Mutation.cpp
// Purpose      : Mutates random boards using various methods
// Author       : Matthew Jacques
// Date         : 23/01/2016


#include "Mutation.h"
#include "BoardManager.h"     // Access to boards
#include "GeneticAlgorithm.h" // Get random number
#include <iostream>           // Debug output


// Initialise to nullptr
Mutation* Mutation::pInstance = nullptr;


Mutation::Mutation()
{ // Initialise mutation method

  mutType = SWAP;

} // Mutation()

void Mutation::GetRandPiece(int index[2])
{ // Sets the 2 dimensional index for a random piece out of the inner pieces
  // of the board

  // Set the indexs in the array to two random number between 1 and the size
  // of the board - 1
  index[0] = GeneticAlgorithm::GetInstance()->GenRandomNum(1, 
              BoardManager::GetInstance()->boardSize - 1);
  index[1] = GeneticAlgorithm::GetInstance()->GenRandomNum(1, 
              BoardManager::GetInstance()->boardSize - 1);

} // GetRandPiece()


void Mutation::Swap(int boardID)
{ // Swaps two random pieces in the board that has the ID given as the parameter

  int pieceIndex1[2];     // Holds index of the first piece
  int pieceIndex2[2];     // Holds index of second piece
  PuzzlePiece temp;       // Temp puzzle piece to use during the swap

  // Create pointer to board to work with and initalise to point to board
  Board* pBoard = &BoardManager::GetInstance()->currBoards->at(boardID);

  // Get random indexes of puzzle pieces to swap
  GetRandPiece(pieceIndex1);
  GetRandPiece(pieceIndex2);

  // Move puzzle piece to temp storage
  temp = pBoard->boardVec[pieceIndex1[0]][pieceIndex1[1]];

  // Place the second puzzle piece where first puzzle piece was located
  pBoard->boardVec[pieceIndex1[0]][pieceIndex1[1]] =
    pBoard->boardVec[pieceIndex2[0]][pieceIndex2[1]];

  // Place first puzzle piece back in second puzzle piece slot
  pBoard->boardVec[pieceIndex2[0]][pieceIndex2[1]] = temp;

} // Swap()


Mutation * Mutation::GetInstance()
{ // Returns the current instance of the class, creates if needed

  if (!pInstance)
    pInstance = new Mutation;

  return pInstance;

} // GetInstance()


void Mutation::SetMethod(MutateType type)
{ // Sets the method of mutation to use for this attempt

  mutType = type;

} // SetMethod()


void Mutation::DoMutation()
{

  int boardID;

  for (int i = 0; i < mutNum; i++)
  {

    boardID = GeneticAlgorithm::GetInstance()->GenRandomNum(0,
                (int)BoardManager::GetInstance()->currBoards->size());

    if (mutType == SWAP)
      Swap(boardID);
    //else if (mutType == ROTATE)
    //  Rotate();
    //else if (mutType == ROTATESWAP)
    //  RotateSwap();
    //else if (mutType == REGIONSWAP)
    //  RegionSwap();
    //else if (mutType == REGIONROTATE)
    //  RegionRotate();
    else
      std::cout << "Mutation method not recognised" << std::endl;

  }

} // DoMutation()
