// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h" // Include header for the class
#include <iostream>           // DEBUG


// Initialise the pointer to the instance
GeneticAlgorithm* GeneticAlgorithm::pAlgorithm = nullptr;


GeneticAlgorithm::GeneticAlgorithm(double eliteRate, double crossRate, 
                                   double mutationRate, int population, 
                                   int boardSize)
{ // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation

  double total = 0;

  elitism = eliteRate;
  crossover = crossRate;
  mutation = mutationRate;
  popSize = population;
  

  total = elitism + crossover + mutation;

  elitism = (int)(elitism * popSize / total);
  crossover = (int)(crossover * popSize / total);
  mutation = (int)(mutation * popSize / total);

  BoardManager::GetInstance()->InitialiseData(boardSize, 22);

} // GeneticAlgorithm()


void GeneticAlgorithm::CreateInstance(double eliteRate, double crossRate,
                                      double mutationRate, int popSize, 
                                      int boardSize)
{ // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor

  if (!pAlgorithm)
  { // If the instance of the algorithm has not already been created, create
    // a new instance by calling the constructor
    pAlgorithm = new GeneticAlgorithm(eliteRate, crossRate, mutationRate, 
                                      popSize, boardSize);
  }

} // CreateInstance()


GeneticAlgorithm* GeneticAlgorithm::GetInstance()
{ // Returns the current instance of the class

  return pAlgorithm; // Return the pointer

} // GetInstance()

int GeneticAlgorithm::GetPopSize()
{ // Returns the size of the population

  return popSize;

} // GetPopSize()


void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  for (int i = 0; i < popSize; i++)
  {
    Board newBoard;
    BoardManager::GetInstance()->CreateInitialBoard(&newBoard);
    BoardManager::GetInstance()->boards.push_back(newBoard);
  }

  std::cout << FitnessFunction::GetInstance()->CheckFitness(&BoardManager::GetInstance()->boards[0]);
  /*
  start
  set generation
  get population size
  randomly create first generation
  evaluate generation
  sort generation by fitness
  decide elites
  crossover
  mutation
  end 
  
  */

} // RunGA()


GeneticAlgorithm::~GeneticAlgorithm()
{ // Destructor that deletes the instance of the class

  // Delete the instance of the GA
  delete pAlgorithm;

  // Change to pointer to null
  pAlgorithm = nullptr;

} // ~GeneticAlgorithm()
