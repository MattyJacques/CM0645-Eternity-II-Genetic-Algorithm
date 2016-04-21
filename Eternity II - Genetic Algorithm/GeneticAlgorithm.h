// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once                        // Only include once


#include "FitnessFunction.h"        // Evalute fitness in GA process
#include "Crossover.h"              // Crossover of candidates
#include "Mutation.h"               // Mutation of candidates


class GeneticAlgorithm
{

private:
  int popSize;                          // Holds the population size
  int maxFitness;                       // Fitness of 100% candidate
  int currFitness;                      // Holds the max fitness of current gen
  int currMatches;                      // Max num pattern matches reached
  int maxMatches;                       // Max num of pattern matches at 100%
  int genCount;                         // Count of how many generations passed
  bool isStartPiece;                    // Switch for start piece constraint
  Crossover theCrossover;               // Object to handle crossover
  Mutation theMutation;                 // Object to handle mutation
  FitnessFunction theFitness;           // Object ot handle calculating fitness

  // Output to the user that there was an issue with the settings file and if
  // the application should continue with default settings (best of from 
  // experiments)
  void CheckIfDefault(bool* isContinue);       // *Out*

  // Outputs an error to user that the application can not recover from which is
  // a data file existing without enough pieces
  void OutputError();

  // Takes in the size of board and calculates what the fitness of a 100%
  // solved candidate would be so the algorithm can quit when goal is achieved
  // along with calculating how many matches are in a 100% board
  void calcMaxFitness(int boardSize);          // *In*

  // Initialise random population of candidates. Used at the start of the 
  // algorithm to get the initial population and also used for the scramble
  // repair method if fitness has not increased within a period of generations
  void initRandomPopulation();

  // Checks the fitness of the population and checks to see if there is a new
  // fitness or pattern match record
  void doFitness();

  // Outputs all of the loaded settings so the user can see what methods are
  // used for solving attempt
  void outputSettings(int boardSize,           // *In*
                      int patternNum,          // *In*
                      int popSize,             // *In*
                      int selectMethod,        // *In*
                      int crossMethod,         // *In*
                      int mutMethod,           // *In*
                      double mutRate,          // *In*
                      int eliteRate);          // *In*

  // Calls to ouput the current fitness to the file for record of performance
  void outputFitness();

  // Output the solved bored along with how many generation it took to solve
  void outputSolved();

public:
  // Setup the algorithm with the elite, crossover and mutation rates, along 
  // with the size of the population for each generation. Also handles crossover 
  // and mutation methods, isSuccess returns false to exit application
  void setup(bool* isSuccess);                 // *Out*

  // Main function of the GA that continually runs
  void runGA();

  // Generates a random number between min and max in randNum parameter
  static void genRandomNum(int min,            // *In*
                           int max,            // *In*
                           int* randNum);      // *Out*

}; // GeneticAlgorithm

