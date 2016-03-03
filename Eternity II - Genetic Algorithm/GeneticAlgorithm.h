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
  static GeneticAlgorithm* pInstance;   // Holds the current instance for class
  int popSize;                          // Holds the population size
  int maxFitness;                       // Holds the maximum fitness reached
  int maxFitnessOfGen;                  // Holds the max fitness of current gen
  int genCount;                         // Count of how many generations passed
  Crossover theCrossover;               // Object to handle crossover
  Mutation theMutation;                 // Object to handle mutation
  FitnessFunction theFitness;           // Object ot handle calculating fitness


  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation. Also handles crossover and
  // mutation methods
  GeneticAlgorithm(int eliteRate, double mutationRate, int popSize, 
                   int boardSize, int patNum, CrossoverType crossType,
                   SelectionType selectType, MutateType mutType);

public:
  // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor
  static void CreateInstance(int eliteRate, double mutationRate, int popSize,
                             int boardSize, int patNum, CrossoverType crossType,
                             SelectionType selectType, MutateType mutType);

  // Returns the current instance of the class
  static GeneticAlgorithm* GetInstance();

  // Main function of the GA that continually runs
  void RunGA();

  // Generates a random number between min and max
  int GenRandomNum(int min, int max);

  // Destructor that deletes the instance of the class
  ~GeneticAlgorithm();

};

