// Title        : Mutation.h
// Purpose      : Mutates random boards using various methods
// Author       : Matthew Jacques
// Date         : 23/01/2016


#pragma once


#include "BoardManager.h"    // PieceType enum


// Holds all of the different mutation methods that can be used
enum MutateType { SWAP, ROTATE, ROTATESWAP, REGIONSWAP, REGIONROTATE};


class Mutation
{

private:
  MutateType mutType;                  // Holds method of mutation
  double mutNum;                       // Number to mutate per generation 

  // Calculates how many mutations to implement per generation
  void CalcMutRate(double rate,        // *In*
                   int popSize);       // *In*

  // Sets the 2 dimensional index for a random piece with the type given
  // as a parameter (0 = corner, 1 = edge, 2 = inner)
  void GetRandPiece(int index[2],      // *Out*
                    int type,          // *In*
                    bool startPiece,   // *In*
                    bool region);      // *In*

  // Sets the 2 dimensional index for a random corner piece
  void GetRandCorner(int index[2]);    // *Out*

  // Set the indexs in the array to two index taken from the top, right, left
  // or bottom edges. The edge is randomly selected then select random piece
  // from chosen edge
  void GetRandEdge(int index[2]);      // *Out*

  // Set the indexs in the array to two random number between 1 and the size
  // of the board - 1 to get two inner type pieces
  void GetRandInner(int index[2],      // *Out*
                    bool startPiece,   // *In*
                    bool region);      // *In*

  // Swaps two pieces within the board with the ID given. Pieces to swap
  // also given as parameters
  void SwapPiece(int boardID,          // *In*
                 int pieceIndex1[2],   // *In*
                 int pieceIndex2[2]);  // *In*

  // Rotates a piece 90 degrees clockwise, checks for orientation overflow.
  // Board and piece index given as parameters
  void RotatePiece(int boardID,        // *In*
                   int pieceIndex[2]); // *In*

  // Process the Swap mutation method as described in chapter 3 of the report.
  // Swaps two random pieces in the board that has the ID given as the parameter
  void Swap(int boardID,               // *In*
            bool startPiece);          // *In*

  // Process the Rotate mutation method as described in chapter 3 of the report.
  // Generates a random index of a puzzle piece and rotates the piece 
  // orientation by 90 degrees clockwise. Calls to swap a different piece
  // as rotate by itself would not solve a puzzle Different to rotate and swap 
  // as different pieces recieve the swap to the rotate
  void Rotate(int boardID,             // *In*
              bool startPiece);        // *In*

  // Process the Rotate & Swap mutation method as described in chapter 3 of the
  // report. Generates two random indexes of pieces within the board, rotates
  // clockwise 90 degress and swap the locations of the pieces Due to not being
  // able to rotate the border, swapping a different piece is called to let
  // the border be mutated. 
  void RotateSwap(int boardID,         // *In*
                  bool startPiece);    // *In*

  // Process the Region Rotate mutation method as described in chapter 3 of
  // report. Generates a random index of a puzzle piece to use as the top left
  // piece of a 2 x 2 region. Rotates the entire 2 x 2 region 90 degrees
  // clockwise. Swap is also called as rotating alone will not solve. Different
  // to rotate and swap as different pieces recieve the swap to the rotate
  void RegionRotate(int boardID,       // *In*
                    bool startPiece);  // *In*

  // Process the Region Swap mutation method as described in chapter 3 of the
  // report. Generates two random indexes which will be used as the top left
  // pieces of two 2 x 2 regions then swaps locations of regions. 1 piece swap
  // is also called so that the border has a chance of mutation
  void RegionSwap(int boardID,         // *In*
                  bool startPiece);    // *In*

public:
  // Initialise mutation method
  Mutation();

  // Sets the method of mutation to use for this attempt
  void Setup(MutateType type,          // *In*
             double rate,              // *In*
             int popSize);             // *In*

  // Randomly selects a board from the current population to be mutated
  // then calls the appropriate subroutine that will do the needed mutation
  // method on that board.
  void DoMutation(bool startPiece);    // *In*

}; // Mutation

