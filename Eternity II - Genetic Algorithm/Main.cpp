// Title        : Main.cpp
// Purpose      : Main function of code
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h"     // To create and call the GA


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;                       // Create algorithm object
  bool isSuccess = true;                        // Algorithm can solve

  theGA.setup(&isSuccess);                      // Read algorithms settings

  if (isSuccess == true)
  { // If everything loaded fine or user wants to use defaults, run algorithm
    theGA.runGA();                                
  }

} // main()