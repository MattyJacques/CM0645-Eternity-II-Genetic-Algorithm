#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileReader.h"           // To read in settings


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;                       // Create algorithm object
  FileReader theReader;                         // Create file reader object
  theGA.Setup(theReader.ReadSettingsFile());    // Read algorithms settings
  theGA.RunGA();                                // Run the algorithm

} // main()