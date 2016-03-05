// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include "FitnessFunction.h"        // Evalute fitness in GA process
#include "Crossover.h"              // Crossover of candidates
#include "Mutation.h"               // Mutation of candidates


class GeneticAlgorithm
{

private:
  int popSize;                          // Holds the population size
  int maxFitness;                       // Holds the maximum fitness reached
  int maxFitnessOfGen;                  // Holds the max fitness of current gen
  int genCount;                         // Count of how many generations passed
  bool startPiece;                      // Switch for start piece constraint
  Crossover theCrossover;               // Object to handle crossover
  Mutation theMutation;                 // Object to handle mutation
  FitnessFunction theFitness;           // Object ot handle calculating fitness

public:
  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation. Also handles crossover and
  // mutation methods
  void Setup(int eliteRate, double mutationRate, int popSize,
             int boardSize, int patNum, CrossoverType crossType,
             SelectionType selectType, MutateType mutType, bool startCons);

  // Main function of the GA that continually runs
  void RunGA();

  // Generates a random number between min and max
  static int GenRandomNum(int min, int max);
};

