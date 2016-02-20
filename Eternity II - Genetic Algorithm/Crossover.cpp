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


void Crossover::DoCrossover(int popSize)
{ // Selects the parent candidates then commences with crossover with chosen
  // methods

  std::vector<Board> newVec;
  BoardManager::GetInstance()->prevBoards = BoardManager::GetInstance()->currBoards;
  BoardManager::GetInstance()->currBoards = std::make_shared<std::vector<Board>>(newVec);

  while (BoardManager::GetInstance()->currBoards->size() < popSize)
  { // While the new vector is not filled with the right population size
    // make more candidates

    int parents[2] = { -1, -1 };    // Board ID of the parents

    // Loop to make sure both selected parents are not the same candidate
    while (parents[0] == parents[1])
      SelectParents(parents, popSize);

    std::cout << parents[0] << " " << parents[1] << std::endl;

    Reproduce(parents);

  }

} // DoCrossover()


void Crossover::SetMethod(CrossoverType cross, SelectionType select)
{ // Sets the crossover and selection type to use for crossover

  crossType = cross;
  selectType = select;

} // SetMethod()


void Crossover::SelectParents(int parents[2], int popSize)
{ // Selects with candidates to use for reproduction with the selection method  
  // chosen on start up

  if (selectType == ROULETTE) // If roulette method was chosen
    RouletteSelect(parents);
  else if (selectType == TOURNAMENT) // If tournament method was chosen
    TournamentSelect(parents, popSize);
  else // Output error, method not recognised
    std::cout << "Selection method not recognised" << std::endl;


} // SelectParents()


void Crossover::RouletteSelect(int parents[2])
{ // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3

  int totalFitness = 0;   // Holds the combined fitness of all boards
  int oldFitness = 0;     // Holds the older combined fitness score

  for (Board i : *BoardManager::GetInstance()->currBoards)
  { // Loops through all boards and total up all fitness scores from boards

    totalFitness += i.GetFitScore();
  }

  // Seeds time and gets two random numbers to use to pick from wheel
  //srand((unsigned int)time(NULL));
  parents[0] = GenRandomNum(totalFitness);
  parents[1] = GenRandomNum(totalFitness);

  totalFitness = 0;   // Set to 0 to accumulate total fitness again

  for (unsigned int i = 0; i < BoardManager::GetInstance()->currBoards->size(); i++)
  { // Loops through all boards accumulating the fitness scores, if random number
    // is between total fitness and the previous total fitness, set the ID of
    // the parent

    // Set the previous fitness and add on the fitness score of the next board
    oldFitness = totalFitness;
    totalFitness += BoardManager::GetInstance()->currBoards->at(i).GetFitScore();

    // If section has been found for first parent, set ID
    if (totalFitness >= parents[0] && parents[0] >= oldFitness)
      parents[0] = i;

    // If section has been found for first parent, set ID
    if (totalFitness >= parents[1] && parents[1] >= oldFitness)
      parents[1] = i;

  }

} // RouletteSelect()


void Crossover::TournamentSelect(int parents[2], int popSize)
{ // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3. Does not remove candidate from selection
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
      index = GenRandomNum(popSize - 1);
      if ((BoardManager::GetInstance()->prevBoards->at(index).GetFitScore()) > highfitness)
      {
        parents[i] = index;
        highfitness = BoardManager::GetInstance()->prevBoards->at(index).GetFitScore();
      }

    } // for j < tournamentSize

  } // for i < 2

} // TournamentSelect()

void Crossover::Reproduce(int parents[2])
{ // Calls whichever crossover method that has been selected during the start of
  // the application

  if (crossType == ONEPOINT)
    OnePoint(parents);
  else if (crossType == TWOPOINT)
    //TwoPoint(parents);
    int i = 0;
  else
    std::cout << "Crossover method not recognised" << std::endl;

} // Reproduce()


void Crossover::OnePoint(int parents[2])
{ // Takes two candidates, selects a point of the candidate to slice and exchanges
  // the data after that point with the second parent, explained fully in the
  // report, chapter 3

  Board offspring[2];
  int xIndex = 0;
  int yIndex = 0;

  for (int i = 0; i < 2; i++)
    BoardManager::GetInstance()->InitEmptyBoard(&offspring[i]);

  for (int i = 0; i < BoardManager::GetInstance()->pieceVec.size(); i++)
  {
    if (i <= BoardManager::GetInstance()->pieceVec.size() / 2)
    { // If we are below 50% of pieces in board

      offspring[0].boardVec[yIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[0]).boardVec[yIndex][xIndex]);

      offspring[1].boardVec[yIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[1]).boardVec[yIndex][xIndex]);

    }
    else if (i > BoardManager::GetInstance()->pieceVec.size() / 2)
    { // If we are greater than 50% pieces in board

      offspring[0].boardVec[yIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[1]).boardVec[yIndex][xIndex]);

      offspring[1].boardVec[yIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[0]).boardVec[yIndex][xIndex]);

    }

    xIndex++;

    if (xIndex == BoardManager::GetInstance()->boardSize + 1)
    { // If we have reached the end of the line of the board, increment
      xIndex = 0;
      yIndex++;
    }

  } // for i < pieceVec.size()

  
  for (int i = 0; i < 2; i++)
  {
    offspring[i].SetBoardID((int)BoardManager::GetInstance()->currBoards->size() + 1);
    BoardManager::GetInstance()->currBoards->push_back(offspring[i]);
  }

} // OnePoint()


int Crossover::GenRandomNum(int max)
{ // Generates a random number between 0 and max

  int randomInt = 0;
  max += 1;

  randomInt = (int)(max * rand() / (RAND_MAX + 1.0));

  return randomInt;

} // GenerateRandomNum()
