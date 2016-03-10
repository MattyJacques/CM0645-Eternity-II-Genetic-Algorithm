// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h" // Include header for the class
#include <iostream>           // DEBUG


void GeneticAlgorithm::Setup(Settings theSettings)
{ // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation. Also handles crossover and
  // mutation methods

  // Set up the crossover object using the input methods
  theCrossover.SetMethod(theSettings.crossType, theSettings.selectType, 
                         theSettings.eliteRate);

  // Set up the mutation method using the input methods
  theMutation.Setup(theSettings.mutType, theSettings.mutRate, 
                    theSettings.popSize);

  popSize = theSettings.popSize; // Set the population for each generation
  maxFitness = 0;                // Initialise maximum fitness GA has reached
  maxFitnessOfGen = 0;           // Init maximum fitness of current generation
  genCount = 0;                  // Init generation count

  // Set if the start piece constraint is active
  startPiece = theSettings.startCons;

  // Output all settings to user
  OutputSettings(theSettings);

  // Initialise the board manager with the board size and number of patterns
  BoardManager::GetInstance()->InitialiseData(theSettings.boardSize, 
                                              theSettings.patternNum);

} // GeneticAlgorithm()


void GeneticAlgorithm::OutputSettings(Settings theSettings)
{ // Outputs all of the loaded settings so the user can see what methods are
  // used for solving attempt

  // Print out the data on the board along with the population size, mutation
  // rate and elitism rate
  printf("Board Size: %i\nNumber of Patterns: %i\nPopulation Size: %i\n"
         "Mutation Rate: %.2f%%\nElitism Rate: %i\n", theSettings.boardSize, 
         theSettings.patternNum, theSettings.popSize, theSettings.mutRate, 
         theSettings.eliteRate);

  // Print out the enum value as a string for selection
  if (theSettings.selectType == ROULETTE)
    printf("Selection: Roulette\n");
  else if (theSettings.selectType == TOURNAMENT)
    printf("Selection: Tournament\n");

  // Print out the enum value as a string for crossover
  if (theSettings.crossType == ONEPOINT)
    printf("Crossover: One-Point\n");
  else if (theSettings.crossType == TWOPOINT)
    printf("Crossover: Two-Point\n");

  // Print out the enum value as a string for mutation
  if (theSettings.mutType == SWAP)
    printf("Mutation: Swap\n\n");
  else if (theSettings.mutType == ROTATE)
    printf("Mutation: Rotate\n\n");
  else if (theSettings.mutType == ROTATESWAP)
    printf("Mutation: Rotate & Swap\n\n");
  else if (theSettings.mutType == REGIONROTATE)
    printf("Mutation: Region Rotate\n\n");
  else if (theSettings.mutType == REGIONSWAP)
    printf("Mutation: Region Swap\n\n");

} // OutputSettings()


void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  for (int i = 0; i < popSize; i++)
  { // Create initialise population of boards with randomised boards
    Board newBoard;
    BoardManager::GetInstance()->InitFullBoard(&newBoard, startPiece);
    BoardManager::GetInstance()->currBoards->push_back(newBoard);
  }

  while (true)
  { // While the solution has not been found, continue working towards solution

    genCount++;            // Increment the count of generations
    maxFitnessOfGen = 0;   // Set maximum fitness of this generation

    for (int i = 0; i < popSize; i++)
    { // Loop through every boards of population checking the fitness

      if (theFitness.CheckFitness(&BoardManager::GetInstance()->currBoards->at(i))
                                  > maxFitnessOfGen)
      { // If next maximum fitness of generation found, store new max fitness
        // and check for max fitness this run of algorithm

        if (maxFitness < BoardManager::GetInstance()->currBoards->at(i).fitScore)
        { // Check to see if this fitness is the new maximum fitness of run if
          // so, store new fitness record
          maxFitness = BoardManager::GetInstance()->currBoards->at(i).fitScore;
        }

        // Set the new maximum fitness of generation
        maxFitnessOfGen = BoardManager::GetInstance()->currBoards->at(i).fitScore;
      }
    }

    // Output summary of generation
    std::cout << "Generation " << genCount << ": " << maxFitness << std::endl;

    // Complete crossover of population
    theCrossover.DoCrossover(popSize);

    // Complete mutation of population
    theMutation.DoMutation(startPiece);

  } // Main algorithm loop

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