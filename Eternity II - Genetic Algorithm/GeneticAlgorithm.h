// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include "FitnessFunction.h"        // Evalute fitness in GA process
#include "Crossover.h"              // Crossover of candidates
#include "Mutation.h"               // Mutation of candidates


struct Settings
{ // Holds data loaded from the settings file to be used to set up the
  // algorithm.

  int boardSize;               // Size of the board
  int patternNum;              // Number of patterns in board
  int popSize;                 // How many candidates in generation
  int eliteRate;               // How many candidates to save per generation
  SelectionType selectType;    // Which selection method to use
  CrossoverType crossType;     // Which crossover method to use
  MutateType mutType;          // Which mutation method to use
  double mutRate;              // Rate of mutation to apply
  bool startCons;              // Whether the start constraint is active
};


class GeneticAlgorithm
{

private:
  int popSize;                          // Holds the population size
  int maxFitness;                       // Fitness of 100% candidate
  int maxFitnessReach;                  // Holds the max fitness of current gen
  int genCount;                         // Count of how many generations passed
  bool startPiece;                      // Switch for start piece constraint
  Crossover theCrossover;               // Object to handle crossover
  Mutation theMutation;                 // Object to handle mutation
  FitnessFunction theFitness;           // Object ot handle calculating fitness

  // Outputs all of the loaded settings so the user can see what methods are
  // used for solving attempt
  void OutputSettings(Settings theSettings);

  // Takes in the size of board and calculates what the fitness of a 100%
  // solved candidate would be so the algorithm can quit when goal is achieved
  void CalcMaxFitness(int boardSize);

public:
  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation. Also handles crossover and
  // mutation methods
  void Setup(Settings theSettings);

  // Main function of the GA that continually runs
  void RunGA();

  // Generates a random number between min and max
  static int GenRandomNum(int min, int max);
};

