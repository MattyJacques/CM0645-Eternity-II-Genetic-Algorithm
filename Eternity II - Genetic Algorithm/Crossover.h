// Title        : Crossover.h
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#pragma once


enum CrossoverType { ONEPOINT, TWOPOINT };
enum SelectionType { ROULETTE, TOURNAMENT };


class Crossover
{

private:
  static Crossover* pInstance;          // Holds current instance of class
  CrossoverType crossType;              // Holds what crossover method to use
  SelectionType selectType;             // Holds method of candidate selection

  // Private for singleton
  Crossover();

  // Selects with candidates to use for reproduction with the selection method
  // chosen on start up
  void SelectParents(int parents[2]);

  // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3, section 3.5.1
  void RouletteSelect(int parents[2]);

  // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3, section 3.5.3
  void TournamentSelect(int parents[2]);

  // WRITE COMMENT
  void Reproduce(int parent1, int parent2);

public:
  // Returns the current instance of the class, creates if needed
  Crossover* GetInstance();

  // Selects the parent candidates then commences with crossover with chosen
  // methods
  void DoCrossover();
};

