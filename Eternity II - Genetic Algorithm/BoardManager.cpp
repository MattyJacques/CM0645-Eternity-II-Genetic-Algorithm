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
{ // Creates the inital board filled with randomised order of pieces

  // Set the ID of the board
  pBoard->boardID = (int)currBoards->size() + 1;

  // Fill vector of vectors with empty vectors
  InitEmptyBoard(pBoard);

  for (int i = 0; i < 3; i++)
  { // Loop to shuffle all piece types

    // Shuffle order of puzzle pieces
    std::random_shuffle(pieceVec[i].begin(), pieceVec[i].end());
  }

  // Add pieces to empty vectors
  AddPieces(pBoard);

} // CreateInitialBoard()


void BoardManager::AddPieces(Board* pBoard)
{ // Adds pieces to the empty boards, top edge first, moving on to inner slots
  // then finally filling in the corners, side edges and bottom edge

  int index = 1;    // Index of current vector to place piece in
  int count = 0;    // Count how mant pieces places to increment index

  // Add edge pieces to vectors that will contain inner pieces
  InitTopEdge(pBoard);

  for (PuzzlePiece piece : pieceVec[INNER])
  { // Loop through the collection of pieces

    // Add piece to line on board
    pBoard->boardVec[index].push_back(piece);
    count++;

    // If reached the end of the line for the board, move onto next 
    if (count == boardSize - 1)
    {
      index++;
      count = 0;
    }
  }

  // Add rest of border pieces
  InitCornersSides(pBoard);

} // AddPieces()


void BoardManager::InitTopEdge(Board* pBoard)
{ // Initialises the top edge (not including corners) of the board read for
  // inner pieces to be inserted

  for (int i = 0; i <= boardSize - 2; i++)
  { // Add edge pieces to inner 13 vectors (so not board edges)

    pBoard->boardVec[i + 1].push_back(pieceVec[EDGE][i]);
  }

} // InitTopEdge()


void BoardManager::InitCornersSides(Board* pBoard)
{ // Initialises the left, right and bottom edges of the board along with
  // the corner slots of the board

  pBoard->boardVec[0].push_back(pieceVec[CORNER][0]);
  pBoard->boardVec[boardSize].push_back(pieceVec[CORNER][1]);

  for (int i = 0; i < boardSize - 1; i++)
  {
    pBoard->boardVec[0].push_back(pieceVec[EDGE][i + boardSize - 1]);
    pBoard->boardVec[boardSize].push_back(pieceVec[EDGE]
                                                  [i + (boardSize * 2) - 2]);
  }

  for (int i = 0; i <= boardSize - 2; i++)
  {
    pBoard->boardVec[i + 1].push_back(pieceVec[EDGE][i + (boardSize * 3) - 3]);
  }

  pBoard->boardVec[0].push_back(pieceVec[CORNER][2]);
  pBoard->boardVec[boardSize].push_back(pieceVec[CORNER][3]);

} // InitCornerSides()


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
