// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h" // Include header for the class
#include "FileReader.h"
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
  currentFitness = 0;           // Init maximum fitness GA has reached
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
  maxFitness += 8 * CORNERMATCH;
  maxMatches += 8;

  // Calc max fitness & max pattern matches of edge pieces pattern match
  maxFitness += ((((boardSize - 2) * 2) - 1) * 4) * EDGEMATCH;
  maxMatches += (((boardSize - 2) * 2) - 1) * 4;

  // Calc max fitness & max pattern matches of inner pieces pattern match
  maxFitness += ((boardSize - 3) * (boardSize - 2) * 2) * INNERMATCH;
  maxMatches += (boardSize - 3) * (boardSize - 2) * 2;

} // CalcMaxFitness()


void GeneticAlgorithm::InitRandomPopulation()
{ // Initialise random population of candidates. Used at the start of the 
  // algorithm to get the initial population and also used for the scramble
  // repair method if fitness has not increased within a period of generations

  // Create a new vector for new population
  std::vector<Board> newVec;

  // Set the current population pointer to new population vector
  BoardManager::GetInstance()->currBoards = 
                                  std::make_shared<std::vector<Board>>(newVec);

  for (int i = 0; i < popSize; i++)
  { // Create initialise population of boards with randomised boards
    Board newBoard;
    BoardManager::GetInstance()->InitFullBoard(&newBoard, startPiece);
    BoardManager::GetInstance()->currBoards->push_back(newBoard);
  }

} // InitRandomPopulation()


void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  int stuckCounter = 200;    // Counts down from 200 for test if stuck
  int prevFitness = 0;       // Holds the previous fitness to check if stuck

  // Initialise the first population
  InitRandomPopulation();

  while (currentFitness != maxFitness)
  { // While the solution has not been found, continue working towards solution

    if (prevFitness < currentFitness)
    { // If fitness has improved, reset the stuck counter and set new high
      // score for fitness
      prevFitness = currentFitness;
      stuckCounter = 200;
    }
    else
    { // If fitness has no improved, decrememnt to stuck counter
      stuckCounter--;
    }

    genCount++;            // Increment the count of generations
    matchCount = 0;        // Reset max amount of matches found
    currentFitness = 0;    // Reset max fitness reached

    // Check fitness of the population
    DoFitness();

    // Output summary of generation
    float fitPercent = ((float)currentFitness / maxFitness) * 100.0f;
    float matchPercent = ((float)matchCount / maxMatches) * 100.0f;
    printf("Generation %d: Fitness %d/%d %.2f%%, Match Count %d/%d %.2f%%\n", 
            genCount, currentFitness, maxFitness, fitPercent, matchCount, 
            maxMatches, matchPercent);

    if (stuckCounter > 0)
    { // If fitness improvement has been made in past 200 generations, keep
      // trying to solve

      // Complete crossover of population
      theCrossover.DoCrossover(popSize);

      // Complete mutation of population
      theMutation.DoMutation(startPiece);
    }
    else
    { // If 200 generations have passed without immproved fitness, reset 
      // population and try again
      InitRandomPopulation();
      stuckCounter = 200;
    }

    OutputFitness();

  } // Main algorithm loop

  OutputSolved(); // Output the solved board

} // RunGA()


void GeneticAlgorithm::OutputFitness()
{ // Calls to ouput the current fitness to the file for record of performance

  FileReader fileMan;                       // Create object for output
  fileMan.OutputFitness(currentFitness);    // Call to output the fitness

} // OutputFitness()


void GeneticAlgorithm::OutputSolved()
{ // Output the solved bored along with how many generation it took to solve

  for (int i = 0; i < popSize; i++)
  { // Loop through the population, checking to see which candidate is solved

    if (BoardManager::GetInstance()->currBoards->at(i).fitScore == maxFitness)
    { // If board fitness is max fitness, call to output the board
      FileReader fileMan;
      fileMan.OutputBoard(&BoardManager::GetInstance()->currBoards->at(i), 
                          genCount);
    }
  }

} // OutputSolved


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
        currentFitness)
    { // If next maximum fitness of generation found, store new max fitness
      currentFitness = BoardManager::GetInstance()->currBoards->at(i).fitScore;
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