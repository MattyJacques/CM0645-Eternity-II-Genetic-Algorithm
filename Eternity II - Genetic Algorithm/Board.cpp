// Title        : Board.cpp
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015

#include "Board.h" // Include the header file


// Initialise the pointer to the instance
Board* Board::pInstance = nullptr;

// Initialise the pointer to the puzzle board
PuzzlePiece* Board::pBoardArray = nullptr;

// Initialise the pointer to the piece collection
PuzzlePiece* Board::pPieceArray = nullptr;


Board::Board(int puzzleSize, int pattern)
{ // Constructor that sets puzzle size, number of colours and number of shapes
  
  boardSize = puzzleSize; // Sets the puzzle size for the puzzle
  patternNum = pattern;  // Sets the number of shapes for the puzzle
  CreateInitialBoard();
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

  // Work out how many tiles will be on board
  int pieceNum = boardSize * boardSize;

  // Create an array with elements for each piece
  pPieceArray = new PuzzlePiece[pieceNum];

  // Create array to hold piece placements on board
  pBoardArray = new PuzzlePiece[boardSize, boardSize];

} //CreateInitialBoard()


void Board::ReadPieceFile(const char* fileName)
{ 
} // ReadTileFile()


Board::~Board()
{ // Destructor that deletes the instance of the class

  delete pInstance;       // Delete the instance
  delete pPieceArray;     // Delete pointer to piece collection
  delete pBoardArray;     // Delete pointer to board information

  // Change the pointers to null
  pInstance = nullptr;
  pPieceArray = nullptr;  
  pBoardArray = nullptr;

} // ~Board()
