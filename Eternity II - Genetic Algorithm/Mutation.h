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

  // Swaps two random pieces in the board that has the ID given as the parameter
  void Swap(int boardID, bool startPiece);

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

