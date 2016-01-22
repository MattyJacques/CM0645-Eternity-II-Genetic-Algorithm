// Title        : GeneticAlgorithm.h
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#pragma once


#include "BoardManager.h"           // Board creation, access to board vector
#include "FitnessFunction.h"        // Evalute fitness in GA process


class Board;                        // Allows to create board objects


class GeneticAlgorithm
{

private:
  static GeneticAlgorithm* pAlgorithm;  // Holds the current instance for class
  double elitism;                       // Holds the elitism rate
  double crossover;                     // Holds the crossover rate
  double mutation;                      // Holds the mutation rate
  int popSize;                          // Holds the population size
  int boardSize;                        // Holds the board size

  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation
  GeneticAlgorithm(double eliteRate, double crossRate,
    double mutationRate, int popSize, int boardSize);

public:
  // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor
  static void CreateInstance(double eliteRate, double crossRate,
                             double mutationRate, int popSize, int boardSize);

  // Returns the current instance of the class
  static GeneticAlgorithm* GetInstance();

  // Returns the size of the population
  int GetPopSize();

  // Main function of the GA that continually runs
  void RunGA();

  // Destructor that deletes the instance of the class
  ~GeneticAlgorithm();

};

