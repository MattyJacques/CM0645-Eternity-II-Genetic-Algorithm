// Title        : FitnessFunction.h
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#pragma once


#include "Board.h"


class FitnessFunction
{
protected:
  static FitnessFunction* pInstance;    // Holds the current instance of class

  // Protected to avoid unauth creation of instances
  FitnessFunction();

public:
  // Returns the current instance of the class, creates if needed
  static FitnessFunction* GetInstance();

  // Checks the fitness of the candidate provided by the GA
  int CheckFitness();

  // Checks if the given slot is on a corner of the board using the indexes
  bool IsCornerSlot(int x, int y);

  // Checks if the given slot is on a edge of the board using the indexes
  bool IsEdgeSlot(int x, int y);

};

