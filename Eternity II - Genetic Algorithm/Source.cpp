#include <iostream>
#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileReader.h"


using namespace std;


void Initialise(GeneticAlgorithm* theGA)
{ // Calls to read all the data from the settings file and data files. Sets up
  // the algorithm with the loaded values
  
  FileReader theReader;
  theGA->Setup(theReader.ReadSettingsFile());
  
  theReader.ReadDataFile("Puzzles/256 - 22.e2");
  
} // Initialise()


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;    // Create algorithm object
  Initialise(&theGA);        // Initialise object methods
  theGA.RunGA();               // Run the algorithm loop

} // main()