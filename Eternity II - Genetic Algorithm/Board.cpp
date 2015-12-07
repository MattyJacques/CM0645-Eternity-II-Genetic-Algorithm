// Title        : Board.cpp
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015

#include "Board.h" // Include the header file


// Initialise the pointer to the instance
Board* Board::pInstance = nullptr;


Board::Board()
{ 

} // Board()


//Board* Board::GetInstance()
//{ // Returns the current instance of the class
//
//  return pInstance; // Return the pointer
//
//} // GetInstace()


//void Board::CreateInstance(int puzzleSize, int pattern)
//{ // Creates the static instance for the class, calling the constructor with
//  // the puzzle size along with the number of colours and shapes
//
//  if (!pInstance)
//  { // If the pointer to the instance is null, create a new instance
//
//    pInstance = new Board(puzzleSize, pattern);
//  }
//
//} // CreateInstance()


void Board::CreateInitialBoard()
{ // Creates the board and sets inital tile placement

  int index = 0;

  for (int i = 0; i <= boardSize; i++)
  {
    std::vector<PuzzlePiece> newVec;
    boardVec.push_back(newVec);
  }

  for (PuzzlePiece piece : pieceVec)
  { // Loop through the collection of pieces

    boardVec[index].push_back(piece);
    index++;

    if (index > boardSize)
      index = 0;
  }

} //CreateInitialBoard()


Board::~Board()
{ // Destructor that deletes the instance of the class

  delete pInstance;       // Delete the instancen

  // Change the pointer to null
  pInstance = nullptr;

} // ~Board()
