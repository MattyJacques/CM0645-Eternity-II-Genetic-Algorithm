// Title        : FitnessFunction.cpp
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#include "FitnessFunction.h"    // Include the header file


FitnessFunction::FitnessFunction()
{ // Initialse board and board manager pointers

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
    pBoard->fitScore += 25;

    if (IsMatchSide(0, 0, 0, 1))
    { // Check to see if the pattern on the right of the corner piece matches
      // the pattern on the left of the piece to the right
      pBoard->fitScore += 10;

    }

    if (IsMatchBottom(0, 0, 1, 0))
    { // Check to see if the pattern on the bottom of the corner piece matches
      // the pattern on the top of the piece below
      pBoard->fitScore += 10;

    }

  } // if corner type

} // CheckTopLeft()


void FitnessFunction::CheckTopRight()
{ // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (pBoard->boardVec[0][pBoardMan->boardSize].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score
    pBoard->fitScore += 25;

    if (IsMatchSide(0, pBoardMan->boardSize - 1, 0, pBoardMan->boardSize))
    { // Check to see if the pattern on the right side of the piece to left of
      // the corner, matches the pattern on the left of the corner piece
      pBoard->fitScore += 10;
    }

    if (IsMatchBottom(0, pBoardMan->boardSize, 1, pBoardMan->boardSize))
    { // Check to see if the pattern on the bottom of the corner piece matches
      // the pattern on the top of the piece below
      pBoard->fitScore += 10;
    }

  } // if corner type

} // CheckTopRight()


void FitnessFunction::CheckBottomLeft()
{ // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (pBoard->boardVec[pBoardMan->boardSize][0].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score
    pBoard->fitScore += 25;

    if (IsMatchSide(pBoardMan->boardSize, 0, pBoardMan->boardSize, 1))
    { // Check to see if the pattern on the right side of the corner piece 
      // matches the pattern located on the left side of the piece to the right
      pBoard->fitScore += 10;
    }

    if (IsMatchBottom(pBoardMan->boardSize - 1, 0, pBoardMan->boardSize, 0))
    { // Check to see if the pattern on the bottom of the piece above the 
      // corner matches the pattern located on the top of the corner
      pBoard->fitScore += 10;
    }

  } // if corner type

} //CheckBottomLeft()


void FitnessFunction::CheckBottomRight()
{ // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (pBoard->boardVec[pBoardMan->boardSize][pBoardMan->boardSize].type == CORNER)
  { // Check to see if the piece in the slot is a corner, if so add 25 to score
    pBoard->fitScore += 25;

    if (IsMatchSide(pBoardMan->boardSize, pBoardMan->boardSize - 1, 
        pBoardMan->boardSize, pBoardMan->boardSize))
    { // Check to see if the pattern on the right side of the piece to the left
      // of the corner matches the pattern located on the left of the corner
      pBoard->fitScore += 10;
    }

    if (IsMatchBottom(pBoardMan->boardSize - 1, pBoardMan->boardSize, 
                      pBoardMan->boardSize, pBoardMan->boardSize))
    { // Check to see if the pattern located on the bottom of the piece located
      // above the corner matches the pattern located on the top of the corner
      pBoard->fitScore += 10;
    }

  } // if corner type

} // CheckBottomRight()


void FitnessFunction::CheckTopEdge()
{ // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  { // Loop right the edge pieces on the top edge of the board checking fitness

    if (pBoard->boardVec[0][i].type == EDGE)
    { // If edge piece on edge slot add score and continue checks
      pBoard->fitScore += 15;

      if (IsMatchSide(0, i, 0, i + 1))
      { // If edge pattern matches edge piece to the right add score
        pBoard->fitScore += 5;
      }

      if (IsMatchBottom(0, i, 1, i))
      { // If edge pattern matches inner connection on bottom add score
        pBoard->fitScore += 5;
      }
    }
  }

  if (pBoard->boardVec[0][pBoardMan->boardSize - 1].type == EDGE)
  { // If the piece in the slot 1 to the left of top right corner is edge add 
    // score
    pBoard->fitScore += 15;

    if (IsMatchBottom(0, pBoardMan->boardSize - 1, 1, pBoardMan->boardSize - 1))
    { // If piece left of the top right corner bottom pattern matches piece
      // underneath add score
      pBoard->fitScore += 5;
    }
  }

} // CheckTopEdge()


void FitnessFunction::CheckLeftEdge()
{ // Checks the slots along the left edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {// Loop right the edge pieces on the top left of the board checking fitness

    if (pBoard->boardVec[i][0].type == EDGE)
    { // If piece is of edge type add score and continue checks
      pBoard->fitScore += 15;

      if (IsMatchSide(i, 0, i, 1))
      { // If edge piece right pattern matches inner connection add score
        pBoard->fitScore += 5;
      }

      if (IsMatchBottom(i, 0, i + 1, 0))
      { // If edge piece bottom pattern matches top of piece underneath add score
        pBoard->fitScore += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize - 1][0].type == EDGE)
  { // If piece above bottom left corner is edge type add score
    pBoard->fitScore += 15;

    if (IsMatchSide(pBoardMan->boardSize - 1, 0, pBoardMan->boardSize - 1, 1))
    { // If piece above bottom left corner right pattern matches inner 
      // connection add score
      pBoard->fitScore += 5;
    }
  }

} // CheckLeftEdge()


void FitnessFunction::CheckRightEdge()
{ // Checks the slots along the right edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  { // Loop right the edge pieces on the right edge of the board checking fitness

    if (pBoard->boardVec[i][pBoardMan->boardSize].type == EDGE)
    { // If piece is edge type add score and continue checks
      pBoard->fitScore += 15;

      if (IsMatchSide(i, pBoardMan->boardSize - 1, i, pBoardMan->boardSize))
      { // If edge piece matches inner connection to right add score
        pBoard->fitScore += 5;
      }

      if (IsMatchBottom(i, pBoardMan->boardSize, i + 1, pBoardMan->boardSize))
      { // If edge piece matches piece below add score
        pBoard->fitScore += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize - 1]
      [pBoardMan->boardSize].type == EDGE)
  { // If piece above bottom right corner is edge add score
    pBoard->fitScore += 15;

    if (IsMatchSide(pBoardMan->boardSize - 1, pBoardMan->boardSize - 1, 
                    pBoardMan->boardSize - 1, pBoardMan->boardSize))
    { // If piece matches connection to the left add score
      pBoard->fitScore += 5;
    }
  }

} // CheckRightEdge()


void FitnessFunction::CheckBottomEdge()
{ // Checks the slots along the bottom edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {// Loop right the edge pieces on the bottom edge of the board checking fitness

    if (pBoard->boardVec[pBoardMan->boardSize][i].type == EDGE)
    { // If piece is edge type add score and continue checks
      pBoard->fitScore += 15;

      if (IsMatchSide(pBoardMan->boardSize, i, pBoardMan->boardSize, i + 1))
      { // If right pattern matches next piece left add score
        pBoard->fitScore += 5;
      }

      if (IsMatchBottom(pBoardMan->boardSize - 1, i, pBoardMan->boardSize, i))
      {  // If top matched piece above add score
        pBoard->fitScore += 5;
      }
    }
  }

  if (pBoard->boardVec[pBoardMan->boardSize]
    [pBoardMan->boardSize - 1].type == EDGE)
  { // If piece left of bottom right corner is edge add score
    pBoard->fitScore += 15;

    if (IsMatchBottom(pBoardMan->boardSize - 1, pBoardMan->boardSize - 1, 
                      pBoardMan->boardSize, pBoardMan->boardSize - 1))
    { // If top pattern matched inner connection add score
      pBoard->fitScore += 5;
    }
  }

} // CheckBottomEdge()


void FitnessFunction::CheckInnerPieces()
{ // Checks the pieces in the inner slots of the board for matching connections

  for (int i = 1; i < pBoardMan->boardSize - 1; i++)
  {  // Y index for checking slots, after line has completed, increment row

    for (int j = 1; j < pBoardMan->boardSize - 1; j++)
    { // X index for checking slots, after line has completed, increment coloumn

      if (IsMatchSide(i, j, i, j + 1))
      { // If right side matches piece to the right add score
        pBoard->fitScore += 1;
      }

      if (IsMatchBottom(i, j, i + 1, j))
      { // If bottom side matches piece below add score
        pBoard->fitScore += 1;
      }
    }
  }

} // CheckInnerPieces()


int FitnessFunction::CheckFitness(Board* theBoard)
{ // Checks the fitness of the candidate provided by the GA

  
  pBoard = theBoard;    // Defines the board pointer to the current board
  pBoard->fitScore = 0; // Sets the score to 0 to begin calculation

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

  // Return fitness score for easier maximum fitness tracking
  return pBoard->fitScore;

} // CheckFitness()