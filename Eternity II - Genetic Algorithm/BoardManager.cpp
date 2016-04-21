// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#include "BoardManager.h"     // Class declaration
#include "GeneticAlgorithm.h" // Random number generation
#include <algorithm>          // Random_shuffle()
#include <time.h>             // time()


// Initialise to null
BoardManager* BoardManager::instance = nullptr;


BoardManager::BoardManager()
{ // Seeds rand() with time and initalises currBoards

  std::vector<Board> newPop;    // Create a new vector for first population

  // Set current population pointer to new board vector
  currBoards = std::make_shared<std::vector<Board>>(newPop);
  
  prevBoards = nullptr;    // Init previous generation to null
  boardSize = 15;          // Set default board size to Eternity II size 
  patternNum = 22;         // Set default number of patterns to Eternity II

  // Seed rand with time for random number generation
  srand((unsigned int)time(NULL));

} // BoardManager()


BoardManager* BoardManager::getInstance()
{ // Returns the instance to the class, if none currently exists, creates one

  if (!instance)
  { // If not pInstance, create one
    instance = new BoardManager;
  }

  return instance;   // Return the instance

} // getInstance()


void BoardManager::switchPop()
{ // Switch the current population to the old population and create a new
  // shared vector for the new population

  std::vector<Board> newPop; // Create vector for new population

  prevBoards = currBoards;   // Make current population the previous population

  // Make the new vector shared and set to current populuation
  currBoards = std::make_shared<std::vector<Board>>(newPop);

} // switchPop()


void BoardManager::initialiseData(int size,                            // *In*
                                  int patNum)                          // *In*
{ // Sets how many patterns to be used and how many pieces there are per board

  boardSize = size - 1;    // boardSize - 1 to include 0 index
  patternNum = patNum;     // Set number of patterns for the board

} // initialiseData()


void BoardManager::initEmptyBoard(Board* theBoard)                     // *Out*  
{ // Initialises the boards vector of vectors with empty vectors

  for (int i = 0; i <= boardSize; i++)
  { // Loop through and create vector of pieces for the board and push on to
    // vector of vectors
    std::vector<PuzzlePiece> newCol;         // Create new vector for pieces
    theBoard->boardVecs.push_back(newCol);   // Push onto vector of vectors
  }

} // initEmptyBoard()


void BoardManager::initFullBoard(Board* theBoard,                      // *Out*
                                 bool startPiece)                      // *In*
{ // Creates the inital board filled with randomised order of pieces

  theBoard->boardID = (int)currBoards->size() + 1;  // Set the ID of the board

  initEmptyBoard(theBoard);       // Fill vector of vectors with empty vectors

  for (int i = 0; i < 3; i++)
  { // Loop to shuffle all piece types
    std::random_shuffle(pieceVec[i].begin(), pieceVec[i].end());
  }

  // Add pieces to empty vectors
  addPieces(theBoard);

  if (startPiece && theBoard->boardVecs[7][8].pieceID != 139)
  { // If starting piece constraint is active and piece 139 is not in slot [7][8]
    // place piece 139 in slot [7][8]
    fixStartPiece(theBoard);
  }
} // initFullBoard()


int BoardManager::getPattern(Board* theBoard,                          // *In*
                             int xIndex,                               // *In*
                             int yIndex,                               // *In*
                             segLocation segment)                      // *In*
{ // Returns the ID of the pattern located on the piece with in the index 
  // provided taking into consideration the orientation of the piece

  // Get the index of the segment taking into account orientation
  int index = (segment - theBoard->boardVecs[xIndex][yIndex].orientation);

  if (index < 0 && index > -4)
  { // If index is below limit, cycle back around
    switch (index)
    {
      case -1:     // Segment is left
        index = 3;
        break;

      case -2:     // Segment is bottom
        index = 2;
        break;

      case -3:     // Segment is right
        index = 1;
        break;

      default:     // Do nothing
        index = index;
    }
  }

  // Modular of 4 to keep segment index 3 or below
  return theBoard->boardVecs[xIndex][yIndex].segments[index];

} // getPattern()


void BoardManager::fixOrien(PuzzlePiece* piece,                        // *Out*
                            int index[2],                                // *In*
{ // Rotates the piece to match the edge of the board by setting the orientation
  // so that the edge pattern matches the edge of the board. 

  if (piece->type == EDGE)         
  { // If piece is edge, call to rotate edge
    rotateEdge(piece, index[0], index[1]);
  }
  else if (piece->type == CORNER)  
  { // If piece is corner, call to rotate corner
    rotateCorner(piece, index[0], index[1]);
  }

} // fixOrientation()


void BoardManager::generateBoard(int size,                             // *In*
                                 int pattern)                          // *In*
{ // Generates a new board with random pieces placing all the pieces within
  // the pieces vectors

  Board newBoard;              // Create new board

  boardSize = size - 1;         // Set the board size
  patternNum = pattern;         // Set the number of patterns

  initEmptyBoard(&newBoard);   // Fill board vector with vectors

  // Generate top left corner piece
  newBoard.boardVecs[0].push_back(genCorner(-1, -1));

  // Generate first piece on top edge, needed as corner piece will not match
  newBoard.boardVecs[1].push_back(genEdge(-1, newBoard.boardVecs[0][0].
                                          segments[0], RIGHT));

  for (int i = 1; i < boardSize - 1; i++)
  { // Generate the next piece of the top edge, using the pattern of the piece
    // created before for pattern match
    newBoard.boardVecs[i + 1].push_back(genEdge(-1, newBoard.boardVecs[i][0].
                                                segments[3], RIGHT));
  }

  // Generate the top right corner piece
  newBoard.boardVecs[boardSize].push_back(genCorner(-1, newBoard.boardVecs
                                                    [boardSize - 1][0].
                                                    segments[3]));

  for (int i = 0; i < boardSize - 1; i++)
  { // Generate the left edge using the pattern from the piece in the slot
    // above
    newBoard.boardVecs[0].push_back(genEdge(-1, newBoard.boardVecs[0][i].
                                            segments[1], LEFT));
  }

  // Generate the inner piece in [1][1] for match with edge piece above
  newBoard.boardVecs[1].push_back(genInner(newBoard.boardVecs[1][0].
                                           segments[0], newBoard.
                                           boardVecs[0][1].segments[0]));

  for (int i = 1; i < boardSize - 1; i++)
  { // Generate the inner pieces in col 1, this is so the pieces can
    // match the pattern of the other pieces after correct orientation
    newBoard.boardVecs[1].push_back(genInner(newBoard.boardVecs[1][i].
                                             segments[2], newBoard.
                                             boardVecs[0][i + 1].segments[0]));
  }

  for (int i = 2; i < boardSize; i++)
  {
    // Generate the first row of inner pieces to make it easier to calculate the 
    // matching pattern
    newBoard.boardVecs[i].push_back(genInner(newBoard.boardVecs[i][0].
                                             segments[0],
                                             newBoard.boardVecs[i - 1][1].
                                             segments[1]));

  }

  for (int j = 2; j < boardSize; j++)
  { // Loop through to add all inner type pieces
    for (int i = 2; i < boardSize; i++)
    { // Add a piece to col i, matching pattern of piece above and to the left
      newBoard.boardVecs[i].push_back(genInner(newBoard.boardVecs[i][j - 1].
                                               segments[2], newBoard.
                                               boardVecs[i - 1][j].
                                               segments[1]));
    }
  }

  // Generate first piece of right edge, matching pattern with top right corner
  newBoard.boardVecs[boardSize].push_back(genEdge(newBoard.
                                                  boardVecs[boardSize - 1][1].
                                                  segments[1], newBoard.
                                                  boardVecs[boardSize][0].
                                                  segments[0], RIGHT));

  for (int i = 1; i < boardSize - 1; i++)
  { // Generate the pieces for the right edge, making sure patterns match
    newBoard.boardVecs[boardSize].push_back(genEdge(newBoard.
                                                    boardVecs[boardSize - 1][i + 1]
                                                    .segments[1], newBoard.
                                                    boardVecs[boardSize][i].
                                                    segments[3], RIGHT));
  }

  // Generate the bottom left corner piece, matching the pattern of the piece
  // above
  newBoard.boardVecs[0].push_back(genCorner(newBoard.boardVecs[0][boardSize - 1]
                                            .segments[1], -1));

  for (int i = 1; i < boardSize; i++)
  { // Generate the bottom edge of the board, making sure the patterns match
    // piece above and piece to the left
    newBoard.boardVecs[i].push_back(genEdge(newBoard.   
                                            boardVecs[i][boardSize - 1].
                                            segments[2], newBoard.
                                            boardVecs[i - 1][boardSize].
                                            segments[1], LEFT));
  }

  // Generate the bottom right corner piece, matching the piece to the left and
  // above
  newBoard.boardVecs[boardSize].push_back(genCorner(newBoard.boardVecs
                                                    [boardSize - 1][boardSize].
                                                    segments[1], newBoard.
                                                    boardVecs
                                                    [boardSize][boardSize - 1].
                                                    segments[3]));

  // Population piece vecs with pieces from board
  populatePieces(&newBoard);

} // generateBoard()


void BoardManager::initTopEdge(Board* theBoard)                        // *Out* 
{ // Initialises the top edge (not including corners) of the board read for
  // inner pieces to be inserted

  for (int i = 0; i <= boardSize - 2; i++)
  { // Add edge pieces to inner 13 vectors (so not left and right most vectors)
    // Rotate as needed
    fixOrien(&pieceVec[EDGE][i], i + 1, 0);                  // Fix orientation
    theBoard->boardVecs[i + 1].push_back(pieceVec[EDGE][i]); // Store piece
  }

} // initTopEdge()


void BoardManager::initCornersSides(Board* theBoard)                   // *Out*
{ // Initialises the left, right and bottom edges of the board along with
  // the corner slots of the board

  // Fix orientation for top left corner and store in top left corner
  fixOrien(&pieceVec[CORNER][0], 0, 0);                      // Fix orientation
  theBoard->boardVecs[0].push_back(pieceVec[CORNER][0]);     // Store piece

  // Fix orientation for top right corner and store on top right corner
  fixOrien(&pieceVec[CORNER][1], boardSize, 0);              // Fix orientation
  theBoard->boardVecs[boardSize].push_back(pieceVec[CORNER][1]); // Store piece

  for (int i = 0; i < boardSize - 1; i++)
  { // Loops through and push edge pieces on to the left and right edges
    // of the board rotating as needed

    // Fix orientation for left edge and store next piece on left edge
    fixOrien(&pieceVec[EDGE][i + boardSize - 1], 0, i + 1);
    theBoard->boardVecs[0].push_back(pieceVec[EDGE][i + boardSize - 1]);

    // Fix orientation for right edge and store next piece on right edge
    fixOrien(&pieceVec[EDGE][i + (boardSize * 2) - 2], boardSize, i + 1);
    theBoard->boardVecs[boardSize].push_back(pieceVec[EDGE]
                                             [i + (boardSize * 2) - 2]);
  }

  for (int i = 0; i <= boardSize - 2; i++)
  { // Loops through and adds an edge piece to each vector for the bottom edge
    // of the board rotating as needed

    // Fix orienation for bottom edge and store next piece on bottom edge
    fixOrien(&pieceVec[EDGE][i + (boardSize * 3) - 3], i + 1, boardSize);
    theBoard->boardVecs[i + 1].push_back(pieceVec[EDGE]
                                         [i + (boardSize * 3) - 3]);
  }

  // Fix orientation for bottom left corner and store on bottom left corner
  fixOrien(&pieceVec[CORNER][2], 0, boardSize);
  theBoard->boardVecs[0].push_back(pieceVec[CORNER][2]);

  // Fix orientation for bottom right corner and store the next piece
  fixOrien(&pieceVec[CORNER][3], boardSize, boardSize);
  theBoard->boardVecs[boardSize].push_back(pieceVec[CORNER][3]);

} // initCornersSides()


void BoardManager::addPieces(Board* theBoard)                          // *Out*
{ // Adds pieces to the empty boards, top edge first, moving on to inner slots
  // then finally filling in the corners, side edges and bottom edge

  int index[2] = { 0, 1 };        // Index of current vector to place piece in

  // Add edge pieces to vectors that will contain inner pieces
  initTopEdge(theBoard);

  for (PuzzlePiece piece : pieceVec[INNER])
  { // Loop through the collection of pieces

    theBoard->boardVecs[index[1]].push_back(piece); // Add piece to board
    index[0]++;                                     // Increment counter

    if (index[0] == boardSize - 1)
    { // If reached the end of the line for the board, move onto next line
      // of the board
      index[1]++;                                   // Increment row
      index[0] = 0;                                 // Reset col
    }
  }

  // Add rest of border pieces
  initCornersSides(theBoard);

} // addPieces()


void BoardManager::fixStartPiece(Board* theBoard)                      // *Out*
{ // If an official Eternity II solve attempt, make sure the start piece
  // constraint is met by placing the piece with the ID 139 on slot [7][8]

  int index[2] = { 1, 1 };     // Index for slot

  for (int i = 0; i < (int)pieceVec[INNER].size(); i++)
  { // Loop through for every inner piece within the board

    if (theBoard->boardVecs[index[0]][index[1]].pieceID == 139 && index[0] != 7 &&
        index[1] != 8)
    { // If the ID of piece is 139 call subroutine to place piece 139 in slot
      // [7][8] then break out of loop as no further action is needed
      swapStartPiece(theBoard, index);
      break;
    }
      
    if (index[0] == boardSize - 1)
    { // If reached the end of the line for the board, move onto next line
      // of the board
      index[1]++;           // Increment y
      index[0] = 0;         // reset x
    }
  }

} // fixStartPiece()


void BoardManager::swapStartPiece(Board* theBoard,                     // *Out*
                                  int index[2],                        // *In*  
{ // Swap the piece with the given index with the piece in the starting piece
  // slot according to the Eternity II rule book. (Slot [7][8])

  // Store piece that is in the starting slot in temp storage
  PuzzlePiece swapTemp = theBoard->boardVecs[7][8];

  // Place starting piece in the starting slot
  theBoard->boardVecs[7][8] = theBoard->boardVecs[index[0]][index[1]];

  // Place piece back in to the slot that the starting piece was located
  theBoard->boardVecs[index[0]][index[1]] = swapTemp;

} // swapStartPiece()


void BoardManager::rotateEdge(PuzzlePiece* piece,                      // *Out*  
                              int index[2],                            // *In*
{ // Rotates an edge piece so the edge pattern matches the edge of the board

  if (index[0] >= 1 && index[0] <= boardSize - 1 && index[1] == 0)
  { // If y = 0 and x is between 1 and boardSize - 1 set orientation to 2
    piece->orientation = 2;
  }
  else if (index[0] == boardSize && index[1] >= 1 && index[1] <= boardSize - 1)
  { // If y is between 1 and boardSize - 1 and x = boardSize set orientation to 
    // 3
    piece->orientation = 3;
  }
  else if (index[0] >= 1 && index[0] <= boardSize - 1 && index[1] == boardSize)
  { // If y = boardSize and x is between 1 and boardSize - 1, set orientation to
    // 0
    piece->orientation = 0;
  }
  else if (index[0] == 0 && index[1] >= 1 && index[1] <= boardSize - 1)
  { // If y is between 1 and boardSize - 1 and x = 0, set orientation to 1
    piece->orientation = 1;
  }

} // rotateEdge()


void BoardManager::rotateCorner(PuzzlePiece* piece,                    // *Out*
                                int index[2],                          // *In*
{ // Rotates an corner piece so the corner pattern matches the edge of the board

  if (index[0] == 0 && index[1] == 0)
  { // If y = 0 and x = 0 set mode to top left corner
    piece->orientation = 1;
  }
  else if (index[0] == boardSize && index[1] == 0)
  { // If y = 0 and x = boardSize set mode to top right corner
    piece->orientation = 2;
  }
  else if (index[0] == 0 && index[1] == boardSize)
  { // If y = boardSize and x = 0 set mode to bottom left corner
    piece->orientation = 0;
  }
  else if (index[0] == boardSize && index[1] == boardSize)
  { // If y = boardSize and x = boardSize set mode to bottom right corner
    piece->orientation = 3;
  }

} // rotateCorner()


PuzzlePiece BoardManager::genCorner(int pattern1,                      // *In*
                                    int pattern2)                      // *In*
{ // Generate a random corner piece, two parameters so that the corner can
  // match up to two other pieces. If no match is needed parameter equals -1.
  // pattern1 is segment 0, pattern2 is segment 1

  PuzzlePiece newPiece;       // Create a new puzzle piece for board
  newPiece.type = CORNER;     // Set the piece type
  newPiece.orientation = 0;   // Set the orientation of the piece to default

  if (pattern1 > 0)
  { // If corner needs to match adjacent piece, set the correct pattern
    newPiece.segments[0] = pattern1;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[0]);
  }

  if (pattern2 > 0)
  { // If corner needs to match adjacent piece, set the correct pattern
    newPiece.segments[1] = pattern2;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[1]);
  }

  newPiece.segments[2] = 0;   // Set the edge pattern ID
  newPiece.segments[3] = 0;   // Set the edge pattern ID

  return newPiece;            // Return the new puzzle piece

} // genCorner()


PuzzlePiece BoardManager::genEdge(int pattern1,                        // *In*
                                  int pattern2,                        // *In*
                                  segLocation location)                // *In*
{ // Generate a random edge piece, two parameters so that the edge piece can
  // match the pattern on up to two other pieces. If no match is needed
  // parameter equals -1. pattern1 sets segment 0, pattern2 is placed on the
  // segment given as the location parameter

  PuzzlePiece newPiece;       // Create a new puzzle piece for board
  newPiece.type = EDGE;       // Set the piece type
  newPiece.orientation = 0;   // Set the orientation of the piece to default

  if (pattern1 > 0)
  { // If edge needs to match adjacent piece, set the correct pattern
    newPiece.segments[0] = pattern1;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[0]);
  }

  if (location == RIGHT && pattern2 > 0)
  { // If the pattern needs to match the pattern on another piece, set pattern
    // ID
    newPiece.segments[1] = pattern2;
  }
  else
  { // If no match needed, generate random pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[1]);
  }

  newPiece.segments[2] = 0;    // Set edge pattern ID

  if (location == LEFT && pattern2 > 0)
  { // If edge piece needs to match adjacent piece, set pattern ID
    newPiece.segments[3] = pattern2;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[3]);
  }

  return newPiece; // Return the new piece

} // genEdge()


PuzzlePiece BoardManager::genInner(int pattern1,                       // *In*
                                   int pattern2)                       // *In*
{ // Generates a random inner piece, two parameters so that the piece can match
  // up to two other pieces. If no match needed, parameter equals -1. pattern1
  // equals segment[0] and pattern2 equals segments[3]

  PuzzlePiece newPiece;       // New puzzle piece for board
  newPiece.type = INNER;      // Set piece type
  newPiece.orientation = 0;   // Set the orientation of the piece to default

  if (pattern1 > 0)
  { // If piece needs to match adjacent piece, set the correct pattern
    newPiece.segments[0] = pattern1;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[0]);
  }

  // Generate random pattern for right edge of piece
  GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[1]);

  // Generate random pattern for bottom edge of piece
  GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[2]);

  if (pattern2 > 0)
  { // If piece needs to match adjacent piece, set the correct pattern
    newPiece.segments[3] = pattern2;
  }
  else
  { // If no match needed, generate pattern ID
    GeneticAlgorithm::genRandomNum(1, patternNum, &newPiece.segments[3]);
  }

  return newPiece; // Return the new piece

} // genInner()


void BoardManager::populatePieces(Board* theBoard)                     // *In*
{ // Fill the piece vectors with the pieces located within the board given as
  // the parameter

  int pieceCount = 1;     // Counts number of pieces for pieceID

  // Push top left corner on to corner piece vector, set ID and increment count
  pieceVec[0].push_back(theBoard->boardVecs[0][0]); 
  pieceVec[0][0].pieceID = pieceCount;              // Set piece ID
  pieceCount++;                                     // Increment piece count

  // Push top left corner on to corner piece vector, set ID and increment count
  pieceVec[0].push_back(theBoard->boardVecs[boardSize][0]); 
  pieceVec[0][1].pieceID = pieceCount;                      // Set piece ID
  pieceCount++;                                         // Increment piece count

  // Push top left corner on to corner piece vector, set ID and increment count
  pieceVec[0].push_back(theBoard->boardVecs[0][boardSize]); 
  pieceVec[0][2].pieceID = pieceCount;                      // Set piece ID
  pieceCount++;                                         // Increment piece count

  // Push top left corner on to corner piece vector, set ID and increment count
  pieceVec[0].push_back(theBoard->boardVecs[boardSize][boardSize]);
  pieceVec[0][3].pieceID = pieceCount;                     // Set piece ID
  pieceCount++;                                            // Increment count

  for (int i = 1; i < boardSize; i++)
  { // Push edge on to edge piece vector
    
    // Push the next top edge piece to the edge piece vectors
    pieceVec[1].push_back(theBoard->boardVecs[i][0]);
    pieceVec[1].back().pieceID = pieceCount;          // Set piece ID
    pieceCount++;                                     // Increment piece count
  
    // Push the next left edge piece to the edge piece vectors
    pieceVec[1].push_back(theBoard->boardVecs[0][i]);
    pieceVec[1].back().pieceID = pieceCount;          // Set piece ID
    pieceCount++;                                     // Increment piece count


    // Push the next right edge piece to the edge piece vectors
    pieceVec[1].push_back(theBoard->boardVecs[boardSize][i]);
    pieceVec[1].back().pieceID = pieceCount;          // Set piece ID
    pieceCount++;                                     // Increment piece count

    // Push the next bottom edgepiece to the edge piece vectors
    pieceVec[1].push_back(theBoard->boardVecs[i][boardSize]);
    pieceVec[1].back().pieceID = pieceCount;          // Set piece ID
    pieceCount++;                                     // Increment piece count

  } // for (int i = 1; i < boardSize; i++)

  for (int i = 1; i < boardSize; i++)
  { // X index to push the inner puzzle pieces within the piece vectors
    for (int j = 1; j < boardSize; j++)
    { // Y index to push pieces to vector

      // Push next inner piece onto inner vector
      pieceVec[2].push_back(theBoard->boardVecs[i][j]);
      pieceVec[2].back().pieceID = pieceCount;          // Set piece ID
      pieceCount++;                                     // Increment piece count
    }
  }

} // populatePieces()


BoardManager::~BoardManager()
{ // Destructor to delete the instance of the class

  currBoards = nullptr;     // Delete current generation
  prevBoards = nullptr;     // Delete previous generation
  delete instance;         // Delete instance
  instance = nullptr;      // Null instance

} // ~BoardManager()