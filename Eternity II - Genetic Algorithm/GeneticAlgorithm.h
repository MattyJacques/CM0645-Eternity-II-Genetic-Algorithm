#pragma once

#include "Board.h"

class GeneticAlgorithm
{

protected:


public:
  double elitism;   // Holds the elitism rate
  double crossover; // Holds the crossover rate
  double mutation;  // Holds the mutation rate
  int popSize;      // Holds the population size
  static GeneticAlgorithm* pAlgorithm;  // Holds the current instance for class
  Board* pTheBoard;                     // Holds the instance of the board

  // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation
  GeneticAlgorithm(double eliteRate, double crossRate, 
                   double mutationRate, int popSize);

  // Destructor that deletes the instance of the class
  ~GeneticAlgorithm();

  // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor
  static void CreateInstance(double eliteRate, double crossRate,
                             double mutationRate, int popSize);

  // Returns the current instance of the class
  static GeneticAlgorithm* GetInstance();

  // Main function of the GA that continually runs
  void RunGA();

};

