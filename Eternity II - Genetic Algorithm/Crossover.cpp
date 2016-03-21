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

  // Switch the current generation to the previous generation and create new
  // vector for current generation
  std::vector<Board> newVec;
  BoardManager::GetInstance()->prevBoards = BoardManager::GetInstance()->currBoards;
  BoardManager::GetInstance()->currBoards = std::make_shared<std::vector<Board>>(newVec);

  DoElitism(); // Transfer the elites over to new generation

  while (BoardManager::GetInstance()->currBoards->size() < popSize - 1)
  { // While the new vector is not filled with the right population size
    // make more candidates

    int parents[2] = { -1, -1 };    // Board ID of the parents

    // Loop to make sure both selected parents are not the same candidate
    while (parents[0] == parents[1])
      SelectParents(parents, popSize);

    Reproduce(parents);  // Breed the parents together
    CheckDuplication();  // Check for any duplicate pieces
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
  // chosen when the application was started

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
  parents[0] = GeneticAlgorithm::GenRandomNum(0, totalFitness);
  parents[1] = GeneticAlgorithm::GenRandomNum(0, totalFitness);

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
      index = GeneticAlgorithm::GenRandomNum(0, popSize - 1);
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

  Board offspring[2];      // Holds the two new offspring boards
  int xIndex = 0;          // xIndex of the current piece to copy over
  int yIndex = 0;          // yIndex of the current piece to copy over

  // Work out number of pieces to avoid calculations for each check below.
  // + 1 to include the 0 index
  int numOfPieces = (BoardManager::GetInstance()->boardSize + 1) *
                    (BoardManager::GetInstance()->boardSize + 1);

  // Get random crossover point to split the boards
  int crossPoint = GeneticAlgorithm::GenRandomNum(1, numOfPieces);

  for (int i = 0; i < 2; i++)
  { // Initialise new empty boards and give the boards an ID

    BoardManager::GetInstance()->InitEmptyBoard(&offspring[i]);
    offspring[i].boardID = (int)BoardManager::GetInstance()->currBoards->
                                size() + (i + 1);
  }

  for (int i = 0; i < numOfPieces; i++)
  { // Loop for all pieces to copy over in to new board

    if (i <= crossPoint)
    { // If we are below the crossover point

      // Add the piece from parent 1 to offspring 1
      offspring[0].boardVec[xIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[0]).boardVec[xIndex][yIndex]);

      // Add the piece from parent 2 to offspring 2
      offspring[1].boardVec[xIndex].push_back(BoardManager::GetInstance()->
        prevBoards->at(parents[1]).boardVec[xIndex][yIndex]);
    }
    else if (i > crossPoint)
    { // If we are greater than crossover point

      // Add the piece from parent 1 to offspring 2
      offspring[0].boardVec[xIndex].push_back(BoardManager::GetInstance()->
                          prevBoards->at(parents[1]).boardVec[xIndex][yIndex]);

      // Add the piece from parent 2 to offspring 1
      offspring[1].boardVec[xIndex].push_back(BoardManager::GetInstance()->
                          prevBoards->at(parents[0]).boardVec[xIndex][yIndex]);
    }

    xIndex++;  // Increment the xIndex to move to next slot in row

    if (xIndex == BoardManager::GetInstance()->boardSize + 1)
    { // If we have reached the end of the line of the board, increment row

      xIndex = 0;
      yIndex++;
    }

  } // for i < pieceVec.size()
  
  for (int i = 0; i < 2; i++)
  { // Push the new offspring on to the new population

    BoardManager::GetInstance()->currBoards->push_back(offspring[i]);
  }

} // OnePoint()


void Crossover::CheckDuplication()
{ // Scans through candidate to check if puzzle pieces end up appearing twice
  // within the same candidate, taking the duplicate list from one candidate
  // to place pieces within the candidate that no longer has them

  // Holds location of both offspring
  Board* pOffspring[2];   

  // Holds list of duplicate pieces found
  std::vector<PuzzlePiece> pieces[2];  

  // Holds the indexes of duplicate pieces
  std::vector<std::vector<int>> indexes[2];

  // Get the addresses of the last two boards created
  pOffspring[0] = &BoardManager::GetInstance()->currBoards->at(
                    BoardManager::GetInstance()->currBoards->size() - 2);
  pOffspring[1] = &BoardManager::GetInstance()->currBoards->at(
                    BoardManager::GetInstance()->currBoards->size() - 1);

  // Call to find out which pieces are duplicates, storing in the pieces
  // and index vectors ready for fixing
  for (int i = 0; i < 2; i++)
    GetDuplicates(pOffspring[i], &pieces[i], &indexes[i]);

  // Calls to repair the boards after duplication was found, giving the pieces
  // and indexes of the duplicate slots, pieces vector switched to add pieces
  // that were found twice in other offspring
  for (int i = 0; i < 2; i++)
    FixDuplicates(pOffspring[i], pieces[1 - i], indexes[i]);

} // CheckDuplication()


void Crossover::GetDuplicates(Board* pBoard, 
                                std::vector<PuzzlePiece>* pieces,
                                std::vector<std::vector<int>>* indexes)
{
  // Store the boardSize for easier code readability and quicker access
  int boardSize = BoardManager::GetInstance()->boardSize;

  // Vector to check IDs against
  std::vector<bool> checkIDs(BoardManager::GetInstance()->pieceVec[CORNER].size() +
                             BoardManager::GetInstance()->pieceVec[EDGE].size() +
                             BoardManager::GetInstance()->pieceVec[INNER].size(), 
                             false);

  // Check the corner slots for duplicates
  CheckCorners(pBoard, pieces, indexes, boardSize, &checkIDs);

  // Check the edge slots for duplicates
  CheckEdges(pBoard, pieces, indexes, boardSize, &checkIDs);

  // Check the inner slots for duplicates
  CheckInners(pBoard, pieces, indexes, boardSize, &checkIDs);

} // GetDuplicates()


void Crossover::CheckCorners(Board* pBoard, std::vector<PuzzlePiece>* pieces, 
                             std::vector<std::vector<int>>* indexes, 
                             int boardSize, 
                             std::vector<bool>* checkIDs)
{ // Checks all corners to see if there are any duplicate pieces within the
  // corner slots, if so store the piece and the index of the duplicate slot

  // Set the ID of the piece in the top left corner to true, can't be duplicate
  // as is the first corner to be checked
  checkIDs->at(pBoard->boardVec[0][0].pieceID - 1) = true;

  // Check the top right corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVec[0][boardSize].pieceID - 1))
  { 
    checkIDs->at(pBoard->boardVec[0][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVec[0][boardSize]);
    std::vector<int> index = { 0, boardSize };
    indexes->push_back(index);
  }

  // Check the bottom left corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVec[boardSize][0].pieceID - 1))
  {
    checkIDs->at(pBoard->boardVec[boardSize][0].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVec[boardSize][0]);
    std::vector<int> index = { boardSize, 0 };
    indexes->push_back(index);
  }

  // Check the bottom right corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVec[boardSize][boardSize].pieceID - 1))
  {
    checkIDs->at(pBoard->boardVec[boardSize][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVec[boardSize][boardSize]);
    std::vector<int> index = { boardSize, boardSize };
    indexes->push_back(index);
  }

} // CheckCorners()


void Crossover::CheckEdges(Board* pBoard, std::vector<PuzzlePiece>* pieces,
                           std::vector<std::vector<int>>* indexes, int boardSize,
                           std::vector<bool>* checkIDs)
{ // Checks all edges to see if there are any duplicate pieces within the
  // edge slots, if so store the piece and the index of the duplicate slot

  for (int i = 1; i <= boardSize - 1; i++)
  { // Loop through as many times as there are edge slots on a edge checking
    // for duplicates on one slot of each edge at a time
    
    // If ID on top edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVec[0][i].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVec[0][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVec[0][i]);
      std::vector<int> index = { 0, i};
      indexes->push_back(index);
    }

    // If ID on left edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVec[i][0].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVec[i][0].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVec[i][0]);
      std::vector<int> index = { i, 0 };
      indexes->push_back(index);
    }

    // If ID on right edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVec[i][boardSize].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVec[i][boardSize].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVec[i][boardSize]);
      std::vector<int> index = { i, boardSize };
      indexes->push_back(index);
    }

    // If ID on bottom edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVec[boardSize][i].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVec[boardSize][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVec[boardSize][i]);
      std::vector<int> index = { boardSize, i };
      indexes->push_back(index);
    }

  } // for i < boardSize - 2

} // CheckEdges()


void Crossover::CheckInners(Board* pBoard, std::vector<PuzzlePiece>* pieces,
                            std::vector<std::vector<int>>* indexes, int boardSize,
                            std::vector<bool>* checkIDs)
{ // Checks all inner slots to see if there are any duplicate pieces within the
  // inner slots, if so store the piece and the index of the duplicate slot

  int yIndex = 1;  // Y index for slot
  int xIndex = 1;  // X index for slot

  for (int i = 0; i < BoardManager::GetInstance()->pieceVec[INNER].size(); i++)
  { // Loop through for every inner slot changing the appropriate element
    // to true if piece was found

    if (!checkIDs->at(pBoard->boardVec[xIndex][yIndex].pieceID - 1))
    { // If piece has not already been found, change element to true
      checkIDs->at(pBoard->boardVec[xIndex][yIndex].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVec[xIndex][yIndex]);
      std::vector<int> index = { xIndex, yIndex };
      indexes->push_back(index);
    }

    xIndex++;    // Increment X index to move to the slot on the right

    if (xIndex == BoardManager::GetInstance()->boardSize)
    { // If we have reached the end of the line of the board, increment
      // to next line
      xIndex = 1;
      yIndex++;
    }
  }

} // CheckInners()


void Crossover::FixDuplicates(Board* pBoard, std::vector<PuzzlePiece> pieces, 
                              std::vector<std::vector<int>> indexes)
{ // Uses the vector of pieces along with the vector of slot indexes to place
  // pieces that are needed to to have every unique puzzle piece with in the
  // candidate

  for (int i = 0; i < pieces.size(); i++)
  { // Loop right placing the next piece that needs to be placed within the
    // board to the next slot with a duplicate piece and call to fix orientation
    pBoard->boardVec[indexes[i][0]][indexes[i][1]] = pieces[i];
    
    if (pBoard->boardVec[indexes[i][0]][indexes[i][1]].type == CORNER ||
        pBoard->boardVec[indexes[i][0]][indexes[i][1]].type == EDGE)
    { // If piece is an edge or corner piece, make sure piece is properly rotated
      BoardManager::GetInstance()->FixOrientation(&pBoard->boardVec[indexes[i][0]]
                                                  [indexes[i][1]], indexes[i][0],
                                                  indexes[i][1]);
    }
  }

} // FixDuplicates()