// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include "BoardManager.h"           // Board creation, access to board vector
#include "FitnessFunction.h"        // Evalute fitness in GA process
#include "Crossover.h"
#include "Mutation.h"


class GeneticAlgorithm
{

private:
  static GeneticAlgorithm* pInstance;   // Holds the current instance for class
  double elitism;                       // Holds the elitism rate
  double mutation;                      // Holds the mutation rate
  int popSize;                          // Holds the population size
  int boardSize;                        // Holds the board size
  Crossover theCrossover;               // Object to handle crossover
  Mutation theMutation;                 // Object to handle mutation
  FitnessFunction theFitness;           // Object ot handle calculating fitness


  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation
  GeneticAlgorithm(double eliteRate, double mutationRate, int popSize, 
                   int boardSize);

public:
  // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor
  static void CreateInstance(double eliteRate, double mutationRate, int popSize,
                             int boardSize);

  // Returns the current instance of the class
  static GeneticAlgorithm* GetInstance();

  // Main function of the GA that continually runs
  void RunGA();

  // Generates a random number between min and max
  int GenRandomNum(int min, int max);

  // Destructor that deletes the instance of the class
  ~GeneticAlgorithm();

};

