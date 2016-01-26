// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"
#include "BoardManager.h"       // Access to board vector
#include <iostream>             // Error output
#include <time.h>               // Time for srand


// Initialise to nullptr
Crossover* Crossover::pInstance = nullptr;


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

  int totalFitness = 0;   // Holds the combined fitness of all boards
  int oldFitness = 0;     // Holds the older combined fitness score

  for (Board i : BoardManager::GetInstance()->boards)
  { // Loops through all boards and total up all fitness scores from boards

    totalFitness += i.GetFitScore();
  }

  // Seeds time and gets two random numbers to use to pick from wheel
  srand((unsigned int)time(NULL));
  parents[0] = rand() % totalFitness;
  parents[1] = rand() % totalFitness;

  totalFitness = 0;   // Set to 0 to accumulate total fitness again

  for (unsigned int i = 0; i < BoardManager::GetInstance()->boards.size(); i++)
  { // Loops through all boards accumulating the fitness scores, if random number
    // is between total fitness and the previous total fitness, set the ID of
    // the parent

    // Set the previous fitness and add on the fitness score of the next board
    oldFitness = totalFitness;
    totalFitness += BoardManager::GetInstance()->boards[i].GetFitScore();

    // If section has been found for first parent, set ID
    if (totalFitness >= parents[0] && parents[0] >= oldFitness)
      parents[0] = i;

    // If section has been found for first parent, set ID
    if (totalFitness >= parents[1] && parents[1] >= oldFitness)
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