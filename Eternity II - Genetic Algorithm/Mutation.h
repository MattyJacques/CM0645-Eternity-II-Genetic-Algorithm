// Title        : Mutation.h
// Purpose      : Mutates random boards using various methods
// Author       : Matthew Jacques
// Date         : 23/01/2016


#pragma once


#include "BoardManager.h"    // PieceType enum


// Readability for methods of mutation
enum MutateType { SWAP, ROTATE, ROTATESWAP, REGIONSWAP, REGIONROTATE};


class Mutation
{

private:
  MutateType mutType;                    // Holds method of mutation
  double mutNum;                         // Number to mutate per generation 

  // Calculates how many mutations to implement per generation
  void CalcMutRate(double rate, int popSize);

  // Sets the 2 dimensional index for a random piece with the type given
  // as a parameter
  void GetRandPiece(int index[2], PieceType type, bool startPiece);

  // Process the Swap mutation method as described in chapter 3 of the report.
  // Swaps two random pieces in the board that has the ID given as the parameter
  void Swap(int boardID, bool startPiece);

  // Process the Rotate mutation method as described in chapter 3 of the report.
  // Generates a random index of a puzzle piece and rotates the piece 
  // orientation by 90 degrees clockwise.
  void Rotate(int boardID);

  // Process the Rotate & Swap mutation method as described in chapter 3 of the
  // report. Generates two random indexes of pieces within the board, rotates
  // clockwise 90 degress and swap the locations of the pieces
  void RotateSwap(int boardID);

  // Process the Region Rotate mutation method as described in chapter 3 of
  // report.  Generates two random indexes which will be used as the top left
  // pieces of two 2 x 2 regions then swaps locations of regions 
  void RegionRotate(int boardID);

  // Process the Region Swap mutation method as described in chapter 3 of the
  // report. Generates a random index of a puzzle piece to use as the top left
  // piece of a 2 x 2 region. Rotates the entire 2 x 2 region 90 degrees
  // clockwise
  void RegionSwap(int boardID);

  // Scrambles the entire population as an attampt to repair the algorithm
  // getting stuck in local maxima. Does this by creating new boards with
  // randomised piece order by calling the process ran on application start
  void Scramble();

public:
  // Initialise mutation method
  Mutation();

  // Sets the method of mutation to use for this attempt
  void Setup(MutateType type, double rate, int popSize);

  // Randomly selects a board from the current population to be mutated
  // then calls the appropriate subroutine that will do the needed mutation
  // method on that board.
  void DoMutation(bool startPiece);
};

