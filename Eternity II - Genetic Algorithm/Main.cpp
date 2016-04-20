// Title        : Main.cpp
// Purpose      : Main function of code
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileHandler.h"          // To read in settings


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;                       // Create algorithm object
  FileHandler fileHandle;                       // Create file reader object

  theGA.setup(fileHandle.readSettingsFile());   // Read algorithms settings
  theGA.runGA();                                // Run the algorithm

} // main()