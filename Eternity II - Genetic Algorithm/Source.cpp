#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileHandler.h"          // To read in settings


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;                       // Create algorithm object
  FileHandler fileHandle;                       // Create file reader object
  theGA.Setup(fileHandle.ReadSettingsFile());   // Read algorithms settings
  theGA.RunGA();                                // Run the algorithm

} // main()