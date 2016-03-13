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
  maxFitness = 0;                // Initialise maximum fitness of 100% candidate
  maxFitnessReach = 0;           // Init maximum fitness GA has reached
  genCount = 0;                  // Init generation count
  maxMatches = 0;                // Init maximum matches in candidate

  // Set if the start piece constraint is active
  startPiece = theSettings.startCons;

  // Output all settings to user
  OutputSettings(theSettings);

  // Calculate the maximum fitness of a 100% solved candidate
  CalcMaxFitness(theSettings.boardSize);

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


void GeneticAlgorithm::CalcMaxFitness(int boardSize)
{ // Takes in the size of board and calculates what the fitness of a 100%
  // solved candidate would be so the algorithm can quit when goal is achieved
  // along with calculating how many matches are in a 100% board 
  
  // Calc max fitness & max pattern matches of corner pattern matches
  maxFitness += 4 * 20;
  maxMatches += 8;

  // Calc max fitness & max pattern matches of edge pieces pattern match
  maxFitness += ((((boardSize - 2) * 2) - 1) * 4) * 5;
  maxMatches += (((boardSize - 2) * 2) - 1) * 4;

  // Calc max fitness & max pattern matches of inner pieces pattern match
  maxFitness += (boardSize - 3) * (boardSize - 2) * 2;
  maxMatches += (boardSize - 3) * (boardSize - 2) * 2;

} // CalcMaxFitness()


void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  for (int i = 0; i < popSize; i++)
  { // Create initialise population of boards with randomised boards
    Board newBoard;
    BoardManager::GetInstance()->InitFullBoard(&newBoard, startPiece);
    BoardManager::GetInstance()->currBoards->push_back(newBoard);
  }

  while (maxFitnessReach != maxFitness)
  { // While the solution has not been found, continue working towards solution

    genCount++;            // Increment the count of generations
    matchCount = 0;        // Reset max amount of matches found
    maxFitnessReach = 0;   // Reset max fitness reached

    // Check fitness of the population
    DoFitness();

    // Output summary of generation
    float fitPercent = ((float)maxFitnessReach / maxFitness) * 100.0f;
    float matchPercent = ((float)matchCount / maxMatches) * 100.0f;
    printf("Generation %d: Fitness %d/%d %.2f%%, Match Count %d/%d %.2f%%\n", 
            genCount, maxFitnessReach, maxFitness, fitPercent, matchCount, 
            maxMatches, matchPercent);

    // Complete crossover of population
    theCrossover.DoCrossover(popSize);

    // Complete mutation of population
    //theMutation.DoMutation(startPiece);

  } // Main algorithm loop

} // RunGA()


void GeneticAlgorithm::DoFitness()
{ // Checks the fitness of the population and checks to see if there is a new
  // fitness or pattern match record

  for (int i = 0; i < popSize; i++)
  { // Loop through every boards of population checking the fitness
    int matches = theFitness.CheckFitness(&BoardManager::GetInstance()->
      currBoards->at(i));

    // Check to see if new highest match count
    if (matches > matchCount)
      matchCount = matches;

    if (BoardManager::GetInstance()->currBoards->at(i).fitScore >
      maxFitnessReach)
    { // If next maximum fitness of generation found, store new max fitness
      maxFitnessReach = BoardManager::GetInstance()->currBoards->at(i).fitScore;
    }
  }

} // DoFitness()


int GeneticAlgorithm::GenRandomNum(int min, int max)
{ // Generates a random number between min and max

  // Work out range between min and max (+1 to include max afterwards)
  int range = (max - min) + 1;

  // Get random number between 0 and the range, then add min to get between min
  // and max
  return (std::rand() % range) + min;

} // GenerateRandomNum()