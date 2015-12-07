// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include "Board.h" // Include the board for acces to the board
#include "BoardManager.h"
#include "FitnessFunction.h"


class GeneticAlgorithm
{

protected:


public:
  double elitism;                       // Holds the elitism rate
  double crossover;                     // Holds the crossover rate
  double mutation;                      // Holds the mutation rate
  int popSize;                          // Holds the population size
  int boardSize;                        // Holds the board size
  static GeneticAlgorithm* pAlgorithm;  // Holds the current instance for class

  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation
  GeneticAlgorithm(double eliteRate, double crossRate, 
                   double mutationRate, int popSize, int boardSize);

  // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor
  static void CreateInstance(double eliteRate, double crossRate,
                             double mutationRate, int popSize, int boardSize);

  // Returns the current instance of the class
  static GeneticAlgorithm* GetInstance();

  // Main function of the GA that continually runs
  void RunGA();

  // Check the fitness of a candidate from the GA
  //int CheckFitness();

  // Destructor that deletes the instance of the class
  ~GeneticAlgorithm();

};

