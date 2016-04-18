// Title        : FitnessFunction.cpp
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#include "FitnessFunction.h"    // Include the header file


FitnessFunction::FitnessFunction()
{ // Initialse board and board manager pointers

  pBoard = nullptr;
  pBoardMan = BoardManager::getInstance();

} // FitnessFunction()


void FitnessFunction::checkFitness(Board* theBoard)        // *In-Out*
{ // Checks the fitness of the candidate provided by the GA

  pBoard = theBoard;      // Defines the board pointer to the current board
  pBoard->fitScore = 0;   // Sets the score to 0 to begin calculation
  pBoard->matchCount = 0; // Sets the matches to 0 to begin calculation

  // Runs through and checks all the connections and piece locations of the
  // candidate solutions adding up the score for each correct placement & match
  // also adds up how many patterns are matched in the candidate
  checkTopLeft();
  checkTopEdge();
  checkTopRight();
  checkLeftEdge();
  checkInnerPieces();
  checkRightEdge();
  checkBottomLeft();
  checkBottomEdge();
  checkBottomRight();

} // checkFitness()


bool FitnessFunction::isMatchSide(int xIndex1,             // *In*
                                  int yIndex1,             // *In*
                                  int xIndex2,             // *In*
                                  int yIndex2)             // *In*
{ // Checks to see if the pattern located on right side of piece1 matches the
  // pattern located on the left of piece2
  
  return (pBoardMan->getPattern(pBoard, xIndex1, yIndex1, RIGHT)
          == pBoardMan->getPattern(pBoard, xIndex2, yIndex2, LEFT));

} // isMatchSide()


bool FitnessFunction::isMatchBottom(int xIndex1,           // *In*
                                    int yIndex1,           // *In*
                                    int xIndex2,           // *In*
                                    int yIndex2)           // *In*
{ // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2

  return (pBoardMan->getPattern(pBoard, xIndex1, yIndex1, BOTTOM)
          == pBoardMan->getPattern(pBoard, xIndex2, yIndex2, TOP));

} // isMatchBottom()


void FitnessFunction::checkTopLeft()
{ // Checks the top left corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (isMatchSide(0, 0, 1, 0))
  { // Check to see if the pattern on the right of the corner piece matches
    // the pattern on the left of the piece to the right
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

  if (isMatchBottom(0, 0, 0, 1))
  { // Check to see if the pattern on the bottom of the corner piece matches
    // the pattern on the top of the piece below
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

} // checkTopLeft()


void FitnessFunction::checkTopRight()
{ // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (isMatchSide(pBoardMan->getSize() - 1, 0, pBoardMan->getSize(), 0))
  { // Check to see if the pattern on the right side of the piece to left of
    // the corner, matches the pattern on the left of the corner piece
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

  if (isMatchBottom(pBoardMan->getSize(), 0, pBoardMan->getSize(), 1))
  { // Check to see if the pattern on the bottom of the corner piece matches
    // the pattern on the top of the piece below
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

} // checkTopRight()


void FitnessFunction::checkBottomLeft()
{ // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (isMatchSide(0, pBoardMan->getSize(), 1, pBoardMan->getSize()))
  { // Check to see if the pattern on the right side of the corner piece 
    // matches the pattern located on the left side of the piece to the right
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

  if (isMatchBottom(0, pBoardMan->getSize() - 1, 0, pBoardMan->getSize()))
  { // Check to see if the pattern on the bottom of the piece above the 
    // corner matches the pattern located on the top of the corner
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

} // checkBottomLeft()


void FitnessFunction::checkBottomRight()
{ // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (isMatchSide(pBoardMan->getSize() - 1, pBoardMan->getSize(),
      pBoardMan->getSize(), pBoardMan->getSize()))
  { // Check to see if the pattern on the right side of the piece to the left
    // of the corner matches the pattern located on the left of the corner
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

  if (isMatchBottom(pBoardMan->getSize(), pBoardMan->getSize() - 1,
                    pBoardMan->getSize(), pBoardMan->getSize()))
  { // Check to see if the pattern located on the bottom of the piece located
    // above the corner matches the pattern located on the top of the corner
    pBoard->fitScore += CORNERMATCH;
    pBoard->matchCount++;
  }

} // checkBottomRight()


void FitnessFunction::checkTopEdge()
{ // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces

  for (int i = 1; i < pBoardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the top edge of the board checking fitness

    if (isMatchSide(i, 0, i + 1, 0))
    { // If edge pattern matches edge piece to the right add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }

    if (isMatchBottom(i, 0, i, 1))
    { // If edge pattern matches inner connection on bottom add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }
 
  }

  if (isMatchBottom(pBoardMan->getSize() - 1, 0, pBoardMan->getSize() - 1, 1))
  { // If piece left of the top right corner bottom pattern matches piece
    // underneath add score
    pBoard->fitScore += EDGEMATCH;
    pBoard->matchCount++;
  }

} // checkTopEdge()


void FitnessFunction::checkLeftEdge()
{ // Checks the slots along the left edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the top left of the board checking fitness

    if (isMatchSide(0, i, 1, i))
    { // If edge piece right pattern matches inner connection add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }

    if (isMatchBottom(0, i, 0, i + 1))
    { // If edge piece bottom pattern matches top of piece underneath add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }
  
  }

  if (isMatchSide(0, pBoardMan->getSize() - 1, 1, pBoardMan->getSize() - 1))
  { // If piece above bottom left corner right pattern matches inner 
    // connection add score
    pBoard->fitScore += EDGEMATCH;
    pBoard->matchCount++;
  }

} // checkLeftEdge()


void FitnessFunction::checkRightEdge()
{ // Checks the slots along the right edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the right edge of the board checking fitness

    if (isMatchSide(pBoardMan->getSize() - 1, i, pBoardMan->getSize(), i))
    { // If edge piece matches inner connection to right add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }

    if (isMatchBottom(pBoardMan->getSize(), i, pBoardMan->getSize(), i + 1))
    { // If edge piece matches piece below add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }
    
  }

  if (isMatchSide(pBoardMan->getSize() - 1, pBoardMan->getSize() - 1,
                  pBoardMan->getSize(), pBoardMan->getSize() - 1))
  { // If piece matches connection to the left add score
    pBoard->fitScore += EDGEMATCH;
    pBoard->matchCount++;
  }

} // checkRightEdge()


void FitnessFunction::checkBottomEdge()
{ // Checks the slots along the bottom edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < pBoardMan->getSize() - 1; i++)
  {// Loop right the edge pieces on the bottom edge of the board checking fitness

    if (isMatchSide(i, pBoardMan->getSize(), i + 1, pBoardMan->getSize()))
    { // If right pattern matches next piece left add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }

    if (isMatchBottom(i, pBoardMan->getSize() - 1, i, pBoardMan->getSize()))
    {  // If top matched piece above add score
      pBoard->fitScore += EDGEMATCH;
      pBoard->matchCount++;
    }
    
  }

  if (isMatchBottom(pBoardMan->getSize() - 1, pBoardMan->getSize() - 1,
                    pBoardMan->getSize() - 1, pBoardMan->getSize()))
  { // If top pattern matched inner connection add score
    pBoard->fitScore += EDGEMATCH;
    pBoard->matchCount++;
  }

} // checkBottomEdge()


void FitnessFunction::checkInnerPieces()
{ // Checks the pieces in the inner slots of the board for matching connections

  for (int j = 1; j < pBoardMan->getSize() - 1; j++)
  {  // Y index for checking slots, after line has completed, increment row

    for (int i = 1; i < pBoardMan->getSize() - 1; i++)
    { // X index for checking slots

      if (isMatchSide(i, j, i + 1, j))
      { // If right side matches piece to the right add score
        pBoard->fitScore += INNERMATCH;
        pBoard->matchCount++;
      }

      if (isMatchBottom(i, j, i, j + 1))
      { // If bottom side matches piece below add score
        pBoard->fitScore += INNERMATCH;
        pBoard->matchCount++;
      }

    } // for int i < boardSize

    if (isMatchBottom(pBoardMan->getSize() - 1, j, pBoardMan->getSize() - 1,
      j + 1))
    { // If the last inner in row i piece has a match at bottom, add score
      pBoard->fitScore += INNERMATCH;
      pBoard->matchCount++;
    }

    if (isMatchSide(j, pBoardMan->getSize() - 1, j + 1,
      pBoardMan->getSize() - 1))
    { // If the most bottom inner piece in col i has match at right, add score
      pBoard->fitScore += INNERMATCH;
      pBoard->matchCount++;
    }

  } // for int j < boardSize

} // checkInnerPieces()