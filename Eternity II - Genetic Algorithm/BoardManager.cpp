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

  // Seed rand with time for random number generation
  srand((unsigned int)time(NULL));

  // Create a new vector and set it to the current generation pointer
  std::vector<Board> newVec;
  currBoards = std::make_shared<std::vector<Board>>(newVec);


} // BoardManager()


BoardManager* BoardManager::GetInstance()
{ // Returns the instance to the class, if none currently exists, creates one

  // If not pInstance, create one
  if (!pInstance) 
    pInstance = new BoardManager;

  return pInstance;   // Return the instance

} // GetInstance()


void BoardManager::InitialiseData(int size, int patNum)
{ // Sets how many patterns to be used and how many pieces there are per board

  boardSize = size - 1;    // boardSize - 1 to include 0 index
  patternNum = patNum;     // Set number of patterns for the board

} // InitialiseData()


void BoardManager::InitFullBoard(Board* pBoard, bool startPiece)
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

  // If starting piece constraint is active and piece 139 is not in slot [8][7]
  // place piece 139 in slot [8][7]
  if (startPiece && pBoard->boardVec[8][7].pieceID != 139)
    FixStartPiece(pBoard);

} // CreateInitialBoard()


void BoardManager::FixStartPiece(Board* pBoard)
{ // If an official Eternity II solve attempt, make sure the start piece
  // constraint is met by placing the piece with the ID 139 on slot [8][7]

  int xIndex = 1;     // X index for slot
  int yIndex = 1;     // Y index for slot

  for (int i = 0; i < pieceVec[INNER].size(); i++)
  { // Loop through for every inner piece within the board

    if (pBoard->boardVec[yIndex][xIndex].pieceID == 139 && yIndex != 8 &&
      xIndex != 7)
    { // If the ID of piece is 139 call subroutine to place piece 139 in slot
      // [8][7] then break out of loop as no further action is needed
      SwapStartPiece(pBoard, yIndex, xIndex);
      break;
    }
      
    if (xIndex == boardSize - 1)
    { // If reached the end of the line for the board, move onto next line
      // of the board
      yIndex++;
      xIndex = 0;
    }

  }

} // FixStartPiece()


void BoardManager::SwapStartPiece(Board* pBoard, int yIndex, int xIndex)
{ // Swap the piece with the given index with the piece in the starting piece
  // slot according to the Eternity II rule book. (Slot [8][7])

  // Store piece that is in the starting slot in temp storage
  PuzzlePiece temp = pBoard->boardVec[8][7];

  // Place starting piece in the starting slot
  pBoard->boardVec[8][7] = pBoard->boardVec[yIndex][xIndex];

  // Place piece back in to the slot that the starting piece was located
  pBoard->boardVec[yIndex][xIndex] = temp;

} // SwapPiece()


void BoardManager::AddPieces(Board* pBoard)
{ // Adds pieces to the empty boards, top edge first, moving on to inner slots
  // then finally filling in the corners, side edges and bottom edge

  int index = 1;         // Index of current vector to place piece in
  int count = 0;         // Count how many pieces places to increment index

  // Add edge pieces to vectors that will contain inner pieces
  InitTopEdge(pBoard);

  for (PuzzlePiece piece : pieceVec[INNER])
  { // Loop through the collection of pieces

    // Add piece to line on board and increment push count
    pBoard->boardVec[index].push_back(piece);
    count++;

    if (count == boardSize - 1)
    { // If reached the end of the line for the board, move onto next line
      // of the board
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
  { // Add edge pieces to inner 13 vectors (so not left and right most vectors)
    // Rotate as needed
    FixOrientation(&pieceVec[EDGE][i], 0, i + 1);
    pBoard->boardVec[i + 1].push_back(pieceVec[EDGE][i]);
  }

} // InitTopEdge()


void BoardManager::InitCornersSides(Board* pBoard)
{ // Initialises the left, right and bottom edges of the board along with
  // the corner slots of the board

  // Push the first two corner pieces on to the top left and right corners
  // of the board rotating as needed
  FixOrientation(&pieceVec[CORNER][0], 0, 0);
  pBoard->boardVec[0].push_back(pieceVec[CORNER][0]);
  FixOrientation(&pieceVec[CORNER][1], 0, boardSize);
  pBoard->boardVec[boardSize].push_back(pieceVec[CORNER][1]);

  for (int i = 0; i < boardSize - 1; i++)
  { // Loops through and push edge pieces on to the left and right edges
    // of the board rotating as needed
    FixOrientation(&pieceVec[EDGE][i + boardSize - 1], i, 0);
    pBoard->boardVec[0].push_back(pieceVec[EDGE][i + boardSize - 1]);
    FixOrientation(&pieceVec[EDGE][i + (boardSize * 2) - 2], i, boardSize);
    pBoard->boardVec[boardSize].push_back(pieceVec[EDGE]
                                                  [i + (boardSize * 2) - 2]);
  }

  for (int i = 0; i <= boardSize - 2; i++)
  { // Loops through and adds an edge piece to each vector for the bottom edge
    // of the board rotating as needed
    pBoard->boardVec[i + 1].push_back(pieceVec[EDGE][i + (boardSize * 3) - 3]);
    FixOrientation(&pBoard->boardVec[i + 1].back(), boardSize, i);
  }

  // Push the last two corner pieces on to the bottom left and bottom right
  // corners of the board rotating as needed
  FixOrientation(&pieceVec[CORNER][2], boardSize, 0);
  pBoard->boardVec[0].push_back(pieceVec[CORNER][2]);
  FixOrientation(&pieceVec[CORNER][3], boardSize, boardSize);
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

  
void BoardManager::FixOrientation(PuzzlePiece* piece, int y, int x)
{ // Rotates the piece to match the edge of the board by setting the orientation
  // so that the edge pattern matches the edge of the board. 

  // Calculate which mode to rotate with
  int mode = GetMode(piece, y, x);

  if (piece->type == EDGE)         // If piece is edge, call to rotate edge
    RotateEdge(piece, mode);
  else if (piece->type == CORNER)  // If piece is corner, call to rotate corner
    RotateCorner(piece, mode);

} // RotatePiece()


void BoardManager::RotateEdge(PuzzlePiece* piece, int mode)
{ // Rotates an edge piece so the edge pattern matches the edge of the board
  // using the mode to determine what orientation value to set

  if (mode == 0)
  { // If mode is set to top edge rotate set orientation to 2 so edge pattern 
    // is facing the top of the original orientation
    piece->orientation = 2;
  }
  else if (mode == 1)
  { // If mode is set to right edge rotate set orientation to 3 so edge pattern 
    // is facing the top of the original orientation
    piece->orientation = 3;
  }
  else if (mode == 2)
  { // If mode is set to bottom edge rotate set orientation to 2 so edge pattern 
    // is facing the top of the original orientation
    piece->orientation = 0;
  }
  else if (mode == 3)
  { // If mode is set to left edge rotate set orientation to 2 so edge pattern 
    // is facing the top of the original orientation
    piece->orientation = 1;
  }

} // RotateEdge()


void BoardManager::RotateCorner(PuzzlePiece* piece, int mode)
{ // Rotates an corner piece so the corner pattern matches the edge of the board
  // using the mode to determine what orientation value to set

  if (mode == 0)
  { // If mode is set to top left corner rotate  set orientation to 1 so that the
    // edge pattern is facing the left and top of the origination orientation
    piece->orientation = 1;
  }
  else if (mode == 1)
  { // If mode is set to top right corner rotate  set orientation to 2 so that
    // the edge pattern is facing the left and top of the origination orientation
    piece->orientation = 2;
  }
  else if (mode == 2)
  { // If mode is set to bottom left corner rotate  set orientation to 1 so that
    // the edge pattern is facing the left and top of the origination orientation
    piece->orientation = 0;
  }
  else if (mode == 3)
  { // If mode is set to bottom right corner rotate  set orientation to 1 so that
    // the edge pattern is facing the left and top of the origination orientation
    piece->orientation = 3;
  }

} // RotateEdge()


int BoardManager::GetMode(PuzzlePiece* piece, int yIndex, int xIndex)
{ // Returns an integer that corrospondes to which mode to send the rotate piece
  // subroutines. Mode depends on which border or corner the piece is located

  int mode = -1;    // Holds the mode that has been calculated

  if (piece->type == EDGE)
  { // If working out the mode for an edge piece, check index
    if (yIndex == 0 && xIndex >= 1 && xIndex <= boardSize - 1)
    { // If y = 0 and x is between 1 and boardSize - 1 set mode to top edge
      mode = 0;
    }
    else if (yIndex >= 1 && yIndex <= boardSize - 1 && xIndex == boardSize)
    { // If y is between 1 and boardSize - 1 and x = boardSize set mode to right
      // edge
      mode = 1;
    }
    else if (yIndex == boardSize && xIndex >= 1 && xIndex <= boardSize - 1)
    { // If y = boardSize and x is between 1 and boardSize - 1, set mode to 
      // bottom edge
      mode = 2;
    }
    else if (yIndex >= 1 && yIndex <= boardSize - 1 && xIndex == 0)
    { // If y is between 1 and boardSize - 1 and x = 0, set mode to left edge
      mode = 3;
    }
    
  }
  else if (piece->type == CORNER)
  {  // If working out the mode for an corner piece, check index
    if (yIndex == 0 && xIndex == 0)
    { // If y = 0 and x = 0 set mode to top left corner
      mode = 0;
    }
    else if (yIndex == 0 && xIndex == boardSize)
    { // If y = 0 and x = boardSize set mode to top right corner
      mode = 1;
    }
    else if (yIndex == boardSize && xIndex == 0)
    { // If y = boardSize and x = 0 set mode to bottom left corner
      mode = 2;
    }
    else if (yIndex == boardSize && xIndex == boardSize)
    { // If y = boardSize and x = boardSize set mode to bottom right corner
      mode = 3;
    }
  }

  return mode;     // Return calculated mode

} // GetMode()


BoardManager::~BoardManager()
{ // Destructor to delete the instance of the class

  currBoards = nullptr;     // Delete current generation
  prevBoards = nullptr;     // Delete previous generation
  delete pInstance;         // Delete instance
  pInstance = nullptr;      // Null instance

} // ~BoardManager()
