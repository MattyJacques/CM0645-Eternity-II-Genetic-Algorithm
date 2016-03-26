// Title        : Mutation.cpp
// Purpose      : Mutates random boards using various methods
// Author       : Matthew Jacques
// Date         : 23/01/2016


#include "Mutation.h"
#include "GeneticAlgorithm.h" // Get random number
#include <iostream>           // Debug output


Mutation::Mutation()
{ // Initialise mutation method

  mutType = SWAP;

} // Mutation()


void Mutation::CalcMutRate(double rate, int popSize)
{ // Calculates how many mutations to implement per generation

  mutNum = (rate * popSize) / 100;

} // CalcMutRate()


void Mutation::GetRandPiece(int index[2], PieceType type, bool startPiece,
                            bool region)
{ // Sets the 2 dimensional index for a random piece with the type given
  // as a parameter

  if (type == CORNER)
  { // If the type of piece needs to be a corner, generation a random number
    // between 0 and 3, to randomly select which corner
    int cornerID = GeneticAlgorithm::GenRandomNum(0, 3);

    if (cornerID == 0)
    { // Set index to top left corner
      index[0] = 0;
      index[1] = 0;
    }
    else if (cornerID == 1)
    { // Set index to top right corner
      index[0] = 0;
      index[1] = BoardManager::GetInstance()->boardSize;
    }
    else if (cornerID == 2)
    { // Set index to bottom left corner
      index[0] = BoardManager::GetInstance()->boardSize;
      index[1] = 0;
    }
    else
    { // Set index to bottom right corner
      index[0] = BoardManager::GetInstance()->boardSize;
      index[1] = BoardManager::GetInstance()->boardSize;
    }
  }
  else if (type == EDGE)
  { // Set the indexs in the array to two index taken from the top, right, left
    // or bottom edges. The edge is randomly selected then select random piece
    // from chosen edge
    int mode = GeneticAlgorithm::GenRandomNum(0, 3);

    if (mode == 0)
    { // If mode is top edge set X index to 0 and gen random index between
      // 1 and boardSize - 1 for Y index
      index[0] = 0;
      index[1] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
                                                boardSize - 1);
    }
    else if (mode == 1)
    { // If mode is left edge gen random X index between 1 and boardsize -1
      // and set Y index to 0
      index[0] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
                                                boardSize - 1);
      index[1] = 0;
    }
    else if (mode == 2)
    { // If mode is right edge gen random number between 1 and boardSize - 1
      // and set Y index to boardSize
      index[0] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 1);
      index[1] = BoardManager::GetInstance()->boardSize;
    }
    else
    { // If mode is bottom edge set X index to boardSize and gen random number
      // between 1 and boardSize - 1 for Y index
      index[0] = BoardManager::GetInstance()->boardSize;
      index[1] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 1);
    }
  }
  else
  { // Set the indexs in the array to two random number between 1 and the size
    // of the board - 1 to get two inner type pieces

    if (startPiece)
    { // If starting piece constraint is active, make sure starting piece is not
      // chosen as the random piece

      do
      { // Get a random piece index and loop while that index matches the start
        // slot index
        index[0] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
          boardSize - 1);
        index[1] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
          boardSize - 1);
      } while (index[0] == 8 && index[1] == 7);

    }
    else if (region)
    { // If index is for region, generate index that does not include the right
      // colum or bottom row of inner pieces
      index[0] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 2);
      index[1] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 2);
    }
    else
    { // If start constraint is not active, just chose any random inner piece
      index[0] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 1);
      index[1] = GeneticAlgorithm::GenRandomNum(1, BoardManager::GetInstance()->
        boardSize - 1);
    }
  }

} // GetRandPiece()


void Mutation::SwapPiece(int boardID, int pieceIndex1[2], int pieceIndex2[2])
{ // Swaps two pieces within the board with the ID given. Pieces to swap
  // also given as parameters

  // Create pointer to board to work with and initalise to point to board
  Board* pBoard = &BoardManager::GetInstance()->currBoards->at(boardID);

  // Temp puzzle piece to use during the swap
  PuzzlePiece temp = pBoard->boardVecs[pieceIndex1[0]][pieceIndex1[1]];

  // Place the second puzzle piece where first puzzle piece was located
  pBoard->boardVecs[pieceIndex1[0]][pieceIndex1[1]] =
    pBoard->boardVecs[pieceIndex2[0]][pieceIndex2[1]];

  // Place first puzzle piece back in second puzzle piece slot
  pBoard->boardVecs[pieceIndex2[0]][pieceIndex2[1]] = temp;

  if (temp.type == EDGE || temp.type == CORNER)
  { // Make sure the pieces are roatetd correctly if mutation occured on a 
    // corner or edge piece

    // Rotate piece 1
    BoardManager::GetInstance()->FixOrientation(&pBoard->boardVecs[pieceIndex1[0]]
      [pieceIndex1[1]], pieceIndex1[0],
      pieceIndex1[1]);

    // Rotate piece 2
    BoardManager::GetInstance()->FixOrientation(&pBoard->boardVecs[pieceIndex2[0]]
      [pieceIndex2[1]], pieceIndex2[0],
      pieceIndex2[1]);
  }

} // SwapPiece()


void Mutation::RotatePiece(int boardID, int pieceIndex[2])
{ // Rotates a piece 90 degrees clockwise, checks for orientation overflow.
  // Board and piece index given as parameters

  if (BoardManager::GetInstance()->currBoards->at(boardID).
    boardVecs[pieceIndex[0]][pieceIndex[1]].orientation == 3)
  { // If the orientation is 1 rotation away from full 360 degree rotation
    // reset to original rotation
    BoardManager::GetInstance()->currBoards->at(boardID).
      boardVecs[pieceIndex[0]][pieceIndex[1]].orientation = 0;
  }
  else
  { // If next rotation will not be original orientation, increment orientation
    BoardManager::GetInstance()->currBoards->at(boardID).
      boardVecs[pieceIndex[0]][pieceIndex[1]].orientation++;
  }

} // RotatePiece()


void Mutation::Swap(int boardID, bool startPiece)
{ // Swaps two random pieces in the board that has the ID given as the parameter

  int pieceIndex1[2] = { -1, -1 };     // Holds index of the first piece
  int pieceIndex2[2] = { -1, -1 };     // Holds index of second piece

  // Get random piece type for mutation
  PieceType type = (PieceType)GeneticAlgorithm::GenRandomNum(0, 2);

  while (pieceIndex1[0] == pieceIndex2[0] && pieceIndex1[1] == pieceIndex2[1])
  { // Get two random piece indexed of the generated type, making sure the
    // indexes are not the same
    GetRandPiece(pieceIndex1, type, startPiece, false);
    GetRandPiece(pieceIndex2, type, startPiece, false);
  } 

  // Call to swap the pieces with the index that have been generated
  SwapPiece(boardID, pieceIndex1, pieceIndex2);

} // Swap()


void Mutation::Rotate(int boardID, bool startPiece)
{ // Process the Rotate mutation method as described in chapter 3 of the report.
  // Generates a random index of a puzzle piece and rotates the piece 
  // orientation by 90 degrees clockwise. Calls to swap a different piece
  // as rotate by itself would not solve a puzzle Different to rotate and swap 
  // as different pieces recieve the swap to the rotate

  int pieceIndex[2] = { -1, -1 };     // Holds the piece index to rotate

  // Get a random piece index of type INNER without caring if the piece is the
  // starting piece due to rotate not breaking the constraint. INNER not
  // included due to border rotation already being managed
  GetRandPiece(pieceIndex, INNER, false, false);

  // Rotate the piece with the generated index within the given board
  RotatePiece(boardID, pieceIndex);

  // Call to swap a piece as rotate alone will not be enough to avoid a local
  // maxima or solve the puzzle
  Swap(boardID, startPiece);

} // Rotate()


void Mutation::RotateSwap(int boardID, bool startPiece)
{ // Process the Rotate & Swap mutation method as described in chapter 3 of the
  // report. Generates two random indexes of pieces within the board, rotates
  // clockwise 90 degress and swap the locations of the pieces Due to not being
  // able to rotate the border, swapping a different piece is called to let
  // the border be mutated.

  int pieceIndex1[2] = { -1, -1 };     // Holds index of first piece to mutate
  int pieceIndex2[2] = { -1, -1 };     // Holds index of second piece to mutate

  while (pieceIndex1[0] == pieceIndex2[0] && pieceIndex1[1] == pieceIndex2[1])
  { // Get a random piece index of type INNER without caring if the piece is the
    // starting piece due to rotate not breaking the constraint. INNER not
    // included due to border rotation already being managed. Makes sure pieces
    // are not the same
    GetRandPiece(pieceIndex1, INNER, startPiece, false);
    GetRandPiece(pieceIndex1, INNER, startPiece, false);
  } 

  // Call to rotate both pieces
  RotatePiece(boardID, pieceIndex1);
  RotatePiece(boardID, pieceIndex2);

  // Call to swap the locations of the pieces
  SwapPiece(boardID, pieceIndex1, pieceIndex2);

  // Call to swap a piece as rotate alone will not be enough to avoid a local
  // maxima or solve the puzzle
  Swap(boardID, startPiece);

} // RotateSwap()


void Mutation::RegionRotate(int boardID, bool startPiece)
{ // Process the Region Rotate mutation method as described in chapter 3 of
  // report. Generates a random index of a puzzle piece to use as the top left
  // piece of a 2 x 2 region. Rotates the entire 2 x 2 region 90 degrees
  // clockwise. Swap is also called as rotating alone will not solve. Different
  // to rotate and swap as different pieces recieve the swap to the rotate

  int regionIndex[2] = { -1, -1 };      // Holds top left index of region

  // Generate a random index for the top right of the region, making sure the
  // index is not on the right coloum or bottom row of inner edges. Set region
  // mode for generating index to true
  GetRandPiece(regionIndex, INNER, false, true);

  // Rotate top left piece of region
  RotatePiece(boardID, regionIndex);

  // Set region index to top right of region and rotate
  regionIndex[0]++;
  RotatePiece(boardID, regionIndex);

  // Set region index to bottom right of region and rotate
  regionIndex[1]++;
  RotatePiece(boardID, regionIndex);

  // Set region index to bottom left of region and rotate
  regionIndex[0]--;
  RotatePiece(boardID, regionIndex);

  // Call to swap a piece as rotate alone will not be enough to avoid a local
  // maxima or solve the puzzle
  Swap(boardID, startPiece);

} // RegionRotate()


void Mutation::RegionSwap(int boardID, bool startPiece)
{ // Process the Region Swap mutation method as described in chapter 3 of the
  // report. Generates two random indexes which will be used as the top left
  // pieces of two 2 x 2 regions then swaps locations of regions. 1 piece swap
  // is also called so that the border has a chance of mutation

  int regionIndex1[2] = { -1, -1 };    // Holds top left index of first region
  int regionIndex2[2] = { -1, -1 };    // Holds top left index of second region

  while (regionIndex1[0] == regionIndex2[0] && 
         regionIndex1[1] == regionIndex2[1])
  { // Generate the top left indexes of two regions
    GetRandPiece(regionIndex1, INNER, startPiece, true);
    GetRandPiece(regionIndex2, INNER, startPiece, true);
  }

  // Swap the top left region pieces
  SwapPiece(boardID, regionIndex1, regionIndex2);

  // Set region indexes to top right of regions and swap
  regionIndex1[0]++;
  regionIndex2[0]++;
  SwapPiece(boardID, regionIndex1, regionIndex2);

  // Set region indexes to bottom right of regions and swap
  regionIndex1[1]++;
  regionIndex2[1]++;
  SwapPiece(boardID, regionIndex1, regionIndex2);

  // Set region indexes to bottom left of regions and swap
  regionIndex1[0]--;
  regionIndex2[0]--;
  SwapPiece(boardID, regionIndex1, regionIndex2);

  // Call to swap a piece as rotate alone will not be enough to avoid a local
  // maxima or solve the puzzle
  Swap(boardID, startPiece);

} // RegionSwap()


void Mutation::Setup(MutateType type, double rate, int popSize)
{ // Sets the method of mutation to use for this attempt

  mutType = type;                 // Set mutation method
  CalcMutRate(rate, popSize);     // Calc how many mutation per generation

} // SetMethod()


void Mutation::DoMutation(bool startPiece)
{ // Randomly selects a board from the current population to be mutated
  // then calls the appropriate subrountine that will do the needed mutation
  // method on that board.

  for (int i = 0; i < mutNum; i++)
  { // Loop through to complete as many mutations as was calcualted on 
    // application start up


    // Get a random board ID to complete mutation on
    int boardID = GeneticAlgorithm::GenRandomNum(0, (int)BoardManager::GetInstance()
                                                ->currBoards->size() - 1);

    if (mutType == SWAP)   // If mutation method is swap, do swap
      Swap(boardID, startPiece);
    else if (mutType == ROTATE)
      Rotate(boardID, startPiece);
    else if (mutType == ROTATESWAP)
      RotateSwap(boardID, startPiece);
    else if (mutType == REGIONSWAP)
      RegionSwap(boardID, startPiece);
    else if (mutType == REGIONROTATE)
      RegionRotate(boardID, startPiece);
    else                  // Mutation method not recognised, output error
      std::cout << "Mutation method not recognised" << std::endl;

  }

} // DoMutation()
