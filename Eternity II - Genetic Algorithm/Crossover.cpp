// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"
#include "BoardManager.h"       // Access to board vector
#include <iostream>             // Error output
#include <time.h>               // Time for srand


Crossover::Crossover()
{ // Private for singleton

} // Crossover()


Crossover* Crossover::GetInstance()
{ // Returns the current instance of the class, creates if needed

  if (!pInstance)
    pInstance = new Crossover();

  return pInstance;

} // GetInstance()

void Crossover::DoCrossover()
{ // Selects the parent candidates then commences with crossover with chosen
  // methods

  int parents[2] = { -1, -1 };

  SelectParents(parents);

} // DoCrossover()


void Crossover::SelectParents(int parents[2])
{ // Selects with candidates to use for reproduction with the selection method
  // chosen on start up

  if (selectType == ROULETTE) // If roulette method was chosen
    RouletteSelect(parents);
  else if (selectType == TOURNAMENT) // If tournament method was chosen
    TournamentSelect(parents);
  else // Output error, method not recognised
    std::cout << "Selection method not recognised" << std::endl;


} // SelectParents()


void Crossover::RouletteSelect(int parents[2])
{ // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3, section 3.5.1

  int totalFitness = 0;
  int count = 0;

  for (Board i : BoardManager::GetInstance()->boards)
  {
    totalFitness += i.GetFitScore();
  }

  srand(time(NULL));
  parents[0] = rand() % totalFitness;
  parents[1] = rand() % totalFitness;

  totalFitness = 0;

  for (int i = 0; i < BoardManager::GetInstance()->boards.size(); i++)
  {

    totalFitness += BoardManager::GetInstance()->boards[i].GetFitScore();

    if (totalFitness <= parents[0])
      parents[0] = i;
    if (totalFitness <= parents[1])
      parents[1] = i;

  }

} // RouletteSelect()


void Crossover::TournamentSelect(int parents[2])
{ // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3, section 3.5.3

} // TournamentSelect()


void Crossover::Reproduce(int parent1, int parent2)
{

} // Reproduce()