// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#include "BoardManager.h"
#include <algorithm>          // Random_shuffle()
#include <time.h>             // time()


// Initialise to null
BoardManager* BoardManager::pInstance = nullptr;


BoardManager::BoardManager()
{ // Seeds rand() with time and initalises currBoards

  srand((unsigned int)time(NULL));

  std::vector<Board> newVec;
  currBoards = std::make_shared<std::vector<Board>>(newVec);


} // BoardManager()


BoardManager * BoardManager::GetInstance()
{ // Returns the instance to the class, if none currently exists, creates one

  if (!pInstance)
    pInstance = new BoardManager;

  return pInstance;

} // GetInstance()


void BoardManager::InitialiseData(int size, int patNum)
{ // Sets how many patterns to be used and how many pieces there are per board

  boardSize = size - 1;
  patternNum = patNum;

} // InitialiseData()


void BoardManager::InitFullBoard(Board* pBoard)
{ // Creates the inital generation of boards

  int index = 0;
  int count = 0;

  pBoard->boardID = (int)currBoards->size() + 1;

  // Fill vector of vectors with empty vectors
  InitEmptyBoard(pBoard);

  // Shuffle order of puzzle pieces
  std::random_shuffle(pieceVec.begin(), pieceVec.end());

  for (PuzzlePiece piece : pieceVec)
  { // Loop through the collection of pieces

    // Add piece to line on board
    pBoard->boardVec[index].push_back(piece);
    count++;

    // If reached the end of the line for the board, move onto next 
    if (count == boardSize + 1)
    {
      index++;
      count = 0;
    }

  }

} // CreateInitialBoard()


void BoardManager::InitEmptyBoard(Board* pBoard)
{ // Initialises the boards vector of vectors with empty vectors

  for (int i = 0; i <= boardSize; i++)
  { // Loop through and create vector of vector for the board
    std::vector<PuzzlePiece> newVec;
    pBoard->boardVec.push_back(newVec);
  }

} // InitialiseBoard


int BoardManager::GetPattern(Board* pBoard, int yIndex, int xIndex, 
                             segLocation segment)
{ // Returns the ID of the pattern located on the piece with in the index 
  // provided taking into consideration the orientation of the piece
  
  // Modular of 4 to keep segment index 3 or below
  return pBoard->boardVec[yIndex][xIndex].segments[
                    (segment + pBoard->boardVec[yIndex][xIndex].orientation) % 4];

} // GetPattern()

BoardManager::~BoardManager()
{ // Destructor to delete the instance of the class

  currBoards = nullptr;
  prevBoards = nullptr;
  delete pInstance;
  pInstance = nullptr;

} // ~BoardManager()
