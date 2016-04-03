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
  int matchCount;     // Holds how many pattern matches are in board
  int boardID;        // Holds the ID of the board
  
  // Vector of all pieces within board
  std::vector<std::vector<PuzzlePiece>> boardVecs;

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
  // constraint is met by placing the piece with the ID 139 on slot [7][8]
  void FixStartPiece(Board* pBoard);

  // Swap the piece with the given index with the piece in the starting piece
  // slot according to the Eternity II rule book. (Slot [7][8])
  void SwapStartPiece(Board* pBoard, int xIndex, int yIndex);

  // Rotates an edge piece so the edge pattern matches the edge of the board
  void RotateEdge(PuzzlePiece* piece, int xIndex, int yIndex);

  // Rotates an corner piece so the corner pattern matches the edge of the board
  void RotateCorner(PuzzlePiece* piece, int xIndex, int yIndex);

  // Generate a random corner piece, two parameters so that the corner can
  // match up to two other pieces. If no match is needed parameter equals -1.
  // pattern1 is segment[0], pattern2 is segment[1]
  PuzzlePiece GenCorner(int pattern1, int pattern2);

  // Generate a random edge piece, two parameters so that the edge piece can
  // match the pattern on up to two other pieces. If no match is needed
  // parameter equals -1. pattern1 sets segment[0], pattern2 is placed on the
  // segment given as the location parameter
  PuzzlePiece GenEdge(int pattern1, int pattern2, segLocation location);

  // Generates a random inner piece, two parameters so that the piece can match
  // up to two other pieces. If no match needed, parameter equals -1. pattern1
  // equals segment[0] and pattern2 equals segments[3]
  PuzzlePiece GenInner(int pattern1, int pattern2);

  // Fill the piece vectors with the pieces located within the board given as
  // the parameter
  void PopulatePieces(Board* pBoard);

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
  int GetPattern(Board* pBoard, int xIndex, int yIndex, segLocation segment);

  // Rotates the piece to match the edge of the board by setting the orientation
  // so that the edge pattern matches the edge of the board. 
  void FixOrientation(PuzzlePiece* piece, int yIndex, int xIndex);

  // Generates a new board with random pieces placing all the pieces within
  // the pieces vectors
  void GenerateBoard(int size, int pattern);

  // Destructor to delete the instance of the class
  ~BoardManager();

};

