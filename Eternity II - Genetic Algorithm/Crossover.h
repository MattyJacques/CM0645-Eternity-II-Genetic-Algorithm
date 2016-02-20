// Title        : Crossover.h
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#pragma once


// Readability for methods of crossover
enum CrossoverType { ONEPOINT, TWOPOINT };

// Readability for methods of selection
enum SelectionType { ROULETTE, TOURNAMENT };


class Crossover
{

private:
  static Crossover* pInstance;          // Holds current instance of class
  CrossoverType crossType;              // Holds what crossover method to use
  SelectionType selectType;             // Holds method of candidate selection
  int tournamentSize;                   // Holds how large tournaments are

  // Sets tournament size
  Crossover();

  // Selects with candidates to use for reproduction with the selection method
  // chosen on start up
  void SelectParents(int parents[2], int popSize);

  // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3
  void RouletteSelect(int parents[2]);

  // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3. Does not remove candidate from selection
  // after being selected so candidate can be in tournament multiple times.
  void TournamentSelect(int parents[2], int popSize);

  // Calls whichever crossover method that has been selected during the start of
  // the application
  void Reproduce(int parents[2]);

  // Takes two candidates, selects a point of the candidate to slice and exchanges
  // the data after that point with the second parent, explained fully in the
  // report, chapter 3
  void OnePoint(int parents[2]);

  // Generates a random number between 0 and max
  int GenRandomNum(int max);

public:
  // Returns the current instance of the class, creates if needed
  static Crossover* GetInstance();

  // Selects the parent candidates then commences with crossover with chosen
  // methods
  void DoCrossover(int popSize);

  // Sets the crossover and selection type to use for crossover
  void SetMethod(CrossoverType cross, SelectionType select);
};

