// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h"  // Class declaration
#include "FileHandler.h"       // File input and output
#include <algorithm>           // Sorting
#include <iostream>            // Console output
#include <limits>              // Ignore all chars on input


void GeneticAlgorithm::setup(bool* isSuccess)           // *Out*
{ // Setup the algorithm with the elite, crossover and mutation rates, along 
  // with the size of the population for each generation. Also handles crossover 
  // and mutation methods

  FileHandler inputFile;                         // File handler to do input
  int boardSize = -1;                            // Holds input board size   
  int patternNum = -1;                           // Holds input pattern num
  int eliteRate = -1;                            // Holds input elitism rate
  double mutRate = -1;                           // Holds input mutation rate
  bool inputSuccess = true;                      // Holds if input was success
  SelectionType selectMethod = SELECTDEFAULT;    // Holds input input select
  CrossoverType crossMethod = CROSSDEFAULT;      // Holds input crossover method
  MutateType mutMethod = MUTDEFAULT;             // Holds input mutation method

  maxFitness = 0;                // Initialise maximum fitness of 100% candidate
  currFitness = 0;               // Init maximum fitness GA has reached
  genCount = 0;                  // Init generation count
  maxMatches = 0;                // Init maximum matches in candidate

  // Get the input from the settings file, storing in appropriate variables
  inputFile.readSettingsFile(&boardSize, &patternNum, &popSize, &selectMethod,
                             &crossMethod, &mutMethod, &mutRate, &eliteRate,
                             &isStartPiece, &inputSuccess);

  if (inputSuccess == true)
  { // Set up the crossover object using the input methods
    theCrossover.setMethod(crossMethod, selectMethod, eliteRate);

    // Set up the mutation method using the input methods
    theMutation.setup(mutMethod, mutRate, popSize);

    // Output all settings to user
    outputSettings(boardSize, patternNum, popSize, selectMethod,
                   crossMethod, mutMethod, mutRate, eliteRate);

    // Calculate the maximum fitness of a 100% solved candidate
    calcMaxFitness(boardSize);

    // Initialise the board manager with the board size and number of patterns
    BoardManager::getInstance()->initialiseData(boardSize, patternNum);
  }
  else
  { // Check if user wants to continue with application defaults

    CheckIfDefault(isSuccess);     // Check if user wants to continue 

    if (*isSuccess == true)
    { // If the user wants defaults, set up the rest of the data

    popSize = 1000;                // Set default population size
    isStartPiece = true;           // Set default start constraint to active
    boardSize = 16;                // Set default for piece count check

    // Ouput the settings using all the defaults
    outputSettings(16, 22, 1000, 1, 1, 2, 25, 2);

    // Calculate the maximum fitness of a 100% solved candidate
    calcMaxFitness(boardSize);
    }
  } // if (inputSuccess == true)

  if (((*BoardManager::getInstance()->getPieces())[0].size() +
      (*BoardManager::getInstance()->getPieces())[1].size() +
      (*BoardManager::getInstance()->getPieces())[2].size()) !=
      (boardSize * boardSize))
  { // Calculate how many pieces are in piece vector and how many it takes to
    // fill a board, if not correct, output error and quit
    OutputError();        // Output error
    *isSuccess = false;   // Set sucess to false to quit
  }

} // setup()


void GeneticAlgorithm::CheckIfDefault(bool* isContinue) // *Out*
{ // Output to the user that there was an issue with the settings file and if
  // the application should continue with default settings (best of from 
  // experiments)

  char answer = '\0';                     // Holds user answer to continue

  // Prompt user of corrupt settings
  std::cout << "Corrupt settings.ini, use with default values?" << std::endl;
  
  // Ask if defaults are to be used
  std::cout << "y/n" << std::endl;

  while (answer != 'y' && answer != 'n')
  { // While input is not valid, check again
    std::cin >> answer; // Store user input
  }

  if (answer == 'n')
  { // If answer is no, set isContinue to false to quit
    *isContinue = false;
  }

} // // CheckIfDefault()


void GeneticAlgorithm::OutputError()
{ // Outputs an error to user that the application can not recover from such
  // as a corrupted data file

  // Inform user of the error
  std::cout << "Data file does not contain enough pieces. Exiting" << std::endl;

  // Prompt user to press enter to quit
  std::cout << "Press enter to quit" << std::endl;

  // Read input, ignoring all chars apart from enter
  std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');

} // OutputError()


void GeneticAlgorithm::runGA()
{ // Main function of the GA that continually runs

  int sinceImprove = 200;    // Counts down from 200 for test if stuck
  int prevFitness = 0;       // Holds the previous fitness to check if stuck

  initRandomPopulation();    // Initialise the first population

  while (currFitness != maxFitness)
  { // While the solution has not been found, continue working towards solution

    if (prevFitness < currFitness)
    { // If fitness has improved, reset the stuck counter and set new high
      // score for fitness
      prevFitness = currFitness;
      sinceImprove = 200;
    }
    else
    { // If fitness has no improved, decrememnt to stuck counter
      sinceImprove--;
    }

    genCount++;            // Increment the count of generations
    currMatches = 0;       // Reset max amount of matches found
    currFitness = 0;       // Reset max fitness reached
             
    doFitness();           // Check fitness of the population

    // Calculate the percentage of fitness complete
    float fitPercent = ((float)currFitness / maxFitness) * 100.0f;

    // Calculate the percentage of matches complete
    float matchPercent = ((float)currMatches / maxMatches) * 100.0f;

    if (genCount % 100 == 0)
    { // Output the generation summary to console and file every 100 generations

      // Output to console
      printf("Generation %d: Fitness %d/%d %.2f%%, Match Count %d/%d %.2f%%\n",
             genCount, currFitness, maxFitness, fitPercent, currMatches,
             maxMatches, matchPercent);

      outputFitness();                      // Output to file
    }

    if (sinceImprove > 0)
    { // If fitness improvement has been made in past 200 generations, keep
      // trying to solve

      // Switch current population to previous and create new population
      BoardManager::getInstance()->switchPop();

      theCrossover.doCrossover(popSize);    // Complete crossover of population
      theMutation.doMutation(isStartPiece); // Complete mutation of population
    }
    else
    { // If 200 generations have passed without immproved fitness, reset 
      // population and fitness to try again
      initRandomPopulation();               // Get new population
      sinceImprove = 200;                   // Reset counter
      prevFitness = 0;                      // Reset previous fitness
      currFitness = 0;                      // Reset current fitness
    }

  } // while (currFitness != maxFitness)

  outputSolved();                         // Output the solved board

} // runGA()


void GeneticAlgorithm::genRandomNum(int min,            // *In*
                                    int max,            // *In*
                                    int* randNum)       // *Out*
{ // Generates a random number between min and max in randNum parameter

  // Work out range between min and max (+1 to include max afterwards)
  int range = (max - min) + 1;

  // Get random number between 0 and the range, then add min to get between min
  // and max
  *randNum = (std::rand() % range) + min;

} // genRandomNum()


void GeneticAlgorithm::calcMaxFitness(int boardSize)          // *In*
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

} // calcMaxFitness()


void GeneticAlgorithm::initRandomPopulation()
{ // Initialise random population of candidates. Used at the start of the 
  // algorithm to get the initial population and also used for the scramble
  // repair method if fitness has not increased within a period of generations

  // Create a new vector for new population
  std::vector<Board> newPop;

  // Make sure the population is empty in case of scramble repair
  BoardManager::getInstance()->getPop()->clear();

  // Set the current population pointer to new population vector
  BoardManager::getInstance()->getPop() =
    std::make_shared<std::vector<Board>>(newPop);

  for (int i = 0; i < popSize; i++)
  { // Create initialise population of boards with randomised boards
    Board newBoard;   // Create new board

    // Fill board with random pieces
    BoardManager::getInstance()->initFullBoard(&newBoard, isStartPiece);

    // Set board ID
    newBoard.boardID = (int)BoardManager::getInstance()->getPop()->size() + 1;

    // Push onto population
    BoardManager::getInstance()->getPop()->push_back(newBoard);
  }

} // initRandomPopulation()


void GeneticAlgorithm::doFitness()
{ // Checks the fitness of the population and checks to see if there is a new
  // fitness or pattern match record

  for (int i = 0; i < popSize; i++)
  { // Loop through every boards of population checking the fitness
    theFitness.checkFitness(&BoardManager::getInstance()->getPop()->at(i));

    if (BoardManager::getInstance()->getPop()->at(i).matchCount > currMatches)
    { // Check to see if new highest match count
      currMatches = BoardManager::getInstance()->getPop()->at(i).matchCount;
    }

    if (BoardManager::getInstance()->getPop()->at(i).fitScore > currFitness)
    { // If next maximum fitness of generation found, store new max fitness
      currFitness = BoardManager::getInstance()->getPop()->at(i).fitScore;
    }
  }

} // doFitness()


void GeneticAlgorithm::outputSettings(int boardSize,           // *In*
                                      int patternNum,          // *In*
                                      int popSize,             // *In*
                                      int selectMethod,        // *In*
                                      int crossMethod,         // *In*
                                      int mutMethod,           // *In*
                                      double mutRate,          // *In*
                                      int eliteRate)           // *In* 
{ // Outputs all of the loaded settings so the user can see what methods are
  // used for solving attempt

  // Print out the data on the board along with the population size, mutation
  // rate and elitism rate
  printf("Board Size: %i\nNumber of Patterns: %i\nPopulation Size: %i\n"
         "Mutation Rate: %.2f%%\nElitism Rate: %i\n", boardSize, patternNum, 
         popSize, mutRate, eliteRate);

  // Print out the enum value as a string for selection
  if (selectMethod == 0)
  { // If 0, print roulette 
    printf("Selection: Roulette\n");
  }
  else if (selectMethod == 1)
  { // If 1, print tournament 
    printf("Selection: Tournament\n");
  }

  // Print out the enum value as a string for crossover
  if (crossMethod == 0)
  { // If 0, print one point
    printf("Crossover: One-Point\n");
  }
  else if (crossMethod == 1)
  { // If 1, print two point
    printf("Crossover: Two-Point\n");
  }

  // Print out the enum value as a string for mutation
  if (mutMethod == 0)
  { // If 0, print swap
    printf("Mutation: Swap\n\n");
  }
  else if (mutMethod == 1)
  { // If 1, print rotate 
    printf("Mutation: Rotate\n\n");
  }
  else if (mutMethod == 2)
  { // If 2, print rotate & swap
    printf("Mutation: Rotate & Swap\n\n");
  }
  else if (mutMethod == 3)
  { // If 3, print region swap
    printf("Mutation: Region Swap\n\n");
  }
  else if (mutMethod == 4)
  { // If 4, print region rotate
    printf("Mutation: Region Rotate\n\n");
  }

} // outputSettings()


void GeneticAlgorithm::outputFitness()
{ // Calls to ouput the current fitness to the file for record of performance

  FileHandler fileHandle;                          // Create object for output
  fileHandle.outputFitness(genCount, currFitness); // Call to output the fitness

} // outputFitness()


void GeneticAlgorithm::outputSolved()
{ // Output the solved bored along with how many generation it took to solve

  FileHandler fileHandle;                     // Filehandler to output to file

  for (int i = 0; i < popSize; i++)
  { // Loop through the population, checking to see which candidate is solved

    if (BoardManager::getInstance()->getPop()->at(i).fitScore == maxFitness)
    { // If board fitness is max fitness, call to output the board
      fileHandle.outputBoard(&BoardManager::getInstance()->getPop()->at(i),
                          genCount);
      break;        // Break out of loop when board has been output
    }
  }

} // outputSolved()