// Title        : FitnessFunction.cpp
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#include "FitnessFunction.h"    // Include the header file


FitnessFunction::FitnessFunction()
{ // Initialse board and board manager pointers

  theBoard = nullptr;
  BoardMan = BoardManager::getInstance();

} // FitnessFunction()


void FitnessFunction::checkFitness(Board* theBoard)        // *In-Out*
{ // Checks the fitness of the candidate provided by the GA

  theBoard = theBoard;      // Defines the board pointer to the current board
  theBoard->fitScore = 0;   // Sets the score to 0 to begin calculation
  theBoard->matchCount = 0; // Sets the matches to 0 to begin calculation

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
  
  return (theBoardMan->getPattern(theBoard, xIndex1, yIndex1, RIGHT)
          == theBoardMan->getPattern(theBoard, xIndex2, yIndex2, LEFT));

} // isMatchSide()


bool FitnessFunction::isMatchBottom(int xIndex1,           // *In*
                                    int yIndex1,           // *In*
                                    int xIndex2,           // *In*
                                    int yIndex2)           // *In*
{ // Checks to see if the pattern located on bottom side of piece1 matches the
  // pattern located on the top of piece2

  return (theBoardMan->getPattern(theBoard, xIndex1, yIndex1, BOTTOM)
          == theBoardMan->getPattern(theBoard, xIndex2, yIndex2, TOP));

} // isMatchBottom()


void FitnessFunction::checkTopLeft()
{ // Checks the top left corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (isMatchSide(0, 0, 1, 0))
  { // Check to see if the pattern on the right of the corner piece matches
    // the pattern on the left of the piece to the right
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

  if (isMatchBottom(0, 0, 0, 1))
  { // Check to see if the pattern on the bottom of the corner piece matches
    // the pattern on the top of the piece below
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

} // checkTopLeft()


void FitnessFunction::checkTopRight()
{ // Checks the top right corner to see if the piece is the correct type and if
  // the edges of that piece match adjacent pieces

  if (isMatchSide(boardMan->getSize() - 1, 0, boardMan->getSize(), 0))
  { // Check to see if the pattern on the right side of the piece to left of
    // the corner, matches the pattern on the left of the corner piece
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

  if (isMatchBottom(boardMan->getSize(), 0, boardMan->getSize(), 1))
  { // Check to see if the pattern on the bottom of the corner piece matches
    // the pattern on the top of the piece below
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

} // checkTopRight()


void FitnessFunction::checkBottomLeft()
{ // Checks the bottom left corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (isMatchSide(0, boardMan->getSize(), 1, boardMan->getSize()))
  { // Check to see if the pattern on the right side of the corner piece 
    // matches the pattern located on the left side of the piece to the right
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

  if (isMatchBottom(0, boardMan->getSize() - 1, 0, boardMan->getSize()))
  { // Check to see if the pattern on the bottom of the piece above the 
    // corner matches the pattern located on the top of the corner
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

} // checkBottomLeft()


void FitnessFunction::checkBottomRight()
{ // Checks the bottom right corner to see if the piece is the correct type and
  // if the edges of that piece match adjacent pieces

  if (isMatchSide(boardMan->getSize() - 1, boardMan->getSize(),
      boardMan->getSize(), boardMan->getSize()))
  { // Check to see if the pattern on the right side of the piece to the left
    // of the corner matches the pattern located on the left of the corner
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

  if (isMatchBottom(boardMan->getSize(), boardMan->getSize() - 1,
                    boardMan->getSize(), boardMan->getSize()))
  { // Check to see if the pattern located on the bottom of the piece located
    // above the corner matches the pattern located on the top of the corner
    theBoard->fitScore += CORNERMATCH;
    theBoard->matchCount++;
  }

} // checkBottomRight()


void FitnessFunction::checkTopEdge()
{ // Checks the slots along the top edge to see if the piece placed within that
  // slot is of the right piece type and checks if any edges of those pieces
  // match with adjacent pieces

  for (int i = 1; i < boardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the top edge of the board checking fitness

    if (isMatchSide(i, 0, i + 1, 0))
    { // If edge pattern matches edge piece to the right add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }

    if (isMatchBottom(i, 0, i, 1))
    { // If edge pattern matches inner connection on bottom add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }
 
  }

  if (isMatchBottom(boardMan->getSize() - 1, 0, boardMan->getSize() - 1, 1))
  { // If piece left of the top right corner bottom pattern matches piece
    // underneath add score
    theBoard->fitScore += EDGEMATCH;
    theBoard->matchCount++;
  }

} // checkTopEdge()


void FitnessFunction::checkLeftEdge()
{ // Checks the slots along the left edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < boardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the top left of the board checking fitness

    if (isMatchSide(0, i, 1, i))
    { // If edge piece right pattern matches inner connection add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }

    if (isMatchBottom(0, i, 0, i + 1))
    { // If edge piece bottom pattern matches top of piece underneath add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }
  
  }

  if (isMatchSide(0, boardMan->getSize() - 1, 1, boardMan->getSize() - 1))
  { // If piece above bottom left corner right pattern matches inner 
    // connection add score
    theBoard->fitScore += EDGEMATCH;
    theBoard->matchCount++;
  }

} // checkLeftEdge()


void FitnessFunction::checkRightEdge()
{ // Checks the slots along the right edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < boardMan->getSize() - 1; i++)
  { // Loop right the edge pieces on the right edge of the board checking fitness

    if (isMatchSide(boardMan->getSize() - 1, i, boardMan->getSize(), i))
    { // If edge piece matches inner connection to right add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }

    if (isMatchBottom(boardMan->getSize(), i, boardMan->getSize(), i + 1))
    { // If edge piece matches piece below add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }
    
  }

  if (isMatchSide(boardMan->getSize() - 1, boardMan->getSize() - 1,
                  boardMan->getSize(), boardMan->getSize() - 1))
  { // If piece matches connection to the left add score
    theBoard->fitScore += EDGEMATCH;
    theBoard->matchCount++;
  }

} // checkRightEdge()


void FitnessFunction::checkBottomEdge()
{ // Checks the slots along the bottom edge to see if the piece placed within
  // that slot is of the right piece type and checks if any edges of those
  // pieces match with adjacent pieces

  for (int i = 1; i < boardMan->getSize() - 1; i++)
  {// Loop right the edge pieces on the bottom edge of the board checking fitness

    if (isMatchSide(i, boardMan->getSize(), i + 1, boardMan->getSize()))
    { // If right pattern matches next piece left add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }

    if (isMatchBottom(i, boardMan->getSize() - 1, i, boardMan->getSize()))
    {  // If top matched piece above add score
      theBoard->fitScore += EDGEMATCH;
      theBoard->matchCount++;
    }
    
  }

  if (isMatchBottom(boardMan->getSize() - 1, boardMan->getSize() - 1,
                    boardMan->getSize() - 1, boardMan->getSize()))
  { // If top pattern matched inner connection add score
    theBoard->fitScore += EDGEMATCH;
    theBoard->matchCount++;
  }

} // checkBottomEdge()


void FitnessFunction::checkInnerPieces()
{ // Checks the pieces in the inner slots of the board for matching connections

  for (int j = 1; j < boardMan->getSize() - 1; j++)
  {  // Y index for checking slots, after line has completed, increment row

    for (int i = 1; i < boardMan->getSize() - 1; i++)
    { // X index for checking slots

      if (isMatchSide(i, j, i + 1, j))
      { // If right side matches piece to the right add score
        theBoard->fitScore += INNERMATCH;
        theBoard->matchCount++;
      }

      if (isMatchBottom(i, j, i, j + 1))
      { // If bottom side matches piece below add score
        theBoard->fitScore += INNERMATCH;
        theBoard->matchCount++;
      }

    } // for int i < boardSize

    if (isMatchBottom(boardMan->getSize() - 1, j, boardMan->getSize() - 1,
      j + 1))
    { // If the last inner in row i piece has a match at bottom, add score
      theBoard->fitScore += INNERMATCH;
      theBoard->matchCount++;
    }

    if (isMatchSide(j, boardMan->getSize() - 1, j + 1,
      boardMan->getSize() - 1))
    { // If the most bottom inner piece in col i has match at right, add score
      board->fitScore += INNERMATCH;
      board->matchCount++;
    }

  } // for int j < boardSize

} // checkInnerPieces()