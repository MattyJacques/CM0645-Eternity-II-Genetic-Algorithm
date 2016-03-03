// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h" // Include header for the class
#include <iostream>           // DEBUG


// Initialise the pointer to the instance
GeneticAlgorithm* GeneticAlgorithm::pInstance = nullptr;


GeneticAlgorithm::GeneticAlgorithm(int eliteRate, double mutationRate, 
                                   int population, int boardSize, int patNum,  
                                   CrossoverType crossType, 
                                   SelectionType selectType, 
                                   MutateType mutType)
{ // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation. Also handles crossover and
  // mutation methods

  theCrossover.SetMethod(crossType, selectType, eliteRate);
  theMutation.Setup(mutType, mutationRate, population);

  popSize = population;

  maxFitness = 0;
  maxFitnessOfGen = 0;
  genCount = 0;

  BoardManager::GetInstance()->InitialiseData(boardSize, patNum);

} // GeneticAlgorithm()


void GeneticAlgorithm::CreateInstance(int eliteRate, double mutationRate, 
                                      int popSize, int boardSize, int patNum,
                                      CrossoverType crossType, 
                                      SelectionType selectType, 
                                      MutateType mutType)
{ // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor

  if (!pInstance)
  { // If the instance of the algorithm has not already been created, create
    // a new instance by calling the constructor
    pInstance = new GeneticAlgorithm(eliteRate, mutationRate, popSize, boardSize,
                                     patNum, crossType, selectType, mutType);
  }

} // CreateInstance()


GeneticAlgorithm* GeneticAlgorithm::GetInstance()
{ // Returns the current instance of the class

  return pInstance; // Return the pointer

} // GetInstance()

void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  for (int i = 0; i < popSize; i++)
  {
    Board newBoard;
    BoardManager::GetInstance()->InitFullBoard(&newBoard);
    BoardManager::GetInstance()->currBoards->push_back(newBoard);
  }

  while (true)
  {
    genCount++;
    maxFitnessOfGen = 0;

    for (int i = 0; i < popSize; i++)
    {

      if (theFitness.CheckFitness(&BoardManager::GetInstance()->currBoards->at(i)) > maxFitnessOfGen)
      {
        if (maxFitness < BoardManager::GetInstance()->currBoards->at(i).fitScore)
        {
          maxFitness = BoardManager::GetInstance()->currBoards->at(i).fitScore;
        }

        maxFitnessOfGen = BoardManager::GetInstance()->currBoards->at(i).fitScore;
      }
    }

    std::cout << "Generation " << genCount << ": This gen: " << maxFitnessOfGen << " Max Reached: " << maxFitness << std::endl;

    theCrossover.DoCrossover(popSize);
    theMutation.DoMutation();
  }

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


int GeneticAlgorithm::GenRandomNum(int min, int max)
{ // Generates a random number between min and max

  // Work out range between min and max (+1 to include max afterwards)
  int range = (max - min) + 1;

  // Get random number between 0 and the range, then add min to get between min
  // and max
  return (std::rand() % range) + min;

} // GenerateRandomNum()


GeneticAlgorithm::~GeneticAlgorithm()
{ // Destructor that deletes the instance of the class

  // Delete the instance of the GA
  delete pInstance;

  // Change to pointer to null
  pInstance = nullptr;

} // ~GeneticAlgorithm()
