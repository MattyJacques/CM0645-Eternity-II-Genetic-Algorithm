// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#pragma once                    // Only include once


#include <vector>               // Creating vectors
#include <memory>               // Shared pointers


// Enum so the code can use colours for the tile pieces
enum segLocation { TOP, RIGHT, BOTTOM, LEFT };

// Ease of use for the type of puzzle piece
enum PieceType { CORNER, EDGE, INNER, DEFAULT };


struct PuzzlePiece
{ // Holds all the data needed to a puzzle piece of a board, including pattern
  // IDs and the type of piece

  int pieceID;      // Holds the ID of the puzzle piece
  PieceType type;   // Holds the type of the puzzle piece, example corner, edge
  int orientation;  // Holds the rotation of the puzzle piece
  int segments[4];  // Holds the pattern for each segment of the piece

}; // PuzzlePiece


struct Board
{ // Holds the data needed for the board, this includes the fitness score of
  // the board, the ID of the board and the location of the pieces within the
  // board

  int fitScore;       // Holds fitness score for the board
  int matchCount;     // Holds how many pattern matches are in board
  int boardID;        // Holds the ID of the board
  
  // Vector of all pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVecs;

  // Overrides the less than operator for sorting to sort by fitness
  bool operator < (const Board theBoard) const
  {
    
    bool result = false;

    if (fitScore < theBoard.fitScore)
    { // If fitness score is less than other board, set result to true
      result = true;
    }
    else if (fitScore == theBoard.fitScore && matchCount < theBoard.matchCount)
    { // If the fitness scores are the same, test to see if the second board
      // has more matches, if both are true, set result to true
      result = true;
    }

    return result;  // Return result

  } // bool operator <

}; // Board


class BoardManager
{

private:
  int patternNum;                     // Amount of patterns within board
  int boardSize;                      // Holds the size of the board
  static BoardManager* pInstance;     // Current instance of class

  // Collection of all pieces to be used split into corner, edge and inner types
  std::vector<std::vector<PuzzlePiece>> pieceVec;

  // Boards in current generation
  std::shared_ptr<std::vector<Board>> currBoards;

  // Board in previous generation
  std::shared_ptr<std::vector<Board>> prevBoards;

  // Seeds rand() with time and initalises currBoards
  BoardManager();

  // Initialises the top edge (not including corners) of the board read for
  // inner pieces to be inserted
  void initTopEdge(Board* theBoard);                       // *Out*  

  // Initialises the left, right and bottom edges of the board along with
  // the corner slots of the board
  void initCornersSides(Board* theBoard);                  // *Out*

  // Adds pieces to the empty boards, top edge first, moving on to inner slots
  // then finally filling in the corners, side edges and bottom edge
  void addPieces(Board* theBoard);                         // *Out*

  // If an official Eternity II solve attempt, make sure the start piece
  // constraint is met by placing the piece with the ID 139 on slot [7][8]
  void fixStartPiece(Board* theBoard);                     // *Out*

  // Swap the piece with the given index with the piece in the starting piece
  // slot according to the Eternity II rule book. (Slot [7][8])
  void swapStartPiece(Board* theBoard,                     // *Out*
                      int xIndex,                          // *In*  
                      int yIndex);                         // *In*

  // Rotates an edge piece so the edge pattern matches the edge of the board
  void rotateEdge(PuzzlePiece* piece,                      // *Out*  
                  int xIndex,                              // *In*
                  int yIndex);                             // *In*

  // Rotates an corner piece so the corner pattern matches the edge of the board
  void rotateCorner(PuzzlePiece* piece,                    // *Out*
                    int xIndex,                            // *In*
                    int yIndex);                           // *In*

  // Generate a random corner piece, two parameters so that the corner can
  // match up to two other pieces. If no match is needed parameter equals -1.
  // pattern1 is segment[0], pattern2 is segment[1]
  PuzzlePiece genCorner(int pattern1,                      // *In*
                        int pattern2);                     // *In*

  // Generate a random edge piece, two parameters so that the edge piece can
  // match the pattern on up to two other pieces. If no match is needed
  // parameter equals -1. pattern1 sets segment[0], pattern2 is placed on the
  // segment given as the location parameter
  PuzzlePiece genEdge(int pattern1,                        // *In*
                      int pattern2,                        // *In*
                      segLocation location);               // *In*

  // Generates a random inner piece, two parameters so that the piece can match
  // up to two other pieces. If no match needed, parameter equals -1. pattern1
  // equals segment[0] and pattern2 equals segments[3]
  PuzzlePiece genInner(int pattern1,                       // *In*
                       int pattern2);                      // *In*

  // Fill the piece vectors with the pieces located within the board given as
  // the parameter
  void populatePieces(Board* theBoard);                    // *In*

public:
  // Returns the instance to the class, if none currently exists, creates one
  static BoardManager* getInstance();

  // Returns the board size
  inline int getSize()
  {
    return boardSize; // Return the size of the board
  } // getSize()

  // Returns the vector of vectors containing the puzzle pieces
  inline std::vector<std::vector<PuzzlePiece>>* getPieces()
  { 
    return &pieceVec;   // Return the piece vector of vectors
  } // getPieces()

  // Returns a pointer to the vector that holds the current population
  inline std::shared_ptr<std::vector<Board>> getPop()
  { 
    return currBoards;  // Return the current population
  } // getPop()

  // Returns a pointer to the vector that holds the previous population
  inline std::shared_ptr<std::vector<Board>> getOldPop()
  {
    return prevBoards;  // Return the previous population
  } // getOldPop()

  // Switch the current population to the old population and create a new
  // shared vector for the new population
  void switchPop();

  // Sets how many patterns to be used and how many pieces there are per board
  void initialiseData(int size,                            // *In*
                      int patNum);                         // *In*

  // Initialises the boards vector of vectors with empty vectors
  void initEmptyBoard(Board* theBoard);                    // *Out*

  // Creates the inital board filled with randomised order of pieces
  void initFullBoard(Board* theBoard,                      // *Out*
                     bool startPiece);                     // *In*

  // Returns the ID of the pattern located on the piece with in the index 
  // provided taking into consideration the orientation of the piece
  int getPattern(Board* theBoard,                          // *In*
                 int xIndex,                               // *In*
                 int yIndex,                               // *In*
                 segLocation segment);                     // *In*

  // Rotates the piece to match the edge of the board by setting the orientation
  // so that the edge pattern matches the edge of the board. 
  void fixOrien(PuzzlePiece* piece,                        // *Out*
                int yIndex,                                // *In*
                int xIndex);                               // *In*

  // Generates a new board with random pieces placing all the pieces within
  // the pieces vectors
  void generateBoard(int size,                             // *In*
                     int pattern);                         // *In*

  // Destructor to delete the instance of the class
  ~BoardManager();

}; // BoardManager

