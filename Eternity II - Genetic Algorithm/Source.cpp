#include <iostream>
#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileReader.h"


using namespace std;


void Initialise(GeneticAlgorithm* theGA)
{ // Creates instances for the puzzle board and genetic algorithm, asks the
  // user for input for the custom variables for the solver.

  int boardSize = 0;            // Holds grid size, official EII = 16
  int patternNum = 0;           // Holds how many patterns to use in board
  double eliteRate = 0;         // Holds how many elites in population
  double mutationRate = 0;      // Holds the rate of mutation in GA
  int popSize = 0;
  
  // INSERT FILE READING SETTINGS

  theGA->Setup(1, 1, 1000, 16, 22, ONEPOINT, TOURNAMENT, SWAP);
  
  FileReader theReader;
  theReader.OpenFile("Puzzles/256 - 22.e2");
  
} // Initialise()


void main()
{ // Creates the algorithm object, initialise the methods to be used this run
  // of the genetic algorithm then call the main running of the genetic
  // algorithm loop

  GeneticAlgorithm theGA;    // Create algorithm object
  Initialise(&theGA);        // Initialise object methods
  theGA.RunGA;               // Run the algorithm loop

} // main()