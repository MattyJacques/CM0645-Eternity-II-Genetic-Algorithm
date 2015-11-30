#include <iostream>
#include "Board.h"                // To create board instance
#include "GeneticAlgorithm.h"     // To create and call the GA
#include "FileReader.h"


using namespace std;


void Initialise()
{ // Creates instances for the puzzle board and genetic algorithm, asks the
  // user for input for the custom variables for the solver.

  int boardSize = 0;            // Holds grid size, official EII = 16
  int patternNum = 0;           // Holds how many patterns to use in board
  double eliteRate = 0;         // Holds how many elites in population
  double crossRate = 0;         // Holds the rate for crossovers in GA
  double mutationRate = 0;      // Holds the rate of mutation in GA
  int popSize = 0;
  
  //// Output asking for board size then handles user input
  //cout << "Board Size: ";
  //cin >> boardSize;

  //// Output asking for how many patterns to use in board, handles user input
  //cout << "Number of patterns: ";
  //cin >> patternNum;

  //// Output asking how many elite for generations in GA then handles user input
  //cout << "Elitism Rate: ";
  //cin >> eliteRate;

  //// Output asking for the rate of crossovers then handles user input
  //cout << "Crossover Rate: ";
  //cin >> crossRate;

  //// Output asking for mutation rate for GA then handles user input
  //cout << "Mutation Rate: ";
  //cin >> mutationRate;

  //// Output asking for population size for a generation then handles user input
  //cout << "Population Size: ";
  //cin >> popSize;

  // Creates an instance for the board and GA, giving respective user input
  Board::CreateInstance(16, patternNum);
  FileReader* pInst = FileReader::CreateInstance("Puzzles/256.e2");
  pInst->ReadPieceFile();
  GeneticAlgorithm::CreateInstance(eliteRate, crossRate, 
                                   mutationRate, popSize);

} // Initialise()


void main()
{
  Initialise();
 /* while (solution not found)
  {
    selection;
    reproduce;
    crossover;
    mutation;
    evaluation;
  }*/

} // main()