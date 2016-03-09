// Title        : BoardManager.h
// Purpose      : Holds information on the boards, such as puzzle piece data
// Author       : Matthew Jacques
// Date         : 6/12/2015


#pragma once


#include <vector>               // Creating vectors
#include <memory>               // Shared pointers


// Enum so the code can use colours for the tile pieces
enum segLocation { TOP, RIGHT, BOTTOM, LEFT };

// Ease of use for the type of puzzle piece
enum PieceType { CORNER, EDGE, INNER };


struct PuzzlePiece
{ // Holds all the data needed to a puzzle piece of a board, including pattern
  // IDs and the type of piece

  int pieceID;      // Holds the ID of the puzzle piece
  PieceType type;   // Holds the type of the puzzle piece, example corner, edge
  int orientation;  // Holds the rotation of the puzzle piece
  int segments[4];  // Holds the pattern for each segment of the piece
};


struct Board
{ // Holds the data needed for the board, this includes the fitness score of
  // the board, the ID of the board and the location of the pieces within the
  // board

  int fitScore;       // Holds fitness score for the board
  int boardID;        // Holds the ID of the board
  
  // Vector of all pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVec;

  // Overrides the less than operator for sorting to sort by fitness
  bool operator < (const Board theBoard) const
  {
    // return if true fitness score is less than comparison
    return (fitScore < theBoard.fitScore);
  }
};


class BoardManager
{

private:
  int patternNum;                     // Amount of patterns within board
  static BoardManager* pInstance;     // Current instance of class

  // Seeds rand() with time and initalises currBoards
  BoardManager();

  // Initialises the top edge (not including corners) of the board read for
  // inner pieces to be inserted
  void InitTopEdge(Board* pBoard);

  // Initialises the left, right and bottom edges of the board along with
  // the corner slots of the board
  void InitCornersSides(Board* pBoard);

  // Adds pieces to the empty boards, top edge first, moving on to inner slots
  // then finally filling in the corners, side edges and bottom edge
  void AddPieces(Board* pBoard);

  // If an official Eternity II solve attempt, make sure the start piece
  // constraint is met by placing the piece with the ID 139 on slot [8][7]
  void FixStartPiece(Board* pBoard);

  // Swap the piece with the given index with the piece in the starting piece
  // slot according to the Eternity II rule book. (Slot [8][7])
  void SwapStartPiece(Board* pBoard, int yIndex, int xIndex);

  // Rotates an edge piece so the edge pattern matches the edge of the board
  // using the mode to determine what orientation value to set
  void RotateEdge(PuzzlePiece* piece, int mode);

  // Rotates an corner piece so the corner pattern matches the edge of the board
  // using the mode to determine what orientation value to set
  void RotateCorner(PuzzlePiece* piece, int mode);

  // Returns an integer that corrospondes to which mode to send the rotate piece
  // subroutines. Mode depends on which border or corner the piece is located
  int GetMode(PuzzlePiece* piece, int yIndex, int xIndex);

public:
  int boardSize;                      // How many pieces per board

  // Collection of all pieces to be used split into corner, edge and inner types
  std::vector<std::vector<PuzzlePiece>> pieceVec;    

  // Boards in current generation
  std::shared_ptr<std::vector<Board>> currBoards;

  // Board in previous generation
  std::shared_ptr<std::vector<Board>> prevBoards;

  // Returns the instance to the class, if none currently exists, creates one
  static BoardManager* GetInstance();

  // Sets how many patterns to be used and how many pieces there are per board
  void InitialiseData(int size, int patNum);

  // Creates the inital board filled with randomised order of pieces
  void InitFullBoard(Board* pBoard, bool startPiece);

  // Initialises the boards vector of vectors with empty vectors
  void InitEmptyBoard(Board* pBoard);

  // Returns the ID of the pattern located on the piece with in the index 
  // provided taking into consideration the orientation of the piece
  int GetPattern(Board* pBoard, int yIndex, int xIndex, segLocation segment);

  // Rotates the piece to match the edge of the board by setting the orientation
  // so that the edge pattern matches the edge of the board. 
  void FixOrientation(PuzzlePiece* piece, int y, int x);

  // Destructor to delete the instance of the class
  ~BoardManager();

};

