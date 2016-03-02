// Title        : FitnessFunction.cpp
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#include "FitnessFunction.h"    // Include the header file


// Initialise the pointer to the instance
FitnessFunction* FitnessFunction::pInstance = nullptr;


FitnessFunction::FitnessFunction()
{

  int boardSize = 0;
  int score = 0;
  pBoard = nullptr;
  pBoardMan = BoardManager::GetInstance();

} // FitnessFunction()


bool FitnessFunction::IsMatchSide(int yIndex1, int xIndex1, int yIndex2, 
                                  int xIndex2)
{ // Checks to see if the pattern located on right side of piece1 matches the
  // pattern located on the left of piece2

  return (BoardManager::GetInstance()->GetPattern(pBoard, yIndex1, xIndex1, RIGHT)
    == BoardManager::GetInstance()->GetPattern(pBoard, yIndex2, xIndex2, LEFT));

} // IsMatchSide()


bool FitnessFunction::IsMatchBottom(int yIndex1, int xIndex1, int yIndex2,
                                    int xIndex2)
{ // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2

  return (BoardManager::GetInstance()->GetPattern(pBoard, yIndex1, xIndex1, BOTTOM)
    == BoardManager::GetInstance()->GetPattern(pBoard, yIndex2, xIndex2, TOP));

} // IsMatchBottom()


void FitnessFunction::CheckTopLeft()
{ // Checks the top left corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (pBoard->boardVec[0][0].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score

    score += 25;

    if (IsMatchSide(0, 0, 0, 1))
    { // Check to see if the pattern on the right of the corner piece matches
      // the pattern on the left of the piece to the right

      score += 10;

    }

    if (IsMatchBottom(0, 0, 1, 0))
    { // Check to see if the pattern on the bottom of the corner piece matches
      // the pattern on the top of the piece below

      score += 10;

    }

  } // if corner type

} // CheckTopLeft()


void FitnessFunction::CheckTopRight()
{ // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (pBoard->boardVec[0][pBoardMan->boardSize].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score

    score += 25;

    if (IsMatchSide(0, pBoardMan->boardSize - 1, 0, pBoardMan->boardSize))
    { // Check to see if the pattern on the right side of the piece to left of
      // the corner, matches the pattern on the left of the corner piece

      score += 10;

    }

    if (IsMatchBottom(0, pBoardMan->boardSize, 1, pBoardMan->boardSize))
    { // Check to see if the pattern on the bottom of the corner piece matches
      // the pattern on the top of the piece below

      score += 10;

    }

  } // if corner type

} // CheckTopRight()


void FitnessFunction::CheckBottomLeft()
{ // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (pBoard->boardVec[pBoardMan->boardSize][0].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score

    score += 25;

    if (IsMatchSide(pBoardMan->boardSize, 0, pBoardMan->boardSize, 1))
    { // Check to see if the pattern on the right side of the corner piece 
      // matches the pattern located on the left side of the piece to the right

      score += 10;

    }

    if (IsMatchBottom(pBoardMan->boardSize - 1, 0, pBoardMan->boardSize, 0))
    { // Check to see if the pattern on the bottom of the piece above the 
      // corner matches the pattern located on the top of the corner

      score += 10;

    }

  } // if corner type

} //CheckBottomLeft()


void FitnessFunction::CheckBottomRight()
{ // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (pBoard->boardVec[pBoardMan->boardSize][pBoardMan->boardSize].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score

    score += 25;

    if (IsMatchSide(pBoardMan->boardSize, pBoardMan->boardSize - 1, 
        pBoardMan->boardSize, pBoardMan->boardSize))
    { // Check to see if the pattern on the right side of the piece to the left
      // of the corner matches the pattern located on the left of the corner

      score += 10;

    }

    if (IsMatchBottom(pBoardMan->boardSize - 1, pBoardMan->boardSize, 
                      pBoardMan->boardSize, pBoardMan->boardSize))
    { // Check to see if the pattern located on the bottom of the piece located
      // above the corner matches the pattern located on the top of the corner

      score += 10;

    }

  } // if corner type

} // CheckBottomRight()


void FitnessFunction::CheckTopEdge()
{ // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {
    if (pBoard->boardVec[0][i].type == EDGE)
    { // If edge piece on edge slot

      score += 15;

      if (IsMatchSide(0, i, 0, i + 1))
      { // If edge pattern matches edge piece to the right

        score += 5;
      }

      if (IsMatchBottom(0, i, 1, i))
      { // If edge pattern matches inner connection on bottom

        score += 5;
      }
    }
  }

  if (pBoard->boardVec[0][pBoardMan->boardSize - 1].type == EDGE)
  { // If the piece in the slot 1 to the left of top right corner is edge

    score += 15;

    if (IsMatchBottom(0, pBoardMan->boardSize - 1, 1, pBoardMan->boardSize - 1))
    { // If piece left of the top right corner bottom pattern matches piece
      // underneath

      score += 5;
    }
  }

} // CheckTopEdge()


void FitnessFunction::CheckLeftEdge()
{ // Checks the slots along the left edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {
    if (pBoard->boardVec[i][0].type == EDGE)
    { // If piece is of edge type

      score += 15;

      if (IsMatchSide(i, 0, i, 1))
      { // If edge piece right pattern matches inner connection

        score += 5;
      }

      if (IsMatchBottom(i, 0, i + 1, 0))
      { // If edge piece bottom pattern matches top of piece underneath

        score += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize - 1][0].type == EDGE)
  { // If piece above bottom left corner is edge type

    score += 15;

    if (IsMatchSide(pBoardMan->boardSize - 1, 0, pBoardMan->boardSize - 1, 1))
    { // If piece above bottom left corner right pattern matches inner 
      // connection

      score += 5;
    }
  }

} // CheckLeftEdge()


void FitnessFunction::CheckRightEdge()
{ // Checks the slots along the right edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {
    if (pBoard->boardVec[i][pBoardMan->boardSize].type == EDGE)
    { // If piece is edge type

      score += 15;

      if (IsMatchSide(i, pBoardMan->boardSize - 1, i, pBoardMan->boardSize))
      { // If edge piece matches inner connection to right

        score += 5;
      }

      if (IsMatchBottom(i, pBoardMan->boardSize, i + 1, pBoardMan->boardSize))
      { // If edge piece matches piece below

        score += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize - 1]
    [pBoardMan->boardSize].type == EDGE)
  { // If piece above bottom right corner is edge

    score += 15;

    if (IsMatchSide(pBoardMan->boardSize - 1, pBoardMan->boardSize - 1, 
                    pBoardMan->boardSize - 1, pBoardMan->boardSize))
    { // If piece matches connection to the left

      score += 5;
    }
  }

} // CheckRightEdge()


void FitnessFunction::CheckBottomEdge()
{ // Checks the slots along the bottom edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {
    if (pBoard->boardVec[pBoardMan->boardSize][i].type == EDGE)
    { // If piece is edge type

      score += 15;

      if (IsMatchSide(pBoardMan->boardSize, i, pBoardMan->boardSize, i + 1))
      { // If right pattern matches next piece left

        score += 5;
      }

      if (IsMatchBottom(pBoardMan->boardSize - 1, i, pBoardMan->boardSize, i))
      {  // If top matched piece above

        score += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize]
    [pBoardMan->boardSize - 1].type == EDGE)
  { // If piece left of bottom right corner is edge

    score += 15;

    if (IsMatchBottom(pBoardMan->boardSize - 1, pBoardMan->boardSize - 1, 
                      pBoardMan->boardSize, pBoardMan->boardSize - 1))
    { // If top pattern matched inner connection

      score += 5;
    }
  }

} // CheckBottomEdge()


void FitnessFunction::CheckInnerPieces()
{ // Checks the pieces in the inner slots of the board for matching connections

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {
    for (int j = 1; j < pBoardMan->boardSize - 1; j++)
    {
      if (IsMatchSide(i, j, i, j + 1))
      { // If right side matches piece to the right

        score += 1;
      }

      if (IsMatchBottom(i, j, i + 1, j))
      { // If bottom side matches piece below

        score += 1;
      }
    }
  }

} // CheckInnerPieces()


FitnessFunction* FitnessFunction::GetInstance()
{ // Returns the current instance of the class, creates if needed

  if (!pInstance)
    pInstance = new FitnessFunction();

  return pInstance;

} // GetInstance()


int FitnessFunction::CheckFitness(Board* theBoard)
{ // Checks the fitness of the candidate provided by the GA

  score = 0;            // Resets the current score to 0
  pBoard = theBoard;    // Defines the board pointer to the current board

                        // Runs through and checks all the connections and piece locations of the
                        // candidate solutions adding up the score for each correct placement & match
  CheckTopLeft();
  CheckTopEdge();
  CheckTopRight();
  CheckLeftEdge();
  CheckInnerPieces();
  CheckRightEdge();
  CheckBottomLeft();
  CheckBottomEdge();
  CheckBottomRight();

  pBoard->fitScore = score;

  return score;

} // CheckFitness()


FitnessFunction::~FitnessFunction()
{ // Destructor that deletes the instance of the class

  // Delete the instance of the fitness function
  delete pInstance;

  // Change to pointer to null
  pInstance = nullptr;

}
