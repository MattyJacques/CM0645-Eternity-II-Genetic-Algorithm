// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"
#include "BoardManager.h"       // Access to board vector
#include "GeneticAlgorithm.h"   // Population size
#include <iostream>             // Error output
#include <time.h>               // Time for srand
#include <vector>               // Create vector


// Initialise to nullptr
Crossover* Crossover::pInstance = nullptr;


Crossover::Crossover()
{ // Sets tournament size

  tournamentSize = 20;

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

  int parents[2] = { -1, -1 };    // Board ID of the parents

  // Loop to make sure both selected parents are not the same candidate
  while (parents[0] == parents[1])
    SelectParents(parents);

  std::cout << parents[0] << " " << parents[1] << std::endl;

} // DoCrossover()


void Crossover::SetMethod(CrossoverType cross, SelectionType select)
{ // Sets the crossover and selection type to use for crossover

  crossType = cross;
  selectType = select;

} // SetMethod()


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
  //srand((unsigned int)time(NULL));
  parents[0] = GenRandomNum(totalFitness);
  parents[1] = GenRandomNum(totalFitness);

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
  // report in chapter 3 section 3.5.3 Does not remove candidate from selection
  // after being selected so candidate can be in tournament multiple times.

  int highfitness;          // Holds the current highest fitness found
  int index;                // Holds random index to compare fitness

  for (int i = 0; i < 2; i++)
  { // Loop to find both parents

    // Initialise to -1 ready for next tournament
    highfitness = -1;
    index = -1;

    for (int j = 0; j < tournamentSize; j++)
    { // Test 20 random candidates storing highest fitness and ID

      // Generate a random index then test to see if the fitness score of that
      // candidate is highest than the current stored fitness
      index = GenRandomNum(GeneticAlgorithm::GetInstance()->GetPopSize() - 1);
      if (BoardManager::GetInstance()->boards[index].GetFitScore() > highfitness)
      {
        parents[i] = index;
        highfitness = BoardManager::GetInstance()->boards[index].GetFitScore();
      }

    } // for j < tournamentSize

  } // for i < 2

} // TournamentSelect()


void Crossover::Reproduce(int parent1, int parent2)
{

} // Reproduce()


void Crossover::OnePoint(int parent1, int parent2)
{ // Takes two candidates, selects a point of the candidate to slice and exchanges
  // the data after that point with the second parent, explained fully in the
  // report, chapter 3, section 3.4.1

  std::vector<PuzzlePiece> offspring1;
  std::vector<PuzzlePiece> offspring2;

  // Maybe better to actually use parents, but what about elitism

  for (int i = 0; i < BoardManager::GetInstance()->pieceVec.size(); i++)
  {
    if (i <= BoardManager::GetInstance()->pieceVec.size() / 2)
    {

    }
  }


} // OnePoint()


int Crossover::GenRandomNum(int max)
{ // Generates a random number between 0 and max

  int randomInt = 0;
  max += 1;

  randomInt = (int)(max * rand() / (RAND_MAX + 1.0));

  return randomInt;

} // GenerateRandomNum()
