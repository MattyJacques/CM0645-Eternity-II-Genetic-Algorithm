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

  theGA.setup();                                // Read algorithms settings
  theGA.runGA();                                // Run the algorithm

} // main()