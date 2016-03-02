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
  PieceType type;    // Holds the type of the puzzle piece, example corner, edge
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
};


class BoardManager
{

private:
  int patternNum;                     // Amount of patterns within board
  static BoardManager* pInstance;     // Current instance of class

  // Seeds rand() with time and initalises currBoards
  BoardManager();

public:
  int boardSize;                        // How many pieces per board
  std::vector<PuzzlePiece> pieceVec;    // Collection of all pieces to be used

  // Boards in current generation
  std::shared_ptr<std::vector<Board>> currBoards;

  std::shared_ptr<std::vector<Board>> prevBoards;

  // Returns the instance to the class, if none currently exists, creates one
  static BoardManager* GetInstance();

  // Sets how many patterns to be used and how many pieces there are per board
  void InitialiseData(int size, int patNum);

  // Creates the inital generation of boards
  void InitFullBoard(Board* pBoard);

  // Initialises the boards vector of vectors with empty vectors
  void InitEmptyBoard(Board* pBoard);

  // Returns the ID of the pattern located on the piece with in the index 
  // provided taking into consideration the orientation of the piece
  int GetPattern(Board* pBoard, int yIndex, int xIndex, segLocation segment);

  // Destructor to delete the instance of the class
  ~BoardManager();

};

