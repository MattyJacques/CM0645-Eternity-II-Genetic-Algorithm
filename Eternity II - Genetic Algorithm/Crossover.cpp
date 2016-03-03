// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"
#include "GeneticAlgorithm.h"   // Random number generation
#include <iostream>             // Error output
#include <algorithm>            // Sorting functionality


Crossover::Crossover()
{ // Sets tournament size, initalise methods to default

  tournamentSize = 20;
  crossType = ONEPOINT;
  selectType = ROULETTE;

} // Crossover()


void Crossover::DoCrossover(int popSize)
{ // Selects the parent candidates then commences with crossover with chosen
  // methods

  std::vector<Board> newVec;
  BoardManager::GetInstance()->prevBoards = BoardManager::GetInstance()->currBoards;
  BoardManager::GetInstance()->currBoards = std::make_shared<std::vector<Board>>(newVec);

  DoElitism();

  while (BoardManager::GetInstance()->currBoards->size() < popSize - 1)
  { // While the new vector is not filled with the right population size
    // make more candidates

    int parents[2] = { -1, -1 };    // Board ID of the parents

    // Loop to make sure both selected parents are not the same candidate
    while (parents[0] == parents[1])
      SelectParents(parents, popSize);

    Reproduce(parents);
    CheckDuplication();

  }

} // DoCrossover()


void Crossover::DoElitism()
{ // Adds the best and worst candidtes from the previous generation to the new
  // generation. The amount of candidates is declared in eliteRate

  // Sort the vector in ascending order to for easy access to elites
  std::sort(BoardManager::GetInstance()->prevBoards->begin(), 
            BoardManager::GetInstance()->prevBoards->end());

  for (int i = 1; i < eliteRate + 1; i++)
  { // Take the best and the worst candidates from the previous generation
    // and push to the new generation vector

    // Push worst on to new population
    BoardManager::GetInstance()->currBoards->push_back(BoardManager::GetInstance()
                                                        ->prevBoards->begin()[i]);

    // Push best on to new population
    BoardManager::GetInstance()->currBoards->push_back(BoardManager::GetInstance()
                                                        ->prevBoards->end()[-i]);
  }

} // DoElitism()


void Crossover::SetMethod(CrossoverType cross, SelectionType select, int elite)
{ // Sets the crossover and selection type to use for crossover along with
  // the elitism rate

  crossType = cross;
  selectType = select;
  eliteRate = elite;

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

    totalFitness += i.fitScore;
  }

  // Seeds time and gets two random numbers to use to pick from wheel
  //srand((unsigned int)time(NULL));
  parents[0] = GeneticAlgorithm::GetInstance()->GenRandomNum(0, totalFitness);
  parents[1] = GeneticAlgorithm::GetInstance()->GenRandomNum(0, totalFitness);

  totalFitness = 0;   // Set to 0 to accumulate total fitness again

  for (unsigned int i = 0; i < BoardManager::GetInstance()->currBoards->size(); i++)
  { // Loops through all boards accumulating the fitness scores, if random number
    // is between total fitness and the previous total fitness, set the ID of
    // the parent

    // Set the previous fitness and add on the fitness score of the next board
    oldFitness = totalFitness;
    totalFitness += BoardManager::GetInstance()->currBoards->at(i).fitScore;

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
      index = GeneticAlgorithm::GetInstance()->GenRandomNum(0, popSize - 1);
      if ((BoardManager::GetInstance()->prevBoards->at(index).fitScore) > highfitness)
      {
        parents[i] = index;
        highfitness = BoardManager::GetInstance()->prevBoards->at(index).fitScore;
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
  {
    BoardManager::GetInstance()->InitEmptyBoard(&offspring[i]);
    offspring[i].boardID = (int)BoardManager::GetInstance()->currBoards->
                                size() + (i + 1);
  }

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
    BoardManager::GetInstance()->currBoards->push_back(offspring[i]);
  }

} // OnePoint()


void Crossover::CheckDuplication()
{ // Scans through candidate to check if puzzle pieces end up appearing twice
  // within the same candidate, taking the duplicate list from one candidate
  // to place pieces within the candidate that no longer has them

  Board* pOffspring[2];
  std::vector<PuzzlePiece> pieces[2];
  std::vector<std::vector<int>> indexes[2];

  pOffspring[0] = &BoardManager::GetInstance()->currBoards->at(
                    BoardManager::GetInstance()->currBoards->size() - 2);
  pOffspring[1] = &BoardManager::GetInstance()->currBoards->at(
                    BoardManager::GetInstance()->currBoards->size() - 1);

  for (int i = 0; i < 2; i++)
    GetDuplicates(pOffspring[i], &pieces[i], &indexes[i]);

  for (int i = 0; i < 2; i++)
    FixDuplicates(pOffspring[i], pieces[1 - i], indexes[i]);

  pieces[0].clear();
  indexes[0].clear();
  pieces[1].clear();
  indexes[1].clear();

  for (int i = 0; i < 2; i++)
    GetDuplicates(pOffspring[i], &pieces[i], &indexes[i]);

} // CheckDuplication()


void Crossover::GetDuplicates(Board* pBoard, std::vector<PuzzlePiece>* pieces,
                              std::vector<std::vector<int>>* indexes)
{ // Scans through the candidate board to see if there are any pieces that appear
  // more than once within the candidate, uses vector to store puzzle pieces

  std::vector<bool> checkIDs(pBoard->boardVec.size() * pBoard->boardVec.size(), 
                             false);
  int xIndex = 0;
  int yIndex = 0;

  for (int i = 0; i < checkIDs.size(); i++)
  {
    if (!checkIDs[pBoard->boardVec[yIndex][xIndex].pieceID - 1])
    {
      checkIDs[pBoard->boardVec[yIndex][xIndex].pieceID - 1] = true;
    }
    else
    {
      pieces->push_back(pBoard->boardVec[yIndex][xIndex]);
      std::vector<int> index = { yIndex, xIndex };
      indexes->push_back(index);
    }

    xIndex++;

    if (xIndex == BoardManager::GetInstance()->boardSize + 1)
    { // If we have reached the end of the line of the board, increment
      xIndex = 0;
      yIndex++;
    }
  }

} // GetDuplicates()


void Crossover::FixDuplicates(Board* pBoard, std::vector<PuzzlePiece> pieces, 
                              std::vector<std::vector<int>> indexes)
{ // Uses the vector of pieces along with the vector of slot indexes to place
  // pieces that are needed to to have every unique puzzle piece with in the
  // candidate

  for (int i = 0; i < pieces.size(); i++)
  {
    pBoard->boardVec[indexes[i][0]][indexes[i][1]] = pieces[i];
  }

} // FixDuplicates()