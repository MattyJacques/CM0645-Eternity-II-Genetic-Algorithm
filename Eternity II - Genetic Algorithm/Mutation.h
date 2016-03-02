// Title        : Mutation.h
// Purpose      : Mutates random boards using various methods
// Author       : Matthew Jacques
// Date         : 23/01/2016


#pragma once


// Readability for methods of mutation
enum MutateType { SWAP, ROTATE, ROTATESWAP, REGIONSWAP, REGIONROTATE};


class Mutation
{

private:
  MutateType mutType;                    // Holds method of mutation
  double mutNum;                         // Number to mutate per generation 

  // Sets the 2 dimensional index for a random piece out of the inner pieces
  // of the board
  void GetRandPiece(int index[2]);

  // Swaps two random pieces in the board that has the ID given as the parameter
  void Swap(int boardID);

public:
  // Initialise mutation method
  Mutation();

  // Sets the method of mutation to use for this attempt
  void SetMethod(MutateType type);

  void DoMutation();
};

