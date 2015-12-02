// Title        : FitnessFunction.cpp
// Purpose      : Tests the fitness of a candidate created by GA
// Author       : Matthew Jacques
// Date         : 2/12/2015


#include "FitnessFunction.h"    // Include the header file

// Initialise the pointer to the instance
FitnessFunction* FitnessFunction::pInstance = nullptr;


FitnessFunction::FitnessFunction()
{

} // FitnessFunction()

FitnessFunction* FitnessFunction::GetInstance()
{ // Returns the current instance of the class, creates if needed

  if (!pInstance)
    pInstance = new FitnessFunction();

  return pInstance;

} // GetInstance()


int FitnessFunction::CheckFitness()
{ // Checks the fitness of the candidate provided by the GA

  int score = 0;

} // CheckFitness()

bool FitnessFunction::IsCornerSlot(int x, int y)
{ // Checks if the given slot is on a corner of the board using the indexes

  return false;

} // IsCornerSlot()

bool FitnessFunction::IsEdgeSlot(int x, int y)
{ // Checks if the given slot is on a edge of the board using the indexes

  return false;

} // IsEdgeSlot()
