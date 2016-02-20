// Title        : Board.cpp
// Purpose      : Holds the information for the eternity ii board
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "Board.h" // Include the header fiele


Board::Board()
{ // Initialises the fitness score

  fitScore = 0;

} // Board()

int Board::GetFitScore()
{ // Returns the fitness score of the board

  return fitScore;

} // GetFitScore()

void Board::SetFitScore(int score)
{ // Sets the fitness score for this particular board

  fitScore = score;

} // SetFireScore()

void Board::SetBoardID(int id)
{ // Sets the ID of the board

  boardID = id;

} // SetBoardID()

Board::~Board()
{ // Destructor that deletes the instance of the class

  boardVec.clear();

} // ~Board()
