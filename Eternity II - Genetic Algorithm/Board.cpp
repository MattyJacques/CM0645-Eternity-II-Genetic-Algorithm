// Title        : Board.cpp
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015

#include "Board.h" // Include the header file


// Initialise the pointer to the instance
Board* Board::pInstance = nullptr;


Board::Board(int puzzleSize, int pattern)
{ // Constructor that sets puzzle size, number of colours and number of shapes
  
  boardSize = puzzleSize - 1; // Sets the puzzle size for the puzzle
  patternNum = pattern;   // Sets the number of shapes for the puzzle

} // Board()


Board* Board::GetInstance()
{ // Returns the current instance of the class

  return pInstance; // Return the pointer

} // GetInstace()


void Board::CreateInstance(int puzzleSize, int pattern)
{ // Creates the static instance for the class, calling the constructor with
  // the puzzle size along with the number of colours and shapes

  if (!pInstance)
  { // If the pointer to the instance is null, create a new instance

    pInstance = new Board(puzzleSize, pattern);
  }

} // CreateInstance()


void Board::CreateInitialBoard()
{ // Creates the board and sets inital tile placement

  for (PuzzlePiece piece : pieceVec)
  { // Loop through the collection of pieces

    for (int i = 0; i <= boardSize; i++)
    {
      boardVec[i].push_back(piece);
    }
  }

} //CreateInitialBoard()


Board::~Board()
{ // Destructor that deletes the instance of the class

  delete pInstance;       // Delete the instancen

  // Change the pointer to null
  pInstance = nullptr;

} // ~Board()
